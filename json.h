
#ifndef __JSON_H__
#define __JSON_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define json_null() ((json_element_t*)NULL)
/* type(s) declaration */


typedef struct json_element_s json_element_t;
typedef struct json_object_s json_object_t;

typedef enum{
	JSON_NULL,
	JSON_BOOLEAN,
	JSON_NUMBER,
	JSON_STRING,
	JSON_ARRAY,
	JSON_OBJECT,
}json_type_t;




/* function prototypes */


json_type_t json_type(const json_element_t* e);
//BOOLEEN
json_element_t* json_boolean(bool b);
bool json_as_boolean(const json_element_t* e);
//NOMBRES
json_element_t* json_integer(long i);
json_element_t* json_double(double d);
bool json_is_integer(const json_element_t* e);
bool json_is_double(const json_element_t* e);
long json_as_integer(const json_element_t* e) ;
double json_as_double(const json_element_t* e) ;
//Chaînes de caractères
json_element_t* json_string(const char* s) ;
const char* json_as_string(const json_element_t* e);
//Tableaux
json_element_t* json_array();
json_element_t* json_array_get(const json_element_t* e, size_t i);
void json_array_set(json_element_t* e, size_t i, json_element_t* se);
//Objets
json_element_t* json_object();
json_element_t* json_object_get(const json_element_t* e, const char* key);
json_object_t* create_oject(json_element_t* value, const char* key);
void json_object_set( json_element_t* e, const char* key, json_element_t* value);
//Destructeur
void json_free(json_element_t* e);
//EXPOT
void json_put(FILE* fd, const json_element_t* e);
void json_save(const char* fname, json_element_t* e);
//IMPORT
json_element_t* json_get(FILE* fd);
char next_char(FILE* fd);
void check_next_char(FILE* fd, char c);
bool is_next_char(FILE* fd, char c, bool cons);
void check_next_word(FILE* fd, const char* w);
char* next_string(FILE* fd);
char* next_digits(FILE* fd);
size_t length_of_array(json_element_t* e);
json_element_t* json_load(const char* fname);

/*
char* next_string(FILE* fd);
char* next_digits(FILE* fd);
json_element_t* json_load(const char* fname) ;
*/
#endif
