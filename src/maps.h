#include <stdbool.h>

#define LIMITE_SEGMENTOS 500
#define LIMITE_BUMPERS 500

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
};

struct pinball_bumper	// Nuevo 5/2/2020 (PENDIENTE PROBAR)
{
	struct punto centro;		// Bumper center point
	float radio;			// Bumper radius
	float velocidad_salida;		// Coin exit speed after hitting the bumper
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
};


bool GuardarMapaEnArchivo( struct mapa* mapa_a_guardar, char *nombre_archivo );
// Faltan más argumentos

struct mapa CargarMapaDesdeArchivo( char *nombre_archivo );
// Faltan más argumentos

bool VerificarMapa( int num_puntos, struct punto *mapa  );

//bool GiraMapa( int num_puntos, struct segmento * mapa_original, struct segmento * mapa_girado, double angulo );
// Nota: esta función debe calcular los valores de seno y coseno al principio, para no cargar la CPU
// Ya declarado en geometry.c, asi que borrar esto

