



// Listado de las pantallas del menu principal
enum pantalla_menu_principal
{
	menu_bienvenida,	// Welcome screen.
	menu_principal,		// Incluye botones: jugar, opciones, creditos, salir
	menu_jugar,		// Incluye botones: Campañas, Practicar
	menu_campana,
	menu_practicar,
	menu_opciones,		// Incluye botones: Video, Sonido, Controles, Juego, Idioma, Atras
	menu_creditos,
	NUM_TOTAL_PANTALLAS_MENU
};


// Listado de los botones de los menus
// Menu buttons list
enum listado_botones
{
	// Menu principal
	boton_jugar,		// Play button
	boton_opciones,		// Option button
	boton_creditos,		// Credits button
	boton_salir,		// Quit button

	// Menu opciones
	boton_video,		// Video options button
	boton_sonido,		// Sound options button
	boton_controles,	// Controls options button
	boton_opc_juego,	// Gameplay options button
	boton_idioma,		// Language options button
	boton_opc_atras		// Go back
};

enum tipo_boton
{
	boton_pulsar,			// Push button
	boton_conmutar,			// Toggle button
	boton_seleccionar_lista		// Select from list button
};

struct boton_menu
{
	int identificador_boton;			// Button identifier number
	enum tipo_boton clase_boton;			// Button type
	char texto[100];				// Button text
	int borde_izquierdo;				// Button left border
	int borde_derecho;				// Button right border
	int borde_arriba;				// Button upper border
	int borde_abajo;				// Button lower border
	//SDL_Surface* superficie_texto;		// Button text surface
	SDL_Texture* textura_boton_reposo;		// Button texture when not selected
	int textura_dim_x;				// Original texture x dimension (texture dimension, not button size)
	int textura_dim_y;				// Original texture y dimension (texture dimension, not button size)
	SDL_Texture* textura_boton_seleccionado;	// Button texture when selected
	// (TODO) Añadir más texturas, para estado reposo, raton encima, y hecho clic

	int boton_anterior;				// Previous button
	int boton_siguiente;				// Next button
};

struct pantalla_menu
{
	enum pantalla_menu_principal identificador_pantalla;		// Menu screen identifier
	char titulo[100];						// Menu screen title
	SDL_Texture* imagen_fondo_pantalla;				// Menu screen background image
	SDL_Texture* textura_titulo;					// Menu screen title text texture
	//int textura_titulo_dim_x;
	//int textura_titulo_dim_y;
	int borde_izquierdo_titulo;
	int borde_derecho_titulo;
	int borde_arriba_titulo;
	int borde_abajo_titulo;
	struct boton_menu* botones_pantalla;				// Puntero a array de botones --- Menu buttons pointer array
	int numero_botones;						// Buttons number in this screen
};



enum resultado
{
	abortado,		// Game aborted by the user
	victoria,		// Game finished in victory
	derrota			// Game finished in defeat
};

bool raton_sobre_boton (struct boton_menu, int, int );

bool inicializar_menu_principal( /*struct pantalla_menu*, */int );

bool liberar_memoria_menu_principal( /*struct pantalla_menu*,*/ int );

void bucle_principal_menu_principal( void );



