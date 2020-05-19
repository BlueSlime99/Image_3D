#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__
 
#include "json.h"
#include "scene3D.h"
#include "bmp.h"
 
typedef struct {
  double focal;
  point3D_t pos;
  vector3D_t dir;
  vector3D_t vert;
  vector3D_t hori;
 
  size_t width;
  size_t height;
} camera_t;

color_t pixel(size_t i, size_t j, void** args);
 
camera_t camera(point3D_t p, vector3D_t dir, vector3D_t vert, double f, size_t w, size_t h);
json_element_t* camera_to_json(camera_t c);
camera_t camera_from_json(const json_element_t* e);
 
void camera_raytracing(const char* fname, size_t depth, camera_t c, scene3D_t* s);
 
#endif