#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <png.h>
#include <zlib.h>

#include "primitive.h"
#include "sphere.h"
#include "plane.h"

#define WIDTH 1280
#define HEIGHT 800

#define EPS 0.0005

typedef struct light {
  vector location;
  vector color;
  double intensity;
} Light;

Line *makeLine(vector start, vector end) {
  Line *retval = malloc(sizeof(Line));
  
  retval->start = start;
  retval->end = end;
  
  return retval;
}

vector rayTrace(Line *ray, Primitive **primitives, Light **lights, float r_idx) {
  Intersection *bestIntersection = NULL;
  vector retval = vectorCreate(0.0, 0.0, 0.0);
  
  for (int p = 0; p < 10; p++) {
    if (!primitives[p])
      break;
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
  
  if (bestIntersection) {
    Primitive *primitive = bestIntersection->primitive;
    Material *material = primitive->material;

    vector mcolor = vectorMultiply(material->color, 0.0);
    
    vector N = primitiveNormal(primitive, bestIntersection);
    vector V = vectorUnit(vectorSubtraction(ray->end, ray->start));
    
    for (int l = 0; l < 2; l++) {
      Light *light = lights[l];
      vector lvec = vectorSubtraction(light->location, bestIntersection->intersectionPoint);
      
      vector pcolor = material->color;
      vector lcolor = light->color;

      Intersection *linter = NULL;
      Line *lightRay = makeLine(bestIntersection->intersectionPoint, light->location);
      
      float shade = 0.0;
      
      for (int p = 0; p < 3; p++) {
        if (primitives[p] == primitive)
          continue;
        if ((linter = primitiveIntersect(primitives[p], lightRay)))
          shade += 1.0 - linter->primitive->material->transparency;
      }
      
      free(lightRay);
      
      if (shade < 1.0) {
        if (material->specular > 0.0) {
          float sintensity = 0.0;
          
          vector L = vectorUnit(lvec);          
          vector R = vectorSubtraction(L, vectorMultiply(N, 2 * vectorDotProduct(L,N)));
          
          float dot = vectorDotProduct(V, R);
          
          if (dot > 0.0) {
            sintensity = pow(dot, 20) * material->specular * light->intensity * (1.0 - shade);
          }
          
          if (sintensity > 0.0) {
            mcolor = vectorAddition(mcolor, vectorMultiply(lcolor, sintensity));
          }
        }

        if (material->diffuse > 0.0) {
          float dintensity = material->diffuse * vectorDotProduct(vectorUnit(lvec), primitive->normal(bestIntersection->primitive, bestIntersection)) * light->intensity * (1.0 - shade);

          if (dintensity > 0.0) {
            mcolor = vectorAddition(mcolor, vectorMultiply(vectorCProduct(pcolor, lcolor), dintensity));
          }
        }
      }
      
      free(linter);
    }
    
    if (material->reflection > 0.0) {
      vector R = vectorUnit(vectorSubtraction(V, vectorMultiply(N, 2 * vectorDotProduct(V,N))));
      Line *rline = makeLine(vectorAddition(bestIntersection->intersectionPoint, vectorMultiply(R, EPS)), vectorAddition(bestIntersection->intersectionPoint, vectorMultiply(R, 30)));

      vector rcolor = rayTrace(rline, primitives, lights, r_idx);

      mcolor = vectorAddition(vectorMultiply(mcolor, 1.0 - material->reflection), vectorMultiply(rcolor, material->reflection));

      free(rline);
    }
    
    if (material->transparency > 0) {
      float refraction = material->refraction;
      float n = r_idx / refraction;
      vector Nr = vectorMultiply(N, bestIntersection->direction);
      float cosI = - vectorDotProduct(Nr, V);
      float cosT2 = 1.0 - n * n * (1.0 - cosI * cosI);
      if (cosT2 > 0.0) {
        vector T = vectorAddition(vectorMultiply(V, n), vectorMultiply(Nr, n * cosI - sqrt(cosT2)));

        Line *rline = makeLine(vectorAddition(bestIntersection->intersectionPoint, vectorMultiply(T, EPS)), vectorAddition(bestIntersection->intersectionPoint, vectorMultiply(T, 30)));
        vector rfcol = rayTrace(rline, primitives, lights, r_idx);

        mcolor = vectorAddition(vectorMultiply(mcolor, 1.0 - material->transparency), vectorMultiply(rfcol, material->transparency));
        
        free(rline);
      }
    }

    retval = mcolor;
    
    free(bestIntersection);
  }  
  return retval;
}

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

  vector planePoint = vectorCreate(-8.0 + 16.0 * ((double)x / (double)WIDTH), 0.0, 10.0 - 10.0 * ((double)y / (double)HEIGHT));
  vector dir = vectorSubtraction(planePoint, camera);

  retval->end = vectorAddition(camera, vectorMultiply(dir,50));

  return retval;
}

int main (int argc, const char **argv, const char **env) {
  Light *light = malloc(sizeof(Light));

  light->location = vectorCreate(0.0, 10.0, 10.0);
  light->color = vectorCreate(1.0, 1.0, 1.0);
  light->intensity = 1.0;

  Light **lights = calloc(3, sizeof(Light*));

  lights[0] = light;

  light = malloc(sizeof(Light));

  light->location = vectorCreate(0.0, 3.0, 10.0);
  light->color = vectorCreate(1.0, 0.4, 1.0);
  light->intensity = 1.0;

  lights[1] = light;

  Primitive **primitives = calloc(10,sizeof(Primitive *));

  primitives[0] = createSphere((vector){3.0, 7.0, 2.0}, 2.0);
  primitives[1] = createSphere((vector){0.0, 10.0, 1.9}, 1.9);
  primitives[2] = createPlane(vectorCreate(0.0, 0.0, 1.0), vectorCreate(0.0, 0.0, 0.0));

  primitives[0]->material = malloc(sizeof(Material));
  primitives[1]->material = malloc(sizeof(Material));
  primitives[2]->material = malloc(sizeof(Material));

  primitives[0]->material->color = vectorCreate(1.0, 1.0, 1.0);
  primitives[1]->material->color = vectorCreate(1.0, 1.0, 1.0);
  primitives[2]->material->color = vectorCreate(0.0, 0.7, 0.0);

  primitives[0]->material->specular = 0.7;
  primitives[1]->material->specular = 0.3;
  primitives[2]->material->specular = 0.0;

  primitives[0]->material->diffuse = 1.0;
  primitives[1]->material->diffuse = 0.5;
  primitives[2]->material->diffuse = 1.0;

  primitives[0]->material->reflection = 0.5;
  primitives[1]->material->reflection = 0.5;
  primitives[2]->material->reflection = 0.0;
  
  primitives[0]->material->refraction = 0.0;
  primitives[1]->material->refraction = 1.1;
  primitives[2]->material->refraction = 0.0;
  
  primitives[0]->material->transparency = 0.0;
  primitives[1]->material->transparency = 0.7;
  primitives[2]->material->transparency = 0.0;
  
  primitives[3] = createPlane(vectorCreate(0.0, -1.0, 0.0), vectorCreate(0.0, 15.0, 0.0));
  primitives[3]->material = malloc(sizeof(Material));
  
  primitives[3]->material->color = vectorCreate(0.3, 0.2, 7.0);
  
  primitives[3]->material->specular = 0.0;
  primitives[3]->material->diffuse  = 1.0;
  primitives[3]->material->reflection = 0.0;
  primitives[3]->material->refraction = 0.0;
  primitives[3]->material->transparency = 0.0;

  vector camera = vectorCreate(0.0, -5.0, 5.0);

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

      vector color = rayTrace(ray, primitives, lights, 1.0);
      
      free(ray);
      
      if (color.x >= 1.0) color.x = 1.0;
      if (color.y >= 1.0) color.y = 1.0;
      if (color.z >= 1.0) color.z = 1.0;
      
      imageBuffer[y][x * 3] = color.x * 255;
      imageBuffer[y][x * 3 + 1] = color.y * 255;
      imageBuffer[y][x * 3 + 2] = color.z * 255;
    }
  
  }
  
  writePNG(imageBuffer, width, height, 0);
    
  for (png_uint_32 i = 0; i < height; i++) {
    free(imageBuffer[i]);
  }    
  free(imageBuffer);

  return 0;
}
