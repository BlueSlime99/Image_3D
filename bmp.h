#ifndef __RAYTRACER_BMP_H__
#define __RAYTRACER_BMP_H__

#include <stdlib.h>
#include "color.h"
#include <math.h>
#include "raytracer.h"
#include "json.h"


void bmp_save(const char* fname, size_t width, size_t height, size_t depth, color_t (*pixel)(size_t i, size_t j, void** args), void** args);
 

 #endif

