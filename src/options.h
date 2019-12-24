

#include <stdbool.h>

// La siguiente estructura opciones contiene todos los ajustes del juego
// The following structure "options" contais all game settings
struct opciones
{
	bool fullscreen;
	int screen_x_resolution;
	int screen_y_resolution;
};

// Opciones por defecto
// Default options
#define OPTIONS_DEFAULT_FULLSCREEN false;
#define OPTIONS_DEFAULT_SCREEN_X_RESOLUTION 800;
#define OPTIONS_DEFAULT_SCREEN_Y_RESOLUTION 600;




// Declaración de funciones
// Function declaration
struct opciones CargarArchivoOpciones();

void GuardarArchivoOpciones ( struct opciones );
