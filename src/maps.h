#include <stdbool.h>

#define LIMITE_SEGMENTOS 50

// Tipos de segmentos
enum tipo_segmento 
{ 	
	pared, 	/* Pared, la moneda no atravesará */
	meta,	/* Meta, al tocar se gana el juego */
	muerte 	/* Muerte, al tocar se pierde el juego */
};

enum tipo_giro
{
	camara,		// El mapa gira alrededor de la cámara
	punto_fijo,	// El mapa gira alrededor de un punto fijo
	moneda		// El mapa gira alrededor de la moneda
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
};

struct mapa
{
	char NombreMapa[100];
	char DescripcionMapa[200];
	int NumeroSegmentos;
	enum tipo_giro TipoGiro;
	struct punto PuntoGiroFijo;
	float AnguloMax;
	struct punto PuntoInicialMoneda;
	float Gravedad;
	struct segmento *Mapa;
	char RutaImagenMoneda[255];
	char RutaImagenFondo[255];
};


bool GuardarMapaEnArchivo( struct mapa* mapa_a_guardar, char *nombre_archivo );
// Faltan más argumentos

struct mapa CargarMapaDesdeArchivo( char *nombre_archivo );
// Faltan más argumentos

bool VerificarMapa( int num_puntos, struct punto *mapa  );

//bool GiraMapa( int num_puntos, struct segmento * mapa_original, struct segmento * mapa_girado, double angulo );
// Nota: esta función debe calcular los valores de seno y coseno al principio, para no cargar la CPU
// Ya declarado en geometry.c, asi que borrar esto

