

#include <stdbool.h>

// La siguiente estructura opciones contiene todos los ajustes del juego
// The following structure "options" contais all game settings
struct opciones
{
	bool fullscreen;
	int screen_x_resolution;
	int screen_y_resolution;

	bool wireframe;
	bool textured_objects;

	float mouse_sensitivity;
	
	bool music_enabled;
	bool sound_enabled;

};

// Opciones por defecto
// Default options
#define OPTIONS_DEFAULT_FULLSCREEN false;
#define OPTIONS_DEFAULT_SCREEN_X_RESOLUTION 800;
#define OPTIONS_DEFAULT_SCREEN_Y_RESOLUTION 600;

#define OPTIONS_DEFAULT_WIREFRAME true;
#define OPTIONS_DEFAULT_TEXTURED_OBJECTS true;

#define OPTIONS_DEFAULT_MOUSE_SENSIVITY 0.1f;

#define OPTIONS_DEFAULT_MUSIC_ENABLED true;
#define OPTIONS_DEFAULT_SOUND_ENABLED true;

// Declaración de funciones
// Function declaration
struct opciones CargarArchivoOpciones();

void GuardarArchivoOpciones ( struct opciones );
