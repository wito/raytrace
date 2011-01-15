#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <png.h>

#include "primitive.h"
#include "sphere.h"
#include "plane.h"

#define WIDTH 1280
#define HEIGHT 800

void writePNG (png_bytepp imageBuffer, png_uint_32 width, png_uint_32 height, int iteration) {
  const char *fname = "out.png";
  
  FILE *fp = fopen(fname, "wb");
  if (!fp) return;
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop  info_ptr = png_create_info_struct(png_ptr);
  png_init_io(png_ptr, fp);
  png_set_filter(png_ptr, PNG_FILTER_TYPE_DEFAULT, PNG_FILTER_NONE);
  png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
  png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
  png_set_rows(png_ptr, info_ptr, imageBuffer);
  png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
  png_destroy_write_struct(&png_ptr, &info_ptr);
  fclose(fp);
}

Line *createLine(vector camera, png_uint_32 x, png_uint_32 y) {
  Line *retval = malloc(sizeof(Line));

  retval->start = camera;

  vector planePoint = vectorCreate(-4.0 + 8.0 * ((double)x / (double)WIDTH), 0.0, 5.0 - 5.0 * ((double)y / (double)HEIGHT));
  vector dir = vectorSubtraction(planePoint, camera);

  retval->end = vectorAddition(camera, vectorMultiply(dir,50));

  return retval;
}

Line *makeLine(vector start, vector end) {
  Line *retval = malloc(sizeof(Line));

  retval->start = start;
  retval->end = end;

  return retval;
}

typedef struct light {
  vector location;
  vector color;
  double intensity;
} Light;

int main (int argc, const char **argv, const char **env) {
  Light *light = malloc(sizeof(Light));

  light->location = vectorCreate(3.0, 3.0, 5.5);
  light->color = vectorCreate(1.0, 1.0, 1.0);
  light->intensity = 1.0;

  Light **lights = calloc(3, sizeof(Light*));

  lights[0] = light;

  light = malloc(sizeof(Light));

  light->location = vectorCreate(-3.0, 3.0, 4.0);
  light->color = vectorCreate(1.0, 1.0, 1.0);
  light->intensity = 1.0;

  lights[1] = light;

  Primitive **primitives = calloc(3,sizeof(Primitive *));

  primitives[0] = createSphere((vector){3.0, 3.0, 2.5}, 1.1);
  primitives[1] = createSphere((vector){-3.0, 4.0, 2.0}, 1.2);
  primitives[2] = createPlane(vectorCreate(0.0, 0.0, 1.0), vectorCreate(0.0, 0.0, 0.0));

  primitives[0]->material = malloc(sizeof(Material));
  primitives[1]->material = malloc(sizeof(Material));
  primitives[2]->material = malloc(sizeof(Material));

  primitives[0]->material->color = vectorCreate(1.0, 0.0, 0.0);
  primitives[1]->material->color = vectorCreate(0.0, 1.0, 0.0);
  primitives[2]->material->color = vectorCreate(1.0, 0.0, 1.0);

  primitives[0]->material->specular = 1.0;
  primitives[1]->material->specular = 0.5;
  primitives[2]->material->specular = 0.0;

  primitives[0]->material->diffuse = 0.2;
  primitives[1]->material->diffuse = 0.5;
  primitives[2]->material->diffuse = 1.0;

  primitives[0]->material->reflection = 0.0;
  primitives[1]->material->reflection = 0.0;
  primitives[2]->material->reflection = 0.0;

  vector camera = vectorCreate(0.0, -5.0, 2.5);

  png_uint_32 height = HEIGHT, width = WIDTH;

  png_bytepp imageBuffer = malloc(sizeof(png_bytep) * height);
  for (png_uint_32 i = 0; i < height; i++) {
    imageBuffer[i] = malloc(sizeof(png_byte) * 3 * width);
    for (png_uint_32 j = 0; j < width * 3; j++) {
      imageBuffer[i][j] = 0;
    }
  }
  
  int x,y;

  for (y = 0; y < HEIGHT; y++) {
    
    for (x = 0; x < WIDTH; x++) {
      Line *ray = createLine(camera, x, y);

      Intersection *bestIntersection = NULL;

      for (int p = 0; p < 3; p++) {
        Intersection *thisIntersection = primitiveIntersect(primitives[p], ray);
        if (!bestIntersection) {
          bestIntersection = thisIntersection;
          continue;
        }
        if (thisIntersection && thisIntersection->distance < bestIntersection->distance) {
          free(bestIntersection);
          bestIntersection = thisIntersection;
        }
      }

      png_byte red,green,blue;

      if (bestIntersection) {
        double lighting = 0.0;
        for (int l = 0; l < 2; l++) {
          light = lights[l];
          vector lvec = vectorSubtraction(light->location, bestIntersection->intersectionPoint);
      
          Intersection *linter = NULL;
          Line *lightRay = makeLine(bestIntersection->intersectionPoint, light->location);
      
          for (int p = 0; p < 3; p++) {
            if (primitives[p] == bestIntersection->primitive)
              continue;
            if ((linter = primitiveIntersect(primitives[p], lightRay)))
              break;
          }
      
          free(lightRay);
      
          if (!linter) {
            float sintensity = 0.0;
      
            vector V = vectorUnit(vectorSubtraction(ray->end, ray->start));
            vector N = primitiveNormal(bestIntersection->primitive, bestIntersection);
      
            vector L = vectorUnit(lvec);
      
            vector R = vectorSubtraction(L, vectorMultiply(N, 2 * vectorDotProduct(L,N)));
      
            float dot = vectorDotProduct(V, R);
      
            if (dot > 0.0) {
              sintensity = pow(dot, 20) * bestIntersection->primitive->material->specular;
            }
      
            float dintensity = bestIntersection->primitive->material->diffuse * vectorDotProduct(vectorUnit(lvec), bestIntersection->primitive->normal(bestIntersection->primitive, bestIntersection));
      
            lighting += dintensity + sintensity;
          }
      
          free(linter);
        }

        if (lighting > 1.0) {
          lighting = 1.0;
        }
        if (lighting < 0.0) {
          lighting = 0.0;
        }

        red = bestIntersection->primitive->material->color.x * lighting * 255;
        green = bestIntersection->primitive->material->color.y * lighting * 255;
        blue = bestIntersection->primitive->material->color.z * lighting * 255;
      } else {
        red = 0;
        green = 0;
        blue = 0;
      }
            
      imageBuffer[y][x * 3] = red;
      imageBuffer[y][x * 3 + 1] = green;
      imageBuffer[y][x * 3 + 2] = blue;
    }
  
  }
  
  writePNG(imageBuffer, width, height, 0);
    
  for (png_uint_32 i = 0; i < height; i++) {
    free(imageBuffer[i]);
  }    
  free(imageBuffer);

  return 0;
}
