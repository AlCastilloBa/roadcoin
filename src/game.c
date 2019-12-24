//Using SDL and standard IO 
#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h> 
#include <stdbool.h>


#include "physics.h"
#include "camera.h"
#include "maps.h"
#include "geometry.h"
#include "options.h"
#include "menu.h"
#include "graphics.h"


// Si definido DEBUG_INFO, mostrar textos de informacion por la terminal (hace el programa más lento)
//#define DEBUG_INFO

//Screen dimension constants 
const int INTRO_SCREEN_WIDTH = 500; 
const int INTRO_SCREEN_HEIGHT = 851;
//const int GAME_SCREEN_WIDTH = 800; 	// Incluido en opciones
//const int GAME_SCREEN_HEIGHT = 600;	// Incluido en opciones


////////////////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES
//The window we'll be rendering to 
SDL_Window* gIntroWindow = NULL; 
SDL_Window* gGameWindow = NULL; 

//The surface contained by the window 
SDL_Surface* gScreenSurface = NULL; 

// Pantalla intro
SDL_Surface* gscreenIntro = NULL; 

//The window renderer 
SDL_Renderer* gRenderer = NULL;

//Texturas 
SDL_Texture* gTexturaMoneda = NULL;
SDL_Texture* gTexturaFondo = NULL;


//Variables globales moneda
int gDimMonedaX;
int gDimMonedaY;
int gRadioMoneda;

//Variables globales control
float inc_angulo_teclado = 1.0f;

// Variables globales opciones
struct opciones opciones_juego;

// Variables globales menu (ver menu.c)
struct pantalla_menu* pantallas_menu_principal;


/*
// Declaración de funciones
bool inicializar_intro(void);
bool inicializar_ventana_juego(void);
bool loadMedia(void);
bool loadMainGameLoopMedia(char*, char*);
bool cerrar_intro(void);
bool close_program(void);
void bucle_principal_juego( void );
*/

void bucle_principal_juego(void);



bool inicializar_intro() 
{ 
	bool success = true; 			//Initialization flag 
	int imgFlags = IMG_INIT_PNG; 		//Initialize PNG loading

	//Initialize SDL (y joystick)
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )  //When there's an error, SDL_Init returns -1
	{ 
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() ); 
		success = false;
		exit(-1);
	} 
	
	//Inicializa SDL_Image
	//Initialize PNG loading 
	if( !( IMG_Init( imgFlags ) & imgFlags ) ) 
	{ 
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() ); 
		success = false;
		exit(-1);
	}


	// Inicializa SDL_ttf
	if( TTF_Init() == -1 )
	{
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}


	// Inicializa SDL_mixer
	// PENDIENTE
 
	//Create window 
	gIntroWindow = SDL_CreateWindow( 	"Intro", 
					SDL_WINDOWPOS_UNDEFINED, 
					SDL_WINDOWPOS_UNDEFINED, 
					INTRO_SCREEN_WIDTH, 
					INTRO_SCREEN_HEIGHT, 
					SDL_WINDOW_SHOWN ); 
	if( gIntroWindow == NULL ) 
	{ 
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() ); 
		success = false;
		exit(-1);
	} 
	 
	//Get window surface 
	gScreenSurface = SDL_GetWindowSurface( gIntroWindow ); 
	
	 return success; 
}


bool inicializar_ventana_juego() 
{
	bool success = true;
	gGameWindow = SDL_CreateWindow( 	"Juego", 
					SDL_WINDOWPOS_UNDEFINED, 
					SDL_WINDOWPOS_UNDEFINED, 
					/*GAME_SCREEN_WIDTH*/ opciones_juego.screen_x_resolution, 
					/*GAME_SCREEN_HEIGHT*/ opciones_juego.screen_y_resolution, 
					SDL_WINDOW_SHOWN ); 

	if( gGameWindow == NULL ) 
	{ 
		printf( "En función inicializar_juego, ventana no creada! SDL_Error: %s\n", SDL_GetError() ); 
		success = false;
		exit(-1);
	} 
 
	//Create renderer for window 
	gRenderer = SDL_CreateRenderer( gGameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ); 

	if( gRenderer == NULL ) 
	{ 
		printf( "En funcion inicializar_juego, Renderer could not be created! SDL Error: %s\n", SDL_GetError() ); 
		success = false; 
	} 
	//Initialize renderer color 
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF ); 

	// Si el archivo de opciones decia pantalla completa, activar pantalla completa --- If options file said fullscreen mode, then enable fullscreen
	if ( opciones_juego.fullscreen == true )
	{
		CambiarModoPantallaCompleta( true, gGameWindow );
	}
	return success;
}

bool loadIntroMedia() 
{ 
	bool success = true; 	//Loading success flag 

	//Load splash image 
	//gscreenIntro = SDL_LoadBMP( "images/maq_frente.bmp" ); 
	gscreenIntro = IMG_Load( "images/maq_frente.png" ); 
	if( gscreenIntro == NULL ) 
	{ 
		printf( "Unable to load image %s! SDL Error: %s\n", "images/maq_frente.bmp", SDL_GetError() ); 
		success = false; 
	} 
	return success; 
}


bool loadMainGameLoopMedia( char* ruta_imagen_moneda, char* ruta_imagen_fondo ) 
{ 
	bool success = true; 	//Loading success flag 

	 //Load PNG texture - Moneda 
	gTexturaMoneda = CargaTextura( ruta_imagen_moneda , &gDimMonedaX, &gDimMonedaY, true ); 
	if( gTexturaMoneda == NULL ) 
	{ 
		printf( "Failed to load texture image!\n" ); 
		success = false; 
	} 

	#ifdef DEBUG_INFO 
	printf("Dimensiones textura moneda: %d x %d\n", gDimMonedaX, gDimMonedaY);
	#endif
	if (gDimMonedaX != gDimMonedaY )
	{
		printf("La textura de la moneda no es cuadrada (dificilmente rodará).\n");
		exit(-1);
	}
	gRadioMoneda = gDimMonedaX/2;
	#ifdef DEBUG_INFO 
	printf("Radio moneda: %d\n", gRadioMoneda);
	#endif

	 //Load PNG texture - Fondo
	gTexturaFondo = CargaTextura( ruta_imagen_fondo , NULL, NULL, false );
	if( gTexturaFondo == NULL ) 
	{ 
		printf( "Failed to load texture image!\n" ); 
		success = false; 
	} 

	return success;
}



bool cerrar_intro() 
{
	//Deallocate surface 
	SDL_FreeSurface( gscreenIntro ); 
	gscreenIntro = NULL; 

	//Destroy window and renderers
	SDL_DestroyWindow( gIntroWindow );
	gIntroWindow = NULL;  
}

bool close_program() 
{
	//Deallocate surface 
	SDL_FreeSurface( gscreenIntro ); 
	gscreenIntro = NULL; 

	//Free loaded image 
	SDL_DestroyTexture( gTexturaMoneda ); 
	gTexturaMoneda = NULL; 


	//Destroy window and renderers
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gIntroWindow );
	SDL_DestroyWindow( gGameWindow );
	gIntroWindow = NULL; 
	gGameWindow = NULL; 

	gRenderer = NULL;

	//Quit SDL subsystems 
	IMG_Quit();
	SDL_Quit(); 
}


int main( int argc, char* args[] ) 
{ 
	#ifdef DEBUG_INFO
	printf("Cargando opciones...\n");
	#endif

	opciones_juego = CargarArchivoOpciones();


	//Start up SDL and create window 
	#ifdef DEBUG_INFO
	printf("Inicializando intro...\n");
	#endif
	if( !inicializar_intro() ) 
	{ 
		printf( "Failed to initialize!\n" );
		exit(-1);
	} 

	//Load media 
	#ifdef DEBUG_INFO
	printf("Cargando archivos intro...\n");
	#endif
	if( !loadIntroMedia() ) 
	{ 
		printf( "Failed to load media!\n" ); 
		exit(-1);
	} 


	// Pantalla intro
	//Apply the image 
	SDL_BlitSurface( gscreenIntro, NULL, gScreenSurface, NULL );

	 //Update the surface 
	SDL_UpdateWindowSurface( gIntroWindow );

	 //Espera para que se vea la imagen 
	SDL_Delay( 3000 ); 
	
	cerrar_intro();


	#ifdef DEBUG_INFO
	printf("Inicializando ventana del juego...\n");
	#endif
	if( !inicializar_ventana_juego() ) 
	{ 
		printf( "Failed to initialize!\n" );
		exit(-1);
	} 


	////////////
	#ifdef DEBUG_INFO
	printf("Inicializando menu...\n");
	#endif
	if( !inicializar_menu_principal( /*pantallas_menu_principal ,*/ NUM_TOTAL_PANTALLAS_MENU ) ) 
	{ 
		printf( "Failed to initialize!\n" );
		exit(-1);
	} 

	#ifdef DEBUG_INFO
	printf("Comenzando bucle principal del menu...\n");
	#endif
	bucle_principal_menu_principal();

	/////////////////


/*

	#ifdef DEBUG_INFO
	printf("Comenzando bucle principal del juego...\n");
	#endif
	bucle_principal_juego();

*/


	// Liberar memoria del menu
	liberar_memoria_menu_principal( NUM_TOTAL_PANTALLAS_MENU );

	
	#ifdef DEBUG_INFO
	printf("Liberando recursos de SDL y cerrando SDL...\n");
	#endif

	//Free resources and close SDL 
	close_program();

	#ifdef DEBUG_INFO
	printf("Saliendo del programa...\n");
	#endif 
	return 0; 
}




void bucle_principal_juego( void )
{
	 
	bool quit = false;	 //Main loop flag
	int contador_frames = 0;
	SDL_Event e;		 //Event handler 
	Uint32 currentTime = 0, lastTime, deltaTime, tiempo_imagen_sobrante; //Current time start time (milisegundos)
	float tiempo_imagen;	//Tiempo de cada imagen, en segundos
	float framerate_deseado = 30;
	int segmento_actual;
	float angulo, angulo_anterior;
	float mouse_sensibility = 0.1f;

	float angulo_rotacion_moneda = 0.0f;	// En radianes
	float vel_angular_moneda = 0.0f;	// En rad/s

	struct punto pos_real_moneda;			// Posicion de centro de la moneda (unidades del juego)
	struct posicion_camara pos_pant_moneda;			// Posicion de centro de la moneda (pixeles de la pantalla)

	struct vector_velocidad velocidad_real_moneda;
	struct vector_aceleracion aceleracion_real_moneda;

	struct mapa mapa_original;		// Mapa cargado desde archivo
	struct segmento* segmentos_girados;

	//bool* interferencia_segmento;		// Vector que indica si hay interferencia entre moneda y segmento
	enum tipo_interseccion_circulo_segmento* tipo_interferencia_segmento;

	struct vector_fuerza gravedad;	
	struct vector_fuerza* fuerzas_normales_segmentos;	// Vector que guarda las fuerzas normales de apoyo sobre segmentos en el fotograma actual

	SDL_Rect renderQuad;		// Rectangulo de SDL para indicar la posicion de los objetos a renderizar

	//////////////////////////////////////////////////////////////////////////////////////////

	//Leer mapa
	//mapa_original = CargarMapaDesdeArchivo( "maps/test_map" );
	//mapa_original = CargarMapaDesdeArchivo( "maps/test_map_2" );
	mapa_original = CargarMapaDesdeArchivo( "maps/monza_1" );
	// DATOS INCIALES, A BORRAR CUANDO SE LEA EL FICHERO
	pos_real_moneda.x = mapa_original.PuntoInicialMoneda.x;			//Pixeles
	pos_real_moneda.y = mapa_original.PuntoInicialMoneda.y;			//Pixeles
	velocidad_real_moneda.vx = 0;						//Pixeles/segundo
	velocidad_real_moneda.vy = 0;						//Pixeles/segundo
	aceleracion_real_moneda.ax = 0;						//Pixeles/segundo²
	aceleracion_real_moneda.ay = 0;	/*mapa_original.Gravedad*/		//Pixeles/segundo²
	gravedad.fx = 0;							//Pixeles/segundo²
	gravedad.fy = mapa_original.Gravedad;					//Pixeles/segundo²

	angulo = 0; angulo_anterior = 0;

	// Reserva memoria dinámica
	// Reservamos memoria para los segmentos girados
	segmentos_girados = calloc(mapa_original.NumeroSegmentos, sizeof(struct segmento) );
	if ( segmentos_girados == NULL )
	{
		printf ( "Error: no se puede reservar memoria para segmentos girados\n");
		exit(-1);
	}

	//interferencia_segmento = calloc(mapa_original.NumeroSegmentos, sizeof(bool) );
	tipo_interferencia_segmento = calloc(mapa_original.NumeroSegmentos, sizeof(enum tipo_interseccion_circulo_segmento) );
	if ( tipo_interferencia_segmento == NULL )
	{
		printf ( "Error: no se puede reservar memoria para tipo_interferencia_segmento\n");
		exit(-1);
	}
	fuerzas_normales_segmentos = calloc(mapa_original.NumeroSegmentos, sizeof(struct vector_fuerza) );
	if ( fuerzas_normales_segmentos == NULL )
	{
		printf ( "Error: no se puede reservar memoria para fuerzas_normales_segmentos\n");
		exit(-1);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Cargamos archivos del juego - Load Game media 
	#ifdef DEBUG_INFO
	printf("Cargando archivos juego...\n");
	#endif
	if( !loadMainGameLoopMedia( mapa_original.RutaImagenMoneda, mapa_original.RutaImagenFondo ) ) 
	{ 
		printf( "Failed to load media!\n" ); 
		exit(-1);
	} 

	//////////////////////////////////////////////////////////////////////////////////////////


	//Ajustar variables ratón
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	//SDL_WarpMouseInWindow(gGameWindow, GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2 );

	///////////////////////////////////////////////////////////////////////////////////////////

	//Reseteamos contador de tiempo (para no tener en cuenta todo lo anterior)
	lastTime = currentTime;
	currentTime = SDL_GetTicks();	

	///////////////////////////////////////////////////////////////////////////////////////////

	//While application is running 
	while( !quit ) 
	{


		// Calcular tiempo para el movimiento
		//SDL_Delay (1000); --> Para pruebas, borrar
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
		printf("Frame %d, time %d, FPS=%f, ang=%f \n", contador_frames, deltaTime, 1/tiempo_imagen, angulo);
		#endif

		//////////////////////////////////////////////////////////////////////////////////////

		//Gestión de entradas (teclado, ratón, joystick)
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
						angulo -= inc_angulo_teclado;
						break;
					case SDLK_RIGHT:
						angulo += inc_angulo_teclado;
						break;
					default:  
						break; 
				}
			}
			else if ( e.type  == SDL_MOUSEMOTION )
			{
				if (contador_frames != 1)
				{
					angulo += (float)(e.motion.xrel) * mouse_sensibility;
					//printf ("Mov raton: %f\n", e.motion.xrel);
					//SDL_WarpMouseInWindow(gGameWindow, GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2 );
				}
			}
		}

		// Limites de angulos
		if ( angulo < -(mapa_original.AnguloMax)  )
		{
			angulo = -(mapa_original.AnguloMax);
		}
		if (angulo > mapa_original.AnguloMax )
		{
			angulo = mapa_original.AnguloMax;
		}

		/////////////////////////////////////////////////////////////////////////////////////	
		// Giramos el mapa
		//if ( mapa_original.TipoGiro == punto_fijo )
		//{
		//	GiraMapaCompleto( mapa_original.Mapa , segmentos_girados, mapa_original.PuntoGiroFijo, mapa_original.NumeroSegmentos, angulo );
		//}
		switch (mapa_original.TipoGiro)
		{
			case punto_fijo:
				GiraMapaCompleto( mapa_original.Mapa , segmentos_girados, mapa_original.PuntoGiroFijo, mapa_original.NumeroSegmentos, angulo );
				break;
			case camara:
				// (TODO)
				printf("ERROR: TODAVIA NO IMPLEMENTADO\n");
				exit(-1);
				break;
			case moneda:
				GiraMapaCompleto( mapa_original.Mapa , segmentos_girados, pos_real_moneda, mapa_original.NumeroSegmentos, angulo );
				break;
			default:
				printf("Al proceder a girar el mapa, error de programacion o error de concepto\n");
				exit(-1);
				break;
		}
		// Nota: (TODO) falta implementar otros casos


		////////////////////////////////////////////////////////////////////////////////////
		// Determinamos intersecciones
		// PROVISIONAL
		
		for ( segmento_actual = 0 ; segmento_actual < mapa_original.NumeroSegmentos ; segmento_actual++ )
		{
			tipo_interferencia_segmento[segmento_actual] =  CirculoTocaSegmento(pos_real_moneda, gRadioMoneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end );

			// Nota: hay que meter la comprobacion de segmentos contando con la posicion anterior (PENDIENTE, TODO)
			// Si hay interferencia, calculamos posicion tangente (primera vez)
			if ( tipo_interferencia_segmento[segmento_actual] != 0 )	// Si hay interferencia
			{
				switch (tipo_interferencia_segmento[segmento_actual])
				{
					case interseccion_central:
						pos_real_moneda = CalculaPosTangenteACentroSegmento ( segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end, pos_real_moneda, gRadioMoneda);
						break;
					case interseccion_extremo_start:
						pos_real_moneda = CalculaPosTangenteAExtremoSegmento ( segmentos_girados[segmento_actual].start, pos_real_moneda, gRadioMoneda);
						break;
					case interseccion_extremo_end:
						pos_real_moneda = CalculaPosTangenteAExtremoSegmento ( segmentos_girados[segmento_actual].end, pos_real_moneda, gRadioMoneda);
						break;
					default:
						#ifdef DEBUG_INFO
						printf("En primer calculo pos tangente. No posible, error de programacion o de concepto.\n");
						#endif
						break;
				}
			}

		}
		////////////////////////////////////////////////////////////////////////////////////
		// En caso de intersección:
		// ESTO ES LO DIFICIL !!!!
		for ( segmento_actual = 0 ; segmento_actual < mapa_original.NumeroSegmentos ; segmento_actual++ )
		{
			if ( tipo_interferencia_segmento[segmento_actual] != 0 )	// Si hay interferencia
			{
				float angulo_segmento_actual;

				angulo_segmento_actual = AnguloSegmento( segmentos_girados[segmento_actual] );


				// Anulamos componente velocidad normal al segmento
				velocidad_real_moneda = AnulaVelocidadNormalASegmento( velocidad_real_moneda, angulo_segmento_actual );

				// Calculamos la nueva velocidad de giro de la moneda provocada por el contacto
				vel_angular_moneda = CalculaVelGiroSobreSegmento( velocidad_real_moneda, angulo_segmento_actual, gRadioMoneda);

				// Calculamos posicion tangente (segunda vez)
				switch (tipo_interferencia_segmento[segmento_actual])
				{
					case interseccion_central:
						pos_real_moneda = CalculaPosTangenteACentroSegmento ( segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end, pos_real_moneda, gRadioMoneda);
						break;
					case interseccion_extremo_start:
						pos_real_moneda = CalculaPosTangenteAExtremoSegmento ( segmentos_girados[segmento_actual].start, pos_real_moneda, gRadioMoneda);
						break;
					case interseccion_extremo_end:
						pos_real_moneda = CalculaPosTangenteAExtremoSegmento ( segmentos_girados[segmento_actual].end, pos_real_moneda, gRadioMoneda);
						break;
					default:
						#ifdef DEBUG_INFO
						printf("En segundo calculo pos tangente. No posible, error de programacion o de concepto.\n");
						#endif
						break;
				}

				// En caso de giro del mapa:
				if ( angulo != angulo_anterior )
				{
					// (TODO) Adecuar para otros tipos de giro
					switch (mapa_original.TipoGiro)
					{
						case punto_fijo:
							// hacemos que tambien gire la moneda
							pos_real_moneda = GiraPunto ( mapa_original.PuntoGiroFijo, pos_real_moneda, angulo - angulo_anterior);

							// Calculamos y sumamos una velocidad debida al giro
							// (TODO) Esto no está bien, aparce una velocidad muy grande (¡¡¡REVISAR!!!!)
							if ( abs( angulo-angulo_anterior) >= 10 /*grados*/)
							{
								velocidad_real_moneda = SumaVelocidad( velocidad_real_moneda,  VelAngular2VelLineal( mapa_original.PuntoGiroFijo, pos_real_moneda /*Inexacto, mejor punto contacto (TODO)*/, (angulo-angulo_anterior), tiempo_imagen ) );
							}
							break;
						case moneda:
							// En este caso, no es necesario hacer nada.
							break;
						case camara:
							// (TODO)
							printf("ERROR: TODAVIA NO IMPLEMENTADO\n");
							exit(-1);
							break;
						default:
							printf("Al proceder a calcular efectos del giro sobre moneda, error de programacion o error de concepto\n");
							exit(-1);
							break;
					}

				}

				// Calculamos fuerzas normales
				switch (tipo_interferencia_segmento[segmento_actual])
				{
					case interseccion_central:
						fuerzas_normales_segmentos[segmento_actual] = CalculaReaccionNormalCentroSegmento( angulo_segmento_actual, gravedad.fy, 1 );	// Masa = 1  de momento
						break;
					case interseccion_extremo_start:
						if (mapa_original.Mapa[segmento_actual].start_adyacente_a_otro == false )
						{
							// No es adyacente a otro, podemos calcular su fuerza
							fuerzas_normales_segmentos[segmento_actual] = CalculaReaccionNormalExtremoSegmento( pos_real_moneda, segmentos_girados[segmento_actual].start, gravedad.fy, 1 );
							#ifdef DEBUG_INFO
							printf("Se aplica fuerza normal en extremo start de segmento %d.\n", segmento_actual);
							#endif
						}
						else
						{
							fuerzas_normales_segmentos[segmento_actual].fx = 0; fuerzas_normales_segmentos[segmento_actual].fy = 0;
							#ifdef DEBUG_INFO
							printf("Segmento %d tiene extremo start adyacente a otro. No se calcula su fuerza normal en extremo.\n", segmento_actual);
							#endif
						}	
						break;
					case interseccion_extremo_end:
						if (mapa_original.Mapa[segmento_actual].end_adyacente_a_otro == false )
						{
							// No es adyacente a otro, podemos calcular su fuerza
							fuerzas_normales_segmentos[segmento_actual] = CalculaReaccionNormalExtremoSegmento( pos_real_moneda, segmentos_girados[segmento_actual].end, gravedad.fy, 1 );
							#ifdef DEBUG_INFO
							printf("Se aplica fuerza normal en extremo end de segmento %d.\n", segmento_actual);
							#endif
						}
						else
						{
							fuerzas_normales_segmentos[segmento_actual].fx = 0; fuerzas_normales_segmentos[segmento_actual].fy = 0;
							#ifdef DEBUG_INFO
							printf("Segmento %d tiene extremo end adyacente a otro. No se calcula su fuerza normal en extremo.\n", segmento_actual);
							#endif
						}
						break;
					default:
						#ifdef DEBUG_INFO
						printf("No posible, error de programacion o de concepto.\n");
						#endif
						break;
				}
				#ifdef DEBUG_INFO
				printf("Interferencia tipo %d con segmento %d. \n", tipo_interferencia_segmento[segmento_actual],segmento_actual); 
				printf("Angulo Segmento: %f \n", angulo_segmento_actual*360/(2*3.1416));
				printf("Velocidad tras anulacion: vx=%f, vy=%f \n", velocidad_real_moneda.vx, velocidad_real_moneda.vy );
				printf("Fx = %f, Fy = %f \n", fuerzas_normales_segmentos[segmento_actual].fx, fuerzas_normales_segmentos[segmento_actual].fy );
				#endif
			}
			else
			{
				// Sin interferencia
				fuerzas_normales_segmentos[segmento_actual].fx = 0; fuerzas_normales_segmentos[segmento_actual].fy = 0;
			}

		}

		////////////////////////////////////////////////////////////////////////////////////

		// Suma fuerzas
		aceleracion_real_moneda = Fuerza2Aceleracion( SumaFuerzas( gravedad, fuerzas_normales_segmentos, mapa_original.NumeroSegmentos ), 1 ); //Masa = 1 de momento (TODO)
		// Actualiza velocidad
		velocidad_real_moneda = Aceleracion2Velocidad( velocidad_real_moneda, aceleracion_real_moneda, tiempo_imagen);
		//Actualiza posición
		pos_real_moneda = Velocidad2Posicion( pos_real_moneda, velocidad_real_moneda, tiempo_imagen);
		// Actualiza angulo de giro de la moneda
		angulo_rotacion_moneda = VelAng2Angulo( angulo_rotacion_moneda, vel_angular_moneda, tiempo_imagen); 

		angulo_anterior = angulo;

		/////////////////////////////////////////////////////////////////////////////////////


		// Convertimos posiciones "reales" en posiciones de la pantalla
		pos_pant_moneda = CalculaCamara(pos_real_moneda );

		//PENDIENTE DE HACER, TRANSFORMAR MAPA A LA PANTALLA (TODO)		

		/////////////////////////////////////////////////////////////////////////////////////
		// Representa todo por pantalla

		 //Clear screen 
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer ); 

		//Dibuja fondo
		SDL_RenderCopy( gRenderer, gTexturaFondo, NULL, NULL );		//Imagen estirada a toda la ventana

		//Render texture to screen - Dibuja moneda
		//Crea un rectangulo en la posicion desesada de la moneda // Nota: SDL_Rect es un struct
		renderQuad.x = pos_pant_moneda.x-gRadioMoneda;		// Coord X de esquina superior izquierda
		renderQuad.y = pos_pant_moneda.y-gRadioMoneda;		// Coord Y de esquina superior izquierda
		renderQuad.w = gDimMonedaX;				// Ancho
		renderQuad.h = gDimMonedaY;				// Alto
		//SDL_RenderCopy( gRenderer, gTexturaMoneda, NULL, &renderQuad ); 	// Version sin rotacion de la moneda
		SDL_RenderCopyEx( gRenderer, gTexturaMoneda, NULL, &renderQuad, angulo_rotacion_moneda*180/PI, NULL /*Rota alrededor del centro*/, SDL_FLIP_NONE );



		//Dibuja lineas
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		//SDL_RenderDrawLine( gRenderer, 0, GAME_SCREEN_HEIGHT / 2, GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT / 2 );
		for ( segmento_actual = 0 ; segmento_actual < mapa_original.NumeroSegmentos ; segmento_actual++ )
		{
			/*SDL_RenderDrawLine( 	gRenderer, 
						mapa_original.Mapa[segmento_actual].start.x ,
						mapa_original.Mapa[segmento_actual].start.y , 
						mapa_original.Mapa[segmento_actual].end.x , 
						mapa_original.Mapa[segmento_actual].end.y );	*/ // Sin giros
			#ifdef DEBUG_INFO
			/*if ( interferencia_segmento[segmento_actual] )
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0x00 );	// Rojo
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );	// Blanco
			}*/
			switch ( tipo_interferencia_segmento[segmento_actual] )
			{
				case sin_interseccion:
					SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );	// Blanco
					break;
				case interseccion_extremo_start:
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0x00 );	// Verde
					break;
				case interseccion_central:
					SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0x00 );	// Rojo
					break;
				case interseccion_extremo_end:
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0x00 );	// Azul
					break;
				default:
					break;
			}
			
			#endif
			SDL_RenderDrawLine( 	gRenderer, 
						segmentos_girados[segmento_actual].start.x ,
						segmentos_girados[segmento_actual].start.y , 
						segmentos_girados[segmento_actual].end.x , 
						segmentos_girados[segmento_actual].end.y );
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
		//system("clear");		// Borra la terminal para leer el estado actual (MUY LENTO)
		#endif

	}
	
	// Liberamos memoria dinamica
	free(mapa_original.Mapa);
	free(segmentos_girados);
	//free(interferencia_segmento);
	free(fuerzas_normales_segmentos);
	free(tipo_interferencia_segmento);
}



