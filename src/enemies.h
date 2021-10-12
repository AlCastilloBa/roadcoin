#include <stdbool.h>
#include <SDL.h>

#define MAX_TIPOS_ENEMIGOS_POR_MAPA 32
#define MAX_SPRITES_ANIMACION 32
#define MAX_WAYPOINTS_POR_ENEMIGO 64


struct waypoint_ruta_enemigo
{
	bool definido;
	struct punto waypoint;
};



struct enemigo_ruta	// Path enemy
{
	bool definido_OK;			// Defined OK
	char nombre_tipo[100];			// Type name

	// Datos tipo enemigo (calculados durante la carga) --- Enemy type (calculated during loading)
	int ID_unico_tipo_enemigo;		// Identificador unico del tipo de enemigo (calculado durante la carga) --- Unique ID for enemy type

	// Datos especificos a cada enemigo individual (leidos desde fichero mapa) --- Specific enemy data (read from map file)
	float speed_multiplier;
	struct waypoint_ruta_enemigo waypoints[MAX_WAYPOINTS_POR_ENEMIGO];
	int num_waypoints;

	// Datos especificos a cada enemigo individual (calculados durante el juego)
	float tolerancia_wp_pos_cuad;		// (move_speed * speed_multiplier * tiempo_imagen) ^ 2
	float tolerancia_wp_ang;		// rot_speed * speed_multiplier * tiempo_imagen
};


struct tipos_enemigos_usados_en_mapa
{
	bool tipo_utilizado;				// This type is used
	int ID_unico_tipo_enemigo;			// Enemy type unique ID
	char nombre_tipo[100];				// Type name
	char RutaSpriteSheet[255];			// Path to sprite sheet image
	int num_anim_sprites;				// Number of sprites of the animation
	int radio;					// Radio del enemigo (pixeles)
	int sprite_size;				// Tamaño de cada sprite de la animacion (pixeles) --- Size of every sprite of the animation
	int frames_per_animation;
	int move_speed;					// pixels/seg
	int rot_speed;					// deg/seg
	SDL_Texture* sprite_sheet;
	int dim_sprite_sheet_X;
	int dim_sprite_sheet_Y;
	SDL_Rect cuadro_sprite[MAX_SPRITES_ANIMACION];	// Array of squares that define every sprite of the animation
};






void Inicializar_Tipos_Enemigos_En_Mapa( struct tipos_enemigos_usados_en_mapa* );

int Clasificar_Tipos_Unicos_Enemigos( struct tipos_enemigos_usados_en_mapa *, struct enemigo_ruta *, int );

void Lista_Tipos_Enemigos_Unicos( struct tipos_enemigos_usados_en_mapa * );

void Leer_Ficheros_Definicion_Tipos_Enemigos( struct tipos_enemigos_usados_en_mapa * );

bool Inicializar_Texturas_Tipos_Enemigos( struct tipos_enemigos_usados_en_mapa * );

void Liberar_Memoria_Tipos_Enemigos( struct tipos_enemigos_usados_en_mapa *);

bool Verificar_Datos_Enemigos( struct enemigo_ruta *, int );

void Calcular_Numero_Waypoints_Enemigos( struct enemigo_ruta *, int );

void Calcula_Nueva_Posicion_Enemigos( int, struct enemigo_ruta *, struct tipos_enemigos_usados_en_mapa *, struct punto *, float*, int*, float);

