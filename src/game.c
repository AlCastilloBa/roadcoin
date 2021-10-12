//Using SDL and standard IO 
#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h> 
#include <stdbool.h>


#include "physics.h"
#include "maps.h"
#include "geometry.h"
#include "options.h"
#include "graphics.h"
#include "menu.h"
#include "camera.h"
#include "sound.h"
#include "enemies.h"



// Si definido DEBUG_INFO, mostrar textos de informacion por la terminal (hace el programa más lento)
// If DEBUG_INFO is defined, information texts will be shown via terminal (but the program will run slower)
// #define DEBUG_INFO

//Screen dimension constants 
const int INTRO_SCREEN_WIDTH = 500; 
const int INTRO_SCREEN_HEIGHT = 851;

#define DESPLAZAMIENTO_CAMARA_USUARIO 10

#define NUM_ANIMS_WORMHOLE 8

////////////////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES
//The window we'll be rendering to 
SDL_Window* gIntroWindow = NULL; 
SDL_Window* gGameWindow = NULL; 

//The surface contained by the window 
SDL_Surface* gScreenSurface = NULL; 

// Pantalla intro --- Intro screen
SDL_Surface* gscreenIntro = NULL;

//The window renderer 
SDL_Renderer* gRenderer = NULL;

//Texturas del juego --- Game textures
SDL_Texture* gTexturaMoneda = NULL;		// Coin texture
SDL_Texture* gTexturaFondo = NULL;		// Background texture
SDL_Texture* gTexturaPausa = NULL;		// Pause texture
SDL_Texture* gPulseTecla = NULL;		// Press any key texture
SDL_Texture* gNombreMapa = NULL;		// Map name texture
SDL_Texture* gDescripcionMapa = NULL;		// Map description texture
SDL_Texture* gIconoVictoria = NULL;		// Victory icon texture
SDL_Texture* gIconoPierde = NULL;		// Lose icon texture
SDL_Texture* gTextoVictoria = NULL;		// Victory text texture
SDL_Texture* gTextoPierde = NULL;		// Lose text texture
SDL_Texture* gTexturaSegmentoPared = NULL;	// Wall Line segment texture (17/5/2020)
SDL_Texture* gTexturaSegmentoMeta = NULL;	// Goal Line segment texture (17/5/2020)
SDL_Texture* gTexturaSegmentoMuerte = NULL;	// Death Line segment texture (17/5/2020)
SDL_Texture* gTexturaFondoGiratorio = NULL;	// Rotating background texture
SDL_Texture* gTexturaPinballBumper = NULL;	// Pinball bumper texture
SDL_Texture* gTexturaPinballFlipperL = NULL;	// Left pinball flipper texture
SDL_Texture* gTexturaPinballFlipperR = NULL;	// Right pinball flipper texture
SDL_Texture* gTexturaCronometro = NULL;		// Stopwatch icon texture
SDL_Texture* gTexturaNumero[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};		// Numbers (0 to 9) texture
SDL_Texture* gTexturaZonaAcelCirc = NULL;	// Round Acceleration Zone Texture
SDL_Texture* gTexturaFondoSemiTranspTexto = NULL;	// Semi-transparent background for texts
SDL_Texture* gTextureLoadingMenu = NULL;	// Pantalla espera carga menu --- Menu loading screen (13/5/2020)
SDL_Texture* gTextureWormholeSpritesheet = NULL;	// Textura de animacion wormhole (27/9/2021) --- Wormhole animation texture

SDL_Rect gCuadroSpriteWormhole[NUM_ANIMS_WORMHOLE];		// Cuadros para animacion wormhole (27/9/2021) --- Squares for defining wormhole animation sprites


TTF_Font *gFuenteTextoJuego = NULL;
SDL_Color gColorBlanco = { 255 , 255 , 255 }; 	// Blanco --- White


//Variables globales moneda --- Coin global variables
int gDimMonedaX; int gDimMonedaY;
int gRadioMoneda;
//Dimensiones texturas --- Texture dimensions
int gDimIconoVictoriaX; int gDimIconoVictoriaY;
int gDimIconoPierdeX; int gDimIconoPierdeY;
int gDimTexturaSegmentoParedX; int gDimTexturaSegmentoParedY;		// 17/5/2020
int gDimTexturaSegmentoMetaX; int gDimTexturaSegmentoMetaY;		// 17/5/2020
int gDimTexturaSegmentoMuerteX; int gDimTexturaSegmentoMuerteY;		// 17/5/2020
int gDimTexturaPinballFlipper_L_X; int gDimTexturaPinballFlipper_L_Y;
int gDimTexturaPinballFlipper_R_X; int gDimTexturaPinballFlipper_R_Y;
int gDimTexturaCronometroX; int gDimTexturaCronometroY;	
int gDimTexturaNumeroX; int gDimTexturaNumeroY; 
int gDimTexturaPausaX; int gDimTexturaPausaY;
int gDimPulseTeclaX; int gDimPulseTeclaY;
int gDimNombreMapaX; int gDimNombreMapaY; 
int gDimDescripcionMapaX; int gDimDescripcionMapaY;
int gDimTextoPierdeX; int gDimTextoPierdeY;
int gDimTextoVictoriaX; int gDimTextoVictoriaY;
int gDimTextureLoadingMenuX; int gDimTextureLoadingMenuY;
int gDimWormholeSprSheetX; int gDimWormholeSprSheetY;		// 27/9/2021

//Variables globales control --- Control global variables
float inc_angulo_teclado = 1.0f;

// Variables globales opciones --- Options global variables
struct opciones opciones_juego;

// Variables globales menu (ver menu.c) --- Menu global variables (see menu.c)
struct pantalla_menu* pantallas_menu_principal;


// Variables globales musica y sonido de juego --- Music and sound global variables in game
Mix_Music *gMusicaJuego = NULL;
Mix_Chunk *gSonidoGolpeMoneda = NULL;
Mix_Chunk *gSonidoPinballBumper = NULL;
Mix_Chunk *gSonidoPinballFlipper = NULL;
Mix_Chunk *gSonidoTeletransporte = NULL;

// Variables globales, tipo de enemigos (17/9/2021)
struct tipos_enemigos_usados_en_mapa gTiposEnemigosMapaActual[MAX_TIPOS_ENEMIGOS_POR_MAPA];


///////////////////////////////////////////////////////////////////////////


bool inicializar_bibliotecas_SDL()		// Nuevo 13/5/2020
{
	bool success = true; 			//Initialization flag 
	int imgFlags = IMG_INIT_PNG; 		//Initialize PNG loading

	//Initialize SDL (y joystick)
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )  //When there's an error, SDL_Init returns -1
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
	//Initialize SDL_mixer
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	 return success; 
}



bool inicializar_intro() 
{ 
	bool success = true; 			//Initialization flag 
	int imgFlags = IMG_INIT_PNG; 		//Initialize PNG loading

/*	//Initialize SDL (y joystick)
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )  //When there's an error, SDL_Init returns -1
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
	//Initialize SDL_mixer
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
*/
 
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
	gGameWindow = SDL_CreateWindow( /*Titulo ventana*/ VERSION_JUEGO , 
					SDL_WINDOWPOS_UNDEFINED, 
					SDL_WINDOWPOS_UNDEFINED, 
					opciones_juego.screen_x_resolution, 
					opciones_juego.screen_y_resolution, 
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


bool loadMainGameLoopMedia( struct mapa mapa_cargado )
{ 
	// Esta función carga las texturas, sonidos, etc asociados al mapa que se va a jugar.
	// This function loads textures, sounds, etc related to the currend map to be played.

	bool success = true; 	//Loading success flag 

	 //Load PNG texture - Moneda  --- Coin
	gTexturaMoneda = CargaTextura( mapa_cargado.RutaImagenMoneda , &gDimMonedaX, &gDimMonedaY, true ); 
	if( gTexturaMoneda == NULL ) 
	{ 
		printf( "Error: no se ha podido cargar la imagen moneda: %s\n", mapa_cargado.RutaImagenMoneda ); 
		success = false; 
	} 

	#ifdef DEBUG_INFO 
	printf("Dimensiones textura moneda: %d x %d\n", gDimMonedaX, gDimMonedaY);
	#endif
	if (gDimMonedaX != gDimMonedaY )
	{
		printf("Error: La textura de la moneda no es cuadrada (dificilmente rodará).\n");
		exit(-1);
	}
	gRadioMoneda = gDimMonedaX/2;
	#ifdef DEBUG_INFO 
	printf("Radio moneda: %d\n", gRadioMoneda);
	#endif

	 //Load PNG texture - Fondo --- Background
	gTexturaFondo = CargaTextura( mapa_cargado.RutaImagenFondo , NULL, NULL, false );
	if( gTexturaFondo == NULL ) 
	{ 
		printf( "Error: no se ha podido cargar la imagen fondo: %s\n", mapa_cargado.RutaImagenFondo ); 
		success = false; 
	} 

	// Cargar la fuente de los textos del juego --- Load font for in-game texts
	#ifdef DEBUG_INFO
	printf("Inicializando la fuente de los textos del juego...\n");
	#endif
	gFuenteTextoJuego = TTF_OpenFont( /*"fonts/lunasol.ttf"*/ "fonts/ethnocen.ttf", /*28*/ 36 );
	if( gFuenteTextoJuego == NULL )
	{
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}

	//Cargar textura de pausa --- Load pause texture
	gTexturaPausa = RenderizaTextoEnTextura("Pausa", gFuenteTextoJuego, gColorBlanco, &gDimTexturaPausaX, &gDimTexturaPausaY );
	// Cargar textura de pulse cualquier tecla --- Load texture Press any key
	gPulseTecla = RenderizaTextoEnTextura("Pulse cualquier tecla", gFuenteTextoJuego, gColorBlanco, &gDimPulseTeclaX, &gDimPulseTeclaY );
	// Cargar textura de Nombre Mapa --- Load Map Name texture
	gNombreMapa = RenderizaTextoEnTextura( mapa_cargado.NombreMapa , gFuenteTextoJuego, gColorBlanco, &gDimNombreMapaX, &gDimNombreMapaY );
	// Cargar textura de Descripcion Mapa --- Load Map Description texture
	gDescripcionMapa = RenderizaTextoEnTextura( mapa_cargado.DescripcionMapa, gFuenteTextoJuego, gColorBlanco, &gDimDescripcionMapaX, &gDimDescripcionMapaY );
	// Cargar textura de icono de victoria --- Load Victory Icon texture
	gIconoVictoria = CargaTextura( "images/finish.png" , &gDimIconoVictoriaX, &gDimIconoVictoriaY, true );
	if( gIconoVictoria == NULL ) 
	{ 
		printf( "Failed to load texture image!\n" ); 
		success = false; 
	} 
	// Cargar textura de icono de derrota --- Load "you lost" icon texture
	gIconoPierde = CargaTextura( "images/cry.png" , &gDimIconoPierdeX, &gDimIconoPierdeY, true );
	if( gIconoPierde == NULL ) 
	{ 
		printf( "Failed to load texture image!\n" ); 
		success = false; 
	} 
	//Cargar textura de texto de victoria --- Load victory text texture
	gTextoVictoria = RenderizaTextoEnTextura("Completado", gFuenteTextoJuego, gColorBlanco, &gDimTextoVictoriaX, &gDimTextoVictoriaY );
	//Cargar textura de texto de perder --- Load lose text texture
	gTextoPierde = RenderizaTextoEnTextura("Pierdes una moneda", gFuenteTextoJuego, gColorBlanco, &gDimTextoPierdeX, &gDimTextoPierdeY );

	// Nuevo 9/1/2020
	gTexturaPinballBumper = CargaTextura( "images/pinball_bumper.png" , NULL, NULL, true );
	if( gTexturaPinballBumper == NULL ) 
	{ 
		printf( "Failed to load pinball bumper texture image!\n" ); 
		success = false; 
	} 
	gTexturaPinballFlipperL = CargaTextura( "images/pinball_flipper_L.png" , &gDimTexturaPinballFlipper_L_X, &gDimTexturaPinballFlipper_L_Y, true );
	if( gTexturaPinballFlipperL == NULL ) 
	{ 
		printf( "Failed to load left pinball flipper texture image!\n" ); 
		success = false; 
	}
	gTexturaPinballFlipperR = CargaTextura( "images/pinball_flipper_R.png" , &gDimTexturaPinballFlipper_R_X, &gDimTexturaPinballFlipper_R_Y, true );
	if( gTexturaPinballFlipperR == NULL ) 
	{ 
		printf( "Failed to load right pinball flipper texture image!\n" ); 
		success = false; 
	}

	gTexturaCronometro = CargaTextura( "images/stopwatch_icon.png" , &gDimTexturaCronometroX, &gDimTexturaCronometroY, true );
	if( gTexturaCronometro == NULL ) 
	{ 
		printf( "Failed to load stopwatch texture image!\n" ); 
		success = false; 
	}
	gTexturaNumero[0] = CargaTextura( "images/chars/etnocentric/0.png" , &gDimTexturaNumeroX, &gDimTexturaNumeroY, true );
	gTexturaNumero[1] = CargaTextura( "images/chars/etnocentric/1.png" , NULL, NULL, true );
	gTexturaNumero[2] = CargaTextura( "images/chars/etnocentric/2.png" , NULL, NULL, true );
	gTexturaNumero[3] = CargaTextura( "images/chars/etnocentric/3.png" , NULL, NULL, true );
	gTexturaNumero[4] = CargaTextura( "images/chars/etnocentric/4.png" , NULL, NULL, true );
	gTexturaNumero[5] = CargaTextura( "images/chars/etnocentric/5.png" , NULL, NULL, true );
	gTexturaNumero[6] = CargaTextura( "images/chars/etnocentric/6.png" , NULL, NULL, true );
	gTexturaNumero[7] = CargaTextura( "images/chars/etnocentric/7.png" , NULL, NULL, true );
	gTexturaNumero[8] = CargaTextura( "images/chars/etnocentric/8.png" , NULL, NULL, true );
	gTexturaNumero[9] = CargaTextura( "images/chars/etnocentric/9.png" , NULL, NULL, true );
	if ( gTexturaNumero[0] == NULL || gTexturaNumero[1] == NULL || gTexturaNumero[2] == NULL || gTexturaNumero[3] == NULL || gTexturaNumero[4]== NULL || 
		gTexturaNumero[5] == NULL || gTexturaNumero[6] == NULL || gTexturaNumero[7] == NULL || gTexturaNumero[8] == NULL || gTexturaNumero[9] == NULL )
	{
		printf( "Failed to load number texture image!\n" ); 
		success = false; 
	}

	gTexturaZonaAcelCirc = CargaTextura( "images/zona_aceleracion_circ.png" , NULL, NULL, true );
	if( gTexturaZonaAcelCirc == NULL ) 
	{ 
		printf( "Failed to load round acceleration zone texture!\n" ); 
		success = false; 
	}


	gTexturaFondoSemiTranspTexto = CargaTextura( "images/fondo_semitransp_texto.png", NULL, NULL, true );
	if( gTexturaFondoSemiTranspTexto == NULL ) 
	{ 
		printf( "Failed to load semi transparent background for text!\n" ); 
		success = false; 
	}
	SDL_SetTextureBlendMode( gTexturaFondoSemiTranspTexto, SDL_BLENDMODE_BLEND /*alpha blending*/ );
	SDL_SetTextureAlphaMod( gTexturaFondoSemiTranspTexto, 0.6*255 /*alpha=opacity*/);

	// 17/5/2020
	gTexturaSegmentoPared = CargaTextura( /*"images/metal_1x5.png" (TODO) */mapa_cargado.RutaImagenSegmentoPared , &gDimTexturaSegmentoParedX, &gDimTexturaSegmentoParedY, false );
	if( gTexturaSegmentoPared == NULL ) 
	{ 
		printf( "Error: no se ha podido cargar la imagen del segmento pared %s\n", mapa_cargado.RutaImagenSegmentoPared ); 
		success = false; 
	} 
	gTexturaSegmentoMeta = CargaTextura( mapa_cargado.RutaImagenSegmentoMeta , &gDimTexturaSegmentoMetaX, &gDimTexturaSegmentoMetaY, false );
	if( gTexturaSegmentoMeta == NULL ) 
	{ 
		printf( "Error: no se ha podido cargar la imagen del segmento meta %s\n", mapa_cargado.RutaImagenSegmentoMeta ); 
		success = false; 
	}
	gTexturaSegmentoMuerte = CargaTextura( mapa_cargado.RutaImagenSegmentoMuerte , &gDimTexturaSegmentoMuerteX, &gDimTexturaSegmentoMuerteY, false );
	if( gTexturaSegmentoMuerte == NULL ) 
	{ 
		printf( "Error: no se ha podido cargar la imagen del segmento muerte %s\n", mapa_cargado.RutaImagenSegmentoMuerte ); 
		success = false; 
	}



	if ( mapa_cargado.HayFondoGiratorio )
	{
		gTexturaFondoGiratorio = CargaTextura( mapa_cargado.RutaImagenFondoGiratorio , NULL, NULL, true );
		if( gTexturaFondoGiratorio == NULL ) 
		{ 
			printf( "Error: no se ha podido cargar la imagen del fondo giratorio %s\n", mapa_cargado.RutaImagenFondoGiratorio ); 
			success = false; 
		} 
	}

	// Cargar musica y sonido --- Load music and sound
	gMusicaJuego = Mix_LoadMUS( mapa_cargado.RutaMusica );
	if( gMusicaJuego == NULL )
	{
	    printf( "Error: Error al cargar la musica del juego. SDL_mixer Error: %s\n", Mix_GetError() );
	    //success = false;
	}
	gSonidoPinballBumper = Mix_LoadWAV( "sound/pinball_bumper.wav" );
	if( gSonidoPinballBumper == NULL )
	{
	    printf( "Error: Error al cargar sonido pinball bumper. SDL_mixer Error: %s\n", Mix_GetError() );
	    success = false;
	}

	gSonidoPinballFlipper = Mix_LoadWAV( "sound/pinball_flipper.wav" );
	if( gSonidoPinballFlipper == NULL )
	{
	    printf( "Error: Error al cargar sonido pinball flipper. SDL_mixer Error: %s\n", Mix_GetError() );
	    success = false;
	}

	gSonidoGolpeMoneda = Mix_LoadWAV( "sound/coin_hit_1.wav" );	/* Prueba 15/5/2020 */
	if ( gSonidoGolpeMoneda == NULL )
	{
	    printf( "Error: Error al cargar sonido golpe moneda. SDL_mixer Error: %s\n", Mix_GetError() );
	    success = false;
	}

	gSonidoTeletransporte = Mix_LoadWAV( "sound/teleport_1.wav" );	/* 27/9/2021 */
	if( gSonidoTeletransporte == NULL )
	{
	    printf( "Error: Error al cargar sonido de teletransporte. SDL_mixer Error: %s\n", Mix_GetError() );
	    success = false;
	}

	// Cargar texturas de enemigos tipo ruta (17/9/2021) --- Load textures for path type enemies
	if ( mapa_cargado.NumeroEnemigosRuta != 0 )
	{
		if ( !Inicializar_Texturas_Tipos_Enemigos( gTiposEnemigosMapaActual ) )
		{
			printf( "Error: Error al inicializar texturas de enemigos tipo ruta\n" );
			success = false;
		}
	}

	// Cargar e inicializar sprite sheet de la animacion de wormhole (27/9/2021) --- Load and initialize wormhole animation sprite sheet
	if ( mapa_cargado.NumeroWormholes != 0 )
	{
		int contador_sprites_animacion;

		gTextureWormholeSpritesheet = CargaTextura( "images/wormhole_01.png" , &gDimWormholeSprSheetX, &gDimWormholeSprSheetY, true );
		if( gTextureWormholeSpritesheet == NULL ) 
		{ 
			printf( "Error: no se ha podido cargar el sprite sheet de la animacion de wormhole.\n" ); 
			success = false; 
		}
		// Verificar dimensiones del sprite sheet --- Check sprite sheet dimensions
		if ( gDimWormholeSprSheetX != (gDimWormholeSprSheetY * NUM_ANIMS_WORMHOLE ) )
		{
			printf( "Error: El sprite sheet del wormhole no tiene las dimensiones adecuadas.\n" );
			printf( "Nota: Mide %d x %d. La dimX deberia ser %d veces la dimY.\n", gDimWormholeSprSheetX, gDimWormholeSprSheetY, NUM_ANIMS_WORMHOLE );
			success = false;			
		}

		// Calcular cuadros, el sprite sheet contiene la animacion de izquierda a derecha. --- Calculate squares, the sprite sheet contais the animation from left to right
		// Cada sprite es cuadrado --- Every sprite is square
		for ( contador_sprites_animacion = 0 ; contador_sprites_animacion < NUM_ANIMS_WORMHOLE ;  contador_sprites_animacion++ )
		{
			gCuadroSpriteWormhole[contador_sprites_animacion].x = gDimWormholeSprSheetY * contador_sprites_animacion;
			gCuadroSpriteWormhole[contador_sprites_animacion].y = 0;
			gCuadroSpriteWormhole[contador_sprites_animacion].w = gDimWormholeSprSheetY;
			gCuadroSpriteWormhole[contador_sprites_animacion].h = gDimWormholeSprSheetY;
		}

		// Ajusta transparencia --- Set up transparency
		SDL_SetTextureBlendMode( gTextureWormholeSpritesheet, SDL_BLENDMODE_BLEND /*alpha blending*/ );
		SDL_SetTextureAlphaMod( gTextureWormholeSpritesheet, 0.6*255 /*alpha=opacity*/);
	}

	return success;
}


bool freeMainGameLoopMedia()
{
	// Esta función libera los sonidos, texturas, etc, asociados al mapa que se acaba de jugar
	// This funcion frees sounds, textures, etc, related to the map that as been played
	#ifdef DEBUG_INFO
	printf("Liberando memoria de media del juego...\n");
	#endif

	//Liberar imágenes --- Free loaded images
	SDL_DestroyTexture( gTexturaMoneda );  gTexturaMoneda = NULL;
	gDimMonedaX=0; gDimMonedaY=0; gRadioMoneda=0;
	SDL_DestroyTexture( gTexturaFondo ); gTexturaFondo = NULL;
	SDL_DestroyTexture( gTexturaPausa ); gTexturaPausa = NULL;
	SDL_DestroyTexture( gPulseTecla ); gPulseTecla = NULL;
	SDL_DestroyTexture( gDescripcionMapa ); gDescripcionMapa = NULL;
	SDL_DestroyTexture( gIconoVictoria ); gIconoVictoria = NULL;
	SDL_DestroyTexture( gIconoPierde ); gIconoPierde = NULL;
	SDL_DestroyTexture( gTexturaPausa ); gTexturaPausa = NULL;
	SDL_DestroyTexture( gTexturaPinballBumper ); gTexturaPinballBumper = NULL;
	SDL_DestroyTexture( gTexturaPinballFlipperL ); gTexturaPinballFlipperL = NULL;
	SDL_DestroyTexture( gTexturaPinballFlipperR ); gTexturaPinballFlipperR = NULL;
	SDL_DestroyTexture( gTexturaCronometro ); gTexturaCronometro = NULL;
	SDL_DestroyTexture( gTexturaNumero[0] ); gTexturaNumero[0] = NULL;
	SDL_DestroyTexture( gTexturaNumero[1] ); gTexturaNumero[1] = NULL;
	SDL_DestroyTexture( gTexturaNumero[2] ); gTexturaNumero[2] = NULL;
	SDL_DestroyTexture( gTexturaNumero[3] ); gTexturaNumero[3] = NULL;
	SDL_DestroyTexture( gTexturaNumero[4] ); gTexturaNumero[4] = NULL;
	SDL_DestroyTexture( gTexturaNumero[5] ); gTexturaNumero[5] = NULL;
	SDL_DestroyTexture( gTexturaNumero[6] ); gTexturaNumero[6] = NULL;
	SDL_DestroyTexture( gTexturaNumero[7] ); gTexturaNumero[7] = NULL;
	SDL_DestroyTexture( gTexturaNumero[8] ); gTexturaNumero[8] = NULL;
	SDL_DestroyTexture( gTexturaNumero[9] ); gTexturaNumero[9] = NULL;
	SDL_DestroyTexture( gTexturaZonaAcelCirc ); gTexturaZonaAcelCirc = NULL;
	SDL_DestroyTexture( gTexturaFondoSemiTranspTexto ); gTexturaFondoSemiTranspTexto = NULL;
	SDL_DestroyTexture( gTexturaSegmentoPared ); gTexturaSegmentoPared = NULL;
	SDL_DestroyTexture( gTexturaSegmentoMeta ); gTexturaSegmentoMeta = NULL;
	SDL_DestroyTexture( gTexturaSegmentoMuerte); gTexturaSegmentoMuerte = NULL;
	// Liberar musica y sonidos --- Free music and sounds
	Mix_FreeMusic( gMusicaJuego ); gMusicaJuego = NULL;
	Mix_FreeChunk( gSonidoPinballBumper); gSonidoPinballBumper = NULL;
	Mix_FreeChunk( gSonidoPinballFlipper); gSonidoPinballFlipper = NULL;
	Mix_FreeChunk( gSonidoGolpeMoneda ); gSonidoGolpeMoneda = NULL;
	Mix_FreeChunk( gSonidoTeletransporte ); gSonidoTeletransporte = NULL;
	// Liberar texturas de enemigos --- Free enemy textures
	Liberar_Memoria_Tipos_Enemigos( gTiposEnemigosMapaActual );
	// Liberar texturas de wormhole (27/9/2021) --- Free wormholes textures
	SDL_DestroyTexture( gTextureWormholeSpritesheet); gTextureWormholeSpritesheet = NULL;
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

	//Destroy window and renderers
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gIntroWindow );
	SDL_DestroyWindow( gGameWindow );
	gIntroWindow = NULL; 
	gGameWindow = NULL; 

	gRenderer = NULL;

	//Quit SDL subsystems 
	Mix_Quit();
	IMG_Quit();
	SDL_Quit(); 
}


int main( int argc, char* args[] ) 
{ 
	#ifdef DEBUG_INFO
	printf("Cargando opciones...\n");
	#endif

	opciones_juego = CargarArchivoOpciones();

	// Inicializar bibliotecas SDL --- Start up SDL libraries
	#ifdef DEBUG_INFO
	printf("Inicializando SDL...\n");
	#endif
	if( !inicializar_bibliotecas_SDL() ) 
	{ 
		printf( "Error al inicializar bibliotecas SDL!\n" );
		exit(-1);
	} 


/*	//Create Intro window 
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
	SDL_Delay( 200 ); 
	
	cerrar_intro();
*/
	#ifdef DEBUG_INFO
	printf("Inicializando ventana del juego...\n");
	#endif
	if( !inicializar_ventana_juego() ) 
	{ 
		printf( "Failed to initialize!\n" );
		exit(-1);
	} 

	// Muestra una imagen indicando que se está cargando el menu --- Shows an image that tells that menu is loading
	gTextureLoadingMenu = CargaTextura ( "images/loading_screen_1.jpg" , &gDimTextureLoadingMenuX, &gDimTextureLoadingMenuY, false );
	if( gTextureLoadingMenu == NULL ) 
	{
		printf( "Error al cargar textura imagen cargando menu.\n" ); 
	}
	else
	{
		RepresentaTextura ( gRenderer, gTextureLoadingMenu, 
				gDimTextureLoadingMenuX , gDimTextureLoadingMenuY, 
				0 , opciones_juego.screen_x_resolution , 0 , opciones_juego.screen_y_resolution,
				con_relac_aspecto_rellenar );
		//Update screen 
		SDL_RenderPresent( gRenderer );
		// Borrar textura --- Delete texture
		SDL_DestroyTexture ( gTextureLoadingMenu ); gTextureLoadingMenu = NULL;
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
	// Liberar memoria del menu --- Free menu memory
	liberar_memoria_menu_principal( NUM_TOTAL_PANTALLAS_MENU );

	#ifdef DEBUG_INFO
	printf("Guardando opciones del juego...\n");
	#endif

	GuardarArchivoOpciones ( opciones_juego );
	
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




enum resultado bucle_principal_juego( char* ruta_mapa )
{
	 
	bool quit = false;		//Main loop flag
	bool pause = false;	 	// Game is paused
	bool intro_mapa = true;		// Game is in "intro map scene", showing map and description
	bool win = false;		// Player won, and the game is frozen displaying results
	bool lose = false;		// Player lost, and the game is frozen displaying results

	bool pinball_flipper_L_activo = false, pinball_flipper_R_activo = false;
	bool flanco_asc_pinball_flipper_L = false, flanco_asc_pinball_flipper_R = false;	
	bool flanco_desc_pinball_flipper_L = false, flanco_desc_pinball_flipper_R = false;	// Pruebas 22/4/2020, Pinball flippers, flanco descendente

	bool contacto_con_algun_segmento = false;	// Pruebas 12/5/2020 --- Contact between the coin and any line segment on current frame

	enum resultado resultado_final = abortado;

	int contador_frames = 0;
	SDL_Event e;		 //Event handler 
	Uint32 currentTime = 0, lastTime, deltaTime, tiempo_imagen_sobrante; //Current time start time (milisegundos)

	int TiempoInicialCuentaAtras, TiempoActualCuentaAtras;	// Tiempo restante partida (milisegundos) --- Remaining game time (milliseconds)

	float tiempo_imagen;	//Tiempo de cada imagen, en segundos --- Time for each frame, in seconds
	float framerate_deseado = 30;
	int segmento_actual, bumper_actual, zona_acel_circ_actual;
	int enemigo_ruta_actual;	// (17/9/2021)
	int wormhole_actual;		// (27/9/2021)
	float angulo, angulo_anterior;
	bool existe_limite_angulo;

	float angulo_rotacion_moneda = 0.0f;	// En radianes --- Radians
	float vel_angular_moneda = 0.0f;	// En rad/s --- Rad/s

	struct punto pos_real_moneda;			// Posicion de centro de la moneda (unidades del juego) --- Coin center position (on game units)
	struct punto pos_real_moneda_anterior;		// Posicion del centro de la moneda, pero en el fotograma anterior --- Coin center position, on previous frame

	struct vector_velocidad velocidad_real_moneda;
	struct vector_velocidad velocidad_real_moneda_anterior;		// Pruebas 12/5/2020 (TODO)

	struct vector_aceleracion aceleracion_real_moneda;

	struct mapa mapa_original;		// Mapa cargado desde archivo --- Map loaded from file
	struct segmento* segmentos_girados;
	struct segmento* pos_camara_segmentos_girados;		// Posición en la pantalla de los segmentos girados --- Position on screen of rotated line segments

	struct punto* centros_bumpers_girados;			// Vector de posiciones de los bumpers despues de giro --- Array of pinball bumper positions after map rotation
	struct punto* centros_zonaacelcirc_girados;		// Vector de posiciones de los centros de las zonas circulares de aceleración despues de giro --- Array of center positions of round aceleration zones after map rotation
	struct punto* pos_pant_centros_bumpers_girados;		// Vector de posiciones de los bumpers en la pantalla despues de giro --- Array of pinball bumper positions on screen after map rotation
	struct punto* pos_pant_centros_zonaacelcirc_girados;	// Vector de posiciones de los centros de las zonas circulares de aceleración en la pantalla despues de giro --- Array of center positions on screen of round aceleration zones after map rotation

	struct punto* pos_actual_enemigos_ruta;			// (17/9/2021) Vector de posiciones de los centros de los enemigos tipo ruta --- Array of center position of path type enemies
	struct punto* centros_enemigos_ruta_girados;		// (17/9/2021) Vector de posiciones de los centros de los enemigos tipo ruta despues de giro --- Array of center position of path type enemies after map rotation
	struct punto* pos_pant_centros_enemigos_ruta_girados; 	// (17/9/2021) Vector de posiciones de los centros de los enemigos tipo ruta en la pantalla despues de giro --- Array of center position on screen of path type enemies
	int *num_waypoint_destino;				// (17/9/2021) Vector de waypoints destino de los enemigos tipo ruta --- Array of waypoints of path type enemies
	float *angulos_actual_enemigos_ruta;			// (17/9/2021) Vector de angulos actuales de los enemigos tipo ruta --- Array of current angles of path type enemies

	struct wormhole* centros_wormholes_girados;		// (27/9/2021) Vector de posiciones de los "wormholes" despues de giro --- Array of wormholes position after map rotation
	struct wormhole* pos_pant_centros_wormholes_girados;	// (27/9/2021) Vector de posiciones de los "wormholes" en la pantalla despues de giro --- Array of wormholes position on screen after map rotation
	bool imagen_previa_dentro_de_wormhole = false;		// (28/9/2021) En la imagen previa estaba dentro de un wormhole --- On the previous frame, the coin was inside a wormhole
	bool imagen_actual_dentro_de_wormhole = false;		// (28/9/2021) En la imagen actual está dentro de un wormhole --- On the current wormhole, the coin is inside a wormhole

	enum tipo_interseccion_circulo_segmento* tipo_interferencia_segmento;

	struct vector_fuerza gravedad;	
	struct vector_fuerza* fuerzas_normales_segmentos;	// Vector que guarda las fuerzas normales de apoyo sobre segmentos en el fotograma actual --- Array that stores normal contact forces on current frame
	struct vector_fuerza* fuerza_zonas_acel_circ;		// Vector que guarda las fuerzas generadas por las zonas de aceleración circular en el fotograma actual --- Array that stores forces created by acceleration zones, on current frame

	struct punto pos_pant_moneda;			// Posicion de centro de la moneda en la pantalla (pixeles de la pantalla) --- Coin center position on screen (on screen pixels)
	enum modo_camara ModoCamara;

	int pos_cam_fondo_giratorio_izquierda, pos_cam_fondo_giratorio_arriba, pos_cam_fondo_giratorio_derecha, pos_cam_fondo_giratorio_abajo;
	int desplazamiento_camara_usuario_x = 0;
	int desplazamiento_camara_usuario_y = 0; 
	struct punto punto_origen={0, 0};

	SDL_Rect renderQuad;		// Rectangulo de SDL para indicar la posicion de los objetos a renderizar --- SDL rectangle that indicates position of objects to render

	//////////////////////////////////////////////////////////////////////////////////////////
	//Leer mapa
	mapa_original = CargarMapaDesdeArchivo( ruta_mapa );

	// DATOS INCIALES, A BORRAR CUANDO SE LEA EL FICHERO
	pos_real_moneda.x = mapa_original.PuntoInicialMoneda.x;			//Pixeles
	pos_real_moneda.y = mapa_original.PuntoInicialMoneda.y;			//Pixeles
	pos_real_moneda_anterior = pos_real_moneda;				// Nuevo 25/1/2020 --- Guardamos posicion del fotograma anterior --- We store the position on the last frame
	velocidad_real_moneda.vx = 0;						//Pixeles/segundo
	velocidad_real_moneda.vy = 0;						//Pixeles/segundo
	aceleracion_real_moneda.ax = 0;						//Pixeles/segundo²
	aceleracion_real_moneda.ay = 0;	/*mapa_original.Gravedad*/		//Pixeles/segundo²
	gravedad.fx = 0;							//Pixeles/segundo²
	gravedad.fy = mapa_original.Gravedad;					//Pixeles/segundo²

	angulo = 0; angulo_anterior = 0;


	// Nuevo 14/1/2020, todavía no probado
	if ( mapa_original.AnguloMax < 0.01f )
	{
		existe_limite_angulo = false;		// El mapa puede girar sin limite de angulo --- The map can rotate without an maximum angle limit
	}
	else
	{
		existe_limite_angulo = true;		// Existe un limite de angulo --- An maximum angle limit exists
	}

	// Reserva memoria dinámica --- Dynamic memory allocation
	// Reservamos memoria para los segmentos girados --- Allocate memory for rotated line segments
	segmentos_girados = calloc(mapa_original.NumeroSegmentos, sizeof(struct segmento) );
	if ( segmentos_girados == NULL )
	{
		printf ( "Error: no se puede reservar memoria para segmentos girados\n");
		exit(-1);
	}
	// Reservamos memoria dinamica para las posiciones rotadas de los bumpers --- Allocate memory for rotated bumpers positions
	centros_bumpers_girados = calloc(mapa_original.NumeroPinballBumpers, sizeof(struct punto) );
	if ( centros_bumpers_girados == NULL )
	{
		printf ( "Error: no se puede reservar memoria para las posiciones rotadas de los bumpers\n");
		exit(-1);
	}
	// Reservamos memoria dinámica para las posiciones rotadas de las zonas circulares de aceleración (nuevo 22/3/2020) --- Allocate memory for rotated positions of round acceleration zones
	centros_zonaacelcirc_girados = calloc(mapa_original.NumeroZonasAceleracionCircular, sizeof(struct punto) );
	if ( centros_zonaacelcirc_girados == NULL )
	{
		printf ( "Error: no se puede reservar memoria para las posiciones rotadas de las zonas circulares de aceleración\n");
		exit(-1);
	}

	// Reservamos memoria dinamica para el vector de tipos de interferencia con segmentos (ver geometry.h) --- Allocate memory for array of inferference types (see geometry.h)
	tipo_interferencia_segmento = calloc(mapa_original.NumeroSegmentos, sizeof(enum tipo_interseccion_circulo_segmento) );
	if ( tipo_interferencia_segmento == NULL )
	{
		printf ( "Error: no se puede reservar memoria para tipo_interferencia_segmento\n");
		exit(-1);
	}

	// Memoria dinamica para las fuerzas normales provocadas por el contacto con los segmentos --- Allocate memory for normal contact forces between coin and line segments
	fuerzas_normales_segmentos = calloc(mapa_original.NumeroSegmentos, sizeof(struct vector_fuerza) );
	if ( fuerzas_normales_segmentos == NULL )
	{
		printf ( "Error: no se puede reservar memoria para fuerzas_normales_segmentos\n");
		exit(-1);
	}
	// Memoria dinamica para las fuerzas provocadas por las zonas circuales de aceleracion --- Allocate memory for foces created by round acceleration zones
	fuerza_zonas_acel_circ = calloc(mapa_original.NumeroZonasAceleracionCircular, sizeof(struct vector_fuerza) );
	if ( fuerza_zonas_acel_circ == NULL )
	{
		printf ( "Error: no se puede reservar memoria para fuerza_zonas_acel_circ\n");
		exit(-1);
	}

	// Reservamos memoria para la posición de la pantalla de los segmentos (despues de aplicar la camara) --- Allocate memory for on-screen line segment positions (after calculating camera)
	pos_camara_segmentos_girados = calloc(mapa_original.NumeroSegmentos, sizeof(struct segmento) );
	if ( pos_camara_segmentos_girados == NULL )
	{
		printf ( "Error: no se puede reservar memoria para la posición en la pantalla de los segmentos girados\n");
		exit(-1);
	}
	// Reservamos memoria para la posición de la pantalla de los bumpers (despues de aplicar la camara) --- Allocating memory for on-screen bumper positions (after calculating camera)
	pos_pant_centros_bumpers_girados = calloc(mapa_original.NumeroPinballBumpers, sizeof(struct punto) );
	if ( pos_pant_centros_bumpers_girados == NULL )
	{
		printf ( "Error: no se puede reservar memoria para la posición en la pantalla de los bumpers\n");
		exit(-1);
	}
	// Reservamos memoria dinámica para las posiciones rotadas de las zonas circulares de aceleración (después de aplicar la cámara) --- Allocating memory for on-screen rotated positions of round acceleration areas
	pos_pant_centros_zonaacelcirc_girados = calloc(mapa_original.NumeroZonasAceleracionCircular, sizeof(struct punto) );
	if ( pos_pant_centros_zonaacelcirc_girados == NULL )
	{
		printf ( "Error: no se puede reservar memoria para la posición en la pantalla de las zonas de aceleración circulares\n");
		exit(-1);
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	// Carga de datos de tipos de enemigos (17/9/2021) --- Load enemy types data
	if ( mapa_original.NumeroEnemigosRuta != 0 )
	{
		int tipos_encontrados;

		Inicializar_Tipos_Enemigos_En_Mapa( gTiposEnemigosMapaActual );

		tipos_encontrados = Clasificar_Tipos_Unicos_Enemigos( gTiposEnemigosMapaActual, mapa_original.EnemigosRuta, mapa_original.NumeroEnemigosRuta );
		#ifdef DEBUG_INFO
		printf( "Se han encontrado %d tipos de enemigos en el mapa actual.\n", tipos_encontrados );
		Lista_Tipos_Enemigos_Unicos( gTiposEnemigosMapaActual );
		#endif

		Leer_Ficheros_Definicion_Tipos_Enemigos( gTiposEnemigosMapaActual );


		// Reservamos memoria dinamica para las posiciones actuales de los enemigos (coordenadas mapa) --- Allocate dynamic memory for current enemy positions
		pos_actual_enemigos_ruta = calloc(mapa_original.NumeroEnemigosRuta, sizeof(struct punto) );
		if ( pos_actual_enemigos_ruta == NULL )
		{
			printf ( "Error: no se puede reservar memoria para posicion real de los enemigos tipo ruta girados\n");
			exit(-1);
		}

		// Reservamos memoria dinamica para las posiciones rotadas de los enemigos --- Allocate dynamic memory for current enemy positions after map rotation
		centros_enemigos_ruta_girados = calloc(mapa_original.NumeroEnemigosRuta, sizeof(struct punto) );
		if ( centros_enemigos_ruta_girados == NULL )
		{
			printf ( "Error: no se puede reservar memoria para pos rotada de los enemigos tipo ruta girados\n");
			exit(-1);
		}

		// Reservamos memoria dinamica para las posiciones rotadas de los enemigos (despues de aplicar la cámara) --- Allocate dynamic memory for current enemy positions on screen after map rotation
		pos_pant_centros_enemigos_ruta_girados = calloc(mapa_original.NumeroEnemigosRuta, sizeof(struct punto) );
		if ( pos_pant_centros_enemigos_ruta_girados == NULL )
		{
			printf ( "Error: no se puede reservar memoria para la posición en la pantalla de los enemigos tipo ruta\n");
			exit(-1);
		}

		// Reservamos memoria para los waypoints destino de los enemigos tipo ruta --- Allocate dynamic memory for current destination waypoint
		num_waypoint_destino = calloc(mapa_original.NumeroEnemigosRuta, sizeof(int) );
		if ( num_waypoint_destino == NULL )
		{
			printf ( "Error: no se puede reservar memoria para los waypoints destino de los enemigos tipo ruta\n");
			exit(-1);
		}

		// Reservamos memoria para los angulos actual de los enemigos tipo ruta --- Allocate dynamic memory for current enemy angles
		angulos_actual_enemigos_ruta = calloc(mapa_original.NumeroEnemigosRuta, sizeof(float) );
		if ( angulos_actual_enemigos_ruta == NULL )
		{
			printf ( "Error: no se puede reservar memoria para los angulos actuales de los enemigos tipo ruta\n");
			exit(-1);
		}

		// Inicializamos posiciones iniciales enemigos (waypoint numero cero) --- Initialize current enemy position (waypoint number zero)
		for ( enemigo_ruta_actual = 0 ; enemigo_ruta_actual < mapa_original.NumeroEnemigosRuta ; enemigo_ruta_actual++ )
		{
			pos_actual_enemigos_ruta[enemigo_ruta_actual] = mapa_original.EnemigosRuta[enemigo_ruta_actual].waypoints[0].waypoint;
		}

	}
	// Reservamos memoria dinamica para las posiciones rotadas de los wormholes (27/9/2021) --- Allocate dynamic memory for wormholes position after map rotation
	if ( mapa_original.NumeroWormholes != 0 )
	{
		centros_wormholes_girados = calloc(mapa_original.NumeroWormholes, sizeof(struct wormhole) );
		if ( centros_wormholes_girados == NULL )
		{
			printf ( "Error: no se puede reservar memoria las posiciones rotadas de los wormholes.\n");
			exit(-1);
		}
	}

	// Reservamos memoria dinamica para las posiciones rotadas de los wormholes (despues de aplicar la cámara) (27/9/2021) --- Allocate dynamic memory for wormholes position on screen after map rotation
	if ( mapa_original.NumeroWormholes != 0 )
	{
		pos_pant_centros_wormholes_girados = calloc(mapa_original.NumeroWormholes, sizeof(struct wormhole) );
		if ( pos_pant_centros_wormholes_girados == NULL )
		{
			printf ( "Error: no se puede reservar memoria la posicion en pantalla de las posiciones rotadas de los wormholes.\n");
			exit(-1);
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// Cargamos archivos del juego - Load Game media 
	#ifdef DEBUG_INFO
	printf("Cargando archivos juego...\n");
	#endif

	if ( !loadMainGameLoopMedia( mapa_original ) )
	{ 
		printf( "Error: Failed to load media!\n" ); 
		exit(-1);
	} 

	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	// Inicializamos datos de cuenta atras --- Initialize countdown data
	if ( mapa_original.CuentaAtras == true ) 
	{
		TiempoActualCuentaAtras = TiempoInicialCuentaAtras = mapa_original.SegundosCuentaAtras * 1000;	// Pasamos de segundos a milisegundos --- Convert seconds to milliseconds
		#ifdef DEBUG_INFO
		printf("Tiempo inicial se ajusta a %d TiempoActualCuentaAtras milisegundos ...\n");
		#endif
	}


	//////////////////////////////////////////////////////////////////////////////////////////
	// Inicializamos modo cámara --- Initialize camera mode
	if ( mapa_original.TipoGiro == punto_fijo )
	{
		ModoCamara = camara_fija_en_origen;
	}
	else if ( mapa_original.TipoGiro == moneda || mapa_original.TipoGiro == origen || mapa_original.TipoGiro == no_rot )
	{
		ModoCamara = camara_sigue_moneda;
	}
	// Ajustar variables ratón --- Set mouse variables
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	//SDL_WarpMouseInWindow(gGameWindow, GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2 );

	//////////////////////////////////////////////////////////////////////////////////////////
	// Desactivamos musica del menu y activamos la musica del juego --- Disable menu music and enable game music
	if ( (opciones_juego.music_enabled) && ( gMusicaJuego != NULL) )
	{
		Mix_HaltMusic( );			// Parar musica del menu si se estaba reproduciendo --- Stop menu music, in case it was playing
		Mix_PlayMusic( gMusicaJuego, -1 );	// Reproducir la musica del nivel --- Play current level music
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	//Reseteamos contador de tiempo (para no tener en cuenta todo lo anterior) --- Reset time counter (in order to not take into account all prevoious operations)
	lastTime = currentTime;
	currentTime = SDL_GetTicks();	

	///////////////////////////////////////////////////////////////////////////////////////////

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
			tiempo_imagen = 1/framerate_deseado;		// CHAPUZA, HACER MEJOR (TODO)
		}
		#ifdef DEBUG_INFO
		printf("Frame %d, time %d, FPS=%f, ang=%f \n", contador_frames, deltaTime, 1/tiempo_imagen, angulo);
		#endif

		flanco_asc_pinball_flipper_L = flanco_asc_pinball_flipper_R = false;
		flanco_desc_pinball_flipper_L = flanco_desc_pinball_flipper_R = false;		// Nuevo 22/4/2020, pruebas pinball flipper flanco descendente (TODO)

		velocidad_real_moneda_anterior = velocidad_real_moneda;		// Nuevo Pruebas 12/5/2020 (TODO)
		contacto_con_algun_segmento = false;		// Nuevo Pruebas 12/5/2020 (TODO)

		//////////////////////////////////////////////////////////////////////////////////////
		//  _____       _                 _              _____                   _   
		// | ____|_ __ | |_ _ __ __ _  __| | __ _ ___   / /_ _|_ __  _ __  _   _| |_ 
		// |  _| | '_ \| __| '__/ _` |/ _` |/ _` / __| / / | || '_ \| '_ \| | | | __|
		// | |___| | | | |_| | | (_| | (_| | (_| \__ \/ /  | || | | | |_) | |_| | |_ 
		// |_____|_| |_|\__|_|  \__,_|\__,_|\__,_|___/_/  |___|_| |_| .__/ \__,_|\__|
		//                                                          |_|              
		//Gestión de entradas (teclado, ratón, joystick (TODO)) --- Input management (keyboard, mouse, joystick (TODO))
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
					case SDLK_p:
						pause = !pause;
						break;
					case SDLK_LEFT:
						if ( !pause && !intro_mapa && !win && !lose )
						{
							angulo -= inc_angulo_teclado;
						}
						break;
					case SDLK_RIGHT:
						if ( !pause && !intro_mapa && !win && !lose )
						{
							angulo += inc_angulo_teclado;
						}
						break;
					case SDLK_c:
						// Gestion de cambio de camara --- Camera mode toggle management
						switch ( ModoCamara )
						{
							case camara_fija_en_origen:
								ModoCamara = camara_sigue_moneda;
								break;
							case camara_sigue_moneda:
								ModoCamara = camara_movil_por_usuario;
								break;
							case camara_movil_por_usuario:
								ModoCamara = camara_fija_en_origen;
								break;
							default:
								ModoCamara = camara_sigue_moneda;
								break;
						}
					case SDLK_KP_8:	// Numeric Keypad 8 (UP)
						if (ModoCamara == camara_movil_por_usuario )
							desplazamiento_camara_usuario_y-=DESPLAZAMIENTO_CAMARA_USUARIO;
						break;
					case SDLK_KP_2: // Numeric Keypad 2 (DOWN)
						if (ModoCamara == camara_movil_por_usuario )
							desplazamiento_camara_usuario_y+=DESPLAZAMIENTO_CAMARA_USUARIO;
						break;
					case SDLK_KP_4: // Numeric Keypad 4 (LEFT)
						if (ModoCamara == camara_movil_por_usuario )
							desplazamiento_camara_usuario_x-=DESPLAZAMIENTO_CAMARA_USUARIO;
						break;
					case SDLK_KP_6: // Numeric Keypad 6 (RIGHT)
						if (ModoCamara == camara_movil_por_usuario )
							desplazamiento_camara_usuario_x+=DESPLAZAMIENTO_CAMARA_USUARIO;
						break;
					default:  
						break; 
				}
				if ( intro_mapa )
				{
					//Se ha pulsado cualquier tecla estando en la intro del mapa --- Any key was pressed while on intro screen
					intro_mapa = false;
				}
				if (win || lose)
				{
					//Se ha pulsado cualquier tecla estando en las pantallas de victoria o derrota --- Any key was pressed while on "you win" or "you lose" screens
					quit = true;
				}

			}
			else if ( e.type  == SDL_MOUSEMOTION )
			{
				if (contador_frames != 1     && !pause && !intro_mapa && !win && !lose )
				{
					angulo += (float)(e.motion.xrel) * opciones_juego.mouse_sensitivity;
					//printf ("Mov raton: %f\n", e.motion.xrel);
					//SDL_WarpMouseInWindow(gGameWindow, GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2 );
				}
			}
			if ( e.type == SDL_MOUSEBUTTONDOWN )
			{
				if ( intro_mapa )
				{
					//Se ha pulsado el raton estando en la intro del mapa --- Mouse button was pressed while on intro screen
					intro_mapa = false;
				}
				if (win || lose)
				{
					//Se ha pulsado el raton estando en las pantallas de victoria o derrota --- Any key was pressed while on "you win" or "you lose" screens
					quit = true;
				}
				//////////////////////////////////////////////
				// Gestion pinball flippers
				if ( e.button.button == SDL_BUTTON_LEFT )
				{
					if ( mapa_original.mapa_contiene_flippers && !pause )
					{
						pinball_flipper_L_activo = true;
						flanco_asc_pinball_flipper_L = true;
						//  Reproducir sonido de flipper --- Play pinball flipper sound
						if ( opciones_juego.sound_enabled )
						{
							Mix_PlayChannel( -1, gSonidoPinballFlipper, 0 );
						}
					}
				}
				if (e.button.button == SDL_BUTTON_RIGHT)
				{
					if ( mapa_original.mapa_contiene_flippers && !pause )
					{
						pinball_flipper_R_activo = true;
						flanco_asc_pinball_flipper_R = true;
						//  Reproducir sonido de flipper --- Play pinball flipper sound
						if ( opciones_juego.sound_enabled )
						{
							Mix_PlayChannel( -1, gSonidoPinballFlipper, 0 );
						}
					}
				}
				//////////////////////////////////////////////
			}

			////////////////////////////////////////
			// Gestion pinball flippers -- Pinball flippers management
			if ( e.type == SDL_MOUSEBUTTONUP )
			{
				if ( e.button.button == SDL_BUTTON_LEFT )
				{
					pinball_flipper_L_activo = false;
					flanco_desc_pinball_flipper_L = true;	// (22/4/2020)
				}
				if (e.button.button == SDL_BUTTON_RIGHT)
				{
					pinball_flipper_R_activo = false;
					flanco_desc_pinball_flipper_R = true;	// (22/4/2020)
				}
			}	
		}
		///////////////////////////////////////////////////////////////////////////
		// Limites de angulos --- Angle limits
		if (existe_limite_angulo )
		{
			if ( angulo < -(mapa_original.AnguloMax)  )
			{
				angulo = -(mapa_original.AnguloMax);
			}
			if (angulo > mapa_original.AnguloMax )
			{
				angulo = mapa_original.AnguloMax;
			}
		}
		else	// AnguloMax = 0, se entiende sin limites de giro --- Max Angle = 0, it is implied that no rotation limits
		{	
			// Devuelve el angulo al intervalo 0-360º -- Converts angle to 0-360deg range
			angulo = WrapAngle_0_360( angulo );
		}

		//////////////////////////////////////////////////////////////////////////////

		if ( !pause && !intro_mapa && !win && !lose )
		{
			/////////////////////////////////////////////////////////////////////////////////////
			// Calculamos posiciones enemigos (17/9/2021) --- Calculate enemy positions
			Calcula_Nueva_Posicion_Enemigos( /*numero_enemigos*/ mapa_original.NumeroEnemigosRuta, 
							 /*lista_enemigos*/ mapa_original.EnemigosRuta, 
							 /*lista_tipos*/ gTiposEnemigosMapaActual,
							 /*posic_actual_enemigos*/ pos_actual_enemigos_ruta, 
							 /*angulo_enemigos*/ angulos_actual_enemigos_ruta, 
							 /*waypoint_destino*/ num_waypoint_destino,
							 /*tiempo_imagen*/ tiempo_imagen );
			

			/////////////////////////////////////////////////////////////////////////////////////	
			// Giramos el mapa --- Rotate the map
			switch (mapa_original.TipoGiro)
			{
				case punto_fijo:
					GiraMapaCompleto( mapa_original.Mapa , segmentos_girados, mapa_original.PuntoGiroFijo, mapa_original.NumeroSegmentos, angulo );
					GiraBumpers( mapa_original.Bumpers, centros_bumpers_girados, mapa_original.PuntoGiroFijo, mapa_original.NumeroPinballBumpers, angulo ); // Nuevo 9/2/2020
					GiraZonasAcelCirc( mapa_original.ZonasAceleracionCircular, centros_zonaacelcirc_girados, mapa_original.PuntoGiroFijo, mapa_original.NumeroZonasAceleracionCircular, angulo );  // Nuevo 22/3/2020
					GiraEnemigosRuta( pos_actual_enemigos_ruta, centros_enemigos_ruta_girados, mapa_original.PuntoGiroFijo, mapa_original.NumeroEnemigosRuta, angulo ); // (17/9/2021)
					GiraWormholes( mapa_original.Wormholes, centros_wormholes_girados, mapa_original.PuntoGiroFijo, mapa_original.NumeroWormholes, angulo ); // (27/9/2021)
					break;
				case camara:
					// (TODO)
					printf("ERROR: TODAVIA NO IMPLEMENTADO\n");
					exit(-1);
					break;
				case moneda:
					GiraMapaCompleto( mapa_original.Mapa , segmentos_girados, pos_real_moneda, mapa_original.NumeroSegmentos, angulo );
					GiraBumpers( mapa_original.Bumpers, centros_bumpers_girados, pos_real_moneda, mapa_original.NumeroPinballBumpers, angulo ); // Nuevo 9/2/2020
					GiraZonasAcelCirc( mapa_original.ZonasAceleracionCircular, centros_zonaacelcirc_girados, pos_real_moneda, mapa_original.NumeroZonasAceleracionCircular, angulo );  // Nuevo 22/3/2020
					GiraEnemigosRuta( pos_actual_enemigos_ruta, centros_enemigos_ruta_girados, pos_real_moneda, mapa_original.NumeroEnemigosRuta, angulo ); // (17/9/2021)
					GiraWormholes( mapa_original.Wormholes, centros_wormholes_girados, pos_real_moneda, mapa_original.NumeroWormholes, angulo ); // (27/9/2021)
					break;
				case origen:
					GiraMapaCompleto( mapa_original.Mapa , segmentos_girados, punto_origen, mapa_original.NumeroSegmentos, angulo );
					GiraBumpers( mapa_original.Bumpers, centros_bumpers_girados, punto_origen, mapa_original.NumeroPinballBumpers, angulo ); // Nuevo 9/2/2020
					GiraZonasAcelCirc( mapa_original.ZonasAceleracionCircular, centros_zonaacelcirc_girados, punto_origen, mapa_original.NumeroZonasAceleracionCircular, angulo );  // Nuevo 22/3/2020
					GiraEnemigosRuta( pos_actual_enemigos_ruta, centros_enemigos_ruta_girados, punto_origen, mapa_original.NumeroEnemigosRuta, angulo ); // (17/9/2021)
					GiraWormholes( mapa_original.Wormholes, centros_wormholes_girados, punto_origen, mapa_original.NumeroWormholes, angulo ); // (27/9/2021)
					// Arrastramos tambien a la moneda en el giro --- The map rotation drags the coin
					if ( angulo != angulo_anterior )
					{
						pos_real_moneda = GiraPunto ( punto_origen, pos_real_moneda, angulo-angulo_anterior );
					}
					break;
				case no_rot:
					// Sin rotacion, los segmentos girados son siempre igual que los originales --- No rotation, rotated segments are always equal to their original ones
					CopiaSegmentosSinGiro( mapa_original.Mapa, segmentos_girados, mapa_original.NumeroSegmentos );
					CopiaBumpersSinGiro( mapa_original.Bumpers, centros_bumpers_girados, mapa_original.NumeroPinballBumpers );
					CopiaZonasAcelCircSinGiro( mapa_original.ZonasAceleracionCircular, centros_zonaacelcirc_girados, mapa_original.NumeroZonasAceleracionCircular);  //Nuevo 22/3/2020
					CopiaEnemigosRutaSinGiro( pos_actual_enemigos_ruta, centros_enemigos_ruta_girados, mapa_original.NumeroEnemigosRuta ); // (17/9/2021)
					CopiaWormholesSinGiro( mapa_original.Wormholes, centros_wormholes_girados, mapa_original.NumeroWormholes );		// (27/9/2021)
					angulo = angulo_anterior = 0;
					break;
				default:
					printf("Al proceder a girar el mapa, error de programacion o error de concepto\n");
					exit(-1);
					break;
			}
			// Nota: (TODO) falta implementar otros casos

			////////////////////////////////////////////////////////////////////////////////////
			//   ____      _            _            ____           _      _             
			//  / ___|__ _| | ___ _   _| | __ _ _ __|  _ \ ___  ___(_) ___(_) ___  _ __  
			// | |   / _` | |/ __| | | | |/ _` | '__| |_) / _ \/ __| |/ __| |/ _ \| '_ \ 
			// | |__| (_| | | (__| |_| | | (_| | |  |  __/ (_) \__ \ | (__| | (_) | | | |
			//  \____\__,_|_|\___|\__,_|_|\__,_|_|  |_|   \___/|___/_|\___|_|\___/|_| |_|
			//
			////////////////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////////////////
			// Wormholes (29/9/2021)
			// Verificar si está dentro y teletransporte a la otra posicion --- Check if coin is inside wormhole, and teleport to the other end
			if (mapa_original.NumeroWormholes != 0 )
			{
				imagen_actual_dentro_de_wormhole = false;
				for ( wormhole_actual = 0 ; wormhole_actual < mapa_original.NumeroWormholes ; wormhole_actual++ )
				{
					int radio_wormh_cuad = mapa_original.Wormholes[wormhole_actual].radio*mapa_original.Wormholes[wormhole_actual].radio;
					// Miramos si moneda dentro de P1 --- Check if coin is inside P1
					if ( LongitudVector_Cuadrado( pos_real_moneda , centros_wormholes_girados[wormhole_actual].p1 ) < radio_wormh_cuad )
					{
						if ( !imagen_previa_dentro_de_wormhole )
						{
							pos_real_moneda = centros_wormholes_girados[wormhole_actual].p2;
							// Reproducir sonido teletransporte --- Play teleport sound
							Mix_PlayChannel( -1, gSonidoTeletransporte, 0 );	
						}
						imagen_actual_dentro_de_wormhole = true;
					}
					// Miramos si moneda dentro de P2 --- Check if coin is inside P2
					else if ( LongitudVector_Cuadrado( pos_real_moneda , centros_wormholes_girados[wormhole_actual].p2 ) < radio_wormh_cuad )
					{
						if ( !imagen_previa_dentro_de_wormhole )
						{
							pos_real_moneda = centros_wormholes_girados[wormhole_actual].p1;
							// Reproducir sonido teletransporte --- Play teleport sound
							Mix_PlayChannel( -1, gSonidoTeletransporte, 0 );	
						}
						imagen_actual_dentro_de_wormhole = true;
					}
				}
				imagen_previa_dentro_de_wormhole = imagen_actual_dentro_de_wormhole;
			}

			//////////////////////////////////////////////////////////////////////////////////
			// Calculo posiciones tras golpes con bumpers --- Calculate position after collision with pinball bumpers
			if ( mapa_original.NumeroPinballBumpers != 0 )
			{
				for ( bumper_actual = 0 ; bumper_actual < mapa_original.NumeroPinballBumpers ; bumper_actual++ )
				{
					// Miramos si hay interseccion entre moneda y bumpers --- Check if intersection between coin and bumper
					if (LongitudVector_Cuadrado( pos_real_moneda , centros_bumpers_girados[bumper_actual] ) <  ( gRadioMoneda + mapa_original.Bumpers[bumper_actual].radio )* ( gRadioMoneda + mapa_original.Bumpers[bumper_actual].radio ) )
					{
						// Posiciona la moneda en posicion tangente --- Move the coin to position tangent to bumper
						pos_real_moneda = CalculaPosTangenteCirculoCirculo( pos_real_moneda, centros_bumpers_girados[bumper_actual], gRadioMoneda, mapa_original.Bumpers[bumper_actual].radio);
						// Cancela velocidad normal al bumper (o la tangencial a la recta que une moneda y bumper) --- Make zero normal speed to bumper (or make zero the speed tangential to the line between coin and bumper centers) 
						velocidad_real_moneda = AnulaVelocidadTangencialARecta( velocidad_real_moneda, pos_real_moneda, centros_bumpers_girados[bumper_actual]);
						// Calcula la nueva velocidad --- Calculate new speed
						velocidad_real_moneda = SumaVelocidad( velocidad_real_moneda, VelocidadSobreRecta ( mapa_original.Bumpers[bumper_actual].velocidad_salida, centros_bumpers_girados[bumper_actual], pos_real_moneda ) );
						// Reproduce sonido --- Play sound
						if ( opciones_juego.sound_enabled )
						{
							Mix_PlayChannel( -1, gSonidoPinballBumper, 0 );
						}
					}
				}
			}
			// Fin de calculo posicion de bumpers --- End of pinball bumper position calculation
			/////////////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////////////
			// Zonas de aceleración circulares --- Round acceleration zones
			if ( mapa_original.NumeroZonasAceleracionCircular != 0 )
			{
				for ( zona_acel_circ_actual = 0 ; zona_acel_circ_actual < mapa_original.NumeroZonasAceleracionCircular ; zona_acel_circ_actual++ )
				{
					// Miramos si la moneda está en la zona de aceleración --- Check if coin is inside an acceleration zone
					if (LongitudVector_Cuadrado( pos_real_moneda , centros_zonaacelcirc_girados[zona_acel_circ_actual] ) <  ( mapa_original.ZonasAceleracionCircular[zona_acel_circ_actual].radio )* ( mapa_original.ZonasAceleracionCircular[zona_acel_circ_actual].radio ) )
					{
						// Está en la zona de aceleración, lo aceleramos en la dirección que marque la zona --- It is indeed inside the acceleration zone. We accelerate in the direction indicated by the zone
						fuerza_zonas_acel_circ[zona_acel_circ_actual].fx = mapa_original.ZonasAceleracionCircular[zona_acel_circ_actual].aceleracion * (sin( (mapa_original.ZonasAceleracionCircular[zona_acel_circ_actual].angulo + angulo) * PI/180 ));
						fuerza_zonas_acel_circ[zona_acel_circ_actual].fy = mapa_original.ZonasAceleracionCircular[zona_acel_circ_actual].aceleracion  * (-cos( (mapa_original.ZonasAceleracionCircular[zona_acel_circ_actual].angulo + angulo) * PI/180 ));
					}
					else
					{
						// La fuerza es cero --- Force is zero
						fuerza_zonas_acel_circ[zona_acel_circ_actual].fx = 0; fuerza_zonas_acel_circ[zona_acel_circ_actual].fy = 0;	
					}
				}
			}


			/////////////////////////////////////////////////////////////////////////////////
			// Enemigos tipo ruta (22/9/2021) --- Path type enemies
			if ( mapa_original.NumeroEnemigosRuta != 0 )
			{
				for ( enemigo_ruta_actual = 0 ; enemigo_ruta_actual < mapa_original.NumeroEnemigosRuta ; enemigo_ruta_actual++ )
				{
					// Miramos si la moneda intersecta con el enemigo
					if ( LongitudVector_Cuadrado( pos_real_moneda , centros_enemigos_ruta_girados[enemigo_ruta_actual] ) < 
						( ( gRadioMoneda + gTiposEnemigosMapaActual[  mapa_original.EnemigosRuta[enemigo_ruta_actual].ID_unico_tipo_enemigo  ].radio  ) * 
						( gRadioMoneda + gTiposEnemigosMapaActual[  mapa_original.EnemigosRuta[enemigo_ruta_actual].ID_unico_tipo_enemigo  ].radio ) ) )
					{
						// El enemigo "se come" a la moneda. Hemos perdido
						lose = true;
						resultado_final = derrota;
					}
				}
			}	

			/////////////////////////////////////////////////////////////////////////////////
			// Aplica giro a los segmentos "flippers" --- Rotation is applied to flipper segments
			if ( mapa_original.mapa_contiene_flippers)
			{
				if (pinball_flipper_L_activo)
				{
					for ( segmento_actual = 0 ; segmento_actual < mapa_original.NumeroSegmentos ; segmento_actual++ )
					{
						if ( mapa_original.Mapa[segmento_actual].type == pinball_flipper_L )
						{
							// Giramos END respecto a START (ya que están siempre orientados de izquierda a derecha --- We rotate END with respect to START (because segments are always oriented from left to right)
							segmentos_girados[segmento_actual].end = GiraPunto ( segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end, -mapa_original.angulo_flippers /*grados*/ );
						}
					}
				}
				if (pinball_flipper_R_activo)
				{
					for ( segmento_actual = 0 ; segmento_actual < mapa_original.NumeroSegmentos ; segmento_actual++ )
					{
						if ( mapa_original.Mapa[segmento_actual].type == pinball_flipper_R )
						{
							// Giramos START respecto a END (ya que están siempre orientados de izquierda a derecha --- We rotate START with respect to END (because segments are always oriented from left to right)
							segmentos_girados[segmento_actual].start = GiraPunto ( segmentos_girados[segmento_actual].end, segmentos_girados[segmento_actual].start, mapa_original.angulo_flippers /*grados*/);
						}
					}
				}	
			}
			/////////////////////////////////////////////////////////////////////////////////


			/////////////////////////////////////////////////////////////////////////////////
			// Determinamos intersecciones --- Calculate intersections
			for ( segmento_actual = 0 ; segmento_actual < mapa_original.NumeroSegmentos ; segmento_actual++ )
			{
				tipo_interferencia_segmento[segmento_actual] =  CirculoTocaSegmento(pos_real_moneda, gRadioMoneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end );

				/*// Nuevo 25/1/2020--- PRUEBAS (TODO) --- TODAVIA NO FUNCIONA -- NOT WORKING YET
				if ( ( tipo_interferencia_segmento[segmento_actual]==0) &&    LongitudVector_Cuadrado(pos_real_moneda,pos_real_moneda_anterior)>(gRadioMoneda*gRadioMoneda) )
				{
					// Si la distancia de la posicion de la moneda entre fotogramas sucesivos es mayor que el radio
					// (o bien el PC es lento, o bien la moneda tiene una velocidad muy alta)
					// podriamos correr el riesgo de atravesar un segmento y no enterarnos
					if ( doIntersect( pos_real_moneda , pos_real_moneda_anterior, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end ) )
					{
						tipo_interferencia_segmento[segmento_actual]=interseccion_atravesado;
					}
				}*/
				// Nota: hay que meter la comprobacion de segmentos contando con la posicion anterior (PENDIENTE, TODO)
				// Si hay interferencia, calculamos posicion tangente (primera vez) --- If there is interferencie, calculate tangent position (first time)
				if ( tipo_interferencia_segmento[segmento_actual] != 0 )	// Si hay interferencia --- If there is interference
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
						/*case interseccion_atravesado:		// Nuevo 25/1/2020 -- PRUEBAS (TODO) TODAVIA NO FUNCIONA -- NOT WORKING YET
							pos_real_moneda = CalculaPosTangenteACentroSegmento ( segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end, pos_real_moneda_anterior, gRadioMoneda);
							break;*/
						default:
							#ifdef DEBUG_INFO
							printf("En primer calculo pos tangente. No posible, error de programacion o de concepto.\n");
							#endif
							break;
					}

					if ( mapa_original.Mapa[segmento_actual].type == meta )
					{
						win = true;
						resultado_final = victoria;
					}
					if ( mapa_original.Mapa[segmento_actual].type == muerte )
					{
						lose = true;
						resultado_final = derrota;
					}
					if ( mapa_original.Mapa[segmento_actual].type == pared )
					{
						contacto_con_algun_segmento = true;	// Pruebas 12/5/2020
					}
				}

			}
			////////////////////////////////////////////////////////////////////////////////////
			// En caso de intersección: --- In case of intersection
			// ESTO ES LO DIFICIL !!!! --- THIS IS THE HARD PART !!!!
			for ( segmento_actual = 0 ; segmento_actual < mapa_original.NumeroSegmentos ; segmento_actual++ )
			{

				if ( tipo_interferencia_segmento[segmento_actual] != 0 )	// Si hay interferencia --- If there is interference
				{
					float angulo_segmento_actual;

					angulo_segmento_actual = AnguloSegmento( segmentos_girados[segmento_actual] );


					// Anulamos componente velocidad normal al segmento --- Cancel speed component normal to segment 
					velocidad_real_moneda = AnulaVelocidadNormalASegmento( velocidad_real_moneda, angulo_segmento_actual );

					// Calculamos la nueva velocidad de giro de la moneda provocada por el contacto --- Calculate new coin spin speed, caused by the contact
					vel_angular_moneda = CalculaVelGiroSobreSegmento( velocidad_real_moneda, angulo_segmento_actual, gRadioMoneda);

					// Calculamos posicion tangente (segunda vez) --- Calculate tangent position (second time)
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
						/*case interseccion_atravesado:	// Nuevo 25/1/2020 --- Pruebas (TODO) TODAVIA NO FUNCIONA -- NOT WORKING YET
							pos_real_moneda = CalculaPosTangenteACentroSegmento ( segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end, pos_real_moneda_anterior, gRadioMoneda);
							break;*/
						default:
							#ifdef DEBUG_INFO
							printf("En segundo calculo pos tangente. No posible, error de programacion o de concepto.\n");
							#endif
							break;
					}

					// En caso de giro del mapa: --- In case of map rotation:
					if ( angulo != angulo_anterior )
					{
						switch (mapa_original.TipoGiro)
						{
							case punto_fijo:
								// hacemos que tambien gire la moneda --- The coin will also rotate
								pos_real_moneda = GiraPunto ( mapa_original.PuntoGiroFijo, pos_real_moneda, angulo - angulo_anterior);

								// Calculamos y sumamos una velocidad debida al giro
								// Es decir, el giro del mapa puede provocar que la moneda salga disparada hacia arriba --- 
								// Nota: como esto convierte el juego en mucho más dificil, se pone como una opción del juego
								
								// We calculate and sum an additional speed caused by the map rotation
								// The map roation can make the coin fly upwards
								// Note: this makes the gameplay harder, therefore it is coded as a game option
								if (opciones_juego.map_rot_makes_coin_fly)
								{
									if ( fabs( angulo-angulo_anterior) >= 2 /*grados*/)
									{
										// La siguiente formula suma una velocidad:
										// -  con sentido normal al segmento en contacto, y 
										// -  modulo de velocidad, la provocada por el giro del segmento (pasamos de velocidad angular a velocidad lineal)

										// The next formula sums a speed so that:
										// -  its direction is normal to the line segment currently in contact
										// -  speed value, is due to the rotation of the line segment (we convert from angular speed to linear speed)
										velocidad_real_moneda = SumaVelocidad( 	velocidad_real_moneda,     
															VelocidadSobreRecta ( 	modulo_vector_velocidad( 	VelAngular2VelLineal( 	mapa_original.PuntoGiroFijo, 
																									PuntoProyectadoSobreRecta( pos_real_moneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end),
																									(angulo-angulo_anterior), 
																									tiempo_imagen 
																									) ), 
																		PuntoProyectadoSobreRecta( pos_real_moneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end), 
																		pos_real_moneda 
																		) 
															);
									}
								}
								break;
							case moneda:
								// En este caso, no es necesario hacer nada. --- In this case, nothing must be done
								break;
							case camara:
								// (TODO)
								printf("ERROR: TODAVIA NO IMPLEMENTADO\n");
								exit(-1);
								break;
							case origen:
								// En este caso, no es necesario hacer nada. --- In this case, nothing must be done
								break;
							case no_rot:
								// En este caso, no es necesario hacer nada. --- In this case, nothing must be done
								break;
							default:
								printf("Al proceder a calcular efectos del giro sobre moneda, error de programacion o error de concepto\n");
								exit(-1);
								break;
						}

					}

					// Calculamos fuerzas normales --- Calculate normal forces
					switch (tipo_interferencia_segmento[segmento_actual])
					{
						case interseccion_central:
							fuerzas_normales_segmentos[segmento_actual] = CalculaReaccionNormalCentroSegmento( angulo_segmento_actual, gravedad.fy, 1 );	// Masa = 1  de momento
							break;
						case interseccion_extremo_start:
						/*case interseccion_atravesado:*/		// Nuevo 25/1/2020 -- PRUEBAS (TODO) TODAVIA NO FUNCIONA -- NOT WORKING YET
							if (mapa_original.Mapa[segmento_actual].start_adyacente_a_otro == false )
							{
								// No es adyacente a otro, podemos calcular su fuerza --- Segment is not next to another, we can calculate its force
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
								// No es adyacente a otro, podemos calcular su fuerza --- Segment is not next to another, we can calculate its force
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

					/////////////////////////////////////////////////////////////////////////////
					// Gestion de pinball flippers, en caso de contacto directo con el segmento tipo flipper --- Pinball flippers management, in case of direct contact of flipper type segment
					if ( mapa_original.mapa_contiene_flippers)
					{
						if ( (mapa_original.Mapa[segmento_actual].type == pinball_flipper_L && flanco_asc_pinball_flipper_L) || (mapa_original.Mapa[segmento_actual].type == pinball_flipper_R && flanco_asc_pinball_flipper_R) )
						{
							// Giramos la moneda para intentar no perder contacto --- We rotate the coin, in order to keep the contact
							if ( mapa_original.Mapa[segmento_actual].type == pinball_flipper_L )
							{
								pos_real_moneda = GiraPunto ( segmentos_girados[segmento_actual].start, pos_real_moneda, -mapa_original.angulo_flippers /*grados*/);
							}
							if ( mapa_original.Mapa[segmento_actual].type == pinball_flipper_R )
							{
								pos_real_moneda = GiraPunto ( segmentos_girados[segmento_actual].end, pos_real_moneda, mapa_original.angulo_flippers /*grados*/);
							}
							// Sumamos a la velocidad actual una velocidad perpendicular al flipper --- We add a speed normal to the flipper to the current speed
							velocidad_real_moneda = SumaVelocidad( 	velocidad_real_moneda,     
												VelocidadSobreRecta ( 	2000 /*modulo_vector_velocidad( VelAngular2VelLineal( 	mapa_original.PuntoGiroFijo, 
																					PuntoProyectadoSobreRecta( pos_real_moneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end),
																					(angulo-angulo_anterior), 
																					tiempo_imagen 
																					) )*/, 
															PuntoProyectadoSobreRecta( pos_real_moneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end), /*Punto 1 recta*/
															pos_real_moneda 	/* Punto 2 recta*/
															) 
													);
						}
					}
					/////////////////////////////////////////////////////////////////////////////

					#ifdef DEBUG_INFO
					printf("Interferencia tipo %d con segmento %d. \n", tipo_interferencia_segmento[segmento_actual],segmento_actual); 
					printf("Angulo Segmento: %f \n", angulo_segmento_actual*360/(2*3.1416));
					printf("Velocidad tras anulacion: vx=%f, vy=%f \n", velocidad_real_moneda.vx, velocidad_real_moneda.vy );
					printf("Fuerzas normales: Fx = %f, Fy = %f \n", fuerzas_normales_segmentos[segmento_actual].fx, fuerzas_normales_segmentos[segmento_actual].fy );
					#endif
				}
				else
				{
					// Sin interferencia
					fuerzas_normales_segmentos[segmento_actual].fx = 0; fuerzas_normales_segmentos[segmento_actual].fy = 0;
				}

			}
			//////////////////////////////////////////////////////////////////////////////
			// Gestion del sonido de golpe de la moneda al chocar contra un segmento
			// Coin impact sound when coin collides with a line segment
			// Nuevo 12/5/2020 (TODO)
			if ( opciones_juego.sound_enabled )
			{
				float diferencia_modulos_velocidades;
				diferencia_modulos_velocidades = fabs (modulo_vector_velocidad_cuadrado(velocidad_real_moneda_anterior) - modulo_vector_velocidad_cuadrado(velocidad_real_moneda) );

				if ( contacto_con_algun_segmento && ( diferencia_modulos_velocidades > 0.1*mapa_original.Gravedad*mapa_original.Gravedad ) )
				{
					Volumen_Chunk_Escala( gSonidoGolpeMoneda, diferencia_modulos_velocidades, 0.5*mapa_original.Gravedad*mapa_original.Gravedad, 0.05*mapa_original.Gravedad*mapa_original.Gravedad);
					Mix_PlayChannel( -1, gSonidoGolpeMoneda, 0 );
				}
			}

			/////////////////////////////////////////////////////////////////////////////////////
			// Pinball flippers, en el caso de que el angulo sea grande de tal forma que la moneda quede por debajo de la posicion elevada del flipper, 
			// pero esté dentro del triangulo que describe....
			// Se pulsa el boton (flanco ascendente)
			// Pinball flippers, in case that the flip angle is so big that the coin is below the risen position of the pinball flipper,
			// but is inside the triangle it describes....
			// The button is pressed (rise edge detection)
			if ( mapa_original.mapa_contiene_flippers && flanco_asc_pinball_flipper_L )
			{
				for ( segmento_actual = 0 ; segmento_actual < mapa_original.NumeroSegmentos ; segmento_actual++ )
				{
					if ((mapa_original.Mapa[segmento_actual].type == pinball_flipper_L) && (tipo_interferencia_segmento[segmento_actual] == 0))
					{
						if ( PuntoDentroDeTriangulo( pos_real_moneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end, GiraPunto( segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end, mapa_original.angulo_flippers) ) )
						{
							pos_real_moneda = GiraPunto ( segmentos_girados[segmento_actual].start, pos_real_moneda, -mapa_original.angulo_flippers /*grados*/);
							velocidad_real_moneda = SumaVelocidad( 	velocidad_real_moneda,     
												VelocidadSobreRecta ( 	2000 /*modulo_vector_velocidad( VelAngular2VelLineal( 	mapa_original.PuntoGiroFijo, 
																					PuntoProyectadoSobreRecta( pos_real_moneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end),
																					(angulo-angulo_anterior), 
																					tiempo_imagen 
																					) )*/, 
															PuntoProyectadoSobreRecta( pos_real_moneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end), /*Punto 1 recta*/
															pos_real_moneda 	/* Punto 2 recta*/
															) 
													);
						}
					}
				}
			}
			if ( mapa_original.mapa_contiene_flippers && flanco_asc_pinball_flipper_R )
			{
				for ( segmento_actual = 0 ; segmento_actual < mapa_original.NumeroSegmentos ; segmento_actual++ )
				{
					if ((mapa_original.Mapa[segmento_actual].type == pinball_flipper_R) && (tipo_interferencia_segmento[segmento_actual] == 0))
					{
						if ( PuntoDentroDeTriangulo( pos_real_moneda, segmentos_girados[segmento_actual].end, segmentos_girados[segmento_actual].start, GiraPunto( segmentos_girados[segmento_actual].end, segmentos_girados[segmento_actual].start, -mapa_original.angulo_flippers) ) )
						{
							pos_real_moneda = GiraPunto ( segmentos_girados[segmento_actual].end, pos_real_moneda, mapa_original.angulo_flippers /*grados*/);
							velocidad_real_moneda = SumaVelocidad( 	velocidad_real_moneda,     
												VelocidadSobreRecta ( 	2000 /*modulo_vector_velocidad( VelAngular2VelLineal( 	mapa_original.PuntoGiroFijo, 
																					PuntoProyectadoSobreRecta( pos_real_moneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end),
																					(angulo-angulo_anterior), 
																					tiempo_imagen 
																					) )*/, 
															PuntoProyectadoSobreRecta( pos_real_moneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end), /*Punto 1 recta*/
															pos_real_moneda 	/* Punto 2 recta*/
															) 
													);
						}
					}
				}
			}
			// Se suelta el boton (flanco descendente) ( pruebas 22/4/2020 TODO)
			// Nota: funciona, pero los mapas no están diseñados para ello, por lo tanto se deshabilita (espero que de momento)
			//if ( mapa_original.mapa_contiene_flippers && flanco_desc_pinball_flipper_L )	// Nuevo 22/4/2020 (TODO)
			//{
			//	for ( segmento_actual = 0 ; segmento_actual < mapa_original.NumeroSegmentos ; segmento_actual++ )
			//	{
			//		if ((mapa_original.Mapa[segmento_actual].type == pinball_flipper_L) && (tipo_interferencia_segmento[segmento_actual] == 0))
			//		{
			//			if ( PuntoDentroDeTriangulo( pos_real_moneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end, GiraPunto( segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end, -mapa_original.angulo_flippers) ) )
			//			{
			//				pos_real_moneda = GiraPunto ( segmentos_girados[segmento_actual].start, pos_real_moneda, mapa_original.angulo_flippers /*grados*/);
			//				velocidad_real_moneda = SumaVelocidad( 	velocidad_real_moneda,     
			//									VelocidadSobreRecta ( 	2000 /*modulo_vector_velocidad( VelAngular2VelLineal( 	mapa_original.PuntoGiroFijo, 
			//																		PuntoProyectadoSobreRecta( pos_real_moneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end),
			//																		(angulo-angulo_anterior), 
			//																		tiempo_imagen 
			//																		) )*/, 
			//												PuntoProyectadoSobreRecta( pos_real_moneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end), /*Punto 1 recta*/
			//												pos_real_moneda 	/* Punto 2 recta*/
			//												) 
			//										);
			//			}
			//		}
			//	}
			//}
			//if ( mapa_original.mapa_contiene_flippers && flanco_desc_pinball_flipper_R )	//Nuevo 22/4/2020 (TODO)
			//{
			//	for ( segmento_actual = 0 ; segmento_actual < mapa_original.NumeroSegmentos ; segmento_actual++ )
			//	{
			//		if ((mapa_original.Mapa[segmento_actual].type == pinball_flipper_R) && (tipo_interferencia_segmento[segmento_actual] == 0))
			//		{
			//			if ( PuntoDentroDeTriangulo( pos_real_moneda, segmentos_girados[segmento_actual].end, segmentos_girados[segmento_actual].start, GiraPunto( segmentos_girados[segmento_actual].end, segmentos_girados[segmento_actual].start, mapa_original.angulo_flippers) ) )
			//			{
			//				pos_real_moneda = GiraPunto ( segmentos_girados[segmento_actual].end, pos_real_moneda, -mapa_original.angulo_flippers /*grados*/);
			//				velocidad_real_moneda = SumaVelocidad( 	velocidad_real_moneda,     
			//									VelocidadSobreRecta ( 	2000 /*modulo_vector_velocidad( VelAngular2VelLineal( 	mapa_original.PuntoGiroFijo, 
			//																		PuntoProyectadoSobreRecta( pos_real_moneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end),
			//																		(angulo-angulo_anterior), 
			//																		tiempo_imagen 
			//																		) )*/, 
			//												PuntoProyectadoSobreRecta( pos_real_moneda, segmentos_girados[segmento_actual].start, segmentos_girados[segmento_actual].end), /*Punto 1 recta*/
			//												pos_real_moneda 	/* Punto 2 recta*/
			//												) 
			//										);
			//			}
			//		}
			//	}
			//}
			//////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////////////////

			// Suma fuerzas --- Sum forces
			aceleracion_real_moneda = Fuerza2Aceleracion( SumaFuerzas( gravedad, fuerzas_normales_segmentos, mapa_original.NumeroSegmentos , /* (TODO) Nuevo 22/3/2020*/ fuerza_zonas_acel_circ , mapa_original.NumeroZonasAceleracionCircular), 1 ); //Masa = 1 de momento (TODO)
			// Actualiza velocidad --- Update speed
			velocidad_real_moneda = Aceleracion2Velocidad( velocidad_real_moneda, aceleracion_real_moneda, tiempo_imagen);

			// Si es mayor que la velocidad maxima permitida, limitarla --- If it is greater than the maximum speed allowed
			// Esto es hacer trampas, para que la moneda no atraviese los segmentos (TODO) --- This is cheating: to prevent that the coin does not go throug a segment
			if (opciones_juego.limit_coin_speed)
			{
				float velocidad_maxima_permitida;
				float modulo_velocidad_actual;
				// Velocidad maxima permitida es tal que la moneda solo puede avanzar como maximo un radio en un fotograma --- Maximum speed allowed so that the coin can only move as much as a radius in a frame
				velocidad_maxima_permitida = gRadioMoneda / tiempo_imagen;
				modulo_velocidad_actual = modulo_vector_velocidad(velocidad_real_moneda);
				if ( modulo_velocidad_actual > velocidad_maxima_permitida )
				{
					velocidad_real_moneda.vx = velocidad_real_moneda.vx * velocidad_maxima_permitida /  modulo_velocidad_actual;
					velocidad_real_moneda.vy = velocidad_real_moneda.vy * velocidad_maxima_permitida /  modulo_velocidad_actual;
				}
			}

			//Actualiza posición --- Update position
			pos_real_moneda_anterior = pos_real_moneda;		// Nuevo 25/1/2020 --- Guardamos posicion del fotograma anterior --- Store last frame position
			pos_real_moneda = Velocidad2Posicion( pos_real_moneda, velocidad_real_moneda, tiempo_imagen);
			// Actualiza angulo de giro de la moneda --- Update coin spin angle
			angulo_rotacion_moneda = VelAng2Angulo( angulo_rotacion_moneda, vel_angular_moneda, tiempo_imagen); 

			angulo_anterior = angulo;


			#ifdef DEBUG_INFO
			printf("Velocidad actual: vx=%f, vy=%f\n", velocidad_real_moneda.vx, velocidad_real_moneda.vy);		
			#endif
			/////////////////////////////////////////////////////////////////////////////////////
		} // If (!pause && !intro_mapa && !win && !lose ) 

		// Convertimos posiciones "reales" en posiciones de la pantalla --- Convert "real" positions into "screen" positions
		pos_pant_moneda = CalculaCamara( 		ModoCamara, opciones_juego.screen_x_resolution, opciones_juego.screen_y_resolution,
								desplazamiento_camara_usuario_x, desplazamiento_camara_usuario_y,
								pos_real_moneda, 
								mapa_original.NumeroSegmentos, 
								segmentos_girados,  pos_camara_segmentos_girados,
								mapa_original.Pos_x_izquierda_fondo_giratorio, mapa_original.Pos_y_arriba_fondo_giratorio, mapa_original.Pos_x_derecha_fondo_giratorio, mapa_original.Pos_y_abajo_fondo_giratorio, 
								&pos_cam_fondo_giratorio_izquierda, &pos_cam_fondo_giratorio_arriba, &pos_cam_fondo_giratorio_derecha, &pos_cam_fondo_giratorio_abajo,
								mapa_original.NumeroPinballBumpers,
								centros_bumpers_girados, pos_pant_centros_bumpers_girados,
								mapa_original.NumeroZonasAceleracionCircular,
								centros_zonaacelcirc_girados, pos_pant_centros_zonaacelcirc_girados,
								mapa_original.NumeroEnemigosRuta, /*(17/9/2021) */
								centros_enemigos_ruta_girados, pos_pant_centros_enemigos_ruta_girados  /*(17/9/2021)*/,
								mapa_original.NumeroWormholes, /*(27/9/2021)*/
								centros_wormholes_girados, pos_pant_centros_wormholes_girados /*(27/9/2021)*/ ); 		

		/////////////////////////////////////////////////////////////////////////////////////
		//  ____  _ _            _       ____             _        _ _       		 ____                      ____                           
		// |  _ \(_) |__  _   _ (_) __ _|  _ \ __ _ _ __ | |_ __ _| | | __ _ 		|  _ \ _ __ __ ___      __/ ___|  ___ _ __ ___  ___ _ __  
		// | | | | | '_ \| | | || |/ _` | |_) / _` | '_ \| __/ _` | | |/ _` |		| | | | '__/ _` \ \ /\ / /\___ \ / __| '__/ _ \/ _ \ '_ \ 
		// | |_| | | |_) | |_| || | (_| |  __/ (_| | | | | || (_| | | | (_| |		| |_| | | | (_| |\ V  V /  ___) | (__| | |  __/  __/ | | |
		// |____/|_|_.__/ \__,_|/ |\__,_|_|   \__,_|_| |_|\__\__,_|_|_|\__,_|		|____/|_|  \__,_| \_/\_/  |____/ \___|_|  \___|\___|_| |_|
		//                    |__/                                           

		// Representa todo por pantalla --- Display everything in the screen

		 //Clear screen 
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer ); 

		//Dibuja fondo --- Draws background
		SDL_RenderCopy( gRenderer, gTexturaFondo, NULL, NULL );		//Imagen estirada a toda la ventana --- Image stretched to fit entire screen

		// Dibuja fondo giratorio (si hubiese) --- Draws rotating background (in case it exists)
		if ( mapa_original.HayFondoGiratorio && opciones_juego.textured_objects )
		{
			SDL_Point centro_giro_fondo_giratorio;

			renderQuad.x = pos_cam_fondo_giratorio_izquierda;										// Coord X de esquina superior izquierda
			renderQuad.y = pos_cam_fondo_giratorio_arriba;										// Coord Y de esquina superior izquierda
			renderQuad.w = pos_cam_fondo_giratorio_derecha - pos_cam_fondo_giratorio_izquierda;					// Ancho
			renderQuad.h = pos_cam_fondo_giratorio_abajo - pos_cam_fondo_giratorio_arriba;						// Alto

			centro_giro_fondo_giratorio.x = mapa_original.CentroGiroFondoGiratorio.x;
			centro_giro_fondo_giratorio.y = mapa_original.CentroGiroFondoGiratorio.y;

			SDL_RenderCopyEx( gRenderer, gTexturaFondoGiratorio, NULL, &renderQuad, angulo, &centro_giro_fondo_giratorio, SDL_FLIP_NONE );
		}

		//Dibuja zonas de aceleración circulares --- Draw round acceleration zones
		if ( mapa_original.NumeroZonasAceleracionCircular != 0 )
		{
			for ( zona_acel_circ_actual = 0 ; zona_acel_circ_actual < mapa_original.NumeroZonasAceleracionCircular ; zona_acel_circ_actual++ )
			{
				if ( mapa_original.ZonasAceleracionCircular[zona_acel_circ_actual].invisible == false )
				{
					renderQuad.x = pos_pant_centros_zonaacelcirc_girados[zona_acel_circ_actual].x - mapa_original.ZonasAceleracionCircular[zona_acel_circ_actual].radio;		// Coord X de esquina superior izquierda --- Upper left corner, X coord
					renderQuad.y = pos_pant_centros_zonaacelcirc_girados[zona_acel_circ_actual].y - mapa_original.ZonasAceleracionCircular[zona_acel_circ_actual].radio;		// Coord Y de esquina superior izquierda --- Upper left corner, Y coord
					renderQuad.w = 2*mapa_original.ZonasAceleracionCircular[zona_acel_circ_actual].radio;							// Ancho --- Width
					renderQuad.h = 2*mapa_original.ZonasAceleracionCircular[zona_acel_circ_actual].radio;							// Alto --- Height
					//SDL_RenderCopy( gRenderer, gTexturaPinballBumper, NULL, &renderQuad ); 	// Version sin rotacion --- Version without texture rotation
					SDL_RenderCopyEx( gRenderer, gTexturaZonaAcelCirc, NULL, &renderQuad, angulo + mapa_original.ZonasAceleracionCircular[zona_acel_circ_actual].angulo, NULL /*Rota alrededor del centro*/, SDL_FLIP_NONE );
				}
			}
		}

		// Dibuja wormholes --- Draw wormholes (27/9/2021)
		if ( mapa_original.NumeroWormholes != 0 )
		{
			for ( wormhole_actual = 0 ; wormhole_actual < mapa_original.NumeroWormholes ; wormhole_actual++ )
			{
				int animacion_actual;

				animacion_actual =  (contador_frames / 2  ) % ( NUM_ANIMS_WORMHOLE ) ;

				// Punto 1 del wormhole --- Wormhole point P1
				renderQuad.x = pos_pant_centros_wormholes_girados[wormhole_actual].p1.x - mapa_original.Wormholes[wormhole_actual].radio;		// Coord X de esquina superior izquierda
				renderQuad.y = pos_pant_centros_wormholes_girados[wormhole_actual].p1.y - mapa_original.Wormholes[wormhole_actual].radio;		// Coord Y de esquina superior izquierda
				renderQuad.w = 2*mapa_original.Wormholes[wormhole_actual].radio;	// Ancho
				renderQuad.h = renderQuad.w;	// Alto

				SDL_RenderCopyEx( gRenderer, gTextureWormholeSpritesheet, 
						/*Indica cuadro sprite actual*/ &( gCuadroSpriteWormhole[animacion_actual] ), 
						&renderQuad, 
						angulo, 
						NULL /*Rota alrededor del centro*/, 
						SDL_FLIP_NONE /* flip*/  );

				// Punto 2 del wormhole --- Wormhole point P2
				renderQuad.x = pos_pant_centros_wormholes_girados[wormhole_actual].p2.x - mapa_original.Wormholes[wormhole_actual].radio;		// Coord X de esquina superior izquierda
				renderQuad.y = pos_pant_centros_wormholes_girados[wormhole_actual].p2.y - mapa_original.Wormholes[wormhole_actual].radio;		// Coord Y de esquina superior izquierda
				renderQuad.w = 2*mapa_original.Wormholes[wormhole_actual].radio;	// Ancho
				renderQuad.h = renderQuad.w;	// Alto

				SDL_RenderCopyEx( gRenderer, gTextureWormholeSpritesheet, 
						/*Indica cuadro sprite actual*/ &( gCuadroSpriteWormhole[animacion_actual] ), 
						&renderQuad, 
						angulo, 
						NULL /*Rota alrededor del centro*/, 
						SDL_FLIP_NONE /* flip*/  );
			}
		}


		//Render texture to screen - Dibuja moneda
		//Crea un rectangulo en la posicion desesada de la moneda --- Create a rectangle in the desired coin position 
		renderQuad.x = pos_pant_moneda.x-gRadioMoneda;		// Coord X de esquina superior izquierda --- Upper left corner, X coord
		renderQuad.y = pos_pant_moneda.y-gRadioMoneda;		// Coord Y de esquina superior izquierda --- Upper left corner, Y coord
		renderQuad.w = gDimMonedaX;				// Ancho --- Width
		renderQuad.h = gDimMonedaY;				// Alto --- Height
		if ( mapa_original.no_rot_moneda == true )	// La moneda no debe rotar
		{
			SDL_RenderCopy( gRenderer, gTexturaMoneda, NULL, &renderQuad ); 	// Version sin rotacion de la moneda --- Version without coin spin
		}
		else	// La moneda si que debe rotar --- The coin must spin
		{
			SDL_RenderCopyEx( gRenderer, gTexturaMoneda, NULL, &renderQuad, angulo_rotacion_moneda*180/PI, NULL /*Rota alrededor del centro*/, SDL_FLIP_NONE );
		}


		// Dibuja lineas (modo con texturas) --- Draw lines (texture mode)
		if (opciones_juego.textured_objects)
		{
			for ( segmento_actual = 0 ; segmento_actual < mapa_original.NumeroSegmentos ; segmento_actual++ )
			{	
				float angulo_segmento_actual;
				SDL_Point centro_giro_textura_segmento;

				if ( mapa_original.Mapa[segmento_actual].invisible == false )
				{
					switch ( mapa_original.Mapa[segmento_actual].type )
					{
						case pinball_flipper_L:
							// Calculamos el angulo --- Calculate the angle
							angulo_segmento_actual = AnguloSegmento( pos_camara_segmentos_girados[segmento_actual] );

							renderQuad.x = pos_camara_segmentos_girados[segmento_actual].start.x;		// Coord X de esquina superior izquierda
							renderQuad.y = pos_camara_segmentos_girados[segmento_actual].start.y - gDimTexturaPinballFlipper_L_Y/2;		// Coord Y de esquina superior izquierda
							renderQuad.w = LongitudVector(pos_camara_segmentos_girados[segmento_actual].start ,pos_camara_segmentos_girados[segmento_actual].end );				// Ancho
							renderQuad.h = gDimTexturaPinballFlipper_L_Y;				// Alto

							// Calculamos punto de giro --- Calculate the rotating point
							centro_giro_textura_segmento.x = 0;
							centro_giro_textura_segmento.y = gDimTexturaPinballFlipper_L_Y/2;
							// Dibujamos textura girada --- Draw a rotated texture
							SDL_RenderCopyEx( gRenderer, gTexturaPinballFlipperL, NULL, &renderQuad, angulo_segmento_actual*180/PI, &centro_giro_textura_segmento, SDL_FLIP_NONE );
							break;
						case pinball_flipper_R:
							// Calculamos el angulo --- Calculate the angle
							angulo_segmento_actual = AnguloSegmento( pos_camara_segmentos_girados[segmento_actual] );

							renderQuad.x = pos_camara_segmentos_girados[segmento_actual].start.x;		// Coord X de esquina superior izquierda
							renderQuad.y = pos_camara_segmentos_girados[segmento_actual].start.y - gDimTexturaPinballFlipper_R_Y/2;		// Coord Y de esquina superior izquierda
							renderQuad.w = LongitudVector(pos_camara_segmentos_girados[segmento_actual].start ,pos_camara_segmentos_girados[segmento_actual].end );				// Ancho
							renderQuad.h = gDimTexturaPinballFlipper_R_Y;				// Alto

							// Calculamos punto de giro --- Calculate the rotating point
							centro_giro_textura_segmento.x = 0;
							centro_giro_textura_segmento.y = /*gDimTexturaSegmentoY (TODO)*/ gDimTexturaPinballFlipper_R_Y/2;
							// Dibujamos textura girada --- Draw a rotated texture
							SDL_RenderCopyEx( gRenderer, gTexturaPinballFlipperR, NULL, &renderQuad, angulo_segmento_actual*180/PI, &centro_giro_textura_segmento, SDL_FLIP_NONE );
							break;
						case meta:	// (TODO) Nuevo 17/5/2020
							// Calculamos el angulo --- Calculate the angle
							angulo_segmento_actual = AnguloSegmento( pos_camara_segmentos_girados[segmento_actual] );

							renderQuad.x = pos_camara_segmentos_girados[segmento_actual].start.x;		// Coord X de esquina superior izquierda
							renderQuad.y = pos_camara_segmentos_girados[segmento_actual].start.y - gDimTexturaSegmentoMetaY/2;		// Coord Y de esquina superior izquierda
							renderQuad.w = LongitudVector(pos_camara_segmentos_girados[segmento_actual].start ,pos_camara_segmentos_girados[segmento_actual].end );				// Ancho
							renderQuad.h = gDimTexturaSegmentoMetaY;				// Alto

							// Calculamos punto de giro --- Calculate the rotating point
							centro_giro_textura_segmento.x = 0;
							centro_giro_textura_segmento.y = gDimTexturaSegmentoMetaY/2;
							// Dibujamos textura girada --- Draw a rotated texture
							SDL_RenderCopyEx( gRenderer, gTexturaSegmentoMeta, NULL, &renderQuad, angulo_segmento_actual*180/PI, &centro_giro_textura_segmento, SDL_FLIP_NONE );
							break;
						case muerte:	// (TODO) Nuevo 17/5/2020
							// Calculamos el angulo --- Calculate the angle
							angulo_segmento_actual = AnguloSegmento( pos_camara_segmentos_girados[segmento_actual] );

							renderQuad.x = pos_camara_segmentos_girados[segmento_actual].start.x;		// Coord X de esquina superior izquierda
							renderQuad.y = pos_camara_segmentos_girados[segmento_actual].start.y - gDimTexturaSegmentoMuerteY/2;		// Coord Y de esquina superior izquierda
							renderQuad.w = LongitudVector(pos_camara_segmentos_girados[segmento_actual].start ,pos_camara_segmentos_girados[segmento_actual].end );				// Ancho
							renderQuad.h = gDimTexturaSegmentoMuerteY;				// Alto

							// Calculamos punto de giro --- Calculate the rotating point
							centro_giro_textura_segmento.x = 0;
							centro_giro_textura_segmento.y = gDimTexturaSegmentoMuerteY/2;
							// Dibujamos textura girada --- Draw a rotated texture
							SDL_RenderCopyEx( gRenderer, gTexturaSegmentoMuerte, NULL, &renderQuad, angulo_segmento_actual*180/PI, &centro_giro_textura_segmento, SDL_FLIP_NONE );
							break;
						case pared:	// (TODO) Nuevo 17/5/2020
						default:	// Default y pared hacen lo mismo --- Default and wall, do the same code
							// Calculamos el angulo --- Calculate the angle
							angulo_segmento_actual = AnguloSegmento( pos_camara_segmentos_girados[segmento_actual] );

							renderQuad.x = pos_camara_segmentos_girados[segmento_actual].start.x;		// Coord X de esquina superior izquierda
							renderQuad.y = pos_camara_segmentos_girados[segmento_actual].start.y - gDimTexturaSegmentoParedY/2;		// Coord Y de esquina superior izquierda
							renderQuad.w = LongitudVector(pos_camara_segmentos_girados[segmento_actual].start ,pos_camara_segmentos_girados[segmento_actual].end );				// Ancho
							renderQuad.h = gDimTexturaSegmentoParedY;				// Alto

							// Calculamos punto de giro --- Calculate the rotating point
							centro_giro_textura_segmento.x = 0;
							centro_giro_textura_segmento.y = gDimTexturaSegmentoParedY/2;
							// Dibujamos textura girada --- Draw a rotated texture
							SDL_RenderCopyEx( gRenderer, gTexturaSegmentoPared, NULL, &renderQuad, angulo_segmento_actual*180/PI, &centro_giro_textura_segmento, SDL_FLIP_NONE );
							break;
					}
				}
			}
		}

		//Dibuja lineas (modo WireFrame) --- Draw lines (wireframe mode)
		if (opciones_juego.wireframe )
		{
			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

			for ( segmento_actual = 0 ; segmento_actual < mapa_original.NumeroSegmentos ; segmento_actual++ )
			{
				if ( mapa_original.Mapa[segmento_actual].invisible ==  false )
				{
					switch ( mapa_original.Mapa[segmento_actual].type ) // Seleccionar color segun tipo de segmento --- Choose color according to line segment type
					{
						case pared:
							SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );	// Blanco --- White
							#ifdef DEBUG_INFO
							// Mostrar informacion con colores sobre tipos de interferencia --- Show interference types by using colors
							switch ( tipo_interferencia_segmento[segmento_actual] )
							{
								case sin_interseccion:
									SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );	// Blanco --- White
									break;
								case interseccion_extremo_start:
									SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0x00 );	// Verde --- Green
									break;
								case interseccion_central:
									SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0x00 );	// Rojo --- Red
									break;
								case interseccion_extremo_end:
									SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0x00 );	// Azul --- Blue
									break;
								/*case interseccion_atravesado:
									SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );	// Negro --- Black --- Nuevo 25/1/2020 -- PRUEBAS (TODO)
									break;	*/						
								default:
									break;
							}
							#endif
							break;
						case meta:
							SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0x00 );	// Verde --- Green
							break;
						case muerte:
							SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0x00 );	// Rojo --- Red
							break;
						default:
							SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );	// Blanco --- White
							break;
					}

					SDL_RenderDrawLine( 	gRenderer, 
								pos_camara_segmentos_girados[segmento_actual].start.x ,
								pos_camara_segmentos_girados[segmento_actual].start.y , 
								pos_camara_segmentos_girados[segmento_actual].end.x , 
								pos_camara_segmentos_girados[segmento_actual].end.y );
				}
			}
		}
		// Dibuja "bumpers" de "pinball" --- Draw pinball bumpers on screen
		if ( mapa_original.NumeroPinballBumpers != 0 )
		{
			for ( bumper_actual = 0 ; bumper_actual < mapa_original.NumeroPinballBumpers ; bumper_actual++ )
			{
				renderQuad.x = pos_pant_centros_bumpers_girados[bumper_actual].x - mapa_original.Bumpers[bumper_actual].radio;		// Coord X de esquina superior izquierda --- Upper left corner X coord
				renderQuad.y = pos_pant_centros_bumpers_girados[bumper_actual].y - mapa_original.Bumpers[bumper_actual].radio;		// Coord Y de esquina superior izquierda --- Upper left corner Y coord
				renderQuad.w = 2*mapa_original.Bumpers[bumper_actual].radio;							// Ancho --- Width
				renderQuad.h = 2*mapa_original.Bumpers[bumper_actual].radio;							// Alto --- Height
				//SDL_RenderCopy( gRenderer, gTexturaPinballBumper, NULL, &renderQuad ); 	// Version sin rotacion
				SDL_RenderCopyEx( gRenderer, gTexturaPinballBumper, NULL, &renderQuad, angulo, NULL /*Rota alrededor del centro*/, SDL_FLIP_NONE );
			}
		}


		// Dibuja enemigos de tipo ruta (17/9/2021) --- Draw path type enemies
		if ( mapa_original.NumeroEnemigosRuta != 0 )
		{
			for ( enemigo_ruta_actual = 0 ; enemigo_ruta_actual < mapa_original.NumeroEnemigosRuta ; enemigo_ruta_actual++ )
			{
				int animacion_actual;
				float angulo_representacion_enemigo;
				SDL_RendererFlip flip_enemigo;

				animacion_actual =  (contador_frames / gTiposEnemigosMapaActual[ mapa_original.EnemigosRuta[enemigo_ruta_actual].ID_unico_tipo_enemigo].frames_per_animation  ) % ( (gTiposEnemigosMapaActual[ mapa_original.EnemigosRuta[enemigo_ruta_actual].ID_unico_tipo_enemigo ].num_anim_sprites)   ) ;

				angulo_representacion_enemigo = WrapAngle_0_360(  angulo + angulos_actual_enemigos_ruta[enemigo_ruta_actual]  );

				// Hacemos que la textura del enemigo nunca esté invertida --- The enemy texture should never be drawn upside down
				if ( angulo_representacion_enemigo > 90 && angulo_representacion_enemigo < 270 )
				{
					// El sprite estaría girado de tal forma que se representaría boca abajo. La invertimos.
					// The sprite would be drawn upside down, therefore we flip it.
					flip_enemigo = SDL_FLIP_VERTICAL;
				}
				else
				{
					// El sprite estaría girado de tal forma que se representaría boca arriba. La representamos tal cual está.
					// No flipping will be needed.
					flip_enemigo = SDL_FLIP_NONE;
				}

				renderQuad.x = pos_pant_centros_enemigos_ruta_girados[enemigo_ruta_actual].x - gTiposEnemigosMapaActual[  mapa_original.EnemigosRuta[enemigo_ruta_actual].ID_unico_tipo_enemigo  ].radio;		// Coord X de esquina superior izquierda
				renderQuad.y = pos_pant_centros_enemigos_ruta_girados[enemigo_ruta_actual].y - gTiposEnemigosMapaActual[  mapa_original.EnemigosRuta[enemigo_ruta_actual].ID_unico_tipo_enemigo  ].radio;		// Coord Y de esquina superior izquierda
				renderQuad.w = 2*gTiposEnemigosMapaActual[  mapa_original.EnemigosRuta[enemigo_ruta_actual].ID_unico_tipo_enemigo  ].radio;	// Ancho --- Width
				renderQuad.h = renderQuad.w;	// Alto --- Height

				SDL_RenderCopyEx( gRenderer, gTiposEnemigosMapaActual[ mapa_original.EnemigosRuta[enemigo_ruta_actual].ID_unico_tipo_enemigo ].sprite_sheet, 
						/*Indica cuadro sprite actual*/ &( gTiposEnemigosMapaActual[ mapa_original.EnemigosRuta[enemigo_ruta_actual].ID_unico_tipo_enemigo ].cuadro_sprite[ animacion_actual ] ), 
						&renderQuad, 
						angulo_representacion_enemigo, 
						NULL /*Rota alrededor del centro*/, 
						flip_enemigo );
			}

		}


		// Dibuja el indicador de tiempo restante (cuenta atras) --- Draw remaining time counter (countdown)
		if ( mapa_original.CuentaAtras == true )
		{
			#ifdef DEBUG_INFO
			printf("Cuenta atras: dig5=%d, dig4=%d, dig3=%d \n", Digito_N_esimo(5,(int)TiempoActualCuentaAtras), Digito_N_esimo(4,(int)TiempoActualCuentaAtras), Digito_N_esimo(3,(int)TiempoActualCuentaAtras) );
			#endif
			// Digito 3 (unidades de segundos) --- 3rd digit (second units)
			renderQuad.x = opciones_juego.screen_x_resolution - gDimTexturaNumeroX;		// Coord X de esquina superior izquierda
			renderQuad.y = opciones_juego.screen_y_resolution - gDimTexturaNumeroY;		// Coord Y de esquina superior izquierda
			renderQuad.w = gDimTexturaNumeroX;				// Ancho
			renderQuad.h = gDimTexturaNumeroY;				// Alto
			SDL_RenderCopy( gRenderer, gTexturaNumero[ Digito_N_esimo(3,TiempoActualCuentaAtras) ], NULL, &renderQuad );
			// Digito 4 (decenas de segundos) --- 4rd digit (10*seconds)
			renderQuad.x = opciones_juego.screen_x_resolution - 2*gDimTexturaNumeroX;		// Coord X de esquina superior izquierda
			renderQuad.y = opciones_juego.screen_y_resolution - gDimTexturaNumeroY;		// Coord Y de esquina superior izquierda
			renderQuad.w = gDimTexturaNumeroX;				// Ancho
			renderQuad.h = gDimTexturaNumeroY;				// Alto
			SDL_RenderCopy( gRenderer, gTexturaNumero[ Digito_N_esimo(4,TiempoActualCuentaAtras) ], NULL, &renderQuad );
			// Digito 5 (centenas de segundos) --- 5th digit (100*seconds)
			renderQuad.x = opciones_juego.screen_x_resolution - 3*gDimTexturaNumeroX;		// Coord X de esquina superior izquierda
			renderQuad.y = opciones_juego.screen_y_resolution - gDimTexturaNumeroY;		// Coord Y de esquina superior izquierda
			renderQuad.w = gDimTexturaNumeroX;				// Ancho
			renderQuad.h = gDimTexturaNumeroY;				// Alto
			SDL_RenderCopy( gRenderer, gTexturaNumero[ Digito_N_esimo(5,TiempoActualCuentaAtras) ], NULL, &renderQuad );
			// Icono del cronometro --- Stopwatch icon
			renderQuad.x = opciones_juego.screen_x_resolution - 4*gDimTexturaNumeroX;		// Coord X de esquina superior izquierda
			renderQuad.y = opciones_juego.screen_y_resolution - gDimTexturaNumeroY;		// Coord Y de esquina superior izquierda
			renderQuad.w = gDimTexturaNumeroX;				// Ancho
			renderQuad.h = gDimTexturaNumeroY;				// Alto
			SDL_RenderCopy( gRenderer, gTexturaCronometro, NULL, &renderQuad );
		}

		// Dibuja textos de intro del mapa --- Draw map intro screen texts
		if ( intro_mapa )
		{
			// Cuadro Nombre del Mapa --- Map name text
			/*renderQuad.x = 0.3*opciones_juego.screen_x_resolution;		// Coord X de esquina superior izquierda
			renderQuad.y = 0.2*opciones_juego.screen_y_resolution;		// Coord Y de esquina superior izquierda
			renderQuad.w = 0.3*opciones_juego.screen_x_resolution;				// Ancho
			renderQuad.h = 0.1*opciones_juego.screen_y_resolution;				// Alto
			SDL_RenderCopy( gRenderer, gTexturaFondoSemiTranspTexto, NULL, &renderQuad );	// Pruebas 4/4/2020
			//SDL_RenderCopy( gRenderer, gNombreMapa, NULL, &renderQuad );	*/
			RepresentaTextura ( gRenderer, gTexturaFondoSemiTranspTexto, 
					0 , 0 , 
					0.3*opciones_juego.screen_x_resolution , (0.3+0.3)*opciones_juego.screen_x_resolution , 0.2*opciones_juego.screen_y_resolution, (0.2+0.1)*opciones_juego.screen_y_resolution, 
					estirar_hasta_rellenar );
			RepresentaTextura ( gRenderer, gNombreMapa, 
					gDimNombreMapaX , gDimNombreMapaY , 
					0.3*opciones_juego.screen_x_resolution , (0.3+0.3)*opciones_juego.screen_x_resolution , 0.2*opciones_juego.screen_y_resolution, (0.2+0.1)*opciones_juego.screen_y_resolution, 
					con_relac_aspecto_no_rellenar );


			// Cuadro Texto Descripción de mapa --- Map description text
			/*renderQuad.x = 0.1*opciones_juego.screen_x_resolution;		// Coord X de esquina superior izquierda
			renderQuad.y = 0.4*opciones_juego.screen_y_resolution;		// Coord Y de esquina superior izquierda
			renderQuad.w = 0.8*opciones_juego.screen_x_resolution;				// Ancho
			renderQuad.h = 0.1*opciones_juego.screen_y_resolution;				// Alto
			SDL_RenderCopy( gRenderer, gTexturaFondoSemiTranspTexto, NULL, &renderQuad );	// Pruebas 4/4/2020
			SDL_RenderCopy( gRenderer, gDescripcionMapa, NULL, &renderQuad );	*/
			RepresentaTextura ( gRenderer, gTexturaFondoSemiTranspTexto, 
					0 , 0 , 
					0.1*opciones_juego.screen_x_resolution , (0.1+0.8)*opciones_juego.screen_x_resolution , 0.4*opciones_juego.screen_y_resolution, (0.4+0.1)*opciones_juego.screen_y_resolution, 
					estirar_hasta_rellenar );
			RepresentaTextura ( gRenderer, gDescripcionMapa, 
					gDimDescripcionMapaX , gDimDescripcionMapaY , 
					0.1*opciones_juego.screen_x_resolution , (0.1+0.8)*opciones_juego.screen_x_resolution , 0.4*opciones_juego.screen_y_resolution, (0.4+0.1)*opciones_juego.screen_y_resolution, 
					con_relac_aspecto_no_rellenar );

		}

		if ( intro_mapa || win || lose )
		{
			// Cuadro Pulse cualquier tecla --- Press Any key label
			/*renderQuad.x = 0.2*opciones_juego.screen_x_resolution;		// Coord X de esquina superior izquierda
			renderQuad.y = 0.8*opciones_juego.screen_y_resolution;		// Coord Y de esquina superior izquierda
			renderQuad.w = 0.6*opciones_juego.screen_x_resolution;				// Ancho
			renderQuad.h = 0.1*opciones_juego.screen_y_resolution;				// Alto
			SDL_RenderCopy( gRenderer, gTexturaFondoSemiTranspTexto, NULL, &renderQuad );	// Pruebas 4/4/2020
			SDL_RenderCopy( gRenderer, gPulseTecla, NULL, &renderQuad );	*/
			RepresentaTextura ( gRenderer, gTexturaFondoSemiTranspTexto, 
					0 , 0 , 
					0.2*opciones_juego.screen_x_resolution , (0.2+0.6)*opciones_juego.screen_x_resolution , 0.8*opciones_juego.screen_y_resolution, (0.8+0.1)*opciones_juego.screen_y_resolution, 
					estirar_hasta_rellenar );
			RepresentaTextura ( gRenderer, gPulseTecla, 
					gDimPulseTeclaX , gDimPulseTeclaY , 
					0.2*opciones_juego.screen_x_resolution , (0.2+0.6)*opciones_juego.screen_x_resolution , 0.8*opciones_juego.screen_y_resolution, (0.8+0.1)*opciones_juego.screen_y_resolution, 
					con_relac_aspecto_no_rellenar );
		}
		
		if ( win )
		{
			// Icono Victoria --- Victory icon
			renderQuad.x = 0.1*opciones_juego.screen_x_resolution;		// Coord X de esquina superior izquierda
			renderQuad.y = 0.4*opciones_juego.screen_y_resolution;		// Coord Y de esquina superior izquierda
			renderQuad.w = gDimIconoVictoriaX;				// Ancho
			renderQuad.h = gDimIconoVictoriaY;				// Alto
			SDL_RenderCopy( gRenderer, gIconoVictoria, NULL, &renderQuad );	


			// Cuadro Texto Victoria --- You win text
			/*renderQuad.x = 0.35*opciones_juego.screen_x_resolution;		// Coord X de esquina superior izquierda
			renderQuad.y = 0.4*opciones_juego.screen_y_resolution;		// Coord Y de esquina superior izquierda
			renderQuad.w = 0.5*opciones_juego.screen_x_resolution;				// Ancho
			renderQuad.h = 0.1*opciones_juego.screen_y_resolution;				// Alto
			SDL_RenderCopy( gRenderer, gTexturaFondoSemiTranspTexto, NULL, &renderQuad );	// Pruebas 4/4/2020
			SDL_RenderCopy( gRenderer, gTextoVictoria, NULL, &renderQuad );	*/
			RepresentaTextura ( gRenderer, gTexturaFondoSemiTranspTexto, 
					0 , 0 , 
					0.35*opciones_juego.screen_x_resolution , (0.35+0.4)*opciones_juego.screen_x_resolution , 0.5*opciones_juego.screen_y_resolution, (0.5+0.1)*opciones_juego.screen_y_resolution, 
					estirar_hasta_rellenar );
			RepresentaTextura ( gRenderer, gTextoVictoria, 
					gDimTextoVictoriaX , gDimTextoVictoriaY , 
					0.35*opciones_juego.screen_x_resolution , (0.35+0.4)*opciones_juego.screen_x_resolution , 0.5*opciones_juego.screen_y_resolution, (0.5+0.1)*opciones_juego.screen_y_resolution, 
					con_relac_aspecto_no_rellenar );
		}

		if( lose )
		{
			// Icono Perder --- Victory icon
			renderQuad.x = 0.1*opciones_juego.screen_x_resolution;		// Coord X de esquina superior izquierda
			renderQuad.y = 0.4*opciones_juego.screen_y_resolution;		// Coord Y de esquina superior izquierda
			renderQuad.w = gDimIconoPierdeX;				// Ancho
			renderQuad.h = gDimIconoPierdeY;				// Alto
			SDL_RenderCopy( gRenderer, gIconoPierde, NULL, &renderQuad );	


			// Cuadro Pulse cualquier tecla --- Press Any key label
			/*renderQuad.x = 0.35*opciones_juego.screen_x_resolution;		// Coord X de esquina superior izquierda
			renderQuad.y = 0.4*opciones_juego.screen_y_resolution;		// Coord Y de esquina superior izquierda
			renderQuad.w = 0.5*opciones_juego.screen_x_resolution;				// Ancho
			renderQuad.h = 0.1*opciones_juego.screen_y_resolution;				// Alto
			SDL_RenderCopy( gRenderer, gTexturaFondoSemiTranspTexto, NULL, &renderQuad );	// Pruebas 4/4/2020
			SDL_RenderCopy( gRenderer, gTextoPierde, NULL, &renderQuad );*/
			RepresentaTextura ( gRenderer, gTexturaFondoSemiTranspTexto, 
					0 , 0 , 
					0.35*opciones_juego.screen_x_resolution , (0.35+0.5)*opciones_juego.screen_x_resolution , 0.4*opciones_juego.screen_y_resolution, (0.4+0.1)*opciones_juego.screen_y_resolution, 
					estirar_hasta_rellenar );
			RepresentaTextura ( gRenderer, gTextoPierde, 
					gDimTextoPierdeX , gDimTextoPierdeY , 
					0.35*opciones_juego.screen_x_resolution , (0.35+0.5)*opciones_juego.screen_x_resolution , 0.4*opciones_juego.screen_y_resolution, (0.4+0.1)*opciones_juego.screen_y_resolution, 
					con_relac_aspecto_no_rellenar );


		}
		// Dibuja cuadro de pausa --- Draw pause text box
		if ( pause )
		{
			/*renderQuad.x = 0.4*opciones_juego.screen_x_resolution;		// Coord X de esquina superior izquierda
			renderQuad.y = 0.4*opciones_juego.screen_y_resolution;		// Coord Y de esquina superior izquierda
			renderQuad.w = 0.2*opciones_juego.screen_x_resolution;				// Ancho
			renderQuad.h = 0.1*opciones_juego.screen_y_resolution;				// Alto
			SDL_RenderCopy( gRenderer, gTexturaFondoSemiTranspTexto, NULL, &renderQuad );	// Pruebas 4/4/2020
			SDL_RenderCopy( gRenderer, gTexturaPausa, NULL, &renderQuad );*/
			RepresentaTextura ( gRenderer, gTexturaFondoSemiTranspTexto, 
					0 , 0 , 
					0.4*opciones_juego.screen_x_resolution , (0.4+0.2)*opciones_juego.screen_x_resolution , 0.4*opciones_juego.screen_y_resolution, (0.4+0.1)*opciones_juego.screen_y_resolution, 
					estirar_hasta_rellenar );
			RepresentaTextura ( gRenderer, gTexturaPausa, 
					gDimTexturaPausaX , gDimTexturaPausaY , 
					0.4*opciones_juego.screen_x_resolution , (0.4+0.2)*opciones_juego.screen_x_resolution , 0.4*opciones_juego.screen_y_resolution, (0.4+0.1)*opciones_juego.screen_y_resolution, 
					con_relac_aspecto_no_rellenar );
		}

		// Update screen 
		SDL_RenderPresent( gRenderer );

		/////////////////////////////////////////////////////////////////////////////////////	

		// Limitar el framerate aproximadamente al framerate deseado --- Framerate capping to the desired framerate (approximately)
		if ((1000/framerate_deseado) > (SDL_GetTicks() - currentTime ) ) 
		{
			tiempo_imagen_sobrante = (1000/framerate_deseado) - (SDL_GetTicks() - currentTime );	//En milisegundos --- In Milliseconds
			#ifdef DEBUG_INFO
			printf("Esperando %d ms para llegar a %d FPS\n", tiempo_imagen_sobrante, (int)framerate_deseado);
			#endif
			SDL_Delay(tiempo_imagen_sobrante);

		}
		///////////////////////////////////////////////////////////////////////////////////////

		// Calcular el tiempo restante de la cuenta atras --- Calculate countdown remaining time
		// Restamos al tiempo de la cuenta atras el tiempo del fotograma actual, solo si el juego no está en pausa --- We substract the duration of the current frame from the countdown time, only in case the game is not paused
		if ( mapa_original.CuentaAtras == true && !pause && !win && !lose && !intro_mapa ) 
		{
			TiempoActualCuentaAtras = TiempoActualCuentaAtras - (SDL_GetTicks() - currentTime);
			if ( TiempoActualCuentaAtras <= 0 )
			{
						// El tiempo ha expirado --- Countdown time expired
						lose = true;
						resultado_final = derrota;
			} 
		}


		#ifdef DEBUG_INFO
		//system("clear");		// Borra la terminal para leer el estado actual (MUY LENTO) --- Crears terminal for reading current statud (SLOW)
		#endif

	}
	// Detenemos la música del juego para volver al menu --- Stop music when returning to the menu
	if ( opciones_juego.music_enabled )
	{
		Mix_HaltMusic( );			// Parar musica del juego si se estaba reproduciendo --- Stop game music in case it was playing
	}	


	// Liberamos memoria dinamica --- Free dynamic memory
	#ifdef DEBUG_INFO
	printf("Liberando la memoria dinámica del juego.\n");
	#endif
	free(mapa_original.Mapa); mapa_original.Mapa = NULL;
	free(segmentos_girados); segmentos_girados = NULL;
	//free(interferencia_segmento);
	free(fuerzas_normales_segmentos); fuerzas_normales_segmentos = NULL;
	free(tipo_interferencia_segmento); tipo_interferencia_segmento = NULL;
	free(pos_camara_segmentos_girados); pos_camara_segmentos_girados = NULL;
	if (mapa_original.NumeroPinballBumpers != 0)	// Nuevo 09/02/2020
	{
		free(mapa_original.Bumpers); mapa_original.Bumpers = NULL;
	}
	free(centros_bumpers_girados); centros_bumpers_girados = NULL;
	free(centros_zonaacelcirc_girados); centros_zonaacelcirc_girados = NULL;
	free(pos_pant_centros_bumpers_girados); pos_pant_centros_bumpers_girados = NULL;
	free(pos_pant_centros_zonaacelcirc_girados); pos_pant_centros_zonaacelcirc_girados = NULL;
	if (mapa_original.NumeroZonasAceleracionCircular != 0)
	{
		free(mapa_original.ZonasAceleracionCircular); mapa_original.ZonasAceleracionCircular = NULL;
	}
	// Liberando posiciones enemigos (17/9/2021) --- Free enemy positions
	if (mapa_original.NumeroEnemigosRuta != 0)
	{
		free(pos_actual_enemigos_ruta); pos_actual_enemigos_ruta = NULL;
		free(centros_enemigos_ruta_girados); centros_enemigos_ruta_girados = NULL;
		free(pos_pant_centros_enemigos_ruta_girados); pos_pant_centros_enemigos_ruta_girados = NULL;
		free(num_waypoint_destino); num_waypoint_destino = NULL;
		free(angulos_actual_enemigos_ruta); angulos_actual_enemigos_ruta = NULL;
	}
	if (mapa_original.NumeroWormholes != 0)
	{
		free(mapa_original.Wormholes); mapa_original.Wormholes = NULL;
		free(centros_wormholes_girados); centros_wormholes_girados = NULL;
		free(pos_pant_centros_wormholes_girados); pos_pant_centros_wormholes_girados = NULL;
	}
	// Liberamos texturas, imagenes, sonidos, etc --- Free textures, images, sounds, etc
	freeMainGameLoopMedia();

	return resultado_final;
}



