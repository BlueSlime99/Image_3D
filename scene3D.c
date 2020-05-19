#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <string.h>
#include "scene3D.h"
#include <math.h>
#include "color.h"
#include "json.h"
#define NO_TYPE (10)
#define prl { printf("\n");}


void  coord3D_print(struct coord3D_s c){
printf("<%g,%g,%g>", c.x,c.y,c.z);
}

vector3D_t vector3D(double x, double y, double z){
	vector3D_t v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

vector3D_t vector3D_add(vector3D_t u, vector3D_t v){
	return vector3D(u.x+v.x, u.y + v.y, u.z + v.z);
}

vector3D_t vector3D_sub(vector3D_t u, vector3D_t v){
	return vector3D(u.x - v.x, u.y - v.y, u.z - v.z);
}
vector3D_t vector3D_scale(double a, vector3D_t u) {

	return vector3D(u.x*a, u.y*a, u.z*a);
}
 double vector3D_dot_product(vector3D_t u, vector3D_t v){
 	return (u.x*v.x + u.y*v.y + u.z*v.z);
 }
 vector3D_t vector3D_cross_product(vector3D_t u, vector3D_t v){
 	return vector3D(u.y*v.z-u.z*v.y, v.x*u.z-u.x*v.z, u.x*v.y-u.y*v.x);
 }
 double vector3D_norm2(vector3D_t u){
 		return  vector3D_dot_product(u,u) ;
 }
 double vector3D_norm(vector3D_t u){
 	return sqrt(vector3D_norm2(u));
 }
 vector3D_t vector3D_normalize(vector3D_t u){
 	return vector3D(u.x/vector3D_norm(u),u.y/vector3D_norm(u),u.z/vector3D_norm(u));
 }
 point3D_t point3D(double x, double y, double z){
 	point3D_t p;
	p.x = x;
	p.y = y;
	p.z = z;
	return p;

}
point3D_t point3D_translate(point3D_t p, vector3D_t u){
	point3D_t pt;
	pt.x = p.x + u.x;
	pt.y = p.y + u.y;
	pt.z = p.z + u.z;
	return pt;
}


 vector3D_t point3D_subtract(point3D_t p, point3D_t q){
 	vector3D_t vs;
 	vs.x = p.x - q.x;
 	vs.y = p.y - q.y;
	vs.z = p.z - q.z;
	return vs;
 }

 ray_t ray(point3D_t p, vector3D_t d) {
 	ray_t r;
 	r.p= p;
 	r.d= vector3D_normalize(d);
 	return r;
 }



typedef struct {
 	point3D_t PositionCentre;
 	double r;
}sphere_t;



sphere_t sphere(point3D_t c, double r){
	sphere_t s;
	s.PositionCentre = c;
	s.r = fabs(r); 
	return s;
}


sphere_t sphere_translate(sphere_t s, vector3D_t v){
  	s.PositionCentre = point3D_translate(s.PositionCentre, v);
  	return s;
}


bool sphere_intersect(sphere_t s, ray_t r, double* pl, point3D_t* pq, vector3D_t* pn){
  	double b = vector3D_dot_product( point3D_subtract(r.p, s.PositionCentre) , r.d);
  	double c =  (vector3D_norm2(point3D_subtract(r.p, s.PositionCentre)) - pow(s.r,2));
  	double l = -b -sqrt((pow(b,2))-c);
  	point3D_t q =  point3D_translate((r.p) , vector3D_scale(l, r.d) );
  	vector3D_t n = vector3D_normalize( point3D_subtract(q, s.PositionCentre) );
  	if((pow(b,2)-c)>=0 && l >=0){
  		if(pl != NULL) *pl = l;
  		if(pq != NULL) 	*pq = q;
  		if(pn != NULL) 	*pn = n;
  		return true;
	}
 	return false;
 	}

typedef struct{
	point3D_t pointPlane;
	vector3D_t vectNormalPlane;
}plane_t;

plane_t plane(point3D_t o, vector3D_t n){
plane_t p;
p.pointPlane = o;
p.vectNormalPlane = vector3D_normalize(n);
return p;
}

plane_t plane_translate(plane_t p, vector3D_t v){
	p.pointPlane = point3D_translate(p.pointPlane, v);
	p.vectNormalPlane = vector3D_normalize(p.vectNormalPlane);
	return p;
}

bool plane_intersect(plane_t p, ray_t r, double* pl, point3D_t* pq, vector3D_t* pn){
 bool intersection = false;
 double l = (vector3D_dot_product(point3D_subtract(p.pointPlane, r.p), p.vectNormalPlane)/(vector3D_dot_product(r.d,p.vectNormalPlane)));
 point3D_t q = point3D_translate((r.p) , vector3D_scale(l, r.d) );
 if(l>=0 && vector3D_dot_product(r.d,p.vectNormalPlane) != 0){
 		if(pl != NULL){
  			*pl = l;
  		}
  		if(pq != NULL){
  			*pq = q;
  		}
  		if(pn != NULL){
  			*pn = p.vectNormalPlane;
  		}
  		intersection = true;
 }
 return intersection;

}

typedef struct{
	union{
	sphere_t s;
	plane_t p;
	};
	primitive_type_t type;
	color_t color;
}primitive_t;

primitive_t primitiveSphere(sphere_t s, color_t c){
	primitive_t prim;
	prim.s = s;
	prim.color = c;
	prim.type = SPHERE;
	return prim;
}

primitive_t primitivePlane(plane_t p, color_t c){
	primitive_t prim;
	prim.p = p;
	prim.color = c;
	prim.type = PLANE;
	return prim;
}

color_t primitive_get_color(primitive_t* p){
	return p->color;
}

void primitive_set_color(primitive_t* p, color_t c){
	p->color = c;
}

bool primitive_intersect(primitive_t* p, ray_t* r, double* pl, point3D_t* pq, vector3D_t* pn){
	if(p!=NULL){
	switch(p->type){
		case(SPHERE) : return(sphere_intersect(p->s, *r, pl, pq, pn));
		break;
		case(PLANE) : return(plane_intersect(p->p, *r, pl, pq, pn));				
		break;
		default: return false;
	}
}else{
	return false;
}
}

typedef struct{
	light_type_t type;
	union{
		point3D_t omni;
		vector3D_t uni;
	};
	color_t color;
}light_t;

light_t ambient(color_t c){
	light_t l;
	l.type = AMBIENT;
	l.color = c;
	return l;
}

light_t omni(color_t c, point3D_t p) {
 	light_t l;
 	l.type = OMNI;
 	l.omni =p;
 	l.color=c;
 	return l;
 }
light_t uni(color_t c, vector3D_t d){
	light_t l;
	l.type = UNI;
	l.uni =  vector3D_normalize(d);
	l.color=c;
	return l;
}
ray_t light_ray(light_t l, point3D_t q){ 
	ray_t r;
	r.p = q;
	switch(l.type){
		case(UNI) : return ray(q,vector3D_scale(-1,(l.uni)));
		break;
		case(OMNI) : return ray(q, point3D_subtract(l.omni, q));
		break;
		case(AMBIENT) : return r;
	}
	
	return r;
}


struct scene3D_s{
	primitive_t p;
	light_t l;
	scene3D_t* scene;
};

scene3D_t* scene3D_new(){
	scene3D_t* scene3D = malloc(sizeof(scene3D_t));
	if(scene3D == NULL)exit(EXIT_FAILURE);
	scene3D->p.type = NO_TYPE;
	scene3D->l.type = NO_TYPE;
	scene3D->scene = NULL;
	return scene3D;
}

void scene3D_free(scene3D_t* s){
	if(s->scene != NULL) scene3D_free(s->scene);

	free(s);
}

void scene3D_primitives_print(scene3D_t* s){
 	static int primitiveId = 0;
 	if(s->p.type == SPHERE){
 		printf("(%d) : Sphere of radius %g centered on ", primitiveId, s->p.s.r); 
 		coord3D_print(s->p.s.PositionCentre);
 		printf(" with color "); 
 		primitiveId++;
 		color_print(s->p.color);
 		prl;
 	}
 	else if(s->p.type == PLANE){
 		printf("(%d) :  Plane of normal  ", primitiveId);
 		coord3D_print(s->p.p.vectNormalPlane);
 		printf(" passing through  "); 
 		coord3D_print(s->p.p.pointPlane);
 		printf(" with color ");  		
 		primitiveId++;
 		color_print(s->p.color);
 		prl;
 	}
 	else if(s->p.type == NO_TYPE && primitiveId == 0){
 		printf("no primitive\n");
 		return;
 	}
 	if(s->scene != NULL) scene3D_primitives_print(s->scene);
 	primitiveId = 0;
}

void scene3D_primitive_print(scene3D_t* s, unsigned int primitiveId) {
	unsigned int i=0;
	scene3D_t* snew = s;
 	while(i!=primitiveId){
	 	if(snew->scene != NULL){
	 		snew = snew->scene; 
		}
			i++;
	}
	if(s->p.type == SPHERE){
		printf("(%d) : Sphere of radius %g centered on ", primitiveId, s->p.s.r); 
		coord3D_print(s->p.s.PositionCentre);
		printf(" with color "); 
		primitiveId++;
	}
	if(s->p.type == PLANE){
		printf("(%d) : Plane of normal  ", primitiveId);
		coord3D_print(s->p.p.vectNormalPlane);
		printf(" passing through  "); 
		coord3D_print(s->p.p.pointPlane);
		printf(" with color ");  		
		primitiveId++;
	}
 	color_print(s->p.color);

 	if(s->p.type == NO_TYPE && primitiveId == 0){
 		printf(" no primitive \n" );
 		return;
 	}

	
}

void scene3D_lights_print(scene3D_t* s){
	static int lightId =0;
	if(s->l.type == UNI){
		printf("(%d) : Unidirectional light of direction ",lightId);
		coord3D_print(s->l.uni);
		printf(" with color ");  		
		lightId++;
		color_print(s->l.color);
		prl;
	}
	else if(s->l.type == OMNI){
		printf("(%d) : Omnidirectional light centered on",lightId);
		coord3D_print(s->l.omni);
		printf(" with color ");  		
		lightId++;
		color_print(s->l.color);
		prl;
	}
	else if(s->l.type == AMBIENT){
		printf("(%d) : Ambient light with color ", lightId);  		
		lightId++;
		color_print(s->l.color);
		prl;
	}
	else if(s->l.type == NO_TYPE && lightId == 0){
		printf("no light\n");
		return;
	}
	if(s->scene!=NULL) scene3D_lights_print(s->scene);
	lightId = 0;
}

void scene3D_light_print(scene3D_t* s, unsigned int lightId){
 	unsigned int i=0;
 	scene3D_t* snew = s;
 	while(i!=lightId){
	 	if(snew->scene != NULL){
	 		snew = snew->scene; 
		}
			i++;
	}
	if(s->l.type == UNI){
		printf("(%d) : Unidirectional light of direction ",lightId);
		coord3D_print(s->l.uni);
		printf(" with color ");  		
		lightId++;
		color_print(s->l.color);

	}
	if(s->l.type == OMNI){
		printf("(%d) : Omnidirectional light centered on",lightId);
		coord3D_print(s->l.omni);
		printf(" with color ");  		
		lightId++;
		color_print(s->l.color);
	}
	if(s->l.type == AMBIENT){
		printf(" Ambient light with color ");  		
		lightId++;
		color_print(s->l.color);
	}
	
	
}


void scene3D_sphere_add(scene3D_t* s, color_t color, point3D_t c, double r){
	if(s->p.type == NO_TYPE){
		s->p = primitiveSphere(sphere(c,r), color);
		return;
	} 
	else if(s->scene!=NULL) {
		scene3D_sphere_add(s->scene, color, c, r);
	}else{
		s->scene = scene3D_new();
		s->scene->p = primitiveSphere(sphere(c,r), color);
	}
}

void scene3D_plane_add(scene3D_t* s, color_t color, point3D_t o, vector3D_t n){	
	if(s->p.type == NO_TYPE){
	s->p = primitivePlane(plane(o, n), color);
	return;
	} 
	else if(s->scene!=NULL) {
		scene3D_plane_add(s->scene, color, o, n);
	}else{
		s->scene = scene3D_new();
		s->scene->p = primitivePlane(plane(o,n), color);
	}
}
 primitive_type_t scene3D_primitive_type(scene3D_t* s, unsigned int primitiveId){
 	unsigned int i=0;
 	scene3D_t* snew = s;
 	while(i!=primitiveId){
	 	if(snew->scene != NULL){
	 		snew = snew->scene; 
		}
			i++;
	}
 	 return snew->p.type; 
 }

void scene3D_sphere_set_radius(scene3D_t* s, unsigned int primitiveId, double radius){
		unsigned int i=0;
 		scene3D_t* snew = s;
 		while(i!=primitiveId){
	 		if(snew->scene != NULL){
	 			snew = snew->scene; 
			}
			i++;
		} 	
 		if(snew->p.type == SPHERE){
 			snew->p.s.r = radius;
 		}
 	
}

void scene3D_plane_set_normal(scene3D_t* s, unsigned int primitiveId, vector3D_t n){
	unsigned int i=0;
	scene3D_t* snew = s;
	while(i!=primitiveId){
		if(snew->scene != NULL){
			snew = snew->scene; 
	}
		i++;
	}
	if(snew->p.type == PLANE){
		snew->p.p.vectNormalPlane = n;
	}
 
}
void scene3D_primitive_set_color(scene3D_t* s, unsigned int primitiveId, color_t color) {
	unsigned int i=0;
 	scene3D_t* snew = s;
 	while(i!=primitiveId){
	 	if(snew->scene != NULL){
	 		snew = snew->scene; 
		}
			i++;
	}
	if(snew->p.type != NO_TYPE) snew->p.color = color;
}
void scene3D_primitive_translate(scene3D_t* s, unsigned int primitiveId, vector3D_t v){
	unsigned int i=0;
	scene3D_t* snew = s;
	while(i!=primitiveId){
		if(snew->scene != NULL){
			snew = snew->scene; 
	}
	i++;
	}
	if(snew->p.type == SPHERE){
		snew->p.s = sphere_translate(snew->p.s, v);
	}
	else if(snew->p.type == PLANE){
		snew->p.p = plane_translate(snew->p.p, v);
	}
}

 void scene3D_primitive_delete(scene3D_t* s, unsigned int primitiveId){
 	unsigned int i=0;
	scene3D_t* snew = s;
	while(i!=primitiveId){
	 	if(snew->scene != NULL){
	 		snew = snew->scene; 
		}
		i++;
	}	
	while(snew->scene != NULL){
		snew->p = snew->scene->p;
		snew = snew->scene; 
	}
	snew->p.type = NO_TYPE;
 }

void scene3D_ambient_add(scene3D_t* s, color_t color){
	if(s->l.type == NO_TYPE){
		s->l=ambient(color);
	}
	
	else if(s->scene!=NULL) {
		scene3D_ambient_add(s->scene, color);
	}else{
		s->scene = scene3D_new();
		s->scene->l = ambient(color);
	}
}


void scene3D_omni_add(scene3D_t* s, color_t color, point3D_t p){
	if(s->l.type == NO_TYPE){
		s->l=omni(color,p);
	
	}
	else if(s->scene!=NULL) {
		scene3D_omni_add(s->scene, color, p);
	}else{
		s->scene = scene3D_new();
		s->scene->l = omni(color,p);
	}
}

 void scene3D_uni_add(scene3D_t* s, color_t color, vector3D_t d){
	if(s->l.type == NO_TYPE){
		s->l=uni(color,d);
		
	}
	else if(s->scene!=NULL) {
		scene3D_uni_add(s->scene, color, d);
	}else{
		s->scene = scene3D_new();
		s->scene-> l = uni(color,d);
	}
}

light_type_t scene3D_light_type(scene3D_t* s, unsigned int lightId){
	unsigned int i=0;
	scene3D_t* snew = s;
 	while(i!=lightId){
	 	if(snew->scene != NULL){
	 		snew = snew->scene;
		}
		i++;
	}
 		
 	return snew->l.type;
 		
}

 void scene3D_omni_set_position(scene3D_t* s, unsigned int lightId, point3D_t p){
 	unsigned int i=0;
	scene3D_t* snew = s;
 	while(i!=lightId){
	 	if(snew->scene != NULL){
	 		snew = snew->scene;
		}
			i++;
	}
		if(snew->l.type == OMNI){
			snew->l.omni = p;
		}
 }

 void scene3D_uni_set_direction(scene3D_t* s, unsigned int lightId, vector3D_t d){

  	unsigned int i=0;
	scene3D_t* snew = s;
 	while(i!=lightId){
	 	if(snew->scene != NULL){
	 		snew = snew->scene;
		}
			i++;
	}
	if(snew->l.type == UNI){
		snew->l.uni = vector3D_normalize(d);
	}
 }

 void scene3D_light_set_color(scene3D_t* s, unsigned int lightId, color_t color){
 	unsigned int i=0;
	scene3D_t* snew = s;
 	while(i!=lightId){
	 	if(snew->scene != NULL){
	 		snew = snew->scene;
		}
			i++;
	}
	if(s->l.type != NO_TYPE) snew->l.color = color;
 }
 void scene3D_light_delete(scene3D_t* s, unsigned int lightId){
 	unsigned int i=0;
	scene3D_t* snew = s;
 	while(i!=lightId){
	 	if(snew->scene != NULL){
	 		snew = snew->scene;
		}
			i++;
		}
		while(snew->scene != NULL){
		snew->l = snew->scene->l;
		snew = snew->scene; 
	}
	snew->l.type = NO_TYPE;
 }

bool scene3D_intersect(scene3D_t* s, ray_t* r, double* ret_l, point3D_t* ret_q, vector3D_t* ret_n, color_t* ret_c){
	bool resultat = false;
	static scene3D_t* scenemin = NULL;
	static double min =-1;
	if(primitive_intersect(&s->p,r,ret_l,ret_q,ret_n)){
		if(*ret_l < min || min==-1){
			min = *ret_l;
			scenemin = s;
		}

	}
	if(s->scene != NULL) scene3D_intersect(s->scene,r,ret_l,ret_q, ret_n,ret_c);
	if(ret_c!=NULL && scenemin != NULL) *ret_c = scenemin->p.color;
	min = -1;
	if(scenemin != NULL) return(primitive_intersect(&scenemin->p,r,ret_l,ret_q,ret_n));
	else return false;
	return resultat = true;
}

unsigned int nmbprim(scene3D_t* s){
	unsigned int cmpt =0;
	scene3D_t* warda = s;
	while (warda!= NULL){
		if(scene3D_primitive_type(s, cmpt) == NO_TYPE) return cmpt;
		cmpt++;
		warda = warda->scene;
	}
	return cmpt;
}
unsigned int nmblight(scene3D_t* s){
	unsigned int cmpt =0;
	scene3D_t* warda = s;
	while (warda!= NULL){
		if(scene3D_light_type(s, cmpt) == NO_TYPE) return cmpt;
		cmpt++;
		warda = warda->scene;
	}
	return cmpt;
}

json_element_t* coord3D_to_json(struct coord3D_s c){
	json_element_t* e=  json_object();
	json_object_set(e, "x", json_double(c.x));
	json_object_set(e, "y", json_double(c.y));
	json_object_set(e, "z", json_double(c.z));
	return e;
}

struct coord3D_s coord3D_from_json(const json_element_t* e){
	vector3D_t v;
	v.x = json_as_double(json_object_get(e, "x"));
	v.y = json_as_double(json_object_get(e, "y"));
	v.z = json_as_double(json_object_get(e, "z"));
	return v;

}



json_element_t* scene3D_to_json(scene3D_t* s){
	scene3D_t* se = s;
	json_element_t* tp = json_array();
	json_element_t* tl = json_array();

	int size = nmbprim(s);
	for(int i=0 ;i<size;i++){
		json_element_t* e = json_object();
	if(se->p.type == SPHERE){
		json_object_set(e,"type",  json_string("SPHERE"));
		json_object_set(e,"color",  color_to_json(se->p.color));
		json_object_set(e,"radius",  json_double(se->p.s.r));
		json_object_set(e,"center",  coord3D_to_json(se->p.s.PositionCentre));
	}else if(se->p.type == PLANE){
		json_object_set(e,"type",  json_string("PLANE"));
		json_object_set(e,"color",  color_to_json(se->p.color));
		json_object_set(e,"normal",  coord3D_to_json(se->p.p.vectNormalPlane));
		json_object_set(e,"center",  coord3D_to_json(se->p.p.pointPlane));
	}

	
		json_array_set(tp, i, e);
		se = se->scene;
	}

	se = s;
	size = nmblight(s);
	for(int i=0 ;i<size;i++){
		json_element_t* e = json_object();
	if(se->l.type == AMBIENT){
		json_object_set(e,"type",  json_string("AMBIENT"));
		json_object_set(e,"color",  color_to_json(se->l.color));

	}else if(se->l.type == OMNI){
		json_object_set(e,"type",  json_string("OMNI"));
		json_object_set(e,"color",  color_to_json(se->l.color));
		json_object_set(e,"position",  coord3D_to_json(se->l.omni));
	}else if(se->l.type == UNI){
		json_object_set(e,"type",  json_string("UNI"));
		json_object_set(e,"color",  color_to_json(se->l.color));
		json_object_set(e,"direction",  coord3D_to_json(se->l.uni));
	}
	json_array_set(tl, i, e);
	se = se->scene;
	}
	json_element_t* e = json_object();
	json_object_set(e,"primitives",tp);
	json_object_set(e,"lights",tl);
	
	return e;
	
}

scene3D_t* scene3D_from_json(const json_element_t* e){
	scene3D_t* scene3D = scene3D_new();
	json_element_t* tableau1 = json_object_get(e, "primitives");
	size_t size1 = length_of_array(tableau1);
	json_element_t* tableau2 = json_object_get(e, "lights");
	size_t size2 = length_of_array(tableau2);
	for(size_t i=0;i<size1;i++){
		json_element_t* elementtableau = json_array_get(tableau1, i);
		if(!strcmp(json_as_string(json_object_get(elementtableau, "type")), "SPHERE")){
			scene3D_sphere_add(scene3D, color_from_json(json_object_get(elementtableau, "color")), coord3D_from_json(json_object_get(elementtableau, "center")), json_as_double(json_object_get(elementtableau,"radius")));
		}else if(!strcmp(json_as_string(json_object_get(elementtableau, "type")), "PLANE")){
			scene3D_plane_add(scene3D, color_from_json(json_object_get(elementtableau, "color")), coord3D_from_json(json_object_get(elementtableau, "point")), coord3D_from_json(json_object_get(elementtableau,"normal")));	
		}
		json_free(elementtableau);
	}
	for(size_t i = 0; i < size2; i++){
		json_element_t* elementtableau = json_array_get(tableau2, i);
		if(!strcmp(json_as_string(json_object_get(elementtableau, "type")), "AMBIENT"))
			scene3D_ambient_add(scene3D, color_from_json(json_object_get(elementtableau, "color")));
		else if(!strcmp(json_as_string(json_object_get(elementtableau, "type")), "OMNI"))
			scene3D_omni_add(scene3D, color_from_json(json_object_get(elementtableau, "color")), coord3D_from_json(json_object_get(elementtableau, "position")));
		else if(!strcmp(json_as_string(json_object_get(elementtableau, "type")), "UNI"))
			scene3D_uni_add(scene3D, color_from_json(json_object_get(elementtableau, "color")), coord3D_from_json(json_object_get(elementtableau, "direction")));
		
		json_free(elementtableau);
	}
	
	
	return scene3D;
	
}





color_t scene3D_light_color(scene3D_t* s, unsigned int lightId){
	unsigned int i=0;
	scene3D_t* snew = s;
 	while(i!=lightId){
	 	if(snew->scene != NULL){
	 		snew = snew->scene;
		}
		i++;
	}
 		
 	return snew->l.color;
 		
}


point3D_t scene3D_omni_position(scene3D_t* s, unsigned int lightId){
	unsigned int i=0;
	scene3D_t* snew = s;
	while(i!=lightId){
 		if(snew->scene != NULL){
 			snew = snew->scene;
		}
		i++;
	}
	if(snew->l.type == OMNI){
		return snew->l.omni;
	}else{
		 exit(EXIT_FAILURE);
	}		
}

vector3D_t scene3D_uni_direction(scene3D_t* s, unsigned int lightId){
	unsigned int i=0;
	scene3D_t* snew = s;
	while(i!=lightId){
 		if(snew->scene != NULL){
 			snew = snew->scene;
		}
		i++;
	}
	if(snew->l.type == UNI){
		return snew->l.uni;
	}else{
		exit(EXIT_FAILURE);
	}		
}


color_t scene3D_enlightement(scene3D_t* s, point3D_t q, vector3D_t n, color_t intra) {
  q = point3D_translate(q, vector3D_scale(1e-13, n));
  color_t c = color(0,0,0);
  ray_t r;
  double l, ll;
  unsigned int i =0;
  unsigned int light = nmblight(s);
  while(i < light) { 
    switch(scene3D_light_type(s,i)) {
    case AMBIENT:
      c = color_add(c, color_mul(scene3D_light_color(s,i), intra));
      break;
    case UNI:
      r = scene3D_light_ray(s, i, q);
      if (!scene3D_intersect(s,&r,&l,NULL,NULL,NULL)) 
        c = color_add(c, color_mul(color_scale(-vector3D_dot_product(scene3D_uni_direction(s,i), n), scene3D_light_color(s,i)), intra));
      break;
    case OMNI:
      r = scene3D_light_ray(s, i, q);
      ll = vector3D_norm(point3D_subtract(q, scene3D_omni_position(s,i)));
      if (!scene3D_intersect(s,&r,&l,NULL,NULL,NULL) || l > ll) 
        c = color_add(c, color_mul(color_scale(vector3D_dot_product(vector3D_normalize(point3D_subtract(scene3D_omni_position(s,i), q)), n), scene3D_light_color(s,i)), intra));
      break;
    default:
      break;
    }
    
    i++;
  }
  return c;
}


ray_t scene3D_light_ray(scene3D_t* s, unsigned int lightId, point3D_t q){
	ray_t r;
	unsigned int i=0;
	scene3D_t* snew = s;
	while(i!=lightId){
 		if(snew->scene != NULL){
 			snew = snew->scene;
		}
		i++;
	}
	r = light_ray(snew->l,q);
	return r;
}

