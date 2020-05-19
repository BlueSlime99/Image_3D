#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "menu.h"

size_t strlen(const char *s);
char* strcpy(char *dest, const char *src);




typedef struct{
	char* descriptifAction;
	void (*f) (void** args);
}action_t;

typedef union {
	menu_t* sousmenus;
	action_t actions;
}Item_t;

typedef enum{
	Sousmenus = 1,
	Actions
}choixitem_t;

typedef struct{
	choixitem_t Choix_items;
	Item_t Item;
}item_t;


struct menu_s{
	menu_t* menuparent;
	char* descriptifMenu;
	item_t items[9];
};



menu_t* createMenu(const char* text){
	menu_t* Menu = malloc(sizeof(menu_t));
	if(Menu==NULL){
			exit(EXIT_FAILURE);
		}
	else{
		Menu->menuparent = NULL;
		char* copie = malloc(strlen(text)+1*sizeof(char));
		if(copie == NULL) exit(EXIT_FAILURE);
		copie = strcpy(copie ,text);
		Menu->descriptifMenu = malloc(strlen(text)+1*sizeof(char));
		if(Menu->descriptifMenu == NULL) exit(EXIT_FAILURE);
		Menu->descriptifMenu = copie;
	}
	return Menu;

}
void addMenuAction(menu_t* m, const char* text, void(*f)(void** args)){
	if(m!=NULL){
	for(int i=0;i<9;i++){
		if(m->items[i].Choix_items!=Sousmenus && m->items[i].Choix_items!=Actions){
			m->items[i].Choix_items = Actions;
			m->items[i].Item.actions.f = f;
			char* copie = malloc(strlen(text)+1*sizeof(char));
			if(copie == NULL) exit(EXIT_FAILURE);
			copie = strcpy(copie ,text);
			m->items[i].Item.actions.descriptifAction =  malloc(strlen(text)+1*sizeof(char));
			if( m->items[i].Item.actions.descriptifAction== NULL) exit(EXIT_FAILURE);
			m->items[i].Item.actions.descriptifAction = copie;	
			break;
		}
	}
}
}

void addSubMenu(menu_t* m, menu_t* sm){
	if(m!=NULL){
		if(sm->menuparent==NULL){
			for(int i=0;i<9;i++){
				if(m->items[i].Choix_items!=Sousmenus && m->items[i].Choix_items!=Actions){
					m->items[i].Choix_items = Sousmenus;
					m->items[i].Item.sousmenus = sm;
					break;
				}
			sm->menuparent =m;
		}
	}
}
}

void deleteMenu(menu_t* m){
	if(m!=NULL && m->menuparent==NULL){
		for(int i=0;i<9;i++){
			if(m->items[i].Choix_items == Actions){
				free(m->items[i].Item.actions.descriptifAction);
			}else if(m->items[i].Choix_items == Sousmenus){
				m->items[i].Item.sousmenus->menuparent = NULL;
				deleteMenu(m->items[i].Item.sousmenus);
			}
		}
		free(m->descriptifMenu);
		free(m);
	}
}


 void launchMenu(menu_t* m, void** args){
 	printf("%s\n\n", m->descriptifMenu);
 	for(int i=0;i<9;i++){
 		if(m->items[i].Choix_items == Actions){
 			printf("%d: %s \n\n", i+1, m->items[i].Item.actions.descriptifAction);
 		}else if(m->items[i].Choix_items == Sousmenus){
 			printf("%d: %s \n\n", i+1, m->items[i].Item.sousmenus->descriptifMenu);
 		}
 	}

 	char carac=0;
 	printf("Choose a number: ");
 	do { scanf("%c",&carac); } while(carac == '\n');
 	if(carac >= '1' && carac <= '9'){
 		if(m->items[carac-'1'].Choix_items == Actions){
 			m->items[carac-'1'].Item.actions.f(args);
 		}else if(m->items[carac-'1'].Choix_items == Sousmenus){
 			system("clear");
 			launchMenu(m->items[carac-'1'].Item.sousmenus,args);
 		}else {
 			system("clear");
 			printf("Please enter a valid number: \n\n\n");
 			launchMenu(m,args);
 		}
 	}else{
 		 if(carac == 'p'){
 			if(m->menuparent!=NULL) launchMenu(m->menuparent,args);
 		}else {
 			system("clear");
 			printf("Please enter a valid number: \n\n\n");
 			launchMenu(m,args);
 		}

 	}

}


/*
void f1() {
  printf("Functionality 1 is called\n");
}
 
void f2() {
  printf("Functionality 2 is called\n");
}
 
bool cont = true;
 
void quit() {
  cont = false;
}
 
int main() {
  menu_t* m;
  menu_t* sm;
 
  m = createMenu("Main menu");
 
  addMenuAction(m,"Apply functionnality 1",f1);
  addMenuAction(m,"Apply functionnality 2",f2);
 
  sm = createMenu("First submenu");
  addMenuAction(sm,"Apply functionnality 2",f2);
  addSubMenu(m,sm);
 
  sm = createMenu("Second submenu");
  addMenuAction(sm,"Apply functionnality 1",f1);
  addSubMenu(m,sm);
 
  addMenuAction(m,"Quit",quit);
 
  while(cont) launchMenu(m);
 
  deleteMenu(m);
 
  return EXIT_SUCCESS;
}
*/
