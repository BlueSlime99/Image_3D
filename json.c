#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "json.h"

#define prl { printf("\n"); }

 typedef struct {
	enum {
		Integer_number,
		Float_number
	}number_type;
	union {
		long integer_number; 
		double float_number;
	};
}number_s;


struct json_object_s{
	char* key;
	json_element_t* value;
	json_object_t* next ;
};

typedef struct {
	int taille;
	json_element_t* *json_array;
}json_array_s;

 
struct json_element_s{
	json_type_t json_type;
	union{
		bool boolean;
		number_s number;
		char* string; 
		json_array_s array;
		json_object_t* object;
	};
};


json_type_t json_type(const json_element_t* e){
	if(e != json_null()){
		return e->json_type;
	}
 	return JSON_NULL; 
}
// BOOLEENS
json_element_t* json_boolean(bool b){
	json_element_t* json_new = malloc(sizeof(json_element_t));
	if(json_new == NULL) exit(1);
	json_new->json_type = JSON_BOOLEAN;
	json_new->boolean = b;
	return json_new;
}

bool json_as_boolean(const json_element_t* e){
 	if(json_type(e)==JSON_BOOLEAN){
 		return e->boolean;
 	}else{
 		fprintf(stderr,"the json-element_t is not bool");
 		exit(1);
 	}

 }

 // NOMBRES 

json_element_t* json_integer(long i){
	json_element_t* json_new_integernumber = malloc(sizeof(json_element_t));
	if(json_new_integernumber==NULL){
		exit(1);
	}
	json_new_integernumber->json_type = JSON_NUMBER;
	json_new_integernumber->number.number_type = Integer_number;
	json_new_integernumber->number.integer_number = i;
	return json_new_integernumber;
}

json_element_t* json_double(double d){
	json_element_t* json_new_floatnumber = malloc(sizeof(json_element_t));
	if(json_new_floatnumber==NULL){
		exit(1);
	}
	json_new_floatnumber->json_type = JSON_NUMBER;
	json_new_floatnumber->number.number_type = Float_number;
	json_new_floatnumber->number.float_number = d;
	return json_new_floatnumber;
}

 bool json_is_integer(const json_element_t* e){
 	if(json_type(e)==JSON_NUMBER){
 		if(e->number.number_type == Integer_number)
 			return true;
 		else 
 			return false;
 	}else {
 		fprintf(stderr,"the json_element_t e is not number type");
 		exit(1);
 	}
}

bool json_is_double(const json_element_t* e){
	if(json_type(e)==JSON_NUMBER){
 		if(e->number.number_type==Float_number)
 			return true;
 		else 
 			return false;
 	}else {
 		fprintf(stderr,"the json_element_t e is not number type");
 		exit(1);
 	}
 }

long json_as_integer(const json_element_t* e){
	if(json_type(e)==JSON_NUMBER){
		if(e->number.number_type == Integer_number)
			return e->number.integer_number;
		else
			return (long)e->number.float_number;
	}else{
		fprintf(stderr,"the e json_element_t is not number type");
		exit(1);
	}
}

 double json_as_double(const json_element_t* e){
 	if(json_type(e)==JSON_NUMBER){
 		if(e->number.number_type==Float_number){
 			return e->number.float_number;
 		}else {
 			return (float)e->number.integer_number;
 		}
 	}else {
 		fprintf(stderr,"the e json_element_t is not a number");
 		exit(1);
 	}
 }

 // chaines de caractéres

 json_element_t* json_string(const char* s){
 	json_element_t* json_string_new = malloc(sizeof(json_element_t));
 	if(json_string_new==NULL){
 		exit(1);
 	}
 	json_string_new->json_type = JSON_STRING;
 	json_string_new->string = malloc(strlen(s)+1);
 	json_string_new->string = strcpy(json_string_new->string,s);
 	return json_string_new;
 }

  const char* json_as_string(const json_element_t* e) {
  	if(json_type(e)==JSON_STRING){
  		return e->string;
  	}else{
  		fprintf(stderr,"the e json_element_t is not a string type");
  		exit(1);
  	}
  }
  // tableaux 
  json_element_t* json_array(){
  	json_element_t* json_new_array = malloc(sizeof(json_element_t));
  	if(json_new_array==NULL){
  		exit(1);
  	}
  	json_new_array->json_type = JSON_ARRAY;
  	json_new_array->array.json_array = NULL;
  	json_new_array->array.taille = 0;
  	return json_new_array;
  } 

 json_element_t* json_array_get(const json_element_t* e, size_t i){
 	if(json_type(e)==JSON_ARRAY){
 		size_t size = e->array.taille;
	 	if(i>size)
	 		return json_null();
	 	else
	 		return e->array.json_array[i];
 	}else{
 		fprintf(stderr,"the json_element_t e is not a array type");
 		exit(1);
 	}
 }

 void json_array_set(json_element_t* e, size_t i, json_element_t* se){
 	if(json_type(e)==JSON_ARRAY){
 		size_t size = e->array.taille;
 		if(i>=size){
 			e->array.json_array = realloc(e->array.json_array,(i+1)*sizeof(json_element_t));
 			if(e->array.json_array == NULL) exit(1);
			for(size_t j=size;j<i;j++){
				e->array.json_array[j] = json_null();
			}
			e->array.json_array[i] = se;
			e->array.taille = i+1;
		}else{
	  		json_free(e->array.json_array[i]);
	  		e->array.json_array[i] = se;
	  	}
 	}else {
 		fprintf(stderr,"the json_element_t e is not array type");
 		exit(1);
	}
}

size_t length_of_array(json_element_t* e){
	if(json_type(e) == JSON_ARRAY){
		return e->array.taille;
	}
	return 0;
}

 //objets
 json_element_t* json_object(){
 	json_element_t* new_json_object = NULL;
 	new_json_object = malloc(sizeof(json_element_t));
 	if(new_json_object==NULL) exit(1);
 	new_json_object->json_type = JSON_OBJECT;
 	new_json_object->object = NULL;
    return new_json_object;
}

int n_element(const json_element_t* e){
	int nb = 0;
    json_object_t* liste = NULL;
	liste = e->object;
    while(liste!= NULL){
    	nb++;
    	liste = liste->next;
    }
    return nb;
}

json_element_t* json_object_get(const json_element_t* e, const char* key){
	if(json_type(e)==JSON_OBJECT){
		int nb_element = n_element(e);
		json_object_t* l = NULL;
		l = e->object;
		int i =0;
		while(l!= NULL){
			if(!strcmp(l->key,key)){
				break;
			}else{
				l = l->next;
				i++;
			}
		}
		if(i==nb_element) return  json_null();
	 	return l->value;
	}else{
		fprintf(stderr,"the e json_element_t is not object type");
		exit(1);
	}
}

void json_object_set(json_element_t* e, const char* key, json_element_t* value){
	if(json_type(e)==JSON_OBJECT){
		if(e->object == NULL){
			e->object = malloc(sizeof(json_object_t));
			if(e->object == NULL) exit(1);
			e->object->key = malloc(strlen(key)+1);
			if(e->object->key == NULL) exit(1);
			e->object->key = strcpy(e->object->key,key);
			e->object->value = value;
			e->object->next = NULL;
			return;
		}
		int nb_element = n_element(e);
		json_object_t* l = NULL;
		json_object_t* prec = NULL;
		l = e->object;
		int i =0;
		while(l!= NULL){
			if(!strcmp(l->key,key)){
				break;
			}else{
				prec = l;
				l = l->next;
				i++;
			}
		}
		if(i==nb_element) {
			json_object_t* new = NULL;
			new = malloc(sizeof(json_object_t));
			if(new == NULL) exit(1);
			new->key = malloc(strlen(key)+1);
			if(new->key == NULL) exit(1);
			new->key = strcpy(new->key,key);
			new->value = value;
			new->next = NULL;
			prec->next = new;

		}
		else{
			json_free(l->value);
	 		l->value = value;
	 	}
	}else{
		fprintf(stderr,"the e json_element_t is not a object type");
		exit(1);
	}
}

void json_free_object(json_object_t* liste){
	if(liste != NULL){
 	 if(liste->next!=NULL) json_free_object(liste->next);
 	 free(liste->key);
 	 json_free(liste->value);
 	 free(liste);
 	}
}

void json_free(json_element_t* e){
		if(json_type(e) == JSON_NULL){
			free(e);
		}
		else if(json_type(e)==JSON_BOOLEAN){
			 free(e);
		}
		 else if(json_type(e)==JSON_NUMBER){
			free(e);
		}
		else if (json_type(e)==JSON_STRING){
			free(e->string);
			free(e);
		}
		 else if (json_type(e)==JSON_ARRAY){
			int size = e->array.taille;
			for(int i=0;i<size;i++){
				json_free(e->array.json_array[i]);
			}
			free(e->array.json_array);
			free(e);
		}
		 else if(json_type(e)==JSON_OBJECT){
			json_object_t* liste = e->object;
			json_free_object(liste);
			free(e);
		}
}

		

		
		
		

//Entrees/sorties



// export
void json_put(FILE* fd, const json_element_t* e){
	if(json_type(e)==JSON_NULL){
		fprintf(fd,"NULL");
	}
	if(json_type(e)==JSON_BOOLEAN){
		fprintf(fd, "%s",json_as_boolean(e)?"true":"false");
	}
	if(json_type(e)==JSON_NUMBER){
		if(json_is_integer(e)){
			fprintf(fd,"%ld",json_as_integer(e));

		}else if(json_is_double(e)){
			fprintf(fd,"%f", json_as_double(e));
		}
	}
	
	if(json_type(e)==JSON_STRING){
		fprintf(fd,"\"%s\"",json_as_string(e));
	}
	if(json_type(e)==JSON_ARRAY){
		fprintf(fd,"[");
		for(int i=0;i<e->array.taille;i++){
		    json_put(fd,e->array.json_array[i]);
			if(i != e->array.taille-1) fprintf(fd, ",");
		}
		fprintf(fd,"]");
	}
	if(json_type(e)==JSON_OBJECT){
	json_object_t* i = e->object;
		fprintf(fd,"{");
		while(i!=NULL){
			fprintf(fd,"\"%s\": ",i->key);
			json_put(fd,i->value);
			if(i->next != NULL) fprintf(fd,",");
			i = i->next;
		}
		
		fprintf(fd,"}");
	}
}	


void json_save(const char* fname, json_element_t* e){
	FILE* fichier = fopen(fname,"w");
	if(fichier==NULL)exit(1);
	json_put(fichier,e);
	fclose(fichier);
}

// import

json_element_t* json_get(FILE* fd) {
  int c = next_char(fd);
  if (c == 'n') {
    ungetc(c,fd);
    check_next_word(fd,"null");
    return json_null();
  }
  if (c == 't') {
    ungetc(c,fd);
    check_next_word(fd,"true");
    return json_boolean(true);
  }
  if (c == 'f') {
    ungetc(c,fd);
    check_next_word(fd,"false");
    return json_boolean(false);
  }
  if (c == '-') {
    json_element_t* se = json_get(fd);
    if (json_type(se) != JSON_NUMBER) {
      fprintf(stderr, "A number is expected after character '-'\n");
      exit(EXIT_FAILURE);
    }
    if (json_is_double(se)) {
      double d = json_as_double(se);
      json_free(se);
      return json_double(-d);
    }
    if (json_is_integer(se)) {
      long i = json_as_integer(se);
      json_free(se);
      return json_integer(-i);
    }
  }
  if ('0' <= c && c <= '9') {
    ungetc(c,fd);
    char* number = next_digits(fd);
    c = fgetc(fd);
    if (c == '.') {
      char* decimal = next_digits(fd);
      number = realloc(number, strlen(number) + strlen(decimal) + 2);
      strcat(number,".");
      strcat(number,decimal);
      json_element_t* e = json_double(strtod(number,NULL));
      free(number);
      free(decimal);
      return e;
    } else {
      ungetc(c,fd);
      json_element_t* e = json_integer(strtol(number,NULL,10));
      free(number);
      return e;
    }
  }
  if (c == '"') {
    ungetc(c,fd);
    char* str = next_string(fd);
    json_element_t* e = json_string(str);
    free(str);
    return e;
  }
  if (c == '[') {
    json_element_t* e = json_array();
    size_t i = 0;
    if (is_next_char(fd,']',true)) return e;
    while(1) {
      json_element_t* se = json_get(fd);
      json_array_set(e,i++,se);
      c = next_char(fd);
      if (c == ']') return e;
      if (c != ',') {
        fprintf(stderr, "Unexpected character '%c'; ']' or ',' excepted\n", c);
        exit(EXIT_FAILURE);
      }
    }
  }
  if (c == '{') {
    json_element_t* e = json_object();
    if (is_next_char(fd,'}',true)) return e;
    while(1) {
      check_next_char(fd,'"');
      ungetc('"',fd);
      char* key = next_string(fd);
      check_next_char(fd,':');
      json_element_t* value = json_get(fd);
      json_object_set(e,key,value);
      free(key);
      c = next_char(fd);
      if (c == '}') return e;
      if (c != ',') {
        fprintf(stderr, "Unexpected character '%c'; ']' or ',' excepted\n", c);
        exit(EXIT_FAILURE);
      }
    }
  }
  fprintf(stderr, "Unexpected character '%c'; 'n', 't', 'f', [0-9], '\"', '[' or '{' excepted\n", c);
  exit(EXIT_FAILURE);
}



char next_char(FILE* fd){
	char i = fgetc(fd);
	while(i!=EOF){
		if (i!=' '&& i!='\n'&& i!='\r'&& i!='\t'){
		return i;
		} 
		i = fgetc(fd);	
	}
	exit(1);
}

void check_next_char(FILE* fd, char c){
  if (next_char(fd)!=c){
  	exit(1);
  }
}

bool is_next_char(FILE* fd, char c, bool cons){
	char p = next_char(fd);
	
	if(p!=c|| cons==false){
		ungetc(p,fd);
		return false;
	}
	return true;
}

void check_next_word(FILE* fd, const char* w){
   	char s[strlen(w)+1]; 
   	fgets(s,strlen(w)+1,fd);
    s[strlen(w)] = '\0';
    if(strcmp(s,w)!=0) exit(EXIT_FAILURE);
}

char* next_string(FILE* fd){
	char* s = NULL;
	s = (char*)malloc(500);
	char c = fgetc(fd);
	int i = 0;
	while(c!='"' && c!= EOF){
		c = fgetc(fd);
	}
	if(c == EOF) exit(EXIT_FAILURE);
	c = fgetc(fd);
	while(c!= '"'){
		s[i] = c;
		i++;
		c = fgetc(fd);
	}

	s[i] = '\0';
	s = realloc(s,i+1);
	return s;
}
char* next_digits(FILE* fd){
	char* s = NULL;
	s = (char*)malloc(500);
	int i = 0;
	char c = fgetc(fd);
	while((c<'0'||c>'9')&& c!= EOF){
		c = fgetc(fd);
	} 
	if(c==EOF) exit(EXIT_FAILURE);
	
	while(c>='0'&& c<='9'){
		s[i] = c;
		i++;
		c = fgetc(fd);
	}
	ungetc(c,fd);
	s[i] = '\0';
	s = realloc(s,i+1);
	return s;

}

json_element_t* json_load(const char* fname){
	FILE* fichier = fopen(fname,"r");
	if(fichier==NULL) exit(EXIT_FAILURE);
	json_element_t* e = json_get(fichier);
	fclose(fichier);
	return e;

}

/*
int main() {
  json_element_t* e = NULL;
  e = json_null(); json_put(stdout, e); prl; json_free(e);
  e = json_boolean(true); json_put(stdout, e); prl; json_free(e);
  e = json_boolean(false); json_put(stdout, e); prl; json_free(e);
  e = json_integer(123456L); json_put(stdout, e); prl; json_free(e);
  e = json_double(123456.987654); json_put(stdout, e); prl; json_free(e);
  e = json_string("Un petit texte"); json_put(stdout, e); prl; json_free(e);
  e = json_array(); json_put(stdout, e); prl;
  json_array_set(e, 0, json_integer(1928374566L));
  json_array_set(e, 3, json_string("Un autre texte"));
  json_array_set(e, 1, json_boolean(false));
  json_put(stdout, e); prl; json_free(e);
  e = json_object(); json_put(stdout, e); prl; 
  json_object_set(e, "key1", json_integer(1928374566L));
  json_object_set(e, "key2", json_string("Un autre texte"));
  json_object_set(e, "key3", json_boolean(false));
  json_object_set(e, "key2", json_string("Un texte de remplacement"));
  json_put(stdout, e); prl; json_free(e);
 
  FILE* fd = fopen("example.json", "r");
  e = json_get(fd);
  fclose(fd);
  json_put(stdout, e); prl;
  json_free(e);
  return EXIT_SUCCESS;
}*/