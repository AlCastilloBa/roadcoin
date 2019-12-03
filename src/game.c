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


// Si definido DEBUG_INFO, mostrar textos de informacion por la terminal
#define DEBUG_INFO

//Screen dimension constants 
const int INTRO_SCREEN_WIDTH = 500; 
const int INTRO_SCREEN_HEIGHT = 851;
const int GAME_SCREEN_WIDTH = 800; 
const int GAME_SCREEN_HEIGHT = 600;


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


bool inicializar_intro(void);
bool inicializar_juego(void);
bool loadMedia(void);
bool cerrar_intro(void);
bool close_program(void);
SDL_Texture* CargaTextura( char*, int*, int*, bool );
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


bool inicializar_juego() 
{
	bool success = true;
	gGameWindow = SDL_CreateWindow( 	"Juego", 
					SDL_WINDOWPOS_UNDEFINED, 
					SDL_WINDOWPOS_UNDEFINED, 
					GAME_SCREEN_WIDTH, 
					GAME_SCREEN_HEIGHT, 
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


bool loadGameMedia() 
{ 
	bool success = true; 	//Loading success flag 

	 //Load PNG texture 
	gTexturaMoneda = CargaTextura( "images/moneda1.png" , &gDimMonedaX, &gDimMonedaY, true ); 
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
	gTexturaFondo = CargaTextura( "images/fondo_nubes.png" , NULL, NULL, false ); 
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




// Función "carga textura". Carga una textura en memoria lista para utilizar con un "renderer"
// Argumentos:
//    - ruta_textura: cadena de caracteres, con la ruta a la imagen.
//    - dim_x: puntero a variable "int" donde se guardará el tamaño en dirección X.
//    - dim_y: puntero a variable "int" donde se guardará el tamaño en dirección Y.
//    - transparente: Indica si el color cyan será transparente
SDL_Texture* CargaTextura( char* ruta_textura, int* dim_x, int* dim_y, bool transparente) 
{ 
	SDL_Texture* newTexture = NULL; 		//The final texture
	SDL_Surface* loadedSurface = NULL;

	//Load image at specified path 
	loadedSurface = IMG_Load( ruta_textura ); 
	if( loadedSurface == NULL ) 
	{ 
		printf( "Unable to load image %s! SDL_image Error: %s\n", ruta_textura, IMG_GetError() );
		exit(-1);
	} 
	////Color key image 
	if (transparente=true)
	{
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );  // Color cyan es transparente
	}
	//Create texture from surface pixels 
	newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface ); 
	if( newTexture == NULL ) 
	{ 
		printf( "Unable to create texture from %s! SDL Error: %s\n", ruta_textura, SDL_GetError() );
	} 
	//Dimensiones de la textura
	if (dim_x != NULL || dim_y != NULL)
	{
		*dim_x = loadedSurface->w; 
		*dim_y = loadedSurface->h;
	}
	//Get rid of old loaded surface 
	SDL_FreeSurface( loadedSurface ); 

	return newTexture; 
}




int main( int argc, char* args[] ) 
{ 
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
	printf("Inicializando juego...\n");
	#endif
	if( !inicializar_juego() ) 
	{ 
		printf( "Failed to initialize!\n" );
		exit(-1);
	} 

	//Load media 
	#ifdef DEBUG_INFO
	printf("Cargando archivos juego...\n");
	#endif
	if( !loadGameMedia() ) 
	{ 
		printf( "Failed to load media!\n" ); 
		exit(-1);
	} 
	#ifdef DEBUG_INFO
	printf("Comenzando bucle principal del juego...\n");
	#endif
	bucle_principal_juego();

	//Free resources and close SDL 
	close_program(); 
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
	float angulo;
	float mouse_sensibility = 0.1f;

	struct punto pos_real_moneda;			// Posicion de centro de la moneda (unidades del juego)
	struct posicion_camara pos_pant_moneda;			// Posicion de centro de la moneda (pixeles de la pantalla)

	struct vector_velocidad velocidad_real_moneda;
	struct vector_aceleracion aceleracion_real_moneda;

	struct mapa mapa_original;		// Mapa cargado desde archivo
	struct segmento* segmentos_girados;

	//bool* interferencia_segmento;		// Vector que indica si hay interferencia entre moneda y segmento
	enum tipo_interseccion_circulo_segmento* tipo_interferencia_segmento;

	struct vector_fuerza* fuerzas_normales_segmentos;	// Vector que guarda las fuerzas normales de apoyo sobre segmentos en el fotograma actual

	//////////////////////////////////////////////////////////////////////////////////////////

	//Leer mapa
	mapa_original = CargarMapaDesdeArchivo( "maps/test_map" );
	// DATOS INCIALES, A BORRAR CUANDO SE LEA EL FICHERO
	pos_real_moneda.x = mapa_original.PuntoInicialMoneda.x;			//Pixeles
	pos_real_moneda.y = mapa_original.PuntoInicialMoneda.y;			//Pixeles
	velocidad_real_moneda.vx = 0;						//Pixeles/segundo
	velocidad_real_moneda.vy = 0;						//Pixeles/segundo
	aceleracion_real_moneda.ax = 0;						//Pixeles/segundo²
	aceleracion_real_moneda.ay = mapa_original.Gravedad;			//Pixeles/segundo²
	angulo = 0;
	// Reservamos memoria para los segmentos girados
	segmentos_girados = calloc(mapa_original.NumeroSegmentos, sizeof(struct segmento) );
	if ( segmentos_girados == NULL )
	{
		printf ( "Error: no se puede reservar memoria para segmentos girados\n");
		exit(-1);
	}

	//interferencia_segmento = calloc(mapa_original.NumeroSegmentos, sizeof(bool) );
	tipo_interferencia_segmento = calloc(mapa_original.NumeroSegmentos, sizeof(enum tipo_interseccion_circulo_segmento) );
	fuerzas_normales_segmentos = calloc(mapa_original.NumeroSegmentos, sizeof(struct vector_fuerza) );

	//////////////////////////////////////////////////////////////////////////////////////////

	//Reseteamos contador de tiempo (para no tener en cuenta todo lo anterior)
	lastTime = currentTime;
	currentTime = SDL_GetTicks();	

	///////////////////////////////////////////////////////////////////////////////////////////

	//Activar variables ratón
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	//SDL_WarpMouseInWindow(gGameWindow, GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2 );

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
		if ( mapa_original.TipoGiro == punto_fijo )
		{
			GiraMapaCompleto( mapa_original.Mapa , segmentos_girados, mapa_original.PuntoGiroFijo, mapa_original.NumeroSegmentos, angulo );
		}
		// Nota: falta implementar otros casos


		////////////////////////////////////////////////////////////////////////////////////
		// Determinamos intersecciones
		// PROVISIONAL
		
		for ( segmento_actual = 0 ; segmento_actual < mapa_original.NumeroSegmentos ; segmento_actual++ )
		{
			/*
			if ( CirculoTocaSegmento(pos_real_moneda, gRadioMoneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end ) )
			//if (CirculoTocaSegmentoExcluyendoExtremos(pos_real_moneda, gRadioMoneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end ) )
			//if ( ProyeccionEstaEnSegmento(pos_real_moneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end ) )
			{
				#ifdef DEBUG_INFO
				printf("Info: moneda toca segmento %d \n", segmento_actual);
				#endif
				interferencia_segmento[segmento_actual] = true;
			}
			else
			{
				interferencia_segmento[segmento_actual] = false;
			}
			*/

			tipo_interferencia_segmento[segmento_actual] =  CirculoTocaSegmento(pos_real_moneda, gRadioMoneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end );

			// Nota: hay que meter la comprobacion de segmentos contando con la posicion anterior (PENDIENTE)

		}
		////////////////////////////////////////////////////////////////////////////////////
		// En caso de intersección:
		// Anulamos componente velocidad normal al segmento
		// Calculamos posicion tangente
		// Calculamos fuerzas normales
		// TODO ESTO ES LO DIFICIL !!!!


		////////////////////////////////////////////////////////////////////////////////////

		// Actualiza velocidad
		velocidad_real_moneda = Aceleracion2Velocidad( velocidad_real_moneda, aceleracion_real_moneda, tiempo_imagen);
		//Actualiza posición
		pos_real_moneda = Velocidad2Posicion( pos_real_moneda, velocidad_real_moneda, tiempo_imagen);



		/////////////////////////////////////////////////////////////////////////////////////


		// Convertimos posiciones "reales" en posiciones de la pantalla
		pos_pant_moneda = CalculaCamara(pos_real_moneda );

		//PENDIENTE DE HACER, TRANSFORMAR MAPA A LA PANTALLA		

		// Representa todo por pantalla

		 //Clear screen 
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer ); 

		//Dibuja fondo
		SDL_RenderCopy( gRenderer, gTexturaFondo, NULL, NULL );		//Imagen estirada a toda la ventana

		//Render texture to screen 
		// No entiendo ¿esto es C o C++? (ACLARAR) (Puede ser un array de C99)
		SDL_Rect renderQuad = { pos_pant_moneda.x-gRadioMoneda, pos_pant_moneda.y-gRadioMoneda, gDimMonedaX, gDimMonedaY }; 	//Crea un rectangulo en la posicion desesada de la moneda
		SDL_RenderCopy( gRenderer, gTexturaMoneda, NULL, &renderQuad ); 


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
	}
	
	// Liberamos memoria dinamica
	free(mapa_original.Mapa);
	free(segmentos_girados);
	//free(interferencia_segmento);
	free(fuerzas_normales_segmentos);
	free(tipo_interferencia_segmento);
}



