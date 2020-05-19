#ifndef __MENU__
#define __MENU__

#include <stdlib.h>
typedef struct menu_s menu_t;

void* myalloc(size_t sz);
menu_t* createMenu(const char* text);
void addMenuAction(menu_t* m, const char* text, void(*f)());
void addSubMenu(menu_t* m, menu_t* sm);
void deleteMenu(menu_t* m);
 void launchMenu(menu_t* m, void** args);


#endif