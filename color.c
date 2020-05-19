
#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include "color.h"
#include"json.h"
#define prl {printf("\n");}

color_t color(double r, double g, double b){
	color_t c;
	c.R = r;
	c.G = g;
	c.B =b;
	return c; 
}

double clamp (double a, double b, double c){
	if(a<b) return b;
	if(a>c) return c;
	return a;
}
//CONSTRUCTEUR
color_t color_add(color_t c1, color_t c2){
	color_t c;
	c.R =  clamp(c1.R + c2.R,0,1);
	c.B = clamp(c1.B + c2.B,0,1);
	c.G = clamp(c1.G + c2.G,0,1); 
	return c;
}
//COULEURS
color_t color_sub(color_t c1, color_t c2){
	color_t c;
	c.R =  clamp(c1.R - c2.R,0,1);
	c.B = clamp(c1.B - c2.B,0,1);
	c.G = clamp(c1.G - c2.G,0,1);  
	return c;
}
color_t color_mul(color_t c1, color_t c2){
	color_t c;
	c.R = clamp(c1.R * c2.R,0,1);
	c.B = clamp(c1.B * c2.B,0,1);
	c.G = clamp(c1.G * c2.G,0,1); 
	return c;
}
color_t color_scale(double a, color_t c){
	c.R = clamp(c.R * a,0,1);
	c.B =clamp(c.B * a,0,1);
	c.G = clamp(c.G * a,0,1); 
	return c;
}
			// CONVERSIONS
void color_to_BGR(color_t c, unsigned char* buffer){
	*buffer = clamp((c.B)*256, 0,255);
	*(buffer+1) = clamp((c.G)*256 , 0,255);
	*(buffer+2) = clamp((c.R)*256,0 ,255);
}

 unsigned char color_to_graylevel(color_t c){
 	unsigned char Gray = clamp((((c.R+c.B+c.G)/3)*256),0 ,255);
 	return Gray;
 }

  bool color_to_BW(color_t c){
  		return ((c.B+c.G+c.R)/3 > 0.5);
  }


 json_element_t* color_to_json(color_t c){
 	json_element_t* e = json_object();
 	json_object_set(e, "r",json_double(c.R)); 
 	json_object_set(e, "b",json_double(c.B));
 	json_object_set(e, "g",json_double(c.G)); 

 	return e;
 }

 color_t color_from_json(json_element_t* e){
 	color_t c;
 	c.R = json_as_double(json_object_get(e, "r"));
 	c.B = json_as_double(json_object_get(e, "b"));
 	c.G = json_as_double(json_object_get(e, "g"));
 	return c;
 }

 void color_print(color_t c){
	printf("<r=%g, g= %g,b=%g > \n", c.R , c.G, c.B);
}



/*int main() {
  color_t c = color(0.123456,0.987654,0.192837);
 
  color_print(c); prl;
  color_print(color_add(c,c)); prl;
  color_print(color_sub(c,c)); prl;
  color_print(color_mul(c,c)); prl;
  color_print(color_scale(0.5,c)); prl;
 
  json_element_t* e = color_to_json(c);
  json_put(stdout, e); prl; 
  color_print(color_from_json(e));
  json_free(e); prl;
 
  unsigned char tab[3];
  color_to_BGR(c, tab);
  printf("0x%hhx, 0x%hhx, 0x%hhx\n", tab[0], tab[1], tab[2]);
  printf("0x%hhx\n", color_to_graylevel(c));
  printf("%d\n", color_to_BW(c));
 
  return EXIT_SUCCESS;
}*/
