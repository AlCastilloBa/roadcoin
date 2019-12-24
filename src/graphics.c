#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h> 
#include <stdbool.h>


// Variables globales
//The window renderer 
extern SDL_Renderer* gRenderer;



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
