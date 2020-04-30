#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h> 
#include <stdbool.h>

#include "graphics.h"

// Variables globales
//The window renderer 
extern SDL_Renderer* gRenderer;

// Si definido DEBUG_INFO, mostrar textos de informacion por la terminal (hace el programa más lento)
// #define DEBUG_INFO

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
	if (transparente ==true)
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



SDL_Texture* RenderizaTextoEnTextura( char* textureText, TTF_Font* gFont, SDL_Color textColor, int* dim_x_textura, int* dim_y_textura )
{
	// Textura a devolver
	SDL_Texture* newTexture = NULL;
	//Render text surface
	SDL_Surface* textSurface = NULL;

	textSurface = TTF_RenderText_Solid( gFont, textureText, textColor );
	if( textSurface == NULL )
	{
 		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			if ( dim_x_textura != NULL || dim_y_textura != NULL )
			{
				//Get image dimensions
				*dim_x_textura = textSurface->w;
				*dim_y_textura = textSurface->h;
			}
		}
		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	//Return success
	return newTexture;
}


void CambiarModoPantallaCompleta( bool PantallaCompleta, SDL_Window* window )
{
	int resultado;
	//SDL_DisplayMode displaymode;

	if (PantallaCompleta == true )
	{
		resultado = SDL_SetWindowFullscreen( window, SDL_WINDOW_FULLSCREEN );
		//resultado = SDL_SetWindowFullscreen( window, SDL_WINDOW_FULLSCREEN_DESKTOP );

		//resultado = SDL_SetWindowDisplayMode(window, /*const SDL_DisplayMode* mode*/ NULL);
	}
	else
	{
		resultado = SDL_SetWindowFullscreen( window, 0 );
	}

}


// Pruebas 19/3/2020 (TODO)
int Digito_N_esimo (int n, int entero)
{
	// Esta función devuelve el digito "n" del numero "entero"
	// n=0 --> devuelve digito de unidades
	// n=1 --> devuelve digito de las decenas
	// n=2 --> devuelve digito de las centenas
	// ...etc
	while(n--)
	{
		entero/=10;
	}
	return entero%10;
}




void RepresentaTextura ( SDL_Renderer* renderizador, SDL_Texture* textura, 
			int dim_X_textura_original, int dim_Y_textura_original, 
			int rectangulo_borde_izquierdo, int rectangulo_borde_derecho, int rectangulo_borde_superior, int rectangulo_borde_inferior,
			enum ModoRepresentacionTextura modo)
{
	// Esta funcion representa una textura en un rectangulo dado (zona de la pantalla), pero 
	// manteniendo la relación de aspecto de la textura original (sin aplastarla o estirarla).
	//  - Si modo = estirar_hasta_rellenar: la textura se estira, sin dejar huecos.
	//  - Si modo = con_relac_aspecto_no_rellenar: la textura se representa completa (dejando huecos vacios).
	//  - Si modo = con_relac_aspecto_rellenar:  la textura llena el rectangulo (sin dejar huecos, pero hay partes de la textura que no se ven).
	// 
	// This function displays a texture in a given rectangle (screen area), but
	// keeping the original texture aspect ratio  (neither squashing nor stretching).
	//  - If mode = estirar_hasta_rellenar (strecth to fill): the texture will be stretched, no empty areas in the rectangle.
	//  - If mode = con_relac_aspecto_no_rellenar: the texture is displayed completely (but leaving empty areas on the rectangle).
	//  - If mode = con_relac_aspecto_rellenar: the texture will fill the rectangle (leaving no empty areas, but some areas of the texture will not be displayed). 
	float relacion_aspecto_rectangulo, relacion_aspecto_textura;
	SDL_Rect RenderQuadDest, RenderQuadOrigen;		// Variable "rectangulo SDL" para indicar donde renderizar los objetos en la pantalla {x, y, w, h}

	#ifdef DEBUG_INFO
	printf("Debug RepresentaTextura: Datos Textura: x=%d, y=%d, Rectangulo: izq=%d, der=%d, arr=%d, aba=%d\n", dim_X_textura_original, dim_Y_textura_original, rectangulo_borde_izquierdo, rectangulo_borde_derecho, rectangulo_borde_superior, rectangulo_borde_inferior );
	#endif

	// Verificaciones iniciales para evitar "violacion de segmento"
	// initial check, to avoid segmentation fault
	if ( renderizador == NULL || textura == NULL )
	{
		printf("RepresentaTextura: Error de programacion, encontrados punteros NULL en renderizador o textura.\n");
		return;
	} 
	
	switch ( modo )
	{
		case estirar_hasta_rellenar:
			RenderQuadDest.x = rectangulo_borde_izquierdo;
			RenderQuadDest.y = rectangulo_borde_superior;
			RenderQuadDest.w = ( rectangulo_borde_derecho - rectangulo_borde_izquierdo );
			RenderQuadDest.h = ( rectangulo_borde_inferior - rectangulo_borde_superior );
			// Representamos -- Display
			SDL_RenderCopy( renderizador, textura, NULL, &RenderQuadDest );
			break;
		case con_relac_aspecto_no_rellenar:
			if ( dim_X_textura_original == 0 || dim_Y_textura_original == 0 )
			{
				printf("RepresentaTextura: Error de programacion, se necesitan las dimensiones de la textura original.\n");
				return;
			}
			relacion_aspecto_rectangulo = (float) ( rectangulo_borde_derecho - rectangulo_borde_izquierdo ) / ( rectangulo_borde_inferior - rectangulo_borde_superior );
			relacion_aspecto_textura = (float) dim_X_textura_original / dim_Y_textura_original;
			if ( relacion_aspecto_rectangulo < relacion_aspecto_textura )
			{
				// En este caso, la textura tocará los bordes izquierdo y derecho del rectangulo
				// In this case, the texture will touch left and right rectangle borders
				RenderQuadDest.w = ( rectangulo_borde_derecho - rectangulo_borde_izquierdo );
				RenderQuadDest.h = (float) RenderQuadDest.w / relacion_aspecto_textura;
				RenderQuadDest.x = rectangulo_borde_izquierdo;
				RenderQuadDest.y = (float) rectangulo_borde_superior + ( rectangulo_borde_inferior - rectangulo_borde_superior )/2 - RenderQuadDest.h/2;
				#ifdef DEBUG_INFO
				printf("Debug RepresentaTextura: caso %d, RA_rect(%f)<RA_text(%f), x=%d, y=%d, w=%d, h=%d\n", con_relac_aspecto_no_rellenar, relacion_aspecto_rectangulo, relacion_aspecto_textura, RenderQuadDest.x, RenderQuadDest.y, RenderQuadDest.w, RenderQuadDest.h);
				#endif
			}
			else
			{
				// En este caso, la textura tocará los bordes superior e inferior del rectangulo
				// In this case, the texture will touch the upper and lower rectangle borders
				RenderQuadDest.h = ( rectangulo_borde_inferior - rectangulo_borde_superior );
				RenderQuadDest.w = (float) RenderQuadDest.h * relacion_aspecto_textura;
				RenderQuadDest.y = rectangulo_borde_superior;
				RenderQuadDest.x = (float)rectangulo_borde_izquierdo + (rectangulo_borde_derecho-rectangulo_borde_izquierdo)/2 - (RenderQuadDest.w/2);
				#ifdef DEBUG_INFO
				printf("Debug RepresentaTextura: caso %d, RA_rect(%f)>RA_text(%f), x=%d, y=%d, w=%d, h=%d\n", con_relac_aspecto_no_rellenar, relacion_aspecto_rectangulo, relacion_aspecto_textura, RenderQuadDest.x, RenderQuadDest.y, RenderQuadDest.w, RenderQuadDest.h);
				#endif
			}
			// Representamos -- Display
			SDL_RenderCopy( renderizador, textura, NULL, &RenderQuadDest );
			break;
		case con_relac_aspecto_rellenar:
			if ( dim_X_textura_original == 0 || dim_Y_textura_original == 0 )
			{
				printf("RepresentaTextura: Error de programacion, se necesitan las dimensiones de la textura original.\n");
				return;
			}
			relacion_aspecto_rectangulo = ( rectangulo_borde_derecho - rectangulo_borde_izquierdo ) / ( rectangulo_borde_inferior - rectangulo_borde_superior );
			relacion_aspecto_textura = dim_X_textura_original / dim_Y_textura_original;
			if ( relacion_aspecto_rectangulo < relacion_aspecto_textura )
			{
				// En este caso, los bordes superior e inferior de la textura coincidirán con los bordes superior e 
				// inferior del rectangulo. La textura "se saldrá" por los bordes izquierdo y derecho.
				// In this case, upper and lower texture borders will be the same as upper and lower rectangle borders.
				// The texture will "spill" across left and right borders.
				RenderQuadOrigen.y = 0;
				RenderQuadOrigen.h = dim_Y_textura_original;
				RenderQuadOrigen.w = dim_Y_textura_original * relacion_aspecto_textura;
				RenderQuadOrigen.x = dim_X_textura_original/2 - RenderQuadOrigen.w/2;
	
				RenderQuadDest.x = rectangulo_borde_izquierdo;
				RenderQuadDest.y = rectangulo_borde_superior;
				RenderQuadDest.w = ( rectangulo_borde_derecho - rectangulo_borde_izquierdo );
				RenderQuadDest.h = ( rectangulo_borde_inferior - rectangulo_borde_superior );
			}
			else
			{
				// En este caso, los bordes izquierdo y derecho de la textura coincidirán con los bordes izquierdo y 
				// derecho del rectangulo. La textura "se saldrá" por los bordes inferior y superior.
				// In this case, left and right texture borders will be the same as left and right rectangle borders.
				// The texture will "spill" across upper and lower borders.
				RenderQuadOrigen.x = 0;
				RenderQuadOrigen.w = dim_X_textura_original;
				RenderQuadOrigen.h = dim_X_textura_original / relacion_aspecto_textura;
				RenderQuadOrigen.y = dim_Y_textura_original/2 - RenderQuadOrigen.h/2;

				RenderQuadDest.x = rectangulo_borde_izquierdo;
				RenderQuadDest.y = rectangulo_borde_superior;
				RenderQuadDest.w = ( rectangulo_borde_derecho - rectangulo_borde_izquierdo );
				RenderQuadDest.h = ( rectangulo_borde_inferior - rectangulo_borde_superior );
			}
			// Representamos -- Display
			SDL_RenderCopy( renderizador, textura, &RenderQuadOrigen, &RenderQuadDest );
			break;
	}

}



