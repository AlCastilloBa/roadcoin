//Using SDL and standard IO 
#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h> 
#include <stdbool.h>
#include <math.h>


#include "maps.h"
#include "game.h"
#include "options.h"
#include "graphics.h"
#include "menu.h"



// Si definido DEBUG_INFO, mostrar textos de informacion por la terminal (hace el programa más lento)
// If DEBUG_INFO is defined, info texts will be shown on terminal (but the program will be slower)
// #define DEBUG_INFO


//#define VERSION_JUEGO "Roadcoin, v0.1-beta3\0"


#define PORCENTAJE_ANCHO_TITULOS 70.0
#define PORCENTAJE_ALTO_TITULOS 16.0
#define PORCENTAJE_ANCHO_BOTONES 70.0
#define PORCENTAJE_MARGEN_SUPERIOR 30.0
//#define PORCENTAJE_MARGEN_INFERIOR 20
#define PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES 10.0
#define PORCENTAJE_ALTO_BOTON 8.0
//#define PORCENTAJE_ANCHO_CARACTER_MENU 3

// Variables que definen la proporcion de los elementos de un boton de tipo "valor numérico"
#define PORCNT_ANCHO_TITULO_BOT_VAL_NUM 40.0
#define PORCNT_ANCHO_VALOR_BOT_VAL_NUM 40.0
#define PORCNT_ANCHO_MAS_BOT_VAL_NUM 10.0
#define PORCNT_ANCHO_MENOS_BOT_VAL_NUM 10.0
//#define PORCNT_ANCHO_DIGITO_BOT_VAL_NUM 12.5


/////////////////////////////////////////////////////////////////////////
// Variables globales --- Global variables
//The window renderer total_digitos
extern SDL_Renderer* gRenderer;				// Definido en "game.c"
// Variables globales opciones --- Options global variables
extern struct opciones opciones_juego;			// Definido en "game.c"
// Variables globales del menu --- Menu global variables
extern struct pantalla_menu* pantallas_menu_principal;	// Definido en "game.c"
extern int numero_pantallas;

extern SDL_Window* gGameWindow; 


TTF_Font* gFuenteTexto = NULL;
SDL_Color gColorTextoReposo = { 255 , 255 , 255 }; 	// Blanco --- White
SDL_Color gColorTextoSeleccionado = { 255 , 255 , 0 }; 	// Amarillo --- Yellow

SDL_Texture* gWhiteCheckBoxChecked = NULL;
SDL_Texture* gWhiteCheckBoxUnchecked = NULL;

SDL_Texture* gWhiteArrowMinus = NULL;
SDL_Texture* gWhiteArrowPlus = NULL;

SDL_Texture* gWhiteLeftArrow = NULL;
SDL_Texture* gWhiteRightArrow = NULL;
SDL_Texture* gYellowLeftArrow = NULL;
SDL_Texture* gYellowRightArrow = NULL;

SDL_Texture* gGameLogo = NULL;
SDL_Texture* gGameLogoCredits = NULL;		// (30/8/2021)

Mix_Music *gMusicaMenu = NULL;
Mix_Chunk *gSonidoClickBoton = NULL;

SDL_Texture* gTexturaNumeroMenu[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};		// Numbers (0 to 9) texture // Pruebas 6/4/2020
SDL_Texture* gTexturaPuntoMenu = NULL;	// Dot character texture (for numbers)
SDL_Texture* gTexturaComaMenu = NULL;	// Comma character texture (for numbers)

// Dimensiones texturas
int gDimWhiteArrowMinusX; int gDimWhiteArrowMinusY; 
int gDimWhiteArrowPlusX; int gDimWhiteArrowPlusY; 
int gDimWhiteLeftArrowX; int gDimWhiteLeftArrowY;
int gDimWhiteRightArrowX; int gDimWhiteRightArrowY;
int gDimGameLogoX; int gDimGameLogoY;
int gDimGameLogoCreditsX; int gDimGameLogoCreditsY;	// (30/8/2021)

// Prueba 8/4/2020
struct ConjuntoMapas* gListaConjuntosMapasJuego = NULL;	// Pointer to game level-sets list
int gNumeroConjuntosMapas = 0;




bool raton_sobre_boton (struct boton_menu boton_consultado, int pos_raton_x, int pos_raton_y )		// Mouse over button
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

bool raton_sobre_boton_mas_valor_numerico ( struct boton_menu boton_consultado, int pos_raton_x, int pos_raton_y )		// Mouse over (+) button on numeric value button
{
	// Esta función indica si el ratón está posicionado sobre el pulsador (+) de un botón tipo "valor numerico"
	// This function tells whether or not the mouse is over the (+) pushbutton inside a "numeric value" type button
	if ( boton_consultado.clase_boton == boton_valor_numerico )
	{
		if ( 	(pos_raton_x >= boton_consultado.borde_izquierdo + (opciones_juego.screen_x_resolution * (PORCENTAJE_ANCHO_BOTONES/100)*(PORCNT_ANCHO_TITULO_BOT_VAL_NUM + PORCNT_ANCHO_VALOR_BOT_VAL_NUM + PORCNT_ANCHO_MENOS_BOT_VAL_NUM)/100) ) && 
			(pos_raton_x <= boton_consultado.borde_izquierdo + (opciones_juego.screen_x_resolution * (PORCENTAJE_ANCHO_BOTONES/100)*(PORCNT_ANCHO_TITULO_BOT_VAL_NUM + PORCNT_ANCHO_VALOR_BOT_VAL_NUM + PORCNT_ANCHO_MENOS_BOT_VAL_NUM + PORCNT_ANCHO_MAS_BOT_VAL_NUM)/100) ) &&
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
	else
	{
		printf("Error de programación: boton número %d no es tipo valor numerico, pero se ha programado como si lo fuera\n", boton_consultado.identificador_boton );
		return false;
	}
} 

bool raton_sobre_boton_menos_valor_numerico ( struct boton_menu boton_consultado, int pos_raton_x, int pos_raton_y )		// Mouse over (-) button on numeric value button
{
	// Esta función indica si el ratón está posicionado sobre el pulsador (-) de un botón tipo "valor numerico"
	// This function tells whether or not the mouse is over the (-) pushbutton inside a "numeric value" type button
	if ( boton_consultado.clase_boton == boton_valor_numerico )
	{
		if ( 	(pos_raton_x >= boton_consultado.borde_izquierdo + (opciones_juego.screen_x_resolution * (PORCENTAJE_ANCHO_BOTONES/100)*(PORCNT_ANCHO_TITULO_BOT_VAL_NUM + PORCNT_ANCHO_VALOR_BOT_VAL_NUM )/100) ) && 
			(pos_raton_x <= boton_consultado.borde_izquierdo + (opciones_juego.screen_x_resolution * (PORCENTAJE_ANCHO_BOTONES/100)*(PORCNT_ANCHO_TITULO_BOT_VAL_NUM + PORCNT_ANCHO_VALOR_BOT_VAL_NUM + PORCNT_ANCHO_MENOS_BOT_VAL_NUM)/100) ) &&
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
	else
	{
		printf("Error de programación: boton número %d no es tipo valor numerico, pero se ha programado como si lo fuera\n", boton_consultado.identificador_boton );
		return false;
	}
} 

bool inicializa_datos_boton( struct boton_menu* boton, int indice_boton, enum listado_botones ident_boton, enum tipo_boton tipo, char* texto_boton, int boton_anterior, int boton_siguiente, enum ModoRepresentacionTextura ModoRepresent  )		// Initialize button data
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
	boton->borde_abajo = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + indice_boton*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES + PORCENTAJE_ALTO_BOTON)/100;
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

	boton->ModoRepresentacion = ModoRepresent;	// Nuevo 19/4/2020

	return exito;
}

bool inicializa_datos_pantalla_menu( struct pantalla_menu* pantalla, enum pantalla_menu_principal numero_pantalla, char* titulo_pantalla, char* ruta_textura_fondo, int numero_botones, enum ModoRepresentacionTextura ModoRepresent )		// Initialize menu screen data
{
	// La funcion "inicializar_menu_principal" es muy extensa, con código muy parecido. Se crea esta función para intentar reducir la extension del código.
	// Function "inicializar_menu_principal" (initialize main menu) is too long. This function is created in an attempt to reduce code lenght.

	bool exito = true;

	/* pantallas_menu_principal[menu_opciones].identificador_pantalla*/ pantalla->identificador_pantalla = numero_pantalla;
	strcpy( pantalla->titulo, titulo_pantalla );
	pantalla->textura_titulo = RenderizaTextoEnTextura( pantalla->titulo, 
								gFuenteTexto, 
								gColorTextoReposo, 
								&(pantalla->textura_titulo_dim_x) /*NULL*/, 
								&(pantalla->textura_titulo_dim_y) /*NULL*/   );
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

	pantalla->ModoRepresentacion = ModoRepresent;

	return exito;
}






//  _                    _                                    _       _
// | |    ___   __ _  __| |  _ __ ___   ___ _ __  _   _    __| | __ _| |_ __ _
// | |   / _ \ / _` |/ _` | | '_ ` _ \ / _ \ '_ \| | | |  / _` |/ _` | __/ _` |
// | |__| (_) | (_| | (_| | | | | | | |  __/ | | | |_| | | (_| | (_| | || (_| |
// |_____\___/ \__,_|\__,_| |_| |_| |_|\___|_| |_|\__,_|  \__,_|\__,_|\__\__,_|



bool inicializar_menu_principal(/* struct pantalla_menu* pantallas_menu_principal, */int numero_pantallas )		// Main menu init
{
	// Esta funcion inicializa los valores de las estructuras de las imagenes del menu.
	// Reserva la memoria para el menu, inicializa variables, e inicializa graficos

	// This function initializes main menu data structures
	// Allocates memory for the menu, initializes variables and initializes graphics

	bool success = true;
	int i, conjunto_actual, mapa_actual;
	bool lista_mapas_ok = true;

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

	gWhiteArrowMinus = CargaTextura( "images/white_arrow_minus.png", &gDimWhiteArrowMinusX, &gDimWhiteArrowMinusY, true );
	gWhiteArrowPlus = CargaTextura( "images/white_arrow_plus.png" , NULL, NULL, true );
	if( gWhiteArrowMinus == NULL || gWhiteArrowPlus == NULL ) 
	{ 
		printf( "Error al cargar texturas flechas mas y menos.\n" ); 
		success = false; 
	}

	gWhiteLeftArrow	= CargaTextura( "images/white_left_arrow.png", &gDimWhiteLeftArrowX, &gDimWhiteLeftArrowY, true );
	gWhiteRightArrow = CargaTextura( "images/white_right_arrow.png", &gDimWhiteRightArrowX, &gDimWhiteRightArrowY, true );
	gYellowLeftArrow = CargaTextura( "images/yellow_left_arrow.png", NULL, NULL, true );
	gYellowRightArrow = CargaTextura( "images/yellow_right_arrow.png", NULL, NULL, true );
	if ( gWhiteLeftArrow == NULL || gWhiteRightArrow == NULL || gYellowLeftArrow == NULL || gYellowRightArrow == NULL )
	{
		printf( "Error al cargar texturas flechas izquierda y derecha.\n" ); 
		success = false; 
	}


	gTexturaNumeroMenu[0] = CargaTextura( "images/chars/lunasol/0.png" , NULL, NULL, true );
	gTexturaNumeroMenu[1] = CargaTextura( "images/chars/lunasol/1.png" , NULL, NULL, true );
	gTexturaNumeroMenu[2] = CargaTextura( "images/chars/lunasol/2.png" , NULL, NULL, true );
	gTexturaNumeroMenu[3] = CargaTextura( "images/chars/lunasol/3.png" , NULL, NULL, true );
	gTexturaNumeroMenu[4] = CargaTextura( "images/chars/lunasol/4.png" , NULL, NULL, true );
	gTexturaNumeroMenu[5] = CargaTextura( "images/chars/lunasol/5.png" , NULL, NULL, true );
	gTexturaNumeroMenu[6] = CargaTextura( "images/chars/lunasol/6.png" , NULL, NULL, true );
	gTexturaNumeroMenu[7] = CargaTextura( "images/chars/lunasol/7.png" , NULL, NULL, true );
	gTexturaNumeroMenu[8] = CargaTextura( "images/chars/lunasol/8.png" , NULL, NULL, true );
	gTexturaNumeroMenu[9] = CargaTextura( "images/chars/lunasol/9.png" , NULL, NULL, true );
	gTexturaPuntoMenu = CargaTextura( "images/chars/lunasol/dot.png" , NULL, NULL, true );
	gTexturaComaMenu = CargaTextura( "images/chars/lunasol/comma.png" , NULL, NULL, true );
	if ( gTexturaNumeroMenu[0] == NULL || gTexturaNumeroMenu[1] == NULL || gTexturaNumeroMenu[2] == NULL || gTexturaNumeroMenu[3] == NULL || gTexturaNumeroMenu[4]== NULL || 
		gTexturaNumeroMenu[5] == NULL || gTexturaNumeroMenu[6] == NULL || gTexturaNumeroMenu[7] == NULL || gTexturaNumeroMenu[8] == NULL || gTexturaNumeroMenu[9] == NULL || 
		gTexturaPuntoMenu == NULL || gTexturaComaMenu == NULL )
	{
		printf( "Failed to load number texture image!\n" ); 
		success = false; 
	}

	// Cargar el logotipo del juego --- Load game logo
	gGameLogo = CargaTextura( "images/Logo_color_1.png" , &gDimGameLogoX, &gDimGameLogoY, true );
	if ( gGameLogo == NULL )
	{
		printf( "Error al cargar el logotipo del juego.\n" ); 
		success = false; 
	}

	// Cargar el logotipo del juego para los creditos --- Load game logo for credits screen (30/8/2021)
	gGameLogoCredits = CargaTextura( "images/Logo_color_2.png" , &gDimGameLogoCreditsX, &gDimGameLogoCreditsY, true );
	if ( gGameLogoCredits == NULL )
	{
		printf( "Error al cargar el logotipo del juego para los creditos.\n" ); 
		success = false; 
	}

	// Cargar la fuente del menu --- Load menu fonts
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
	// Pantalla bienvenida --- Welcome screen
	#ifdef DEBUG_INFO
	printf("Inicializando datos de pantalla bienvenida...\n");
	#endif

	if( !inicializa_datos_pantalla_menu( &(pantallas_menu_principal[menu_bienvenida]), menu_bienvenida, "              \0" ,  "images/2068223.jpg", 2, con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar pantalla bienvenida.\n" ); 
		success = false;
	}
	// Boton 0 - Logotipo del juego --- Game logo
	pantallas_menu_principal[menu_bienvenida].botones_pantalla[0].identificador_boton = boton_bienv_logo;
	pantallas_menu_principal[menu_bienvenida].botones_pantalla[0].clase_boton = boton_pulsar;
	pantallas_menu_principal[menu_bienvenida].botones_pantalla[0].borde_izquierdo = 0.1*(opciones_juego.screen_x_resolution);
	pantallas_menu_principal[menu_bienvenida].botones_pantalla[0].borde_derecho = 0.9*(opciones_juego.screen_x_resolution);
	pantallas_menu_principal[menu_bienvenida].botones_pantalla[0].borde_arriba = 0.2*(opciones_juego.screen_y_resolution);
	pantallas_menu_principal[menu_bienvenida].botones_pantalla[0].borde_abajo = 0.5*(opciones_juego.screen_y_resolution);
	pantallas_menu_principal[menu_bienvenida].botones_pantalla[0].textura_boton_reposo = gGameLogo;
	pantallas_menu_principal[menu_bienvenida].botones_pantalla[0].textura_boton_seleccionado = gGameLogo;
	pantallas_menu_principal[menu_bienvenida].botones_pantalla[0].boton_anterior = 1;
	pantallas_menu_principal[menu_bienvenida].botones_pantalla[0].boton_siguiente = 1;
	pantallas_menu_principal[menu_bienvenida].botones_pantalla[0].ModoRepresentacion = con_relac_aspecto_no_rellenar;		// Nuevo 19/4/2020
	pantallas_menu_principal[menu_bienvenida].botones_pantalla[0].textura_dim_x = gDimGameLogoX;
	pantallas_menu_principal[menu_bienvenida].botones_pantalla[0].textura_dim_y = gDimGameLogoY;


	// Boton 1 - Pulse cualquier tecla --- Press any key
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 0 de pantalla bienvenida...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_bienvenida].botones_pantalla[1]) , 4 /*pos 4 no 1*/ , boton_bienv_pulse, boton_pulsar, "Pulse cualquier tecla\0" , 1, 1, con_relac_aspecto_no_rellenar  ) )
	{
		printf( "Error al inicializar boton 0 en pantalla bienvenida.\n" ); 
		success = false;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla menu_principal --- Main menu screen
	#ifdef DEBUG_INFO
	printf("Inicializando datos de pantalla menu principal...\n");
	#endif
	if( !inicializa_datos_pantalla_menu( &(pantallas_menu_principal[menu_principal]), menu_principal, "Menu principal\0" ,  "images/gradient-abstract.png", 4, con_relac_aspecto_no_rellenar ) )
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
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_principal].botones_pantalla[0]) , 0, boton_jugar, boton_pulsar, "     Jugar      \0" , 3, 1, con_relac_aspecto_no_rellenar  ) )
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
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_principal].botones_pantalla[1]) , 1, boton_opciones, boton_pulsar, "    Opciones    \0" , 0, 2, con_relac_aspecto_no_rellenar  ) )
	{
		printf( "Error al inicializar boton 1 en menu principal.\n" ); 
		success = false;
	}
	//------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 2 de pantalla menu principal...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_principal].botones_pantalla[2]) , 2, boton_creditos, boton_pulsar, "    Creditos    \0" , 1, 3 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 2 en menu principal.\n" ); 
		success = false;
	}
	//-------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 3 de pantalla menu principal...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_principal].botones_pantalla[3]) , 3, boton_salir, boton_pulsar, "     Salir      \0" , 2, 0, con_relac_aspecto_no_rellenar  ) )
	{
		printf( "Error al inicializar boton 2 en menu principal.\n" ); 
		success = false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla opciones --- Options screen
	#ifdef DEBUG_INFO
	printf("Inicializando datos de pantalla opciones...\n");
	#endif
	if( !inicializa_datos_pantalla_menu( &(pantallas_menu_principal[menu_opciones]), menu_opciones, "Menu opciones\0", "images/blue_pink_gradient.png", 6, con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar pantalla opciones.\n" ); 
		success = false;
	}
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 0 de pantalla menu opciones...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opciones].botones_pantalla[0]) , 0, boton_video, boton_pulsar, "     Video      \0" , 5, 1 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 0 en menu opciones.\n" ); 
		success = false;
	}
	//-----------------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 1 de pantalla menu opciones...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opciones].botones_pantalla[1]) , 1, boton_sonido, boton_pulsar, "     Sonido     \0" , 0, 2 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 1 en menu opciones.\n" ); 
		success = false;
	}
	//-----------------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 2 de pantalla menu opciones...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opciones].botones_pantalla[2]) , 2, boton_controles, boton_pulsar, "    Controles    \0" , 1, 3 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 2 en menu opciones.\n" ); 
		success = false;
	}
	//-----------------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 3 de pantalla menu opciones...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opciones].botones_pantalla[3]) , 3, boton_opc_juego, boton_pulsar, "     Juego      \0" , 2, 4 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 3 en menu opciones.\n" ); 
		success = false;
	}
	//-----------------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 3 de pantalla menu opciones...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opciones].botones_pantalla[3]) , 3, boton_opc_juego, boton_pulsar, "     Juego      \0" , 2, 4 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 3 en menu opciones.\n" ); 
		success = false;
	}
	//-----------------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 4 de pantalla menu opciones...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opciones].botones_pantalla[4]) , 4, boton_idioma, boton_pulsar, "     Idioma     \0" , 3, 5 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 4 en menu opciones.\n" ); 
		success = false;
	}
	//-----------------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 5 de pantalla menu opciones...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opciones].botones_pantalla[5]) , 5, boton_opc_atras, boton_pulsar, "     Atras      \0" , 4, 0 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 5 en menu opciones.\n" ); 
		success = false;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla opciones de video --- Video options screen
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla opciones
	#ifdef DEBUG_INFO
	printf("Inicializando datos de pantalla opciones de video...\n");
	#endif
	if( !inicializa_datos_pantalla_menu( &(pantallas_menu_principal[menu_opc_video]), menu_opc_video, "Opciones video\0", "images/muticolor_gradient.jpg", 4 , con_relac_aspecto_no_rellenar) )
	{
		printf( "Error al inicializar pantalla opciones de video.\n" ); 
		success = false;
	}
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 0 de pantalla menu opciones de video...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opc_video].botones_pantalla[0]) , 0, boton_fullscreen, boton_conmutar, "Pantalla completa\0" , 3, 1 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 0 en menu opciones de video.\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_opc_video].botones_pantalla[0].estado_on_off = opciones_juego.fullscreen;
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 1 de pantalla menu opciones de video...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opc_video].botones_pantalla[1]) , 1, boton_wireframe, boton_conmutar, "Dibujar alambres \0" , 0, 2 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 1 en menu opciones de video.\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_opc_video].botones_pantalla[1].estado_on_off = opciones_juego.wireframe;
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 2 de pantalla menu opciones de video...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opc_video].botones_pantalla[2]) , 2, boton_textured_objects, boton_conmutar, "Objetos texturas  \0" , 1, 3 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 2 en menu opciones de video.\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_opc_video].botones_pantalla[2].estado_on_off = opciones_juego.textured_objects;
	//----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 3 de pantalla menu opciones de video...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opc_video].botones_pantalla[3]) , 3, boton_opcvid_atras, boton_pulsar, "      Atras      \0" , 2, 0 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 3 en menu opciones de video.\n" ); 
		success = false;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla opciones de sonido --- Sound options screen
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla opciones
	#ifdef DEBUG_INFO
	printf("Inicializando datos de pantalla opciones de sonido...\n");
	#endif
	if( !inicializa_datos_pantalla_menu( &(pantallas_menu_principal[menu_opc_sonido]), menu_opc_sonido, "Opciones sonido\0", "images/gradient_gray.jpg", 3 , con_relac_aspecto_no_rellenar) )
	{
		printf( "Error al inicializar pantalla opciones de sonido.\n" ); 
		success = false;
	}
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 0 de pantalla menu opciones de sonido...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opc_sonido].botones_pantalla[0]) , 0, boton_activar_musica, boton_conmutar, "      Musica     \0" , 2, 1 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 0 en menu opciones de sonido.\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_opc_sonido].botones_pantalla[0].estado_on_off = opciones_juego.music_enabled;
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 1 de pantalla menu opciones de sonido...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opc_sonido].botones_pantalla[1]) , 1, boton_activar_sonido, boton_conmutar, "     Sonido      \0" , 0, 2 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 1 en menu opciones de sonido.\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_opc_sonido].botones_pantalla[1].estado_on_off = opciones_juego.sound_enabled;
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 2 de pantalla menu opciones de sonido...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opc_sonido].botones_pantalla[2]) , 2, boton_opcsnd_atras, boton_pulsar, "      Atras      \0" , 1, 0 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 2 en menu opciones de sonido.\n" ); 
		success = false;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla opciones de controles --- Controls options screen
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla opciones
	#ifdef DEBUG_INFO
	printf("Inicializando datos de pantalla opciones de controles...\n");
	#endif
	if( !inicializa_datos_pantalla_menu( &(pantallas_menu_principal[menu_opc_control]), menu_opc_control, "Opciones control\0", "images/blob_gradient.jpeg", 3 , con_relac_aspecto_no_rellenar) )
	{
		printf( "Error al inicializar pantalla opciones de sonido.\n" ); 
		success = false;
	}
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 0 de pantalla menu opciones de controles...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opc_control].botones_pantalla[0]) , 0, boton_sensibilidad_raton, boton_valor_numerico, "Sens raton:   \0" , 1, 1 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 0 en menu opciones de controles.\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico = opciones_juego.mouse_sensitivity;
	pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico_rango_minimo = 0.001;
	pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico_rango_maximo = 1;
	pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico_incremento = 0.001;
	pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico_digitos_izq_coma = 1;	
	pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico_digitos_dcha_coma = 4;
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 1 de pantalla menu opciones de controles...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opc_control].botones_pantalla[1]) , 1, boton_opcctl_atras, boton_pulsar, "      Atras      \0" , 0, 0 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 1 en menu opciones de controles.\n" ); 
		success = false;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla opciones de juego --- Game options screen
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla opciones
	#ifdef DEBUG_INFO
	printf("Inicializando datos de pantalla opciones de juego...\n");
	#endif
	if( !inicializa_datos_pantalla_menu( &(pantallas_menu_principal[menu_opc_juego]), menu_opc_juego, "Opciones juego\0", "images/gradient_gray.jpg", 3, con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar pantalla opciones de juego.\n" ); 
		success = false;
	}
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 0 de pantalla menu opciones de juego...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opc_juego].botones_pantalla[0]) , 0, boton_moneda_puede_volar, boton_conmutar, "Moneda vuela (DIFICIL)\0" , 2, 1 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 0 en menu opciones de juego.\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_opc_juego].botones_pantalla[0].estado_on_off = opciones_juego.map_rot_makes_coin_fly;
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 1 de pantalla menu opciones de juego...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opc_juego].botones_pantalla[1]) , 1, boton_limitar_vel_moneda, boton_conmutar, "Limitar Velocidad\0" , 0, 2 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 1 en menu opciones de juego.\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_opc_juego].botones_pantalla[1].estado_on_off = opciones_juego.limit_coin_speed;
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 2 de pantalla menu opciones de juego...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_opc_juego].botones_pantalla[2]) , 2, boton_opcjgo_atras, boton_pulsar, "      Atras      \0" , 1, 0 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 2 en menu opciones de juego.\n" ); 
		success = false;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla creditos --- Credits screen (30/8/2021)
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla creditos
	#ifdef DEBUG_INFO
	printf("Inicializando datos de pantalla de creditos...\n");
	#endif
	if( !inicializa_datos_pantalla_menu( &(pantallas_menu_principal[menu_creditos]), menu_creditos, "Creditos\0", "images/NightMountain.png", 4, con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar pantalla opciones de juego.\n" ); 
		success = false;
	}
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 0 de pantalla de creditos...\n");
	#endif
	// Boton 0 - Logotipo del juego --- Game logo
	pantallas_menu_principal[menu_creditos].botones_pantalla[0].identificador_boton = boton_creditos_logo;
	pantallas_menu_principal[menu_creditos].botones_pantalla[0].clase_boton = boton_pulsar;
	pantallas_menu_principal[menu_creditos].botones_pantalla[0].borde_izquierdo = 0.2*(opciones_juego.screen_x_resolution);
	pantallas_menu_principal[menu_creditos].botones_pantalla[0].borde_derecho = 0.8*(opciones_juego.screen_x_resolution);
	pantallas_menu_principal[menu_creditos].botones_pantalla[0].borde_arriba = 0.3*(opciones_juego.screen_y_resolution);
	pantallas_menu_principal[menu_creditos].botones_pantalla[0].borde_abajo = 0.5*(opciones_juego.screen_y_resolution);
	pantallas_menu_principal[menu_creditos].botones_pantalla[0].textura_boton_reposo = gGameLogoCredits;
	pantallas_menu_principal[menu_creditos].botones_pantalla[0].textura_boton_seleccionado = gGameLogoCredits;
	pantallas_menu_principal[menu_creditos].botones_pantalla[0].boton_anterior = 3;
	pantallas_menu_principal[menu_creditos].botones_pantalla[0].boton_siguiente = 3;
	pantallas_menu_principal[menu_creditos].botones_pantalla[0].ModoRepresentacion = con_relac_aspecto_no_rellenar;
	pantallas_menu_principal[menu_creditos].botones_pantalla[0].textura_dim_x = gDimGameLogoCreditsX;
	pantallas_menu_principal[menu_creditos].botones_pantalla[0].textura_dim_y = gDimGameLogoCreditsY;

	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 1 de pantalla de creditos...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_creditos].botones_pantalla[1]) , 3, boton_creditos_version, boton_pulsar, VERSION_JUEGO, 3, 3 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 1 en pantalla de creditos.\n" ); 
		success = false;
	}
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 2 de pantalla de creditos...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_creditos].botones_pantalla[2]) , 4, boton_creditos_autor, boton_pulsar, "Alberto Castillo Baquero, 2019-2021\0" , 3, 3 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 2 en pantalla de creditos.\n" ); 
		success = false;
	}
	//----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 3 de pantalla de creditos...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_creditos].botones_pantalla[3]) , 5, boton_creditos_atras, boton_pulsar, "      Atras      \0" , 3, 3 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 3 en pantalla de creditos.\n" ); 
		success = false;
	}




	//////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla menu provisional de seleccion de niveles --- Temporary level select screen
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla seleccion provisional de seleccion de niveles
	#ifdef DEBUG_INFO
	printf("Inicializando datos de pantalla seleccion provisional de nivel...\n");
	#endif
	if( !inicializa_datos_pantalla_menu( &(pantallas_menu_principal[menu_niveles_provisional]), menu_niveles_provisional, "Seleccion provisional nivel\0", "images/blob_gradient.jpeg", 6 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar pantalla menu provisional seleccion nivel.\n" ); 
		success = false;
	}

	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 0 de pantalla menu provisional seleccion nivel...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_niveles_provisional].botones_pantalla[0]) , 0, boton_provisional_nivel_01, boton_pulsar, "    Monza        \0" , 5, 1 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 0 en menu provisional seleccion nivel.\n" ); 
		success = false;
	}
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 1 de pantalla menu provisional seleccion nivel...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_niveles_provisional].botones_pantalla[1]) , 1, boton_provisional_nivel_02, boton_pulsar, "Laberinto 01     \0" , 0, 2 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 1 en menu provisional seleccion nivel.\n" ); 
		success = false;
	}
	//-----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 2 de pantalla menu provisional seleccion nivel...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_niveles_provisional].botones_pantalla[2]) , 2, boton_provisional_nivel_03, boton_pulsar, "Piramide          \0" , 1, 3 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 2 en menu provisional seleccion nivel.\n" ); 
		success = false;
	}
	//----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 3 de pantalla menu provisional seleccion nivel...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_niveles_provisional].botones_pantalla[3]) , 3, boton_provisional_nivel_04, boton_pulsar, "Laberinto Circular\0" , 2, 4 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 3 en menu provisional seleccion nivel.\n" ); 
		success = false;
	}
	//----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 4 de pantalla menu provisional seleccion nivel...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_niveles_provisional].botones_pantalla[4]) , 4, boton_provisional_nivel_05, boton_pulsar, "Super Pinball    \0" , 3, 5 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 4 en menu provisional seleccion nivel.\n" ); 
		success = false;
	}
	//----------------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 5 de pantalla menu provisional seleccion nivel...\n");
	#endif
	if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_niveles_provisional].botones_pantalla[5]) , 5, boton_nivel_prov_atras, boton_pulsar, "      Atras      \0" , 4, 0 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar boton 5 en menu provisional seleccion nivel.\n" ); 
		success = false;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla selección de conjunto de niveles --- Level set selection screen
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla selección de conjunto de niveles
	// Esta pantalla no se parece a ninguna de las anteriores, por lo que la configuro a mano
	// This screen is different to all other screens, we configure it manually

	#ifdef DEBUG_INFO
	printf("Inicializando datos de pantalla seleccion de conjunto de niveles...\n");
	#endif
	if( !inicializa_datos_pantalla_menu( &(pantallas_menu_principal[menu_elige_conjunto_niveles]), menu_elige_conjunto_niveles, "Conjunto niveles\0", "images/blue_neon_grid.jpeg", 8 , con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar pantalla seleccion de conjunto de niveles.\n" ); 
		success = false;
	}
	//-------------------------------------------------------
	// Leemos fichero level set
	gListaConjuntosMapasJuego = LeerConjuntosMapasJuego ( &gNumeroConjuntosMapas );
	if ( gListaConjuntosMapasJuego==NULL || gNumeroConjuntosMapas==0 )
	{
		printf( "Ha sucedido algo inesperado al leer la lista de conjuntos de mapas.\n" );
		printf( "Revise que el contenido de maps/levelsets_list es el correcto y que existe al menos un conjunto de mapas\n");
		success = false;
	}
	else
	{
		#ifdef DEBUG_INFO
		printf("Se han cargado datos de %d conjuntos de mapas...\n", gNumeroConjuntosMapas );
		#endif

		// Cargamos las imágenes y creamos las texturas de los textos
		// Load images and create texts textures
		for ( i = 0 ; i < gNumeroConjuntosMapas ; i++ )
		{
			char cadena_numeracion[10];
			#ifdef DEBUG_INFO
			printf("Cargando textura %s...\n", gListaConjuntosMapasJuego[i].RutaImagen);
			#endif
			gListaConjuntosMapasJuego[i].TexturaImagen = CargaTextura( 	gListaConjuntosMapasJuego[i].RutaImagen, 
											&(gListaConjuntosMapasJuego[i].TexturaImagen_DimX), 
											&(gListaConjuntosMapasJuego[i].TexturaImagen_DimY), 
											true );
			if ( gListaConjuntosMapasJuego[i].TexturaImagen == NULL )
			{
				printf("Error al cargar la textura %s \n", gListaConjuntosMapasJuego[i].RutaImagen );
				success = false;
			}
			// TODO, añadir tamaños para mantener la relacion de aspecto
			gListaConjuntosMapasJuego[i].TexturaNombreDir = RenderizaTextoEnTextura( gListaConjuntosMapasJuego[i].Directorio, gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[i].TexturaNombreDir_DimX), &(gListaConjuntosMapasJuego[i].TexturaNombreDir_DimY)   );
			gListaConjuntosMapasJuego[i].TexturaDescripcion = RenderizaTextoEnTextura( gListaConjuntosMapasJuego[i].Descripcion, gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[i].TexturaDescripcion_DimX), &(gListaConjuntosMapasJuego[i].TexturaDescripcion_DimY)   );
			sprintf(cadena_numeracion, "%d/%d\0", i+1, gNumeroConjuntosMapas );
			gListaConjuntosMapasJuego[i].TexturaNumeracion = RenderizaTextoEnTextura( cadena_numeracion, gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[i].TexturaNumeracion_DimX), &(gListaConjuntosMapasJuego[i].TexturaNumeracion_DimY)   );
		}

		// Boton 0 - Flecha izquierda --- Left arrow
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[0].identificador_boton = boton_conj_mapas_flechaizq;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[0].clase_boton = boton_pulsar;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[0].borde_izquierdo = 0.1*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[0].borde_derecho = 0.2*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[0].borde_arriba = 0.45*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[0].borde_abajo = 0.55*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[0].textura_boton_reposo = gWhiteLeftArrow;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[0].textura_boton_seleccionado = gYellowLeftArrow;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[0].boton_anterior = 2;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[0].boton_siguiente = 2;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[0].ModoRepresentacion = con_relac_aspecto_no_rellenar;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[0].textura_dim_x = gDimWhiteLeftArrowX;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[0].textura_dim_y = gDimWhiteLeftArrowY;

		// Boton 1 - Flecha derecha --- Right arrow
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[1].identificador_boton = boton_conj_mapas_flechader;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[1].clase_boton = boton_pulsar;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[1].borde_izquierdo = 0.8*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[1].borde_derecho = 0.9*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[1].borde_arriba = 0.45*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[1].borde_abajo = 0.55*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[1].textura_boton_reposo = gWhiteRightArrow;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[1].textura_boton_seleccionado = gYellowRightArrow;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[1].boton_anterior = 2;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[1].boton_siguiente = 2;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[1].ModoRepresentacion = con_relac_aspecto_no_rellenar;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[1].textura_dim_x = gDimWhiteRightArrowX;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[1].textura_dim_y = gDimWhiteRightArrowY;

		// Boton 2 - Seleccionar conjunto de mapas --- Select level set

		if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[2]) , 5 /*pos 5, no 2*/, boton_conj_mapas_seleccionar, boton_pulsar, "   Seleccionar   \0" , 4, 4  , con_relac_aspecto_no_rellenar) )
		{
			printf( "Error al inicializar boton 2 en seleccion de conjunto de niveles.\n" ); 
			success = false;
		}

		// Boton 3 - Imagen conjunto de mapas --- Level set image
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].identificador_boton = boton_conj_mapas_imagen;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].clase_boton = boton_pulsar;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].borde_izquierdo = 0.55*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].borde_derecho = 0.75*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].borde_arriba = 0.3*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].borde_abajo = 0.7*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].textura_boton_reposo = gListaConjuntosMapasJuego[0].TexturaImagen;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].textura_boton_seleccionado = gListaConjuntosMapasJuego[0].TexturaImagen;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].boton_anterior = 2;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].boton_siguiente = 2;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].ModoRepresentacion = con_relac_aspecto_no_rellenar;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].textura_dim_x = gListaConjuntosMapasJuego[0].TexturaImagen_DimX;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].textura_dim_y = gListaConjuntosMapasJuego[0].TexturaImagen_DimY;


		// Boton 4 - Atras --- Go back
		if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[4]) , 6 /*pos 6, no 4*/, boton_conj_mapas_atras, boton_pulsar, "      Atras      \0" , 2, 2  , con_relac_aspecto_no_rellenar) )
		{
			printf( "Error al inicializar boton 4 en seleccion de conjunto de niveles.\n" ); 
			success = false;
		}

		// Boton 5 - Nombre conjunto de mapas --- Level set name
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].identificador_boton = boton_conj_mapas_directorio;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].clase_boton = boton_pulsar;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].borde_izquierdo = 0.25*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].borde_derecho = 0.50*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].borde_arriba = 0.3*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].borde_abajo = 0.35*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].textura_boton_reposo = gListaConjuntosMapasJuego[0].TexturaNombreDir;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].textura_boton_seleccionado = gListaConjuntosMapasJuego[0].TexturaNombreDir;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].boton_anterior = 2;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].boton_siguiente = 2;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].ModoRepresentacion = con_relac_aspecto_no_rellenar;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].textura_dim_x = gListaConjuntosMapasJuego[0].TexturaNombreDir_DimX;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].textura_dim_y = gListaConjuntosMapasJuego[0].TexturaNombreDir_DimY;

		// Boton 6 - Descripcion conjunto de mapas --- Level set description
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].identificador_boton = boton_conj_mapas_descripcion;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].clase_boton = boton_pulsar;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].borde_izquierdo = 0.25*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].borde_derecho = 0.50*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].borde_arriba = 0.5*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].borde_abajo = 0.55*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].textura_boton_reposo = gListaConjuntosMapasJuego[0].TexturaDescripcion;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].textura_boton_seleccionado = gListaConjuntosMapasJuego[0].TexturaDescripcion;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].boton_anterior = 2;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].boton_siguiente = 2;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].ModoRepresentacion = con_relac_aspecto_no_rellenar;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].textura_dim_x = gListaConjuntosMapasJuego[0].TexturaDescripcion_DimX;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].textura_dim_y = gListaConjuntosMapasJuego[0].TexturaDescripcion_DimY;

		// Boton 7 - Numeracion conjunto de mapas --- Level set numbering
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].identificador_boton = boton_conj_mapas_numeracion;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].clase_boton = boton_pulsar;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].borde_izquierdo = 0.35*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].borde_derecho = 0.45*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].borde_arriba = 0.65*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].borde_abajo = 0.7*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].textura_boton_reposo = gListaConjuntosMapasJuego[0].TexturaNumeracion;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].textura_boton_seleccionado = gListaConjuntosMapasJuego[0].TexturaNumeracion;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].boton_anterior = 2;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].boton_siguiente = 2;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].ModoRepresentacion = con_relac_aspecto_no_rellenar;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].textura_dim_x = gListaConjuntosMapasJuego[0].TexturaNumeracion_DimX;
		pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].textura_dim_y = gListaConjuntosMapasJuego[0].TexturaNumeracion_DimY;
	}



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla selección de nivel (dentro de un conjunto de niveles) --- Level selection screen (inside a level set)
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pantalla selección de nivel
	// Esta pantalla no se parece a ninguna de las anteriores, por lo que la configuro a mano
	// This screen is different to all other screens, we configure it manually
	#ifdef DEBUG_INFO
	printf("Inicializando datos de pantalla seleccion de nivel...\n");
	#endif
	if( !inicializa_datos_pantalla_menu( &(pantallas_menu_principal[menu_elige_nivel]), menu_elige_nivel, "Seleccion  nivel\0", "images/pink_neon_grid.jpg", 12, con_relac_aspecto_no_rellenar ) )
	{
		printf( "Error al inicializar pantalla seleccion de nivel.\n" ); 
		success = false;
	}

	lista_mapas_ok = true;
	// Leemos todos los ficheros level list (uno por cada conjunto)
	// We read all level list files (one file per level set)
	for ( i = 0 ; i < gNumeroConjuntosMapas ; i++ )
	{
		gListaConjuntosMapasJuego[i].mapas_individuales = LeerInfoMapasDeConjunto( gListaConjuntosMapasJuego[i].Directorio , &(gListaConjuntosMapasJuego[i].Numero_Mapas) );
		if ( gListaConjuntosMapasJuego[i].mapas_individuales == NULL || gListaConjuntosMapasJuego[i].Numero_Mapas == 0 )
		{
			printf( "Ha sucedido algo inesperado al leer la lista de mapas del conjunto %s.\n", gListaConjuntosMapasJuego[i].Directorio );
			printf( "Revise que el contenido de maps/%s/level_list es el correcto y que existe al menos un mapa declarado.\n", gListaConjuntosMapasJuego[i].Directorio);
			success = false;
			lista_mapas_ok = false;
		}
	} 


	if (lista_mapas_ok == true)
	{
		// Cargamos las imágenes y texturas de textos asociadas a cada mapa
		// Load all images and textures of texts for each map
		#ifdef DEBUG_INFO
		printf("Cargamos las imágenes y texturas de textos asociadas a cada mapa...\n" );
		#endif
		for ( conjunto_actual = 0 ; conjunto_actual < gNumeroConjuntosMapas ; conjunto_actual++ )
		{
			for ( mapa_actual = 0; mapa_actual < gListaConjuntosMapasJuego[conjunto_actual].Numero_Mapas ; mapa_actual++ )
			{
				char cadena_numeracion[10];
				char cadena_ang_max[30];
				char cadena_gravedad[30];
				char cadena_tiempo[30];
				if ( gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].DatosMapa.RutaImagenDescMenu_Presente )
				{
					// Carga las imagenes de descripcion del mapa
					// Load all description images for each map
					#ifdef DEBUG_INFO
					printf("Cargando textura %s, conjunto %d, mapa %d ...\n", gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].DatosMapa.RutaImagenDescMenu, conjunto_actual, mapa_actual );
					#endif
					gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaImagenMapa = CargaTextura( gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].DatosMapa.RutaImagenDescMenu, &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaImagenMapa_DimX), &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaImagenMapa_DimY), true );
					if ( gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaImagenMapa == NULL )
					{
						printf("Error al cargar la textura %s \n", gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].DatosMapa.RutaImagenDescMenu );
						success = false;
					}
				}
				else  /*Imagen de descripción no presente --- Level description image not present */
				{
					// Cargamos la imagen genérica --- Load generic image
					#ifdef DEBUG_INFO
					printf("Cargando textura %s, conjunto %d, mapa %d ...\n", "images/level_desc/GenericMap.png", conjunto_actual, mapa_actual );
					#endif
					gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaImagenMapa = CargaTextura( "images/level_desc/GenericMap.png", &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaImagenMapa_DimX), &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaImagenMapa_DimY), true );
					if ( gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaImagenMapa == NULL )
					{
						printf("Error al cargar la textura %s \n", gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].DatosMapa.RutaImagenDescMenu );
						success = false;
					}
				}

				// Generamos las texturas asociadas a los textos de los mapas
				// Create all textures related to map texts
				gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaNombreMapa = RenderizaTextoEnTextura( gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].DatosMapa.NombreMapa, gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaNombreMapa_DimX), &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaNombreMapa_DimY)   );
				gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaDescripcionMapa = RenderizaTextoEnTextura( gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].DatosMapa.DescripcionMapa, gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaDescripcionMapa_DimX), &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaDescripcionMapa_DimY)   );
				sprintf(cadena_numeracion, "%d/%d\0", mapa_actual+1, gListaConjuntosMapasJuego[conjunto_actual].Numero_Mapas );
				gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaNumeracion = RenderizaTextoEnTextura( cadena_numeracion, gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaNumeracion_DimX), &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaNumeracion_DimY)   );	
				// Textura asociada al modo de giro
				// Texture for rotation mode text
				switch ( gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].DatosMapa.TipoGiro )
				{
					case camara:
						gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro = RenderizaTextoEnTextura( "Giro alred. camara", gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro_DimX), &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro_DimY)   );
						break;
					case punto_fijo:
						gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro = RenderizaTextoEnTextura( "Giro alred. pto fijo", gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro_DimX), &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro_DimY)   );
						break;
					case moneda:
						gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro = RenderizaTextoEnTextura( "Giro alred. moneda", gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro_DimX), &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro_DimY)    );
						break;
					case origen:
						gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro = RenderizaTextoEnTextura( "Giro alred. origen", gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro_DimX), &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro_DimY)   );
						break;
					case no_rot:
						gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro = RenderizaTextoEnTextura( "Sin giro.", gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro_DimX), &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro_DimY)    );
						break;
					default:
						gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro = RenderizaTextoEnTextura( "Error: Giro desc.", gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro_DimX), &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro_DimY)   );
						break;
				}
				if ( gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].DatosMapa.AnguloMax == 0 )
				{
					gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaAnguloMaximo = RenderizaTextoEnTextura( "Angulo max = inf", gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaAnguloMaximo_DimX), &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaAnguloMaximo_DimY)   );
				}
				else
				{
					sprintf( cadena_ang_max, "Angulo max = %f\0", gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].DatosMapa.AnguloMax );
					gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaAnguloMaximo = RenderizaTextoEnTextura( cadena_ang_max, gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaAnguloMaximo_DimX), &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaAnguloMaximo_DimY)   );
				}
				// Textura gravedad
				// Gravity text texture
				sprintf( cadena_gravedad, "Gravedad: %f\0", gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].DatosMapa.Gravedad );
				gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaGravedad = RenderizaTextoEnTextura( cadena_gravedad, gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaGravedad_DimX), &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaGravedad_DimY)   );
				// Textura tiempo
				// Time (countdown) text texture
				if ( gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].DatosMapa.CuentaAtras )
				{
					sprintf( cadena_tiempo, "Tiempo: %d segundos\0", gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].DatosMapa.SegundosCuentaAtras );
					gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaTiempo = RenderizaTextoEnTextura( cadena_tiempo, gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaTiempo_DimX), &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaTiempo_DimY)   );
				}
				else
				{
					gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaTiempo = RenderizaTextoEnTextura( "Sin cuenta atras\0", gFuenteTexto, gColorTextoReposo, &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaTiempo_DimX), &(gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaTiempo_DimY)   );
				}
			}
		}

		// Boton 0 -- Flecha izquierda --- Left arrow
		#ifdef DEBUG_INFO
		printf("Inicializando datos boton 0 de pantalla seleccion nivel...\n");
		#endif
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[0].identificador_boton = boton_mapa_flechaizq;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[0].clase_boton = boton_pulsar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[0].borde_izquierdo = 0.1*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[0].borde_derecho = 0.2*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[0].borde_arriba = 0.45*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[0].borde_abajo = 0.55*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[0].textura_boton_reposo = gWhiteLeftArrow;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[0].textura_boton_seleccionado = gYellowLeftArrow;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[0].boton_anterior = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[0].boton_siguiente = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[0].ModoRepresentacion = con_relac_aspecto_no_rellenar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[0].textura_dim_x = gDimWhiteLeftArrowX;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[0].textura_dim_y = gDimWhiteLeftArrowY;

		// Boton 1 -- Flecha derecha --- Right arrow
		#ifdef DEBUG_INFO
		printf("Inicializando datos boton 1 de pantalla seleccion nivel...\n");
		#endif
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[1].identificador_boton = boton_mapa_flechader;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[1].clase_boton = boton_pulsar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[1].borde_izquierdo = 0.8*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[1].borde_derecho = 0.9*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[1].borde_arriba = 0.45*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[1].borde_abajo = 0.55*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[1].textura_boton_reposo = gWhiteRightArrow;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[1].textura_boton_seleccionado = gYellowRightArrow;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[1].boton_anterior = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[1].boton_siguiente = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[1].ModoRepresentacion = con_relac_aspecto_no_rellenar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[1].textura_dim_x = gDimWhiteRightArrowX;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[1].textura_dim_y = gDimWhiteRightArrowY;

		// Boton 2 -- Jugar --- Play selected level
		#ifdef DEBUG_INFO
		printf("Inicializando datos boton 2 de pantalla seleccion nivel...\n");
		#endif
		if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_elige_nivel].botones_pantalla[2]) , 5 /*pos 5, no 2*/, boton_mapa_jugar, boton_pulsar, "      Jugar      \0" , 4, 4 , con_relac_aspecto_no_rellenar ) )
		{
			printf( "Error al inicializar boton 2 en seleccion de nivel.\n" ); 
			success = false;
		}

		// Boton 3 -- Imagen Mapa --- Level image
		#ifdef DEBUG_INFO
		printf("Inicializando datos boton 3 de pantalla seleccion nivel...\n");
		#endif
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].identificador_boton = boton_mapa_imagen;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].clase_boton = boton_pulsar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].borde_izquierdo = 0.55*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].borde_derecho = 0.75*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].borde_arriba = 0.3*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].borde_abajo = 0.7*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].textura_boton_reposo = NULL;  /* TODO 12/4/2020 */
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].textura_boton_seleccionado = NULL; /* TODO 12/4/2020 */
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].boton_anterior = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].boton_siguiente = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].ModoRepresentacion = con_relac_aspecto_no_rellenar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].textura_dim_x = 0;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].textura_dim_y = 0;

		// Boton 4 -- Atras --- Go back
		#ifdef DEBUG_INFO
		printf("Inicializando datos boton 4 de pantalla seleccion nivel...\n");
		#endif
		if (!inicializa_datos_boton ( &(pantallas_menu_principal[menu_elige_nivel].botones_pantalla[4]) , 6 /*pos 6, no 4*/, boton_mapa_atras, boton_pulsar, "      Atras      \0" , 2, 2 , con_relac_aspecto_no_rellenar ) )
		{
			printf( "Error al inicializar boton 4 en seleccion de nivel.\n" ); 
			success = false;
		}

		// Boton 5 -- Nombre Mapa --- Level name
		#ifdef DEBUG_INFO
		printf("Inicializando datos boton 5 de pantalla seleccion nivel...\n");
		#endif
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].identificador_boton = boton_mapa_nombre;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].clase_boton = boton_pulsar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].borde_izquierdo = 0.25*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].borde_derecho = 0.50*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].borde_arriba = 0.3*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].borde_abajo = 0.35*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].textura_boton_reposo = NULL;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].textura_boton_seleccionado = NULL;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].boton_anterior = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].boton_siguiente = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].ModoRepresentacion = con_relac_aspecto_no_rellenar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].textura_dim_x = 0;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].textura_dim_y = 0;

		// Boton 6 -- Descrpción del mapa --- Level description
		#ifdef DEBUG_INFO
		printf("Inicializando datos boton 6 de pantalla seleccion nivel...\n");
		#endif
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].identificador_boton = boton_mapa_descripcion;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].clase_boton = boton_pulsar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].borde_izquierdo = 0.25*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].borde_derecho = 0.50*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].borde_arriba = 0.35*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].borde_abajo = 0.4*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].textura_boton_reposo = NULL;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].textura_boton_seleccionado = NULL;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].boton_anterior = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].boton_siguiente = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].ModoRepresentacion = con_relac_aspecto_no_rellenar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].textura_dim_x = 0;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].textura_dim_y = 0;

		// Boton 7 -- Modo de giro --- Rotation mode
		#ifdef DEBUG_INFO
		printf("Inicializando datos boton 7 de pantalla seleccion nivel...\n");
		#endif
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].identificador_boton = boton_mapa_modogiro;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].clase_boton = boton_pulsar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].borde_izquierdo = 0.25*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].borde_derecho = 0.50*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].borde_arriba = 0.4*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].borde_abajo = 0.45*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].textura_boton_reposo = NULL;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].textura_boton_seleccionado = NULL;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].boton_anterior = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].boton_siguiente = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].ModoRepresentacion = con_relac_aspecto_no_rellenar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].textura_dim_x = 0;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].textura_dim_y = 0;

		// Boton 8 -- Angulo máximo --- maximum angle
		#ifdef DEBUG_INFO
		printf("Inicializando datos boton 8 de pantalla seleccion nivel...\n");
		#endif
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].identificador_boton = boton_mapa_angmax;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].clase_boton = boton_pulsar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].borde_izquierdo = 0.25*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].borde_derecho = 0.50*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].borde_arriba = 0.45*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].borde_abajo = 0.5*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].textura_boton_reposo = NULL;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].textura_boton_seleccionado = NULL;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].boton_anterior = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].boton_siguiente = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].ModoRepresentacion = con_relac_aspecto_no_rellenar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].textura_dim_x = 0;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].textura_dim_y = 0;

		// Boton 9 -- Gravedad --- Gravity
		#ifdef DEBUG_INFO
		printf("Inicializando datos boton 9 de pantalla seleccion nivel...\n");
		#endif
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].identificador_boton = boton_mapa_gravedad;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].clase_boton = boton_pulsar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].borde_izquierdo = 0.25*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].borde_derecho = 0.50*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].borde_arriba = 0.5*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].borde_abajo = 0.55*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].textura_boton_reposo = NULL;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].textura_boton_seleccionado = NULL;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].boton_anterior = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].boton_siguiente = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].ModoRepresentacion = con_relac_aspecto_no_rellenar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].textura_dim_x = 0;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].textura_dim_y = 0;

		// Boton 10 -- Tiempo cuenta atras -- Count down time
		#ifdef DEBUG_INFO
		printf("Inicializando datos boton 10 de pantalla seleccion nivel...\n");
		#endif
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].identificador_boton = boton_mapa_tiempo;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].clase_boton = boton_pulsar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].borde_izquierdo = 0.25*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].borde_derecho = 0.50*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].borde_arriba = 0.55*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].borde_abajo = 0.6*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].textura_boton_reposo = NULL;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].textura_boton_seleccionado = NULL;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].boton_anterior = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].boton_siguiente = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].ModoRepresentacion = con_relac_aspecto_no_rellenar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].textura_dim_x = 0;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].textura_dim_y = 0;

		// Boton 11 -- Numeración --- Numbering (ex. 7/9)
		#ifdef DEBUG_INFO
		printf("Inicializando datos boton 11 de pantalla seleccion nivel...\n");
		#endif
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].identificador_boton = boton_mapa_numeracion;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].clase_boton = boton_pulsar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].borde_izquierdo = 0.35*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].borde_derecho = 0.45*(opciones_juego.screen_x_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].borde_arriba = 0.6*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].borde_abajo = 0.65*(opciones_juego.screen_y_resolution);
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].textura_boton_reposo = NULL;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].textura_boton_seleccionado = NULL;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].boton_anterior = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].boton_siguiente = 2;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].ModoRepresentacion = con_relac_aspecto_no_rellenar;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].textura_dim_x = 0;
		pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].textura_dim_y = 0;
	}




	//-----------------------------------------------------------------------------------------
	// Carga sonidos menu --- Load menu sounds
	gMusicaMenu = Mix_LoadMUS( "music/TengoUnaVacaLechera(Instrumental).mp3" );
	if( gMusicaMenu == NULL )
	{
	    printf( "Error al cargar la musica del menu. SDL_mixer Error: %s\n", Mix_GetError() );
	    //success = false;
	}
	gSonidoClickBoton = Mix_LoadWAV( "sound/button_2.wav" );
	if( gSonidoClickBoton == NULL )
	{
	    printf( "Error al cargar sonido clic boton. SDL_mixer Error: %s\n", Mix_GetError() );
	    success = false;
	}

	return success;
}

//  _     _ _
// | |   (_) |__   ___ _ __ __ _ _ __   _ __ ___   ___ _ __ ___
// | |   | | '_ \ / _ \ '__/ _` | '__| | '_ ` _ \ / _ \ '_ ` _ \
// | |___| | |_) |  __/ | | (_| | |    | | | | | |  __/ | | | | |
// |_____|_|_.__/ \___|_|  \__,_|_|    |_| |_| |_|\___|_| |_| |_|
//  _____
// |  ___| __ ___  ___   _ __ ___   ___ _ __ ___   ___  _ __ _   _
// | |_ | '__/ _ \/ _ \ | '_ ` _ \ / _ \ '_ ` _ \ / _ \| '__| | | |
// |  _|| | |  __/  __/ | | | | | |  __/ | | | | | (_) | |  | |_| |
// |_|  |_|  \___|\___| |_| |_| |_|\___|_| |_| |_|\___/|_|   \__, |
//                                                           |___/

bool liberar_memoria_menu_principal( /*struct pantalla_menu* pantallas_menu_principal, */ int numero_pantallas ) 		// Free main menu memory
{
	#ifdef DEBUG_INFO
	printf("Comenzando a liberar memoria del menu...\n");
	#endif 

	// Recorrer con un bucle for liberando la memoria de los botones --- Loop to free memory buttons
	#ifdef DEBUG_INFO
	printf("Comenzando a liberar memoria de los botones...\n");
	#endif
	int menu_actual, boton_actual, conjunto_actual, mapa_actual;
	for ( menu_actual=0 ; menu_actual < numero_pantallas ; menu_actual++ )
	{
		for ( boton_actual=0 ; boton_actual < pantallas_menu_principal[menu_actual].numero_botones ;  boton_actual++ )
		{
			#ifdef DEBUG_INFO
			printf("Liberando memoria de texturas de boton %d de menu %d...\n", boton_actual, menu_actual );
			#endif
			//Free loaded image 
			SDL_DestroyTexture( pantallas_menu_principal[menu_actual].botones_pantalla[boton_actual].textura_boton_reposo ); 
			pantallas_menu_principal[menu_actual].botones_pantalla[boton_actual].textura_boton_reposo = NULL; 
			SDL_DestroyTexture( pantallas_menu_principal[menu_actual].botones_pantalla[boton_actual].textura_boton_seleccionado ); 
			pantallas_menu_principal[menu_actual].botones_pantalla[boton_actual].textura_boton_seleccionado = NULL; 
		}
		#ifdef DEBUG_INFO
		printf("Liberando memoria de texturas titulo de menu %d...\n", menu_actual );
		#endif
		//Free loaded image 
		SDL_DestroyTexture( pantallas_menu_principal[menu_actual].textura_titulo ); 
		pantallas_menu_principal[menu_actual].textura_titulo = NULL; 
		#ifdef DEBUG_INFO
		printf("Liberando memoria de array de botones de menu %d...\n", menu_actual );
		#endif
		// free( pantallas_menu_principal->botones_pantalla );
		free( pantallas_menu_principal[menu_actual].botones_pantalla );
	}
	#ifdef DEBUG_INFO
	printf("Liberando memoria de array de menus...\n" );
	#endif
	free ( pantallas_menu_principal );

	// Liberar conjuntos de mapas e informacion de mapas --- Free level-sets and level information
	#ifdef DEBUG_INFO
	printf("Comenzando a liberar memoria de los conjuntos de mapas e informacion de mapas...\n");
	#endif
	for ( conjunto_actual = 0 ; conjunto_actual < gNumeroConjuntosMapas ; conjunto_actual++ )
	{
		#ifdef DEBUG_INFO
		printf("Liberando textura %s...\n", gListaConjuntosMapasJuego[conjunto_actual].RutaImagen);
		#endif
		SDL_DestroyTexture(  gListaConjuntosMapasJuego[conjunto_actual].TexturaImagen ); gListaConjuntosMapasJuego[conjunto_actual].TexturaImagen = NULL;
		#ifdef DEBUG_INFO
		printf("Liberando texturas textos de conjunto %d...\n", conjunto_actual);
		#endif
		SDL_DestroyTexture(  gListaConjuntosMapasJuego[conjunto_actual].TexturaNombreDir ); gListaConjuntosMapasJuego[conjunto_actual].TexturaNombreDir = NULL;
		SDL_DestroyTexture(  gListaConjuntosMapasJuego[conjunto_actual].TexturaDescripcion ); gListaConjuntosMapasJuego[conjunto_actual].TexturaDescripcion = NULL;
		SDL_DestroyTexture(  gListaConjuntosMapasJuego[conjunto_actual].TexturaNumeracion );  gListaConjuntosMapasJuego[conjunto_actual].TexturaNumeracion = NULL;

		// Liberamos información de los mapas --- Free level info
		for (mapa_actual = 0 ; mapa_actual < gListaConjuntosMapasJuego[conjunto_actual].Numero_Mapas ; mapa_actual++ )
		{
				#ifdef DEBUG_INFO
				printf("Liberando texturas textos de mapa %d de conjunto %d...\n", mapa_actual, conjunto_actual);
				#endif
				SDL_DestroyTexture(  gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaNombreMapa ); gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaNombreMapa = NULL;
				SDL_DestroyTexture(  gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaDescripcionMapa ); gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaDescripcionMapa = NULL;
				SDL_DestroyTexture(  gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaImagenMapa ); gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaImagenMapa = NULL;
				SDL_DestroyTexture(  gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro ); gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaModoGiro = NULL;
				SDL_DestroyTexture(  gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaAnguloMaximo ); gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaAnguloMaximo = NULL;
				SDL_DestroyTexture(  gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaGravedad ); gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaGravedad = NULL;
				SDL_DestroyTexture(  gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaTiempo ); gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaTiempo = NULL;
				SDL_DestroyTexture(  gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaNumeracion ); gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales[mapa_actual].TexturaNumeracion = NULL;
		}
		#ifdef DEBUG_INFO
		printf("Liberando memoria de array de mapas de conjunto %d...\n", conjunto_actual);
		#endif
		free ( gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales ); gListaConjuntosMapasJuego[conjunto_actual].mapas_individuales = NULL;
	} 
	#ifdef DEBUG_INFO
	printf("Liberando memoria de array de conjuntos..\n");
	#endif
	free( gListaConjuntosMapasJuego ); gListaConjuntosMapasJuego = NULL;

	// Liberar texturas generales del menú --- Free general menu textures
	#ifdef DEBUG_INFO
	printf("Liberando memoria de las texturas generales del menu...\n");
	#endif
	SDL_DestroyTexture( gWhiteCheckBoxChecked ); gWhiteCheckBoxChecked = NULL;
	SDL_DestroyTexture( gWhiteCheckBoxChecked ); gWhiteCheckBoxChecked = NULL;
	SDL_DestroyTexture( gWhiteArrowMinus ); gWhiteArrowMinus = NULL;
	SDL_DestroyTexture( gWhiteArrowPlus ); gWhiteArrowPlus = NULL;
	SDL_DestroyTexture( gWhiteLeftArrow ); gWhiteLeftArrow = NULL;
	SDL_DestroyTexture( gWhiteRightArrow ); gWhiteRightArrow = NULL;
	SDL_DestroyTexture( gYellowLeftArrow ); gYellowLeftArrow = NULL;
	SDL_DestroyTexture( gYellowRightArrow ); gYellowRightArrow = NULL;
	SDL_DestroyTexture( gTexturaNumeroMenu[0] ); gTexturaNumeroMenu[0] = NULL;
	SDL_DestroyTexture( gTexturaNumeroMenu[1] ); gTexturaNumeroMenu[1] = NULL;
	SDL_DestroyTexture( gTexturaNumeroMenu[2] ); gTexturaNumeroMenu[2] = NULL;
	SDL_DestroyTexture( gTexturaNumeroMenu[3] ); gTexturaNumeroMenu[3] = NULL;
	SDL_DestroyTexture( gTexturaNumeroMenu[4] ); gTexturaNumeroMenu[4] = NULL;
	SDL_DestroyTexture( gTexturaNumeroMenu[5] ); gTexturaNumeroMenu[5] = NULL;
	SDL_DestroyTexture( gTexturaNumeroMenu[6] ); gTexturaNumeroMenu[6] = NULL;
	SDL_DestroyTexture( gTexturaNumeroMenu[7] ); gTexturaNumeroMenu[7] = NULL;
	SDL_DestroyTexture( gTexturaNumeroMenu[8] ); gTexturaNumeroMenu[8] = NULL;
	SDL_DestroyTexture( gTexturaNumeroMenu[9] ); gTexturaNumeroMenu[9] = NULL;
	SDL_DestroyTexture( gTexturaPuntoMenu ); gTexturaPuntoMenu = NULL;
	SDL_DestroyTexture( gTexturaComaMenu ); gTexturaComaMenu = NULL;
	if ( gGameLogo != NULL ) {SDL_DestroyTexture( gGameLogo ); gGameLogo = NULL; }
	if ( gGameLogoCredits != NULL ) {SDL_DestroyTexture( gGameLogoCredits ); gGameLogoCredits = NULL; }	// (30/8/2021)

	// Liberar musica y sonidos --- Free music and sounds
	#ifdef DEBUG_INFO
	printf("Liberando memoria de musica y sonidos del menu...\n");
	#endif
	Mix_FreeMusic( gMusicaMenu ); gMusicaMenu = NULL;
	Mix_FreeChunk( gSonidoClickBoton); gSonidoClickBoton = NULL;

}

void bucle_principal_menu_principal( void )		// Main menu, main loop
{


	bool quit = false;	 //Main loop flag
	int contador_frames = 0;
	SDL_Event e;		 //Event handler 
	Uint32 currentTime = 0, lastTime, deltaTime, tiempo_imagen_sobrante; //Current time start time (milisegundos)
	float tiempo_imagen;	//Tiempo de cada imagen, en segundos --- Time duration for each frame, in seconds
	float framerate_deseado = 30;


	enum pantalla_menu_principal menu_activo;	// Menu activo actualmente y representado en la pantalla --- Currently active menu, displayed on screen
	int boton_seleccionado, i;
	bool algun_boton_seleccionado, boton_pulsado;

	int conjunto_niveles_seleccionado_actualmente = 0, nivel_seleccionado_actualmente = 0;

	/* Movido a variables globales
	struct pantalla_menu* pantallas_menu_principal;
	int numero_pantallas = 6;*/

	int pos_raton_x, pos_raton_y;

	SDL_Rect renderQuad;		// Variable "rectangulo SDL" para indicar donde renderizar los objetos --- SDL Rectangle variable, tells where to render objects


	//Activar variables ratón --- Set mouse variables
	SDL_ShowCursor(SDL_ENABLE);
	SDL_SetRelativeMouseMode(SDL_FALSE);

	menu_activo = menu_bienvenida;
	boton_seleccionado = 0;

	//While application is running 
	while( !quit ) 
	{
		// Calcular tiempo para el movimiento --- Calculate time for movements
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
						// Techa izquierda --- Left key
						if ( (menu_activo == menu_elige_conjunto_niveles) || (menu_activo == menu_elige_nivel) )
						{
							boton_seleccionado = 0;
						}
						break;
					case SDLK_RIGHT:
						// Tecla derecha --- Right key
						if ( (menu_activo == menu_elige_conjunto_niveles) || (menu_activo == menu_elige_nivel) )
						{
							boton_seleccionado = 1;
						}
						break;
					case SDLK_UP:
						// Techa arriba --- Up key
						if ( pantallas_menu_principal[menu_activo].numero_botones != 0)		// Para evitar violacion de segmento si el numero de botones es cero --- Avoid segmentation fault if number of buttons is zero
						{
							boton_seleccionado = pantallas_menu_principal[menu_activo].botones_pantalla[boton_seleccionado].boton_anterior;
						}
						break;
					case SDLK_DOWN:
						// Tecla abajo -- Down key
						if ( pantallas_menu_principal[menu_activo].numero_botones != 0)		// Para evitar violacion de segmento si el numero de botones es cero --- Avoid segmentation fault if number of buttons is zero
						{
							boton_seleccionado = pantallas_menu_principal[menu_activo].botones_pantalla[boton_seleccionado].boton_siguiente;
						}
						break;
					case SDLK_RETURN:
					case SDLK_KP_ENTER:
					case SDLK_SPACE:
						// Tecla enter o espacio --- Enter or space
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
					// Any key was pressed on the welcome menu
					menu_activo = menu_principal;
					boton_pulsado = false;	// Para que no entremos en un submenu --- To prevent entering into a submenu
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
					// Any key was pressed on the welcome menu
					menu_activo = menu_principal;
					boton_pulsado = false;	// Para que no entremos en un submenu --- To prevent entering into a submenu
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////
		// Gestiona las acciones del usuario --- Manage user actions
		if (( boton_pulsado == true) && (boton_seleccionado != -1) && (boton_seleccionado >= 0) && (boton_seleccionado <= pantallas_menu_principal[menu_activo].numero_botones) )
		{
			// Se ha pulsado algo, hay que hacer lo que corresponda
			// Some button has been pressed, the corresponding action has to be done
			switch ( pantallas_menu_principal[menu_activo].botones_pantalla[boton_seleccionado].identificador_boton )
			{

				case boton_jugar:
				case boton_mapa_atras:
					//menu_activo = menu_niveles_provisional;	// Para DEBUG
					menu_activo = menu_elige_conjunto_niveles;
					boton_seleccionado = 0;
					break;
				case boton_provisional_nivel_01:
					#ifdef DEBUG_INFO
					printf("Comenzando bucle principal del juego...\n");
					#endif
					bucle_principal_juego( "maps/monza_1" );
					//Activar variables ratón una vez el juego ha terminado --- Set mouse variables once the game has ended
					SDL_ShowCursor(SDL_ENABLE);
					SDL_SetRelativeMouseMode(SDL_FALSE);
					break;
				case boton_provisional_nivel_02:
					#ifdef DEBUG_INFO
					printf("Comenzando bucle principal del juego...\n");
					#endif
					bucle_principal_juego( "maps/laberinto01" );
					//Activar variables ratón una vez el juego ha terminado --- Set mouse variables once the game has ended
					SDL_ShowCursor(SDL_ENABLE);
					SDL_SetRelativeMouseMode(SDL_FALSE);
					break;
				case boton_provisional_nivel_03:
					#ifdef DEBUG_INFO
					printf("Comenzando bucle principal del juego...\n");
					#endif
					bucle_principal_juego( "maps/pyramid");
					//Activar variables ratón una vez el juego ha terminado --- Set mouse variables once the game has ended
					SDL_ShowCursor(SDL_ENABLE);
					SDL_SetRelativeMouseMode(SDL_FALSE);
					break;
				case boton_provisional_nivel_04:
					#ifdef DEBUG_INFO
					printf("Comenzando bucle principal del juego...\n");
					#endif
					bucle_principal_juego( "maps/round_labyrinth");
					//Activar variables ratón una vez el juego ha terminado --- Set mouse variables once the game has ended
					SDL_ShowCursor(SDL_ENABLE);
					SDL_SetRelativeMouseMode(SDL_FALSE);
					break;
				case boton_provisional_nivel_05:
					#ifdef DEBUG_INFO
					printf("Comenzando bucle principal del juego...\n");
					#endif
					bucle_principal_juego("maps/super_pinball");
					//Activar variables ratón una vez el juego ha terminado --- Set mouse variables once the game has ended
					SDL_ShowCursor(SDL_ENABLE);
					SDL_SetRelativeMouseMode(SDL_FALSE);
					break;
				case boton_salir:
					quit = true;
					break;
				case boton_opciones:
				case boton_opcvid_atras:
				case boton_opcsnd_atras:
				case boton_opcjgo_atras:
				case boton_opcctl_atras:
					menu_activo = menu_opciones;
					boton_seleccionado = 0;
					break;
				case boton_opc_atras:
				case boton_nivel_prov_atras:
				case boton_conj_mapas_atras:
				case boton_creditos_atras:	// (30/8/2021)
					menu_activo = menu_principal;
					boton_seleccionado = 0;
					break;
				case boton_creditos:	// (30/8/2021)
					menu_activo = menu_creditos;
					boton_seleccionado = 0;
					break;	
				case boton_video:
					menu_activo = menu_opc_video;
					boton_seleccionado = 0;
					break;
				case boton_sonido:
					menu_activo = menu_opc_sonido;
					boton_seleccionado = 0;
					break;
				case boton_opc_juego:
					menu_activo = menu_opc_juego;
					boton_seleccionado = 0;
					break;
				case boton_controles:
					menu_activo = menu_opc_control;
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
				case boton_activar_musica:
					opciones_juego.music_enabled = !opciones_juego.music_enabled;
					pantallas_menu_principal[menu_opc_sonido].botones_pantalla[0].estado_on_off = opciones_juego.music_enabled;
					break;
				case boton_activar_sonido:
					opciones_juego.sound_enabled = !opciones_juego.sound_enabled;
					pantallas_menu_principal[menu_opc_sonido].botones_pantalla[1].estado_on_off = opciones_juego.sound_enabled;
					break;
				case boton_moneda_puede_volar:
					opciones_juego.map_rot_makes_coin_fly = !opciones_juego.map_rot_makes_coin_fly;
					pantallas_menu_principal[menu_opc_juego].botones_pantalla[0].estado_on_off = opciones_juego.map_rot_makes_coin_fly;
					break;
				case boton_limitar_vel_moneda:
					opciones_juego.limit_coin_speed = !opciones_juego.limit_coin_speed;
					pantallas_menu_principal[menu_opc_juego].botones_pantalla[1].estado_on_off = opciones_juego.limit_coin_speed;
					break;
				case boton_sensibilidad_raton:
					if ( raton_sobre_boton_menos_valor_numerico( pantallas_menu_principal[menu_opc_control].botones_pantalla[0], pos_raton_x, pos_raton_y) )
					{
						// Se ha pulsado menos --- Minus has been pressed
						pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico -= pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico_incremento;
						if ( pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico < pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico_rango_minimo )
						{
							// Estamos por debajo del minimo, volvemos al minimo --- Below the minimum value, we return to the minimum
							pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico = pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico_rango_minimo;
						}
						opciones_juego.mouse_sensitivity = pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico;
					}
					if ( raton_sobre_boton_mas_valor_numerico( pantallas_menu_principal[menu_opc_control].botones_pantalla[0], pos_raton_x, pos_raton_y) )
					{
						// Se ha pulsado mas
						pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico += pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico_incremento;
						if ( pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico > pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico_rango_maximo )
						{
							// Estamos por encima del maximo, volvemos al maximo --- Beneath the maximum, we return to the maximum
							pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico = pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico_rango_maximo;
						}
						opciones_juego.mouse_sensitivity = pantallas_menu_principal[menu_opc_control].botones_pantalla[0].valor_numerico;
					}
					break;
				case boton_conj_mapas_flechaizq:
					conjunto_niveles_seleccionado_actualmente++;
					if ( conjunto_niveles_seleccionado_actualmente >= gNumeroConjuntosMapas )
					{
						conjunto_niveles_seleccionado_actualmente = 0;		// Se salimos por la derecha volvemos al principio --- We return to the first level set
					}
					// Actualizamos imagenes y texturas textos --- Update images and texts
					ActualizaPunteros_Menu_EligeConjuntoNiveles ( pantallas_menu_principal, conjunto_niveles_seleccionado_actualmente );
					break;
				case boton_conj_mapas_flechader:
					conjunto_niveles_seleccionado_actualmente--;
					if ( conjunto_niveles_seleccionado_actualmente < 0 )
					{
						conjunto_niveles_seleccionado_actualmente = (gNumeroConjuntosMapas-1);		// Si salimos por la izquierda vamos al máximo --- We go to the last level set
					}
					// Actualizamos imagenes y texturas textos --- Update images and texts
					ActualizaPunteros_Menu_EligeConjuntoNiveles ( pantallas_menu_principal, conjunto_niveles_seleccionado_actualmente );
					break;
				case boton_conj_mapas_seleccionar:
					menu_activo = menu_elige_nivel;
					// Actualizamos la informacion con el primer mapa del conjunto de niveles seleccionado --- Update information with the first level of the selected level-set
					nivel_seleccionado_actualmente = 0;
					boton_seleccionado = 0;
					ActualizaPunteros_Menu_EligeNivel (pantallas_menu_principal, conjunto_niveles_seleccionado_actualmente, nivel_seleccionado_actualmente );
					break;
				case boton_mapa_flechaizq:
					nivel_seleccionado_actualmente--;
					if ( nivel_seleccionado_actualmente < 0 )
					{
						nivel_seleccionado_actualmente = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].Numero_Mapas -1 ;	// Si salimos por la izquierda vamos al máximo --- We go the the last level
					}
					ActualizaPunteros_Menu_EligeNivel (pantallas_menu_principal, conjunto_niveles_seleccionado_actualmente, nivel_seleccionado_actualmente );
					break;
				case boton_mapa_flechader:
					nivel_seleccionado_actualmente++;
					if ( nivel_seleccionado_actualmente >= gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].Numero_Mapas )
					{
						nivel_seleccionado_actualmente = 0;	// Si salimos por la derecha volvemos al principio --- We go to the first level
					}
					ActualizaPunteros_Menu_EligeNivel (pantallas_menu_principal, conjunto_niveles_seleccionado_actualmente, nivel_seleccionado_actualmente );
					break;
				case boton_mapa_jugar:
					#ifdef DEBUG_INFO
					printf("Comenzando bucle principal del juego...\n");
					#endif
					bucle_principal_juego( gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].RutaMapa );
					//Activar variables ratón una vez el juego ha terminado --- Set mouse variables once the game has ended
					SDL_ShowCursor(SDL_ENABLE);
					SDL_SetRelativeMouseMode(SDL_FALSE);
					break;
				default:
					// No hacemos nada --- Do nothing
					break;
			}
			// Reproducimos sonido de clic boton --- Play button click sound
			if ( opciones_juego.sound_enabled )
			{
				Mix_PlayChannel( -1, gSonidoClickBoton, 0 );
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
		/*renderQuad.x = pantallas_menu_principal[menu_activo].borde_izquierdo_titulo; 									// Coord X esquina superior izquierda 
		renderQuad.y = pantallas_menu_principal[menu_activo].borde_arriba_titulo; 										// Coord Y esquina superior izqeuirda 
		renderQuad.w = pantallas_menu_principal[menu_activo].borde_derecho_titulo - pantallas_menu_principal[menu_bienvenida].borde_izquierdo_titulo ; 	// Ancho 
		renderQuad.h = pantallas_menu_principal[menu_activo].borde_abajo_titulo - pantallas_menu_principal[menu_bienvenida].borde_arriba_titulo;		// Alto 
		SDL_RenderCopy( gRenderer, pantallas_menu_principal[menu_activo].textura_titulo, NULL, &renderQuad ); */

		RepresentaTextura ( gRenderer, pantallas_menu_principal[menu_activo].textura_titulo, 
					pantallas_menu_principal[menu_activo].textura_titulo_dim_x , pantallas_menu_principal[menu_activo].textura_titulo_dim_y , 
					pantallas_menu_principal[menu_activo].borde_izquierdo_titulo , pantallas_menu_principal[menu_activo].borde_derecho_titulo , pantallas_menu_principal[menu_bienvenida].borde_arriba_titulo, pantallas_menu_principal[menu_activo].borde_abajo_titulo, 
					pantallas_menu_principal[menu_activo].ModoRepresentacion ); 	// Nuevo 19/4/2020

		// Dibuja los botones del menu
		if ( pantallas_menu_principal[menu_activo].numero_botones != 0 )
		{
			for ( i = 0 ; i<= pantallas_menu_principal[menu_activo].numero_botones -1 ; i++ )
			{
				switch ( pantallas_menu_principal[menu_activo].botones_pantalla[i].clase_boton ) 
				{
					case boton_pulsar:
						/*renderQuad.x = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo; 									// Coord X esquina superior izquierda 
						renderQuad.y = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba; 										// Coord Y esquina superior izqeuirda 
						renderQuad.w = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_derecho - pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo; 	// Ancho 
						renderQuad.h = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_abajo - pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba;*/
						if ( boton_seleccionado == i )
						{
							// SDL_RenderCopy( gRenderer, pantallas_menu_principal[menu_activo].botones_pantalla[i].textura_boton_seleccionado, NULL, &renderQuad );
							RepresentaTextura ( gRenderer, pantallas_menu_principal[menu_activo].botones_pantalla[i].textura_boton_seleccionado, 
										pantallas_menu_principal[menu_activo].botones_pantalla[i].textura_dim_x , pantallas_menu_principal[menu_activo].botones_pantalla[i].textura_dim_y , 
										pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo , pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_derecho , pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba, pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_abajo, 
										pantallas_menu_principal[menu_activo].botones_pantalla[i].ModoRepresentacion ); 	// Nuevo 19/4/2020
						}
						else
						{
							//SDL_RenderCopy( gRenderer, pantallas_menu_principal[menu_activo].botones_pantalla[i].textura_boton_reposo, NULL, &renderQuad );
							RepresentaTextura ( gRenderer, pantallas_menu_principal[menu_activo].botones_pantalla[i].textura_boton_reposo, 
										pantallas_menu_principal[menu_activo].botones_pantalla[i].textura_dim_x , pantallas_menu_principal[menu_activo].botones_pantalla[i].textura_dim_y , 
										pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo , pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_derecho , pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba, pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_abajo, 
										pantallas_menu_principal[menu_activo].botones_pantalla[i].ModoRepresentacion ); 	// Nuevo 19/4/2020
						}
						break;
					case boton_conmutar:
						renderQuad.x = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo + (int) (opciones_juego.screen_y_resolution) * PORCENTAJE_ALTO_BOTON/100 /*Para dejar hueco al checkbox*/; 									/* Coord X esquina superior izquierda */
						renderQuad.y = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba; 										/* Coord Y esquina superior izqeuirda */
						renderQuad.w = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_derecho - pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo - (int) (opciones_juego.screen_y_resolution) * PORCENTAJE_ALTO_BOTON/100 /*Para dejar hueco al checkbox*/; 	/* Ancho */
						renderQuad.h = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_abajo - pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba;
						if ( boton_seleccionado == i )
						{
							SDL_RenderCopy( gRenderer, pantallas_menu_principal[menu_activo].botones_pantalla[i].textura_boton_seleccionado, NULL, &renderQuad );
						}
						else
						{
							SDL_RenderCopy( gRenderer, pantallas_menu_principal[menu_activo].botones_pantalla[i].textura_boton_reposo, NULL, &renderQuad );
						}
						// Dibujar caja que indique el estado de la seleccion --- Draw tickbox
						renderQuad.w =  (int) (opciones_juego.screen_y_resolution) * PORCENTAJE_ALTO_BOTON/100 ; 		/* Ancho */ /* Ancho igual que alto de boton --- Tickbox width equal to button height */
						renderQuad.x = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo /*- renderQuad.w*/; 			/* Coord X esquina superior izquierda */
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
					case boton_valor_numerico:		// Pruebas 6/4/2020
						// Dibujar texto --- Draw text
						renderQuad.x = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo; 									/* Coord X esquina superior izquierda */
						renderQuad.y = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba; 										/* Coord Y esquina superior izqeuirda */
						renderQuad.w = ( pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_derecho - pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo ) * PORCNT_ANCHO_TITULO_BOT_VAL_NUM/100; 	/* Ancho */
						renderQuad.h = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_abajo - pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba;
						if ( boton_seleccionado == i )
						{
							SDL_RenderCopy( gRenderer, pantallas_menu_principal[menu_activo].botones_pantalla[i].textura_boton_seleccionado, NULL, &renderQuad );
						}
						else
						{
							SDL_RenderCopy( gRenderer, pantallas_menu_principal[menu_activo].botones_pantalla[i].textura_boton_reposo, NULL, &renderQuad );
						}
						// Dibujar valor numérico --- Draw numaric value
						// (TODO)
						{
							// Creamos un entero desplazando hacia la izquierda todo (sin coma)
							// Create an integer, moving the decimal point to the right (no decimal point)
							long entero_auxiliar_para_valor_numerico = pantallas_menu_principal[menu_activo].botones_pantalla[i].valor_numerico * pow(10, pantallas_menu_principal[menu_activo].botones_pantalla[i].valor_numerico_digitos_dcha_coma) ;
							int total_digitos; /*Incluida la coma*/
							int digito_actual, posicion_actual;

							total_digitos = pantallas_menu_principal[menu_activo].botones_pantalla[i].valor_numerico_digitos_izq_coma + pantallas_menu_principal[menu_activo].botones_pantalla[i].valor_numerico_digitos_dcha_coma + 1;
							posicion_actual = 0;
							// Representacion_digitos a la izquierda de la coma -- Display digits on the left of the decimal point
							for ( digito_actual = ( pantallas_menu_principal[menu_activo].botones_pantalla[i].valor_numerico_digitos_izq_coma + pantallas_menu_principal[menu_activo].botones_pantalla[i].valor_numerico_digitos_dcha_coma) - 1; digito_actual >= pantallas_menu_principal[menu_activo].botones_pantalla[i].valor_numerico_digitos_dcha_coma ; digito_actual-- )
							{
								// Posicionamos el digito --- Position the digit
								renderQuad.x = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo + (opciones_juego.screen_x_resolution * (PORCENTAJE_ANCHO_BOTONES/100)*(PORCNT_ANCHO_TITULO_BOT_VAL_NUM)/100 ) + posicion_actual*(opciones_juego.screen_x_resolution) * (PORCENTAJE_ANCHO_BOTONES/100) * (PORCNT_ANCHO_VALOR_BOT_VAL_NUM/100) /total_digitos;		// Coord X de esquina superior izquierda
								renderQuad.y = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba;		// Coord Y de esquina superior izquierda
								renderQuad.w = (opciones_juego.screen_x_resolution) * (PORCENTAJE_ANCHO_BOTONES/100) * (PORCNT_ANCHO_VALOR_BOT_VAL_NUM/100) /total_digitos;				// Ancho
								renderQuad.h = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_abajo - pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba;				// Alto
								// Calculamos y a la vez dibujamos el dígito --- Calculate and display the digit
								SDL_RenderCopy( gRenderer, gTexturaNumeroMenu[ Digito_N_esimo(digito_actual ,entero_auxiliar_para_valor_numerico) ], NULL, &renderQuad );
								posicion_actual++;
							}

							// Representación de la coma --- Decimal point representation
							if ( pantallas_menu_principal[menu_activo].botones_pantalla[i].valor_numerico_digitos_dcha_coma > 0 )
							{
								// Posicionamos la coma --- We position the digit
								renderQuad.x = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo + (opciones_juego.screen_x_resolution * (PORCENTAJE_ANCHO_BOTONES/100)*(PORCNT_ANCHO_TITULO_BOT_VAL_NUM)/100 ) + posicion_actual*(opciones_juego.screen_x_resolution) * (PORCENTAJE_ANCHO_BOTONES/100) * (PORCNT_ANCHO_VALOR_BOT_VAL_NUM/100) /total_digitos;		// Coord X de esquina superior izquierda
								renderQuad.y = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba;		// Coord Y de esquina superior izquierda
								renderQuad.w = (opciones_juego.screen_x_resolution) * (PORCENTAJE_ANCHO_BOTONES/100) * (PORCNT_ANCHO_VALOR_BOT_VAL_NUM/100) /total_digitos;				// Ancho
								renderQuad.h = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_abajo - pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba;				// Alto
								// Dibujamos la coma --- We display the decimal point
								SDL_RenderCopy( gRenderer, gTexturaComaMenu, NULL, &renderQuad );
								posicion_actual++;
							}

							// Representación dígitos a la derecha de la coma
							// Display digits on the right of the decimal point
							for ( digito_actual =  pantallas_menu_principal[menu_activo].botones_pantalla[i].valor_numerico_digitos_dcha_coma - 1; digito_actual >= 0 ; digito_actual-- )
							{
								// Posicionamos el digito --- Position the digit
								renderQuad.x = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo + (opciones_juego.screen_x_resolution * (PORCENTAJE_ANCHO_BOTONES/100)*(PORCNT_ANCHO_TITULO_BOT_VAL_NUM)/100 ) + posicion_actual*(opciones_juego.screen_x_resolution) * (PORCENTAJE_ANCHO_BOTONES/100) * (PORCNT_ANCHO_VALOR_BOT_VAL_NUM/100) /total_digitos;		// Coord X de esquina superior izquierda
								renderQuad.y = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba;		// Coord Y de esquina superior izquierda
								renderQuad.w = (opciones_juego.screen_x_resolution) * (PORCENTAJE_ANCHO_BOTONES/100) * (PORCNT_ANCHO_VALOR_BOT_VAL_NUM/100) /total_digitos;				// Ancho
								renderQuad.h = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_abajo - pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba;				// Alto
								// Calculamos y a la vez dibujamos el dígito
								// Calculate and display the digit
								SDL_RenderCopy( gRenderer, gTexturaNumeroMenu[ Digito_N_esimo(digito_actual ,entero_auxiliar_para_valor_numerico) ], NULL, &renderQuad );
								posicion_actual++;
							}
						}
						// Dibujar botones <-/+> --- Draw <-/+> buttons
						// Boton -
						renderQuad.x = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo + (opciones_juego.screen_x_resolution * (PORCENTAJE_ANCHO_BOTONES/100)*(PORCNT_ANCHO_TITULO_BOT_VAL_NUM + PORCNT_ANCHO_VALOR_BOT_VAL_NUM)/100 ); 			/* Coord X esquina superior izquierda */
						renderQuad.y = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba; 							/* Coord Y esquina superior izqeuirda */
						renderQuad.w = (opciones_juego.screen_x_resolution) * (PORCENTAJE_ANCHO_BOTONES/100) * (PORCNT_ANCHO_MENOS_BOT_VAL_NUM/100) ; 		/* Ancho */ /* Ancho igual que alto de boton --- Tickbox width equal to button height */
						renderQuad.h = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_abajo - pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba;												/* Alto */
						SDL_RenderCopy( gRenderer, gWhiteArrowMinus, NULL, &renderQuad );
						// Boton +
						renderQuad.x = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_izquierdo + (opciones_juego.screen_x_resolution * (PORCENTAJE_ANCHO_BOTONES/100)*(PORCNT_ANCHO_TITULO_BOT_VAL_NUM + PORCNT_ANCHO_VALOR_BOT_VAL_NUM + PORCNT_ANCHO_MENOS_BOT_VAL_NUM)/100 ); 			/* Coord X esquina superior izquierda */
						renderQuad.y = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba; 							/* Coord Y esquina superior izqeuirda */
						renderQuad.w = (opciones_juego.screen_x_resolution) * (PORCENTAJE_ANCHO_BOTONES/100) * (PORCNT_ANCHO_MAS_BOT_VAL_NUM/100) ; 		/* Ancho */ /* Ancho igual que alto de boton --- Tickbox width equal to button height */
						renderQuad.h = pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_abajo - pantallas_menu_principal[menu_activo].botones_pantalla[i].borde_arriba;												/* Alto */
						SDL_RenderCopy( gRenderer, gWhiteArrowPlus, NULL, &renderQuad );
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
		// Capping framerate to the desired framerate (apprxomately)
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
		//system("clear");		// Borra la terminal para leer el estado actual (MUY LENTO) --- Clears terminal to read current status (SLOW)
		#endif

		///////////////////////////////////////////////////////////////////////////////////////
		// Gestion musica del menu --- Menu music management
		if ( (opciones_juego.music_enabled) && (gMusicaMenu != NULL) )
		{
			if(  Mix_PlayingMusic() == 0 )	// Si la musica no se está reproduciendo
			{
			     //Play the music
			     Mix_PlayMusic( gMusicaMenu, -1 );
			}
		}
		else
		{
			if(  Mix_PlayingMusic() == 1 )
			{
				Mix_HaltMusic( );
			}
		}

	}

}


void ActualizaPunteros_Menu_EligeConjuntoNiveles ( struct pantalla_menu* pantallas_menu_principal, int conjunto_niveles_seleccionado_actualmente )		// Update pointers in menu select level set
{
	// Esta funcion agrupa codigo repetido a la hora de cambiar de conjunto de niveles en la pantalla de seleccion de conjunto de niveles
	// This function groups repeated code, used when switching between level-sets in the level-set selection screen
	pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].textura_boton_reposo = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].TexturaImagen;
	pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].textura_boton_seleccionado = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].TexturaImagen;
	pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].textura_dim_x = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].TexturaImagen_DimX;
	pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[3].textura_dim_y = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].TexturaImagen_DimY;
	pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].textura_boton_reposo = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].TexturaNombreDir;
	pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].textura_boton_seleccionado = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].TexturaNombreDir;
	pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].textura_dim_x = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].TexturaNombreDir_DimX;
	pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[5].textura_dim_y = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].TexturaNombreDir_DimY;
	pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].textura_boton_reposo = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].TexturaDescripcion;
	pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].textura_boton_seleccionado = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].TexturaDescripcion;
	pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].textura_dim_x = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].TexturaDescripcion_DimX;
	pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[6].textura_dim_y = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].TexturaDescripcion_DimY;
	pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].textura_boton_reposo = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].TexturaNumeracion;
	pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].textura_boton_seleccionado = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].TexturaNumeracion;
	pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].textura_dim_x = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].TexturaNumeracion_DimX;
	pantallas_menu_principal[menu_elige_conjunto_niveles].botones_pantalla[7].textura_dim_y = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].TexturaNumeracion_DimY;

}

void ActualizaPunteros_Menu_EligeNivel (struct pantalla_menu* pantallas_menu_principal, int conjunto_niveles_seleccionado_actualmente, int nivel_seleccionado_actualmente )	// Update pointers in menu select level
{
	// Esta funcion agrupa codigo repetido a la hora de cambiar de nivel en la pantalla de seleccion de nivel
	// This function groups repeated code, used when switching between levels in the level selection screen
	// Nombre del mapa --- Map name
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].textura_boton_reposo = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaNombreMapa;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].textura_boton_seleccionado = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaNombreMapa;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].textura_dim_x = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaNombreMapa_DimX;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[5].textura_dim_y = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaNombreMapa_DimY;
	// Descripción del mapa --- Map description
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].textura_boton_reposo = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaDescripcionMapa;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].textura_boton_seleccionado = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaDescripcionMapa;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].textura_dim_x = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaDescripcionMapa_DimX;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[6].textura_dim_y = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaDescripcionMapa_DimY;
	// Numeración --- Numbering
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].textura_boton_reposo = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaNumeracion;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].textura_boton_seleccionado = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaNumeracion;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].textura_dim_x = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaNumeracion_DimX;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[11].textura_dim_y = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaNumeracion_DimY;
	// Imagen mapa --- Map image
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].textura_boton_reposo = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaImagenMapa;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].textura_boton_seleccionado = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaImagenMapa;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].textura_dim_x = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaImagenMapa_DimX;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[3].textura_dim_y = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaImagenMapa_DimY;
	// Modo de giro --- Rotation mode
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].textura_boton_reposo = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaModoGiro;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].textura_boton_seleccionado = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaModoGiro;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].textura_dim_x = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaModoGiro_DimX;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[7].textura_dim_y = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaModoGiro_DimY;
	// Angulo máximo --- Maximum angle
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].textura_boton_reposo = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaAnguloMaximo;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].textura_boton_seleccionado = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaAnguloMaximo;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].textura_dim_x = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaAnguloMaximo_DimX;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[8].textura_dim_y = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaAnguloMaximo_DimY;
	// Gravedad --- Gravity
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].textura_boton_reposo = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaGravedad;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].textura_boton_seleccionado = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaGravedad;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].textura_dim_x = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaGravedad_DimX;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[9].textura_dim_y = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaGravedad_DimY;
	// Tiempo -- Time
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].textura_boton_reposo = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaTiempo;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].textura_boton_seleccionado = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaTiempo;	
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].textura_dim_x = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaTiempo_DimX;
	pantallas_menu_principal[menu_elige_nivel].botones_pantalla[10].textura_dim_y = gListaConjuntosMapasJuego[conjunto_niveles_seleccionado_actualmente].mapas_individuales[nivel_seleccionado_actualmente].TexturaTiempo_DimY;



}

