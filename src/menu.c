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
#define PORCENTAJE_ANCHO_BOTONES 40
#define PORCENTAJE_MARGEN_SUPERIOR 40
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


TTF_Font *gFuenteTexto = NULL;
SDL_Color gColorTextoReposo = { 255 , 255 , 255 }; 	// Blanco
SDL_Color gColorTextoSeleccionado = { 255 , 255 , 0 }; 	// Amarillo


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



bool inicializar_menu_principal(/* struct pantalla_menu* pantallas_menu_principal, */int numero_pantallas )
{
	// Esta funcion inicializa los valores de las estructuras de las imagenes del menu.
	// Reserva la memoria para el menu, inicializa variables, e inicializa graficos
	// (TODO)
	bool success = true;

	// Cargar la fuente del menu
	#ifdef DEBUG_INFO
	printf("Inicializando la fuente del menu...\n");
	#endif

	gFuenteTexto = TTF_OpenFont( "fonts/lunasol.ttf", 28 );
	if( gFuenteTexto == NULL )
	{
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}


	#ifdef DEBUG_INFO
	printf("Reservando memoria dinámica para las pantallas del menu...\n");
	#endif

	pantallas_menu_principal = calloc( numero_pantallas, sizeof( struct pantalla_menu ) );

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


	// Pantalla menu_principal
	#ifdef DEBUG_INFO
	printf("Inicializando datos de pantalla menu principal...\n");
	#endif
	pantallas_menu_principal[menu_principal].identificador_pantalla = menu_principal;
	strcpy( pantallas_menu_principal[menu_principal].titulo, "Menu principal\0" );
	pantallas_menu_principal[menu_principal].textura_titulo = RenderizaTextoEnTextura( 	pantallas_menu_principal[menu_principal].titulo, 
														gFuenteTexto, 
														gColorTextoReposo, 
														/*&(pantallas_menu_principal[menu_principal].botones_pantalla[0].textura_dim_x)*/ NULL, 
														/*&(pantallas_menu_principal[menu_principal].botones_pantalla[0].textura_dim_y)*/ NULL   );
	pantallas_menu_principal[menu_principal].borde_izquierdo_titulo = (opciones_juego.screen_x_resolution)*(100-PORCENTAJE_ANCHO_TITULOS)/2/100;
	pantallas_menu_principal[menu_principal].borde_derecho_titulo   = (opciones_juego.screen_x_resolution)*(100+PORCENTAJE_ANCHO_TITULOS)/2/100;
	pantallas_menu_principal[menu_principal].borde_arriba_titulo    = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + (-2)*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES)/100;
	pantallas_menu_principal[menu_principal].borde_abajo_titulo	 = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + (-2)*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES + PORCENTAJE_ALTO_TITULOS)/100;

	pantallas_menu_principal[menu_principal].imagen_fondo_pantalla = CargaTextura( "images/28042wide.jpg" , NULL, NULL, false );
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
	//----------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 0 de pantalla menu principal...\n");
	#endif
	pantallas_menu_principal[menu_principal].botones_pantalla[0].identificador_boton = boton_jugar;
	pantallas_menu_principal[menu_principal].botones_pantalla[0].clase_boton = boton_pulsar;
	strcpy( pantallas_menu_principal[menu_principal].botones_pantalla[0].texto, "Jugar\0" );
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
	pantallas_menu_principal[menu_principal].botones_pantalla[0].boton_siguiente = 1;
	//--------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 1 de pantalla menu principal...\n");
	#endif
	pantallas_menu_principal[menu_principal].botones_pantalla[1].identificador_boton = boton_opciones;
	pantallas_menu_principal[menu_principal].botones_pantalla[1].clase_boton = boton_pulsar;
	strcpy( pantallas_menu_principal[menu_principal].botones_pantalla[1].texto, "Opciones\0" );
	pantallas_menu_principal[menu_principal].botones_pantalla[1].borde_izquierdo = (opciones_juego.screen_x_resolution)*(100-PORCENTAJE_ANCHO_BOTONES)/2/100;
	pantallas_menu_principal[menu_principal].botones_pantalla[1].borde_derecho = (opciones_juego.screen_x_resolution)*(100+PORCENTAJE_ANCHO_BOTONES)/2/100;
	pantallas_menu_principal[menu_principal].botones_pantalla[1].borde_arriba = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + 1*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES)/100;
	pantallas_menu_principal[menu_principal].botones_pantalla[1].borde_abajo = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + 1*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES + PORCENTAJE_ALTO_BOTON)/100;;
	pantallas_menu_principal[menu_principal].botones_pantalla[1].textura_boton_reposo = RenderizaTextoEnTextura( 	pantallas_menu_principal[menu_principal].botones_pantalla[1].texto, 
														gFuenteTexto, 
														gColorTextoReposo, 
														&(pantallas_menu_principal[menu_principal].botones_pantalla[1].textura_dim_x), 
														&(pantallas_menu_principal[menu_principal].botones_pantalla[1].textura_dim_y)   );
	if( pantallas_menu_principal[menu_principal].botones_pantalla[1].textura_boton_reposo == NULL ) 
	{
		printf( "Unable to render text surface! SDL_ttf\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_principal].botones_pantalla[1].textura_boton_seleccionado = RenderizaTextoEnTextura( 	pantallas_menu_principal[menu_principal].botones_pantalla[1].texto, 
														gFuenteTexto, 
														gColorTextoSeleccionado, 
														NULL, 
														NULL   );
	if( pantallas_menu_principal[menu_principal].botones_pantalla[1].textura_boton_seleccionado == NULL ) 
	{
		printf( "Unable to render text surface! SDL_ttf\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_principal].botones_pantalla[1].boton_anterior = 0;
	pantallas_menu_principal[menu_principal].botones_pantalla[1].boton_siguiente = 2;
	//------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 2 de pantalla menu principal...\n");
	#endif
	pantallas_menu_principal[menu_principal].botones_pantalla[2].identificador_boton = boton_creditos;
	pantallas_menu_principal[menu_principal].botones_pantalla[2].clase_boton = boton_pulsar;
	strcpy( pantallas_menu_principal[menu_principal].botones_pantalla[2].texto, "Creditos\0" );
	pantallas_menu_principal[menu_principal].botones_pantalla[2].borde_izquierdo = (opciones_juego.screen_x_resolution)*(100-PORCENTAJE_ANCHO_BOTONES)/2/100;
	pantallas_menu_principal[menu_principal].botones_pantalla[2].borde_derecho = (opciones_juego.screen_x_resolution)*(100+PORCENTAJE_ANCHO_BOTONES)/2/100;
	pantallas_menu_principal[menu_principal].botones_pantalla[2].borde_arriba = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + 2*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES)/100;
	pantallas_menu_principal[menu_principal].botones_pantalla[2].borde_abajo = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + 2*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES + PORCENTAJE_ALTO_BOTON)/100;;
	pantallas_menu_principal[menu_principal].botones_pantalla[2].textura_boton_reposo = RenderizaTextoEnTextura( 	pantallas_menu_principal[menu_principal].botones_pantalla[2].texto, 
														gFuenteTexto, 
														gColorTextoReposo, 
														&(pantallas_menu_principal[menu_principal].botones_pantalla[2].textura_dim_x), 
														&(pantallas_menu_principal[menu_principal].botones_pantalla[2].textura_dim_y)   );
	if( pantallas_menu_principal[menu_principal].botones_pantalla[2].textura_boton_reposo == NULL ) 
	{
		printf( "Unable to render text surface! SDL_ttf\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_principal].botones_pantalla[2].textura_boton_seleccionado = RenderizaTextoEnTextura( 	pantallas_menu_principal[menu_principal].botones_pantalla[2].texto, 
														gFuenteTexto, 
														gColorTextoSeleccionado, 
														NULL, 
														NULL   );
	if( pantallas_menu_principal[menu_principal].botones_pantalla[2].textura_boton_seleccionado == NULL ) 
	{
		printf( "Unable to render text surface! SDL_ttf\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_principal].botones_pantalla[2].boton_anterior = 1;
	pantallas_menu_principal[menu_principal].botones_pantalla[2].boton_siguiente = 3;
	//-------------------------------------
	#ifdef DEBUG_INFO
	printf("Inicializando datos boton 3 de pantalla menu principal...\n");
	#endif
	pantallas_menu_principal[menu_principal].botones_pantalla[3].identificador_boton = boton_salir;
	pantallas_menu_principal[menu_principal].botones_pantalla[3].clase_boton = boton_pulsar;
	strcpy( pantallas_menu_principal[menu_principal].botones_pantalla[3].texto, "Salir\0" );
	pantallas_menu_principal[menu_principal].botones_pantalla[3].borde_izquierdo = (opciones_juego.screen_x_resolution)*(100-PORCENTAJE_ANCHO_BOTONES)/2/100;
	pantallas_menu_principal[menu_principal].botones_pantalla[3].borde_derecho = (opciones_juego.screen_x_resolution)*(100+PORCENTAJE_ANCHO_BOTONES)/2/100;
	pantallas_menu_principal[menu_principal].botones_pantalla[3].borde_arriba = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + 3*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES)/100;
	pantallas_menu_principal[menu_principal].botones_pantalla[3].borde_abajo = (opciones_juego.screen_y_resolution)*(PORCENTAJE_MARGEN_SUPERIOR + 3*PORCENTAJE_OFFSET_VERTICAL_ENTRE_BOTONES + PORCENTAJE_ALTO_BOTON)/100;;
	pantallas_menu_principal[menu_principal].botones_pantalla[3].textura_boton_reposo = RenderizaTextoEnTextura( 	pantallas_menu_principal[menu_principal].botones_pantalla[3].texto, 
														gFuenteTexto, 
														gColorTextoReposo, 
														&(pantallas_menu_principal[menu_principal].botones_pantalla[3].textura_dim_x), 
														&(pantallas_menu_principal[menu_principal].botones_pantalla[3].textura_dim_y)   );
	if( pantallas_menu_principal[menu_principal].botones_pantalla[3].textura_boton_reposo == NULL ) 
	{
		printf( "Unable to render text surface! SDL_ttf\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_principal].botones_pantalla[3].textura_boton_seleccionado = RenderizaTextoEnTextura( 	pantallas_menu_principal[menu_principal].botones_pantalla[3].texto, 
														gFuenteTexto, 
														gColorTextoSeleccionado, 
														NULL, 
														NULL   );
	if( pantallas_menu_principal[menu_principal].botones_pantalla[3].textura_boton_seleccionado == NULL ) 
	{
		printf( "Unable to render text surface! SDL_ttf\n" ); 
		success = false;
	}
	pantallas_menu_principal[menu_principal].botones_pantalla[3].boton_anterior = 2;
	pantallas_menu_principal[menu_principal].botones_pantalla[3].boton_siguiente = 0;

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
						boton_seleccionado = pantallas_menu_principal[menu_activo].botones_pantalla[boton_seleccionado].boton_anterior;
						break;
					case SDLK_DOWN:
						// Tecla abajo
						boton_seleccionado = pantallas_menu_principal[menu_activo].botones_pantalla[boton_seleccionado].boton_siguiente;
						break;
					case SDLK_RETURN:
					case SDLK_KP_ENTER:
						// Tecla enter
						boton_pulsado = true;
						break;
					case SDLK_f:
						// Tecla F: Pantalla completa --- Toggle fullscreen
						CambiarModoPantallaCompleta ( !opciones_juego.fullscreen , gGameWindow );
						opciones_juego.fullscreen = !opciones_juego.fullscreen;
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
