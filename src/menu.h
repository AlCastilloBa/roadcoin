



// Listado de las pantallas del menu principal
enum pantalla_menu_principal
{
	menu_bienvenida,	// Welcome screen.
	menu_principal,		// Incluye botones: jugar, opciones, creditos, salir
	menu_jugar,		// Incluye botones: Campañas, Practicar
	menu_campana,
	menu_niveles_provisional,
	menu_practicar,
	menu_opciones,		// Incluye botones: Video, Sonido, Controles, Juego, Idioma, Atras
	menu_creditos,
	menu_opc_video,
	menu_opc_sonido,
	menu_opc_control,
	menu_opc_juego,
	meno_opc_idioma,
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
	boton_opc_atras,	// Go back

	// Menu opciones video
	boton_fullscreen,	// Fullscreen toggle button
	boton_wireframe,	// Wireframe toggle button
	boton_textured_objects,	// Textured lines button
	boton_opcvid_atras,	// Go back

	// Menu seleccion niveles provisional
	boton_provisional_nivel_01, boton_provisional_nivel_02, boton_provisional_nivel_03, boton_provisional_nivel_04, boton_provisional_nivel_05,
	boton_nivel_prov_atras
};

enum tipo_boton
{
	boton_pulsar,				// Push button (OK)
	boton_conmutar,				// Toggle button (OK)
	boton_valor_entero,			// Integer value input button (TODO)
	boton_seleccionar_lista			// Select from list button (TODO)
};

struct boton_menu
{
	// Generales
	int identificador_boton;					// Button identifier number
	enum tipo_boton clase_boton;					// Button type
	char texto[100];						// Button text
	int borde_izquierdo;						// Button left border
	int borde_derecho;						// Button right border
	int borde_arriba;						// Button upper border
	int borde_abajo;						// Button lower border
	//SDL_Surface* superficie_texto;				// Button text surface
	SDL_Texture* textura_boton_reposo;				// Button texture when not selected
	int textura_dim_x;						// Original texture x dimension (texture dimension, not button size)
	int textura_dim_y;						// Original texture y dimension (texture dimension, not button size)
	SDL_Texture* textura_boton_seleccionado;			// Button texture when selected
	// (TODO) Añadir más texturas, para estado reposo, raton encima, y hecho clic

	int boton_anterior;						// Previous button
	int boton_siguiente;						// Next button
	
	// Para boton de conmutación -- For toggle button
	bool estado_on_off;						// On/off status for toggle button
	// Para boton de introducir entero --- For integer value input button
	int valor_entero;						// Integer value for integer value input button
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

bool inicializa_datos_boton( struct boton_menu*, int, enum listado_botones, enum tipo_boton, char*, int, int  );

bool inicializa_datos_pantalla_menu( struct pantalla_menu*, enum pantalla_menu_principal, char* , char*, int );

bool inicializar_menu_principal( /*struct pantalla_menu*, */int );

bool liberar_memoria_menu_principal( /*struct pantalla_menu*,*/ int );

void bucle_principal_menu_principal( void );



