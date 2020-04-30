#include <SDL_ttf.h>

SDL_Texture* CargaTextura( char*, int*, int*, bool);

SDL_Texture* RenderizaTextoEnTextura( char*, TTF_Font*, SDL_Color, int*, int* );

void CambiarModoPantallaCompleta( bool, SDL_Window* );

int Digito_N_esimo (int , int );

enum ModoRepresentacionTextura
{
	estirar_hasta_rellenar,		// Stretch to fill completely
	con_relac_aspecto_no_rellenar,	// Keep aspect ratio, do not fill completely
	con_relac_aspecto_rellenar	// Fill completely, while keeping aspect ratio
};

void RepresentaTextura ( SDL_Renderer*, SDL_Texture*, int, int , int , int , int, int, enum ModoRepresentacionTextura );

