#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "args.h"




typedef struct option_s {
  const char* keyword;
  enum { OptVoid, OptInt, OptString, OptFloat } spec;
  union {
    void (*opt_void)();
    void (*opt_int)(int);
    void (*opt_str)(const char*);
    void (*opt_float)(float);
  } fct;
  struct option_s* next;
} option_t;


option_t* opt_void(option_t* l, const char* kw, void (*f)()){
	option_t* new = (option_t*)malloc(sizeof(option_t));
	if(new==NULL) exit(1);
	option_t* curr = l;
	option_t* prev;
	prev = NULL;
	while(curr!=NULL){
		prev = curr;
		curr = curr->next;
	}
	new->keyword = kw;
	new->spec = OptVoid;
	new->fct.opt_void = f;
	new->next = curr;
	if(prev==NULL) return new;
	prev->next = new;
	return l;
}


option_t* opt_int(option_t* l, const char* kw, void (*f)(int)){
	option_t* new = (option_t*)malloc(sizeof(option_t));
	if(new==NULL) exit(1);
	option_t* curr = l;
	option_t* prev = NULL;
	while(curr!=NULL){
		prev = curr;
		curr = curr->next;
	}
	new->keyword = kw;
	new->spec = OptInt;
	new->fct.opt_int = f;
	new->next = curr;
	if(prev==NULL) return new;
	prev->next = new;
	return l;
}


option_t* opt_int(option_t* l, const char* kw, void (*f)(int)){
	option_t* new = (option_t*)malloc(sizeof(option_t));
	if(new==NULL) exit(1);
	option_t* curr = l;
	option_t* prev = NULL;
	while(curr!=NULL){
		prev = curr;
		curr = curr->next;
	}
	new->keyword = kw;
	new->spec = OptFloat;
	new->fct.opt_float = f;
	new->next = curr;
	if(prev==NULL) return new;
	prev->next = new;
	return l;
}

option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*)){
	option_t* new = (option_t*)malloc(sizeof(option_t));
	if(new==NULL) exit(1);
	option_t* curr = l;
	option_t* prev = NULL;
	while(curr!=NULL){
		prev = curr;
		curr = curr->next;
	}
	new->keyword = kw;
	new->spec = OptString;
	new->fct.opt_str = f;
	new->next = curr;
	if(prev==NULL) return new;
	prev->next = new;
	return l;
}

void opt_delete(option_t* l){
	if(l->next!=NULL){
		opt_delete(l->next);
	}
	free(l);
}


void process_options(option_t* l, int argc, char* *argv){
	option_t* o = NULL;
	for (int i = 0; i < argc; i++)
	{
		o = l;
		if (argv[i][0]=='-')
		{
			while(strcmp(o->keyword,argv[i])){
				o = o->next;
				if (o == NULL){fprintf(stderr, "NOOPTION");exit(EXIT_FAILURE);}
			}
			if (o->spec == OptVoid)
			{
				i++;
				o->fct.opt_void();
			}
			if (o->spec == OptInt)
			{
				i++;
				o->fct.opt_int(atoi(argv[i]));
			}
			if (o->spec==OptFloat)
			{
				i++;
				o->fct.opt_float(atof(argv[i]));
			}
			if (o->spec==OptString)
			{
				i++;
				o->fct.opt_str(argv[i]);
			}
				
		}
	}
}


void f1(const char* str) {
  printf("F1: %s\n", str);
}
 
void f2(int i) {
  printf("F2: %d\n", i);
}
 
void f3() {
  printf("F3: no param\n");
}
 
void f4(float f) {
  printf("F4: %f\n", f);
}
 
 
int main(int argc, char** argv) {
  option_t* opt = NOOPTION;
  opt = opt_string(opt, "-a", f1);
  opt = opt_int(opt, "-b", f2);
  opt = opt_void(opt, "-c", f3);
  opt = opt_float(opt, "-d", f4);
 
  process_options(opt, argc, argv);
 
  opt_delete(opt);
 
  return EXIT_SUCCESS;
}