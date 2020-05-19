#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scene3D.h"
#include "json.h"
#include "color.h"
#include "raytracer.h"
#include "bmp.h"
#include "menu.h"


bool cont = true;
void save(void** args){
	char* string = malloc(100);
	if(string==NULL) exit(1);
	printf("veuillez inserer une chaine de caractéres pour le fichier à enregistrer: ");
	scanf("%s",string);
	json_element_t* e; 
	e = scene3D_to_json(args[0]);
	json_save(string,e);
	json_free(e);
	free(string);

}

void open(void** args){
	char* string = malloc(100);
	if(string==NULL) exit(1);
	if(args[0]!=NULL){
		save(args);
		scene3D_free(args[0]);
	}
	printf("veuillez inserer une chaine de caractéres pour le fichier à ouvrir: ");
	scanf("%s",string);
	args[1] = (void*)string;
	json_element_t* e;
	e = json_load(string);
	scene3D_t* scene = scene3D_from_json(json_object_get(e,"scene3D"));
	camera_t* c = malloc(sizeof(camera_t));
	*c = camera_from_json(json_object_get(e,"camera"));
	args[0] = (void*)scene;
	
	args[2] = (void*)c;
}


void Information(void** args){
	if(args[0]!=NULL){
		printf("le nombre de primitives est :");
		unsigned int n_primitive = nmbprim(args[0]);
		printf("%d\n",(unsigned int)n_primitive);
		printf("\n");
		printf("l'ensemble des primitives est :");
		scene3D_primitives_print((scene3D_t*)args[0]);
		printf("le nombre de lumiéres est :");
		unsigned int n_light = nmblight(args[0]);
		printf("%d\n",(unsigned int)n_light);
		printf("l'ensemble des lumiéres est");
		scene3D_lights_print((scene3D_t*) args[0]);
		printf("le nom du fichier JSON d'origine est :");
		printf("%s",(char*)args[1]);
	}
}

void quit(void** args){
	if(args[0]!=NULL){
		save(args);
		cont = false;
	}
	cont = false;
	}

void Add_Sphere(void** args){
	if(args[0]!=NULL){

		double rayon;
		printf("veullez inserer la valeur du nouveau rayon: ");
		scanf("%lf",&rayon);
		double x; double y; double z;
		printf("veullez inserer les coordonées du point3D: ");
		scanf("%lf",&z);
		printf("veullez inserer les coordonées du point3D: ");
		scanf("%lf",&x);
		printf("veullez inserer les coordonées du point3D: ");
		scanf("%lf",&y);
		point3D_t point = point3D(x,y,z);;
		double R; double G; double B;
		printf("veullez inserer les coordonnées de la  couleur: ");
		scanf("%lf",&R);
		printf("veuillez inserer les coordonnées de la  couleur: ");	
		scanf("%lf",&G);
		printf("veuillez inserer les coordonnées de la  couleur: ");	
		scanf("%lf",&B);
		color_t c = color(R,G,B);
		scene3D_sphere_add((scene3D_t*)args[0],c,point,rayon);
		}
}

void Add_plane(void** args){
	if(args[0]!=NULL){
		double x; double y; double z;
		printf("veuillez inserer les coordonnées du vector3D: ");
		scanf("%lf",&x);
		printf("veuillez inserer les coordonnées du vector3D: ");
		scanf("%lf",&y);
		printf("veuillez inserer les coordonnées du vector3D: ");
		scanf("%lf",&z);
		vector3D_t vector = vector3D(x,y,z);
		double x1; double y1; double z1;
		printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&x1);
		printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&y1);
		printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&z1);
		point3D_t point = point3D(x1,y1,z1);
		double R; double G; double B;
		printf("veuillez inserer les coordonnées de la nouvelle couleur: ");
		scanf("%lf",&R);
		printf("veuillez inserer les coordonnées de la couleur: ");	
		scanf("%lf",&G);
		printf("veuillez inserer les coordonnées de la couleur: ");	
		scanf("%lf",&B);
		color_t c = color(R,G,B);
		scene3D_plane_add((scene3D_t*)args[0],c, point,vector);
	}
}

void Modify(void** args){
	if(args[0]!=NULL){
		scene3D_primitives_print((scene3D_t*)args[0]);
		unsigned int choix;
		scanf("%d",&choix);
		primitive_type_t type = scene3D_primitive_type((scene3D_t*) args[0],choix);
		if(type==SPHERE){
			double rayon;
			printf("veuillez inserer le rayon souhaiter: ");
			scanf("%lf",&rayon);
			double R; double G; double B;
			printf("veuillez inserer les coordonnées de la couleur : ");
			scanf("%lf",&R);
			printf("veuillez inserer les coordonnées de la couleur: ");	
			scanf("%lf",&G);
			printf("veuillez inserer les coordonnées de la couleur: ");	
			scanf("%lf",&B);
			color_t c = color(R,G,B);
			scene3D_sphere_set_radius((scene3D_t*)args[0], choix,rayon);
			scene3D_primitive_set_color((scene3D_t*)args[0],choix,c);
		}else if (type==PLANE){
			double x; double y; double z;
			printf("veuillez inserer les coordonnées du vecteur3D: ");
			scanf("%lf",&x);
			printf("veuillez inserer les coordonnées du vecteur3D: ");
			scanf("%lf",&y);
			printf("veuillez inserer les coordonnées du vecteur3D: ");
			scanf("%lf",&z);
			vector3D_t vector = vector3D(x,y,z);
			double R; double G; double B;
			printf("veuillez inserer les coordonnées de la couleur: ");
			scanf("%lf",&R);
			printf("veuillez inserer les coordonnées de la couleur: ");	
			scanf("%lf",&G);
			printf("veuillez inserer les coordonnées de la couleur: ");	
			scanf("%lf",&B);
			color_t c= color(R,G,B);
			scene3D_plane_set_normal((scene3D_t*)args[0], choix,vector);
			scene3D_primitive_set_color((scene3D_t*)args[0],choix,c);
		}
	}
}	

void Translate(void** args){
	if(args[0]!=NULL){
		scene3D_primitives_print((scene3D_t*)args[0]);
		unsigned int choix;
		printf("veuiller inserer votre choix: ");
		scanf("%u",&choix);
		double x; double y; double z;
		printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&x);
		printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&y);
		printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&z);
		vector3D_t vector = vector3D(x,y,z);
		scene3D_primitive_translate((scene3D_t*) args[0],choix,vector);
	}
}
void Delete(void** args){
	if(args[0]!=NULL){
		scene3D_primitives_print((scene3D_t*)args[0]);
		unsigned int choix;
		printf("veuillez inserer votre choix: ");
		scanf("%u",&choix);
		scene3D_primitive_delete((scene3D_t*)args[0],choix);

		}
}

//lights
void Ambient_Light(void** args){
	if(args[0]!=NULL){
		double R; double G; double B;
		printf("veuillez inserer les courdonnées de la couleur: ");
		scanf("%lf",&R);
		printf("veuillez inserer les courdonnées de la couleur: ");
		scanf("%lf",&G);
		printf("veuillez inserer les courdonnées de la couleur: ");
		scanf("%lf",&B);
		color_t c = color(R,G,B);
		scene3D_ambient_add((scene3D_t*) args[0],c);
	}
}

void Add_Unidirectional_Light(void** args){
	if(args[0]!=NULL){
		double x; double y; double z;
		printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&x);
		printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&y);
		printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&z);
		vector3D_t vector = vector3D(x,y,z);
		double R; double G; double B;
		printf("veuillez inserer les coordonnées de la couleur: ");
		scanf("%lf",&R);
		printf("veuillez inserer les coordonnées de la couleur: ");	
		scanf("%lf",&G);
		printf("veuillez inserer les coordonnées de la couleur: ");	
		scanf("%lf",&B);
		color_t c = color(R,G,B);
		scene3D_uni_add((scene3D_t*)args[0],c,vector);
	}
}

void Add_Omnidirectional_Light(void** args){
	if(args[0]!=NULL){
		double x1; double y1; double z1;
		printf("veuillez inserer les coordonnées du point3D: ");
		scanf("%lf",&x1);
		printf("veuillez inserer les coordonnées du point3D: ");
		scanf("%lf",&y1);
		printf("veuillez inserer les coordonnées du point3D: ");
		scanf("%lf",&z1);

		point3D_t point = point3D(x1,y1,z1);
		double R; double G; double B;
		printf("veuillez inserer les coordonnées de la couleur: ");
		scanf("%lf",&R);
		printf("veuillez inserer les coordonnées de la couleur: ");
		scanf("%lf",&G);
		printf("veuillez inserer les coordonnées de la couleur: ");
		scanf("%lf",&B);
		color_t c = color(R,G,B);
		scene3D_omni_add((scene3D_t*)args[0],c,point);
	}
}

void Modifyl(void** args){
	if(args[0]!=NULL){
		scene3D_lights_print((scene3D_t*) args[0]);
		unsigned int choix ;
		printf("veuillez inserer votre choix");
		scanf("%u",&choix);
		light_type_t type = scene3D_light_type((scene3D_t*) args[0], choix); 
		if(type==UNI){
			double R; double G; double B;
			printf("veuillez inserer les coordonnées de la couleur: ");
			scanf("%lf",&R);
			printf("veuillez inserer les coordonnées de la couleur: ");	
			scanf("%lf",&G);
			printf("veuillez inserer les coordonnées de la couleur: ");	
			scanf("%lf",&B);
			color_t c = color(R,G,B);
			double x; double y; double z;
			printf("veuillez inserer les coordonnées du vecteur3D: ");
			scanf("%lf",&x);
			printf("veuillez inserer les coordonnées du vecteur3D: ");
			scanf("%lf",&y);
			printf("veuillez inserer les coordonnées du vecteur3D: ");
			scanf("%lf",&z);
			vector3D_t vector = vector3D(x,y,z);
			scene3D_light_set_color((scene3D_t*)args[0],choix,c);
			scene3D_uni_set_direction((scene3D_t*) args[0],choix,vector);

		}
		if(type==OMNI){
			double R; double G; double B;
			printf("veuillez inserer les coordonnées de la couleur: ");
			scanf("%lf",&R);
			printf("veuillez inserer les coordonnées de la couleur: ");
			scanf("%lf",&G);
			printf("veuillez inserer les coordonnées de la couleur: ");
			scanf("%lf",&B);
			color_t c = color(R,G,B);
			double x1; double y1; double z1;
			printf("veuillez inserer les coordonnées du point3D: ");
			scanf("%lf",&x1);
			printf("veuillez inserer les coordonnées du point3D: ");
			scanf("%lf",&y1);
			printf("veuillez inserer les coordonnées du point3D: ");
			scanf("%lf",&z1);
			point3D_t point = point3D(x1,y1,z1);
			scene3D_light_set_color((scene3D_t*)args[0],choix,c);
			scene3D_omni_set_position((scene3D_t*) args[0],choix, point);

		}
		if(type==AMBIENT){
			printf("impossible de modifier");
			return;
		}
	}
}

void Deletel(void** args){
	if(args[0]!=NULL){
		scene3D_lights_print((scene3D_t*) args[0]);
		unsigned int choix;
		printf("veuillez inserer votre choix");
		scanf("%u",&choix);
		scene3D_light_delete((scene3D_t*)args[0],choix);
	}
}
//generation
void Export(void** args){
	if(args[0]!=NULL){
		char* string1= malloc(100);
		size_t depth;
		printf("veuillez inserer votre choix de fichier BMP");
		scanf("%s",string1);
		printf("La profondeur: \n");
		scanf("%ld",&depth);
		camera_t c;
		if(args[2]!=NULL){
	  		c = *(camera_t*)args[2];
	  	}
	  	scene3D_t* scene;
		scene = (scene3D_t*)args[0];
	  	if(scene!=NULL && string1!=NULL){
	  		camera_raytracing(string1, depth, c, scene);
	  	}
	  	free(string1);
	}
}



 void position(void** args){
 	if(args[2]!=NULL){
 		camera_t* c = args[2];
	 	double x; double y; double z;
	 	printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&x);
		printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&y);
		printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&z);
	 	point3D_t point = point3D(x,y,z);
	 	unsigned int choix;
	 	printf("veuillez inserer votre choix    1:une nouvelle position   2:la valeur par defaut: ");
	 	scanf("%u",&choix);
	 	if(choix==1){
	 		c->pos = point;
	 	}else if(choix==2){
	 		c->pos = point3D(0,0,0);
	 	}
 	}
}


 void direction(void** args){
 	if(args[2]!=NULL){
 		camera_t* c = args[2];
	 	double x; double y; double z;
		printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&x);
		printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&y);
		printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&z);
		vector3D_t vector = vector3D(x,y,z);
		unsigned int choix;
	 	printf("veuillez inserer votre choix	1:une nouvelle direction 	2:la valeur par defaut: ");
	 	scanf("%u",&choix);
	 	if(choix==1){
	 		c->dir = vector3D_normalize(vector);
	 	}else if(choix==2){
	 		c->dir = vector3D(0,0,-1); 
	 	}
 	}
}

 void Mire(void** args){
 	if(args[2]!=NULL){
 		camera_t* c = args[2];
	 	double x; double y; double z;
	 	printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&x);
		printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&y);
		printf("veuillez inserer les coordonnées du vecteur3D: ");
		scanf("%lf",&z);
	 	point3D_t point = point3D(x,y,z);
	 	c->dir = point3D_subtract(c->pos,point);
	 }
}

void Focal(void** args){
	if(args[2]!=NULL){
		camera_t* c = args[2];
		double new_focal;
		printf("veuillez introduire une nouvelle distance: ");
		scanf("%lf",&new_focal);
		if(new_focal>0){
			c->focal=new_focal;
		}else{
			c->focal = 1;
		}
	}
}
void Dimension(void** args){
	if(args[2]!=NULL){
		camera_t* c = args[2];
		size_t new_width;
		size_t new_height;
		printf("veuillez inserer les coordonnées de la nouvelle dimension: ");
		scanf("%ld",&new_width);
		scanf("%ld",&new_height);
		if(new_width>0 || new_height>0){
			c->width = new_width;
			c->height = new_height;
		}else{
			c->width = 800;
			c->height = 600;
		}
	}
}

void depth(void** args){
	if(args[3]!=NULL){
		size_t* depth = malloc(sizeof(size_t));
		printf("veuillez inserer une profondeur: ");
		scanf("%ld",depth);
		args[3] = (void*)depth;
	}
}


int main(){

	menu_t* menu;
	menu_t* smenu;
	menu = createMenu("main menu");

	smenu = createMenu("file");
	addMenuAction(smenu,"open",open);
	addMenuAction(smenu,"Save",save);
	addMenuAction(smenu,"Information",Information);
	addMenuAction(smenu,"Quitter",quit);
	addSubMenu(menu,smenu);

	smenu = createMenu("3D scene");
	menu_t* ssmenu1 = createMenu("Primitives");
	addMenuAction(ssmenu1,"Add Sphere",Add_Sphere);
	addMenuAction(ssmenu1,"Add Plane",Add_plane);
	addMenuAction(ssmenu1,"Modify", Modify);
	addMenuAction(ssmenu1,"Translate",Translate);
	addMenuAction(ssmenu1,"Delete",Delete);
	addSubMenu(smenu,ssmenu1);

	 ssmenu1 = createMenu("Lights");
	addMenuAction(ssmenu1,"Ambient Light",Ambient_Light);
	addMenuAction(ssmenu1,"Add Unidirectional Light",Add_Unidirectional_Light);
	addMenuAction(ssmenu1,"Add Omnidirectional Light",Add_Omnidirectional_Light);
	addMenuAction(ssmenu1,"Modify", Modify);
	addMenuAction(ssmenu1,"Delete",Delete);
	addSubMenu(smenu,ssmenu1);
	addSubMenu(menu,smenu);

	smenu = createMenu("Generation");
	addMenuAction(smenu,"Export",Export);
	ssmenu1 = createMenu("Camera");
	addMenuAction(ssmenu1,"position",position);
	addMenuAction(ssmenu1,"Direction",direction);
	addMenuAction(ssmenu1,"Mire", Mire);
	addSubMenu(smenu,ssmenu1);

	ssmenu1 = createMenu("Image");
	addMenuAction(ssmenu1,"Focal",Focal);
	addMenuAction(ssmenu1,"Dimension",Dimension);
	addMenuAction(ssmenu1,"Depth", depth);
	//addMenuAction(ssmenu1,"Anti-aliasing",f);
	addSubMenu(smenu,ssmenu1);
	addSubMenu(menu,smenu);

	addMenuAction(menu, "Quitter", quit);

	void* args[4];
	args[0] = (camera_t*) NULL;
	args[1] = (scene3D_t*) NULL;
	args[2] = (size_t*) NULL;
	args[3] = (char*) NULL;

	while(cont) launchMenu(menu, args);
	deleteMenu(menu);
	free(args[0]);
	free(args[1]);
	free(args[2]);
	free(args[3]);

	return 0;
}