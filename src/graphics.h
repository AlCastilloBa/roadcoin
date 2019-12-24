#include <SDL_ttf.h>

SDL_Texture* CargaTextura( char*, int*, int*, bool);

SDL_Texture* RenderizaTextoEnTextura( char*, TTF_Font*, SDL_Color, int*, int* );

void CambiarModoPantallaCompleta( bool, SDL_Window* );
