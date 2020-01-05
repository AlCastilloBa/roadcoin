//Using SDL and standard IO 
#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h> 
#include <stdbool.h>


#include "game.h"
//#include "physics.h"
//#include "camera.h"
//#include "maps.h"
//#include "geometry.h"
#include "options.h"
#include "menu.h"
#include "graphics.h"


// Si definido DEBUG_INFO, mostrar textos de informacion por la terminal (hace el programa más lento)
//#define DEBUG_INFO


#define PORCENTAJE_ANCHO_TITULOS 70
#define PORCENTAJE_ALTO_TITULOS 16
#define PORCENTAJE_ANCHO_BOTONES 70
#define PORCENTAJE_MARGEN_SUPERIOR 30
//#define PORCENTAJE_MARGEN_INFERIOR 20
#define PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES 10
#define PORCENTAJE_ALTO_BOTON 8
//#define PORCENTAJE_ANCHO_CARACTER_MENU 3


/////////////////////////////////////////////////////////////////////////
// Variables globales
//The window renderer 
extern SDL_Renderer* gRenderer;				// Definido en "game.c"
// Variables globales opciones
extern struct opciones opciones_juego;			// Definido en "game.c"
// Variables globales del menu
extern struct pantalla_menu* pantallas_menu_principal;	// Definido en "game.c"
extern int numero_pantallas;

extern SDL_Window* gGameWindow; 


TTF_Font* gFuenteTexto = NULL;
SDL_Color gColorTextoReposo = { 255 , 255 , 255 }; 	// Blanco --- White
SDL_Color gColorTextoSeleccionado = { 255 , 255 , 0 }; 	// Amarillo --- Yellow

SDL_Texture* gWhiteCheckBoxChecked = NULL;
SDL_Texture* gWhiteCheckBoxUnchecked = NULL;


bool raton_sobre_boton (struct boton_menu boton_consultado, int pos_raton_x, int pos_raton_y )
{
	// Esta función indica si el ratón está posicionado sobre un botón
	// This function tells whether or not the mouse is over a button
	if ( 	(pos_raton_x >= boton_consultado.borde_izquierdo) && 
		(pos_raton_x <= boton_consultado.borde_derecho  ) &&
		(pos_raton_y >= boton_consultado.borde_arriba   ) &&
		(pos_raton_y <= boton_consultado.borde_abajo    ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool inicializa_datos_boton( struct boton_menu* boton, int indice_boton, enum listado_botones ident_boton, enum tipo_boton tipo, char* texto_boton, int boton_anterior, int boton_siguiente  )
{
	// La funcion "inicializar_menu_principal" es muy extensa, con código muy parecido. Se crea esta función para intentar reducir la extension del código.
	// Function "inicializar_menu_principal" (initialize main menu) is too long. This function is created in an attempt to reduce code lenght.

	bool exito = true;

	boton->identificador_boton = ident_boton;
	boton->clase_boton = tipo;
	strcpy( boton->texto, texto_boton );
	boton->borde_izquierdo = (opciones_juego.screen_x_resolution)*(100-PORCENTAJE_ANCHO_BOTONES)/2/100;
	boton->borde_derecho = (opciones_juego.screen_x_resolution)*(100+PORCENTAJE_ANCHO_BOTONES)/2/100;
	boton->borde_arriba = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + indice_boton*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES)/100;
	boton->borde_abajo = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + indice_boton*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES + PORCENTAJE_ALTO_BOTON)/100;;
	boton->textura_boton_reposo = RenderizaTextoEnTextura( 	boton->texto, 
								gFuenteTexto, 
								gColorTextoReposo, 
								&(boton->textura_dim_x), 
								&(boton->textura_dim_y)   );
	if( boton->textura_boton_reposo == NULL ) 
	{
		printf( "Unable to render text surface! SDL_ttf\n" ); 
		exito = false;
	}
	boton->textura_boton_seleccionado = RenderizaTextoEnTextura( 	boton->texto, 
									gFuenteTexto, 
									gColorTextoSeleccionado, 
									NULL, 
									NULL   );
	if( boton->textura_boton_seleccionado == NULL ) 
	{
		printf( "Unable to render text surface! SDL_ttf\n" ); 
		exito = false;
	}
	boton->boton_anterior = boton_anterior;
	boton->boton_siguiente = boton_siguiente;

	return exito;
}

bool inicializa_datos_pantalla_menu( struct pantalla_menu* pantalla, enum pantalla_menu_principal numero_pantalla, char* titulo_pantalla, char* ruta_textura_fondo, int numero_botones)
{
	// La funcion "inicializar_menu_principal" es muy extensa, con código muy parecido. Se crea esta función para intentar reducir la extension del código.
	// Function "inicializar_menu_principal" (initialize main menu) is too long. This function is created in an attempt to reduce code lenght.

	bool exito = true;

	/* pantallas_menu_principal[menu_opciones].identificador_pantalla*/ pantalla->identificador_pantalla = numero_pantalla;
	strcpy( pantalla->titulo, titulo_pantalla );
	pantalla->textura_titulo = RenderizaTextoEnTextura( pantalla->titulo, 
								gFuenteTexto, 
								gColorTextoReposo, 
								/*&(pantallas_menu_principal[menu_principal].botones_pantalla[0].textura_dim_x)*/ NULL, 
								/*&(pantallas_menu_principal[menu_principal].botones_pantalla[0].textura_dim_y)*/ NULL   );
	pantalla->borde_izquierdo_titulo = (opciones_juego.screen_x_resolution)*(100-PORCENTAJE_ANCHO_TITULOS)/2/100;
	pantalla->borde_derecho_titulo   = (opciones_juego.screen_x_resolution)*(100+PORCENTAJE_ANCHO_TITULOS)/2/100;
	pantalla->borde_arriba_titulo    = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + (-2)*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES)/100;
	pantalla->borde_abajo_titulo     = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + (-2)*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES + PORCENTAJE_ALTO_TITULOS)/100;

	pantalla->imagen_fondo_pantalla = CargaTextura( ruta_textura_fondo , NULL, NULL, false );
	if( pantalla->imagen_fondo_pantalla == NULL ) 
	{
		printf( "Error al cargar textura fondo.\n" ); 
		exito = false;
	}
	pantalla->numero_botones = numero_botones;
	#ifdef DEBUG_INFO
	printf("Reservando memoria dinámica para los botones de pantalla...\n");
	#endif
	if ( numero_botones > 0 )
	{
		pantalla->botones_pantalla = calloc( pantalla->numero_botones, sizeof( struct boton_menu )  );
		if ( pantalla->botones_pantalla == NULL )
		{
			printf( "Error al reservar memoria dinámica para los botones.\n" ); 
			exito = false;
		}
	}

	return exito;
}



bool inicializar_menu_principal(/* struct pantalla_menu* pantallas_menu_principal, */int numero_pantallas )
{
	// Esta funcion inicializa los valores de las estructuras de las imagenes del menu.
	// Reserva la memoria para el menu, inicializa variables, e inicializa graficos
	// (TODO)
	bool success = true;

	#ifdef DEBUG_INFO
	printf("Cargando texturas generales del menu...\n");
	#endif
	gWhiteCheckBoxChecked = CargaTextura( "images/white_checkbox_check.png", NULL, NULL, true );
	gWhiteCheckBoxUnchecked = CargaTextura( "images/white_checkbox_uncheck.png", NULL, NULL, true );
	if( gWhiteCheckBoxChecked == NULL || gWhiteCheckBoxUnchecked == NULL ) 
	{ 
		printf( "Error al cargar texturas checkbox.\n" ); 
		success = false; 
	}

	// Cargar la fuente del menu
	#ifdef DEBUG_INFO
	printf("Inicializando la fuente del menu...\n");
	#endif

	gFuenteTexto = TTF_OpenFont( "fonts/lunasol.ttf", 48 );
	//gFuenteTexto = TTF_OpenFont( "fonts/ethnocen.ttf" , 48);
	//gFuenteTexto = TTF_OpenFont( "fonts/zorque.ttf", 48 );

	if( gFuenteTexto == NULL )
	{
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}

	#ifdef DEBUG_INFO
	printf("Reservando memoria dinámica para las pantallas del menu...\n");
	#endif

	pantallas_menu_principal = calloc( numero_pantallas, sizeof( struct pantalla_menu ) );

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla bienvenida
	#ifdef DEBUG_INFO
	printf("Inicializando datos de pantalla bienvenida...\n");
	#endif
	pantallas_menu_principal[menu_bienvenida].identificador_pantalla = menu_bienvenida;
	strcpy( pantallas_menu_principal[menu_bienvenida].titulo, "Pulse cualquier tecla\0" );
	pantallas_menu_principal[menu_bienvenida].textura_titulo = RenderizaTextoEnTextura( 	pantallas_menu_principal[menu_bienvenida].titulo, 
														gFuenteTexto, 
														gColorTextoReposo, 
														/*&(pantallas_menu_principal[menu_principal].botones_pantalla[0].textura_dim_x)*/ NULL, 
														/*&(pantallas_menu_principal[menu_principal].botones_pantalla[0].textura_dim_y)*/ NULL   );
	pantallas_menu_principal[menu_bienvenida].borde_izquierdo_titulo = (opciones_juego.screen_x_resolution)*(100-PORCENTAJE_ANCHO_TITULOS)/2/100;
	pantallas_menu_principal[menu_bienvenida].borde_derecho_titulo   = (opciones_juego.screen_x_resolution)*(100+PORCENTAJE_ANCHO_TITULOS)/2/100;
	pantallas_menu_principal[menu_bienvenida].borde_arriba_titulo    = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + (-2)*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES)/100;
	pantallas_menu_principal[menu_bienvenida].borde_abajo_titulo	 = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + (-2)*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES + PORCENTAJE_ALTO_TITULOS)/100;					

	pantallas_menu_principal[menu_bienvenida].imagen_fondo_pantalla = CargaTextura( "images/2068223.jpg" , NULL, NULL, false );
	if( pantallas_menu_principal[menu_bienvenida].imagen_fondo_pantalla == NULL ) 
	{ 
		printf( "Error al cargar textura fondo menu bienvenida.\n" ); 
		success = false; 
	}
	pantallas_menu_principal[menu_bienvenida].numero_botones = 0;			
	pantallas_menu_principal[menu_bienvenida].botones_pantalla = NULL;	// Sin botones --- NO BUTTONS


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla menu_principal
	#ifdef DEBUG_INFO
	printf("Inicializando datos de pantalla menu principal...\n");
	#endif
	if( !inicializa_datos_pantalla_menu( &(pantallas_menu_principal[menu_principal]), menu_principal, "Menu principal\0" ,  "images/gradient-abstract.png", 4 ) )
	{
		printf( "Error al inicializar pantalla menu principal.\n" ); 
		success = false;
	}

	// Lo siguiente se ha dejado como ejemplo de lo que hace la función anterior --- The following remark is left as an example of what the previous function does
/*	pantallas_menu_principal[menu_principal].identificador_pantalla = menu_principal;
	strcpy( pantallas_menu_principal[menu_principal].titulo, "    Menu principal    \0" );
	pantallas_menu_principal[menu_principal].textura_titulo = RenderizaTextoEnTextura( 	pantallas_menu_principal[menu_principal].titulo, 
														gFuenteTexto, 
														gColorTextoReposo, 
														/*&(pantallas_menu_principal[menu_principal].botones_pantalla[0].textura_dim_x)*/// NULL, 
														/*&(pantallas_menu_principal[menu_principal].botones_pantalla[0].textura_dim_y)*/// NULL   );
/*	pantallas_menu_principal[menu_principal].borde_izquierdo_titulo = (opciones_juego.screen_x_resolution)*(100-PORCENTAJE_ANCHO_TITULOS)/2/100;
	pantallas_menu_principal[menu_principal].borde_derecho_titulo   = (opciones_juego.screen_x_resolution)*(100+PORCENTAJE_ANCHO_TITULOS)/2/100;
	pantallas_menu_principal[menu_principal].borde_arriba_titulo    = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + (-2)*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES)/100;
	pantallas_menu_principal[menu_principal].borde_abajo_titulo	 = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + (-2)*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES + PORCENTAJE_ALTO_TITULOS)/100;

	pantallas_menu_principal[menu_principal].imagen_fondo_pantalla = CargaTextura( "images/gradient-abstract.png" , NULL, NULL, false );
	if( pantallas_menu_principal[menu_principal].imagen_fondo_pantalla == NULL ) 
	{
		printf( "Error al cargar textura fondo menu principal.\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_principal].numero_botones = 4;
	#ifdef DEBUG_INFO
	printf("Reservando memoria dinámica para los botones de pantalla menu principal...\n");
	#endif
	pantallas_menu_principal[menu_principal].botones_pantalla = calloc( pantallas_menu_principal[menu_principal].numero_botones, sizeof( struct boton_menu )  );
*/
	//----------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 0 de pantalla menu principal...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_principal].botones_pantalla[0]) , 0, boton_jugar, boton_pulsar, "     Jugar      \0" , 3, 1  ) )
	{
		printf( "Error al inicializar boton 0 en menu principal.\n" ); 
		success = false;
	}
	// Lo siguiente se ha dejado como ejemplo de lo que hace la función anterior --- The following remark is left as an example of what the previous function does
/*	pantallas_menu_principal[menu_principal].botones_pantalla[0].identificador_boton = boton_jugar;
	pantallas_menu_principal[menu_principal].botones_pantalla[0].clase_boton = boton_pulsar;
	strcpy( pantallas_menu_principal[menu_principal].botones_pantalla[0].texto, " Jugar  \0" );
	pantallas_menu_principal[menu_principal].botones_pantalla[0].borde_izquierdo = (opciones_juego.screen_x_resolution)*(100-PORCENTAJE_ANCHO_BOTONES)/2/100;
	pantallas_menu_principal[menu_principal].botones_pantalla[0].borde_derecho = (opciones_juego.screen_x_resolution)*(100+PORCENTAJE_ANCHO_BOTONES)/2/100;
	pantallas_menu_principal[menu_principal].botones_pantalla[0].borde_arriba = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + 0*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES)/100;
	pantallas_menu_principal[menu_principal].botones_pantalla[0].borde_abajo = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + 0*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES + PORCENTAJE_ALTO_BOTON)/100;;
	pantallas_menu_principal[menu_principal].botones_pantalla[0].textura_boton_reposo = RenderizaTextoEnTextura( 	pantallas_menu_principal[menu_principal].botones_pantalla[0].texto, 
														gFuenteTexto, 
														gColorTextoReposo, 
														&(pantallas_menu_principal[menu_principal].botones_pantalla[0].textura_dim_x), 
														&(pantallas_menu_principal[menu_principal].botones_pantalla[0].textura_dim_y)   );
	if( pantallas_menu_principal[menu_principal].botones_pantalla[0].textura_boton_reposo == NULL ) 
	{
		printf( "Unable to render text surface! SDL_ttf\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_principal].botones_pantalla[0].textura_boton_seleccionado = RenderizaTextoEnTextura( 	pantallas_menu_principal[menu_principal].botones_pantalla[0].texto, 
														gFuenteTexto, 
														gColorTextoSeleccionado, 
														NULL, 
														NULL   );
	if( pantallas_menu_principal[menu_principal].botones_pantalla[0].textura_boton_seleccionado == NULL ) 
	{
		printf( "Unable to render text surface! SDL_ttf\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_principal].botones_pantalla[0].boton_anterior = 3;
	pantallas_menu_principal[menu_principal].botones_pantalla[0].boton_siguiente = 1;*/
	//--------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 1 de pantalla menu principal...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_principal].botones_pantalla[1]) , 1, boton_opciones, boton_pulsar, "    Opciones    \0" , 0, 2  ) )
	{
		printf( "Error al inicializar boton 1 en menu principal.\n" ); 
		success = false;
	}
	//------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 2 de pantalla menu principal...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_principal].botones_pantalla[2]) , 2, boton_creditos, boton_pulsar, "    Creditos    \0" , 1, 3  ) )
	{
		printf( "Error al inicializar boton 2 en menu principal.\n" ); 
		success = false;
	}
	//-------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 3 de pantalla menu principal...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_principal].botones_pantalla[3]) , 3, boton_salir, boton_pulsar, "     Salir      \0" , 2, 0  ) )
	{
		printf( "Error al inicializar boton 2 en menu principal.\n" ); 
		success = false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla opciones
	#ifdef DEBUG_INFO
	printf("Inicializando datos de pantalla opciones...\n");
	#endif
	if( !inicializa_datos_pantalla_menu( &(pantallas_menu_principal[menu_opciones]), menu_opciones, "Menu opciones\0", "images/blue_pink_gradient.png", 6 ) )
	{
		printf( "Error al inicializar pantalla opciones.\n" ); 
		success = false;
	}
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 0 de pantalla menu opciones...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opciones].botones_pantalla[0]) , 0, boton_video, boton_pulsar, "     Video      \0" , 5, 1  ) )
	{
		printf( "Error al inicializar boton 0 en menu opciones.\n" ); 
		success = false;
	}
	//-----------------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 1 de pantalla menu opciones...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opciones].botones_pantalla[1]) , 1, boton_sonido, boton_pulsar, "     Sonido     \0" , 0, 2  ) )
	{
		printf( "Error al inicializar boton 1 en menu opciones.\n" ); 
		success = false;
	}
	//-----------------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 2 de pantalla menu opciones...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opciones].botones_pantalla[2]) , 2, boton_controles, boton_pulsar, "    Controles    \0" , 1, 3  ) )
	{
		printf( "Error al inicializar boton 2 en menu opciones.\n" ); 
		success = false;
	}
	//-----------------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 3 de pantalla menu opciones...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opciones].botones_pantalla[3]) , 3, boton_opc_juego, boton_pulsar, "     Juego      \0" , 2, 4  ) )
	{
		printf( "Error al inicializar boton 3 en menu opciones.\n" ); 
		success = false;
	}
	//-----------------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 3 de pantalla menu opciones...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opciones].botones_pantalla[3]) , 3, boton_opc_juego, boton_pulsar, "     Juego      \0" , 2, 4  ) )
	{
		printf( "Error al inicializar boton 3 en menu opciones.\n" ); 
		success = false;
	}
	//-----------------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 4 de pantalla menu opciones...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opciones].botones_pantalla[4]) , 4, boton_idioma, boton_pulsar, "     Idioma     \0" , 3, 5  ) )
	{
		printf( "Error al inicializar boton 4 en menu opciones.\n" ); 
		success = false;
	}
	//-----------------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 5 de pantalla menu opciones...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opciones].botones_pantalla[5]) , 5, boton_opc_atras, boton_pulsar, "     Atras      \0" , 4, 0  ) )
	{
		printf( "Error al inicializar boton 5 en menu opciones.\n" ); 
		success = false;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla opciones de video
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla opciones
	#ifdef DEBUG_INFO
	printf("Inicializando datos de pantalla opciones de video...\n");
	#endif
	if( !inicializa_datos_pantalla_menu( &(pantallas_menu_principal[menu_opc_video]), menu_opc_video, "Opciones video\0", "images/muticolor_gradient.jpg", 4 ) )
	{
		printf( "Error al inicializar pantalla opciones de video.\n" ); 
		success = false;
	}
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 0 de pantalla menu opciones de video...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opc_video].botones_pantalla[0]) , 0, boton_fullscreen, boton_conmutar, "Pantalla completa\0" , 1, 2  ) )
	{
		printf( "Error al inicializar boton 0 en menu opciones de video.\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_opc_video].botones_pantalla[0].estado_on_off = opciones_juego.fullscreen;
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 1 de pantalla menu opciones de video...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opc_video].botones_pantalla[1]) , 1, boton_wireframe, boton_conmutar, "Dibujar alambres \0" , 0, 2  ) )
	{
		printf( "Error al inicializar boton 1 en menu opciones de video.\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_opc_video].botones_pantalla[1].estado_on_off = opciones_juego.wireframe;
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 2 de pantalla menu opciones de video...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opc_video].botones_pantalla[2]) , 2, boton_textured_objects, boton_conmutar, "Objetos texturas  \0" , 1, 3  ) )
	{
		printf( "Error al inicializar boton 2 en menu opciones de video.\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_opc_video].botones_pantalla[2].estado_on_off = opciones_juego.textured_objects;
	//----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 3 de pantalla menu opciones de video...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opc_video].botones_pantalla[3]) , 3, boton_opcvid_atras, boton_pulsar, "      Atras      \0" , 2, 0  ) )
	{
		printf( "Error al inicializar boton 2 en menu opciones de video.\n" ); 
		success = false;
	}

	return success;
}


bool liberar_memoria_menu_principal( /*struct pantalla_menu* pantallas_menu_principal, */ int numero_pantallas ) 
{
	#ifdef DEBUG_INFO
	printf("Liberando memoria del menu...\n");
	#endif 

	// Recorrer con un bucle for liberando la memoria de los botones
	int menu_actual, boton_actual;
	for ( menu_actual=0 ; menu_actual < numero_pantallas ; menu_actual++ )
	{
		for ( boton_actual=0 ; boton_actual < pantallas_menu_principal[menu_actual].numero_botones ;  boton_actual++ )
		{
			//Free loaded image 
			SDL_DestroyTexture( pantallas_menu_principal[menu_actual].botones_pantalla[boton_actual].textura_boton_reposo ); 
			pantallas_menu_principal[menu_actual].botones_pantalla[boton_actual].textura_boton_reposo = NULL; 
			SDL_DestroyTexture( pantallas_menu_principal[menu_actual].botones_pantalla[boton_actual].textura_boton_seleccionado ); 
			pantallas_menu_principal[menu_actual].botones_pantalla[boton_actual].textura_boton_seleccionado = NULL; 
		}
		//Free loaded image 
		SDL_DestroyTexture( pantallas_menu_principal[menu_actual].textura_titulo ); 
		pantallas_menu_principal[menu_actual].textura_titulo = NULL; 

		free( pantallas_menu_principal->botones_pantalla );
	}
	free ( pantallas_menu_principal );

	SDL_DestroyTexture(gWhiteCheckBoxChecked); gWhiteCheckBoxChecked = NULL;
	SDL_DestroyTexture(gWhiteCheckBoxChecked); gWhiteCheckBoxChecked = NULL;

}

void bucle_principal_menu_principal( void )
{


	bool quit = false;	 //Main loop flag
	int contador_frames = 0;
	SDL_Event e;		 //Event handler 
	Uint32 currentTime = 0, lastTime, deltaTime, tiempo_imagen_sobrante; //Current time start time (milisegundos)
	float tiempo_imagen;	//Tiempo de cada imagen, en segundos
	float framerate_deseado = 30;


	enum pantalla_menu_principal menu_activo;	// Menu activo actualmente y representado en la pantalla --- Currently active menu, displayed on screen
	int boton_seleccionado, i;
	bool algun_boton_seleccionado, boton_pulsado;

	/* Movido a variables globales
	struct pantalla_menu* pantallas_menu_principal;
	int numero_pantallas = 6;*/

	int pos_raton_x, pos_raton_y;

	SDL_Rect renderQuad;		// Variable "rectangulo SDL" para indicar donde renderizar los objetos





	//Activar variables ratón
	SDL_ShowCursor(SDL_ENABLE);
	SDL_SetRelativeMouseMode(SDL_FALSE);

	menu_activo = menu_bienvenida;
	boton_seleccionado = 0;

	//While application is running 
	while( !quit ) 
	{
		// Calcular tiempo para el movimiento
		contador_frames++;
		lastTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = currentTime - lastTime;
		if (deltaTime > 10 )
		{		
			tiempo_imagen = deltaTime/1000.f;
		}		
		else
		{	
			tiempo_imagen = 1/framerate_deseado;		// CHAPUZA, HACER MEJOR
		}
		#ifdef DEBUG_INFO
		printf("Frame %d, time %d, FPS=%f \n", contador_frames, deltaTime, 1/tiempo_imagen);
		#endif

		//////////////////////////////////////////////////////////////////////////////////////
		//Gestión de entradas (teclado, ratón, joystick) --- Inputs management (keyboard, mouse, joystick)
		boton_pulsado = false;
		//Handle events on queue 
		while( SDL_PollEvent( &e ) != 0 ) 
		{ 
			//User requests quit 
			if( e.type == SDL_QUIT ) 
			{ 
				quit = true;
			}
			//User presses a key 
			else if( e.type == SDL_KEYDOWN ) 
			{ 
				//Select surfaces based on key press 
				switch( e.key.keysym.sym ) 
				{ 
					case SDLK_ESCAPE: 
						#ifdef DEBUG_INFO
						printf("El usuario ha pusado ESC.\n");
						#endif
						quit = true; 
						break; 
					case SDLK_LEFT:
						// (TODO) Techa izquierda
						break;
					case SDLK_RIGHT:
						// (TODO) Tecla derecha
						break;
					case SDLK_UP:
						// Techa arriba
						if ( pantallas_menu_principal[menu_activo].numero_botones != 0)		// Para evitar violacion de segmento si el numero de botones es cero
						{
							boton_seleccionado = pantallas_menu_principal[menu_activo].botones_pantalla[boton_seleccionado].boton_anterior;
						}
						break;
					case SDLK_DOWN:
						// Tecla abajo
						if ( pantallas_menu_principal[menu_activo].numero_botones != 0)		// Para evitar violacion de segmento si el numero de botones es cero
						{
							boton_seleccionado = pantallas_menu_principal[menu_activo].botones_pantalla[boton_seleccionado].boton_siguiente;
						}
						break;
					case SDLK_RETURN:
					case SDLK_KP_ENTER:
					case SDLK_SPACE:
						// Tecla enter o espacio
						boton_pulsado = true;
						break;
					case SDLK_f:
						// Tecla F: Pantalla completa --- Toggle fullscreen
						CambiarModoPantallaCompleta ( !opciones_juego.fullscreen , gGameWindow );
						opciones_juego.fullscreen = !opciones_juego.fullscreen;
						pantallas_menu_principal[menu_opc_video].botones_pantalla[0].estado_on_off = opciones_juego.fullscreen;
						break;
					default:  
						break; 
				}
				if ( menu_activo == menu_bienvenida )
				{
					// Se ha pulsado cualquier tecla en el menu bienvenida
					menu_activo = menu_principal;
				}
			}
			else if ( e.type  == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP )
			{
				SDL_GetMouseState( &pos_raton_x, &pos_raton_y );	// Guardamos posicion actual del ratón  --- Store current mouse position
				// Comprobamos si el raton está sobre algún boton --- Check if mouse is over any button
				algun_boton_seleccionado = false;
				for ( i = 0 ; i<= pantallas_menu_principal[menu_activo].numero_botones -1 ; i++ )
				{
					if (  raton_sobre_boton (  pantallas_menu_principal[menu_activo].botones_pantalla[i]   , pos_raton_x, pos_raton_y ) )
					{
						boton_seleccionado = i;
						algun_boton_seleccionado = true;
					}
				}
				if ( algun_boton_seleccionado == false )
				{
					boton_seleccionado = -1;
				}
				if (e.type == SDL_MOUSEBUTTONDOWN && algun_boton_seleccionado == true )
				{
					boton_pulsado = true;
				}
				if ( e.type == SDL_MOUSEBUTTONDOWN && menu_activo == menu_bienvenida )
				{
					// Se ha pulsado el raton en el menu bienvenida
					menu_activo = menu_principal;
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////
		// Gestiona las acciones del usuario --- Manage user actions
		if (( boton_pulsado == true) && (boton_seleccionado != -1) && (boton_seleccionado >= 0) && (boton_seleccionado <= pantallas_menu_principal[menu_activo].numero_botones) )
		{
			// Se ha pulsado algo, hay que hacer lo que corresponda
			switch ( pantallas_menu_principal[menu_activo].botones_pantalla[boton_seleccionado].identificador_boton )
			{

				case boton_jugar:
					#ifdef DEBUG_INFO
					printf("Comenzando bucle principal del juego...\n");
					#endif
					bucle_principal_juego();
					//Activar variables ratón una vez el juego ha terminado
					SDL_ShowCursor(SDL_ENABLE);
					SDL_SetRelativeMouseMode(SDL_FALSE);
					break;
				case boton_salir:
					quit = true;
					break;
				case boton_opciones:
				case boton_opcvid_atras:
					menu_activo = menu_opciones;
					boton_seleccionado = 0;
					break;
				case boton_opc_atras:
					menu_activo = menu_principal;
					boton_seleccionado = 0;
					break;
				case boton_video:
					menu_activo = menu_opc_video;
					boton_seleccionado = 0;
					break;
				case boton_fullscreen:
					// Tecla F: Pantalla completa --- Toggle fullscreen
					CambiarModoPantallaCompleta ( !opciones_juego.fullscreen , gGameWindow );
					opciones_juego.fullscreen = !opciones_juego.fullscreen;
					pantallas_menu_principal[menu_opc_video].botones_pantalla[0].estado_on_off = opciones_juego.fullscreen;
					break;
				case boton_wireframe:
					opciones_juego.wireframe = !opciones_juego.wireframe;
					pantallas_menu_principal[menu_opc_video].botones_pantalla[1].estado_on_off = opciones_juego.wireframe;					
					break;
				case boton_textured_objects:
					opciones_juego.textured_objects = !opciones_juego.textured_objects;
					pantallas_menu_principal[menu_opc_video].botones_pantalla[2].estado_on_off = opciones_juego.textured_objects;					
					break;
				default:
					// No hacemos nada --- Do nothing
					break;
			}	
		}
		boton_pulsado = false;

		/////////////////////////////////////////////////////////////////////////////////////
		// Representa todo por pantalla -- Displays everything on the screen

		 //Clear screen 
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer ); 

		// Dibuja fondo --- Draw menu background image
		SDL_RenderCopy( gRenderer, pantallas_menu_principal[menu_activo].imagen_fondo_pantalla, NULL, NULL );		//Imagen estirada a toda la ventana
		// Dibuja titulo --- Draw menu title (Nota: SDL_Rect es un struct)
		renderQuad.x = pantallas_menu_principal[menu_activo].borde_izquierdo_titulo; 									/* Coord X esquina superior izquierda */
		renderQuad.y = pantallas_menu_principal[menu_activo].borde_arriba_titulo; 										/* Coord Y esquina superior izqeuirda */
		renderQuad.w = pantallas_menu_principal[menu_activo].borde_derecho_titulo - pantallas_menu_principal[menu_bienvenida].borde_izquierdo_titulo; 	/* Ancho */
		renderQuad.h = pantallas_menu_principal[menu_activo].borde_abajo_titulo - pantallas_menu_principal[menu_bienvenida].borde_arriba_titulo;		/* Alto */
		SDL_RenderCopy( gRenderer, pantallas_menu_principal[menu_activo].textura_titulo, NULL, &renderQuad );

		// Dibuja los botones del menu
		if ( pantallas_menu_principal[menu_activo].numero_botones != 0 )
		{
			for ( i = 0 ; i<= pantallas_menu_principal[menu_activo].numero_botones -1 ; i++ )
			{
				switch ( pantallas_menu_principal[menu_activo].botones_pantalla[i].clase_boton ) 
				{
					case boton_pulsar:
						renderQuad.x = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo; 									/* Coord X esquina superior izquierda */
						renderQuad.y = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba; 										/* Coord Y esquina superior izqeuirda */
						renderQuad.w = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_derecho - pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo; 	/* Ancho */
						renderQuad.h = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_abajo - pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba;
						if ( boton_seleccionado == i )
						{
							SDL_RenderCopy( gRenderer, pantallas_menu_principal[menu_activo].botones_pantalla[i].textura_boton_seleccionado, NULL, &renderQuad );
						}
						else
						{
							SDL_RenderCopy( gRenderer, pantallas_menu_principal[menu_activo].botones_pantalla[i].textura_boton_reposo, NULL, &renderQuad );
						}
						break;
					case boton_conmutar:
						renderQuad.x = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo; 									/* Coord X esquina superior izquierda */
						renderQuad.y = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba; 										/* Coord Y esquina superior izqeuirda */
						renderQuad.w = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_derecho - pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo; 	/* Ancho */
						renderQuad.h = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_abajo - pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba;
						if ( boton_seleccionado == i )
						{
							SDL_RenderCopy( gRenderer, pantallas_menu_principal[menu_activo].botones_pantalla[i].textura_boton_seleccionado, NULL, &renderQuad );
						}
						else
						{
							SDL_RenderCopy( gRenderer, pantallas_menu_principal[menu_activo].botones_pantalla[i].textura_boton_reposo, NULL, &renderQuad );
						}
						// Dibujar caja que indique el estado de la seleccion --- Draw tickbo
						renderQuad.w =  (int) (opciones_juego.screen_y_resolution) * PORCENTAJE_ALTO_BOTON/100 ; 		/* Ancho */ /* Ancho igual que alto de boton --- Tickbox width equal to button height */
						renderQuad.x = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo - renderQuad.w; 			/* Coord X esquina superior izquierda */
						renderQuad.y = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba; 							/* Coord Y esquina superior izqeuirda */
						renderQuad.h = renderQuad.w;												/* Alto */
						if ( pantallas_menu_principal[menu_activo].botones_pantalla[i].estado_on_off )
						{
							SDL_RenderCopy( gRenderer, gWhiteCheckBoxChecked, NULL, &renderQuad );
						}
						else
						{
							SDL_RenderCopy( gRenderer, gWhiteCheckBoxUnchecked, NULL, &renderQuad );
						}
						break;
					default:
						printf("Error en el menu %d, boton %d no tiene definido el tipo.\n", menu_activo, i);
						break;
				}
			}

		}

		//Update screen 
		SDL_RenderPresent( gRenderer );

		/////////////////////////////////////////////////////////////////////////////////////	

		// Limitar el framerate aproximadamente al framerate deseado
		if ((1000/framerate_deseado) > (SDL_GetTicks() - currentTime ) ) 
		{
			tiempo_imagen_sobrante = (1000/framerate_deseado) - (SDL_GetTicks() - currentTime );	//En milisegundos
			#ifdef DEBUG_INFO
			printf("Esperando %d ms para llegar a %d FPS\n", tiempo_imagen_sobrante, (int)framerate_deseado);
			#endif
			SDL_Delay(tiempo_imagen_sobrante);

		}
		///////////////////////////////////////////////////////////////////////////////////////

		#ifdef DEBUG_INFO
		printf("Pos raton = %d, %d\n", pos_raton_x, pos_raton_y );
		system("clear");		// Borra la terminal para leer el estado actual (MUY LENTO)
		#endif

	// Liberamos memoria dinamica
	}

}
