#include <stdbool.h>
#include <SDL.h>		// Pruebas 8/4/2020

#define LIMITE_SEGMENTOS 1500
#define LIMITE_BUMPERS 100
#define LIMITE_ZONAS_ACEL_CIRC 100
// #define LIMITE_CONJUNTOS_MAPAS 200		--> Ya no es necesario, BORRAR

// Tipos de segmentos --- Line segments types
enum tipo_segmento 
{ 	
	pared = 0, 			/* Pared, la moneda no atravesará --- Wall, the coin will not pass through*/
	meta = 1,			/* Meta, al tocar se gana el juego --- Goal, if touched you will win the game*/
	muerte = 2, 			/* Muerte, al tocar se pierde el juego --- Deathm if touched you will lose the game*/
	pinball_flipper_L = 3,
	pinball_flipper_R = 4
};

enum tipo_giro
{
	camara = 0,		// (0) El mapa gira alrededor de la cámara
	punto_fijo = 1,		// (1) El mapa gira alrededor de un punto fijo
	moneda = 2,		// (2) El mapa gira alrededor de la moneda
	origen = 3,		// (3) El mapa gira alrededor del origen, y arrastra a la moneda en su giro
	no_rot = 4		// (4) El mapa no va a girar
};

// Punto en 2D
struct punto
{
	double x;
	double y;
};

// Segmento con inicio, fin, y se indica el tipo de segmento
struct segmento
{
	struct punto start;
	struct punto end;
	enum tipo_segmento type;
	bool start_adyacente_a_otro;	// Starting point is next to another one
	bool end_adyacente_a_otro;	// Ending point is next to another one
	bool invisible;			// Invisible
	bool definido_OK;		// Is this line segment correctly defined and read?
};

struct pinball_bumper	// Nuevo 5/2/2020 (PENDIENTE PROBAR)
{
	struct punto centro;		// Bumper center point
	float radio;			// Bumper radius
	float velocidad_salida;		// Coin exit speed after hitting the bumper
	bool definido_OK;		// Is this pinball pop bumper correctly defined and read?
};

struct zona_aceleracion_circular		// Nuevo 21/3/2020 (TODO) PRUEBAS
{
	struct punto centro;		// Acceleration zone center
	float radio;			// Acceleration zone radius
	float angulo;			// Acceleration zone angle
	float aceleracion;		// Acceleration zone speed
	bool definido_OK;		// Is this round acceleration zone correctly defined and read?
};

struct mapa
{
	char NombreMapa[100];				// Map name
	char DescripcionMapa[200];			// Map description
	int NumeroSegmentos;				// Number of segment lines
	enum tipo_giro TipoGiro;			// Type of rotation behaviour
	struct punto PuntoGiroFijo;			// Fixed point center of rotation
	float AnguloMax;				// Maximum angle
	struct punto PuntoInicialMoneda;		// Coin starting position
	float Gravedad;					// Gravity
	struct segmento *Mapa;				// Pointer to segment lines memory area
	char RutaImagenMoneda[255];			// Path to coin image
	char RutaImagenFondo[255];			// Path to background image

	bool no_rot_moneda;				// No coin rotation (for this map)

	bool CuentaAtras;				// This map has a time countdown
	int SegundosCuentaAtras;			// Numer or secnods of the countdown

	bool HayFondoGiratorio;				// Rotating background exists
	char RutaImagenFondoGiratorio[255];		// Path to rotating background image
	int Pos_x_izquierda_fondo_giratorio;		// Rotating background left coordinate
	int Pos_y_arriba_fondo_giratorio;		// Rotating background upper coordinate
	int Pos_x_derecha_fondo_giratorio;		// Rotating background right coordinate
	int Pos_y_abajo_fondo_giratorio;		// Rotating background lower coordinate
	struct punto CentroGiroFondoGiratorio;		// Rotating background, center of rotation

	char RutaMusica[255];				// Path to music file for the level
	
	int NumeroPinballBumpers;			
	struct pinball_bumper *Bumpers;			// Pointer to pinball bumpers memory area

	bool mapa_contiene_flippers;			// The map contains flippers
	float angulo_flippers;				// Flippers rotation angle

	int NumeroZonasAceleracionCircular;		// Number of round acceleration zones
	struct zona_aceleracion_circular *ZonasAceleracionCircular;	// Pointer to round acceleration zone memory area
};


// Pruebas 8/4/2020 (TODO)
struct ConjuntoMapas		// Level set
{
	char Directorio[255];			// Level set directory name
	SDL_Texture* TexturaNombreDir;		// Level set directory name texture pointer
	char Descripcion[255];			// Level set description text
	SDL_Texture* TexturaDescripcion;	// Level set description text texture pointer
	char RutaImagen[255];			// Level set image path
	SDL_Texture* TexturaImagen;		// Level set image SDL2 texture pointer
	SDL_Texture* TexturaNumeracion;		// Level set numbering (ex. 5/14 )
	bool dir_definido_OK;			// Is this level set directory name correctly defined and read?
	bool desc_definido_OK;			// Is this level set description correctly defined and read?
	bool imag_definido_OK;			// Is this level set image path correctly defined and read?
	int Numero_Mapas;			// Number of levels in level-set (defined in level_list inside directory)
	struct InfoMapas* mapas_individuales;	// Array of informacion of single levels inside the level set
};

struct InfoMapas		// Level info
{
	char RutaMapa[255];			// Array of paths to maps inside level set (defined in level list inside directory)
	struct mapa DatosMapa;			// Map information array for levels inside level set
	bool mapa_definido_OK;
	SDL_Texture* TexturaNombreMapa;
	SDL_Texture* TexturaDescripcionMapa;
	SDL_Texture* TexturaImagenMapa;
	SDL_Texture* TexturaModoGiro;
	SDL_Texture* TexturaAnguloMaximo;
	SDL_Texture* TexturaGravedad;
	SDL_Texture* TexturaTiempo;
	SDL_Texture* TexturaNumeracion;
};
 


//bool GuardarMapaEnArchivo( struct mapa* mapa_a_guardar, char *nombre_archivo );
// Faltan más argumentos

struct mapa CargarMapaDesdeArchivo( char *nombre_archivo );

struct mapa LeerInfoArchivo( char *nombre_archivo );

struct ConjuntoMapas* LeerConjuntosMapasJuego ( int* argumento_numero_conjunto_mapas );

struct InfoMapas* LeerInfoMapasDeConjunto( char* nombre_directorio, int* argumento_num_mapas_en_conjunto );

void Eliminar_NewLine_En_FinalCadena (char* cadena_original);

// bool VerificarMapa( int num_puntos, struct punto *mapa  );



