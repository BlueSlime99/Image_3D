#include <stdlib.h>
#include <stdio.h>
#include "json.h"
#include "scene3D.h"
#include "raytracer.h"
 
camera_t camera(point3D_t p, vector3D_t dir, vector3D_t vert, double f, size_t w, size_t h) {
  camera_t c;
  c.pos = p;
  c.dir = vector3D_normalize(dir);
  c.vert = vector3D_normalize(vert);
  c.hori = vector3D_cross_product(c.dir, c.vert);
  c.focal = f;
  c.width = w;
  c.height = h;
  return c;
}
 
 
json_element_t* camera_to_json(camera_t c){
  json_element_t* e = json_object();
  json_object_set(e, "position", coord3D_to_json(c.pos)); 
  json_object_set(e, "direction", coord3D_to_json(c.dir)); 
  json_object_set(e, "vertical", coord3D_to_json(c.vert)); 
  json_object_set(e, "focal", json_double(c.focal));
  json_object_set(e, "width", json_integer(c.width));
  json_object_set(e, "height", json_integer(c.height));
  return e;
}
 
camera_t camera_from_json(const json_element_t* e) {
  if (json_type(e) != JSON_OBJECT) exit(EXIT_FAILURE);
  const json_element_t* se;
  point3D_t p;
  vector3D_t dir;
  vector3D_t vert;
  double f;
  size_t w;
  size_t h;
  se = json_object_get(e, "position"); p = coord3D_from_json(se);
  se = json_object_get(e, "direction"); dir = coord3D_from_json(se);
  se = json_object_get(e, "vertical"); vert = coord3D_from_json(se);
  se = json_object_get(e, "focal"); if (json_type(se) != JSON_NUMBER) exit(EXIT_FAILURE); f = json_as_double(se);
  se = json_object_get(e, "width"); if (json_type(se) != JSON_NUMBER) exit(EXIT_FAILURE); w = json_as_integer(se);
  se = json_object_get(e, "height"); if (json_type(se) != JSON_NUMBER) exit(EXIT_FAILURE); h = json_as_integer(se);
 
  return camera(p,dir,vert,f,w,h);
}
 
 

 
void camera_raytracing(const char* fname, size_t depth, camera_t c, scene3D_t* s) {
  void* args[2];  
  args[0] = (void*)&c;
  args[1] = (void*)s;
  bmp_save(fname, c.width, c.height, depth, pixel, args);
}


 color_t pixel(size_t i, size_t j, void** args){
  camera_t c = *(camera_t*)args[0];
  scene3D_t* s = (scene3D_t*)args[1];
 
  vector3D_t d = vector3D_scale(c.focal, c.dir);
  d = vector3D_add(d, vector3D_scale(i-0.5*(c.width-1), c.hori));
  d = vector3D_add(d, vector3D_scale(0.5*(c.height-1)-j, c.vert));
  ray_t r = ray(c.pos,d);
 
  double l;
  point3D_t q;
  vector3D_t n;
  color_t p = color(0,0,0);
  if (scene3D_intersect(s, &r, &l, &q, &n, &p))
    return scene3D_enlightement(s, q, n, p);
  else
    return p;
}
 /*
int main() {
  json_element_t* e = json_load("project.json");
  camera_t c = camera_from_json(json_object_get(e, "camera"));
  scene3D_t* s = scene3D_from_json(json_object_get(e, "scene3D"));
  json_free(e);
  camera_raytracing("project24.bmp", 24, c, s);
  camera_raytracing("project8.bmp", 8, c, s);
  camera_raytracing("project1.bmp", 1, c, s);
  scene3D_free(s);

  return EXIT_SUCCESS;
}*/