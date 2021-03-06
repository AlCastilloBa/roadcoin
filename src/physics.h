
// Nota: como "posicion" se usa "punto", definido en "maps.h".
// Note: "point" is used as "position", it is defined in "maps.h"

struct vector_velocidad		// Speed vector
{
	double vx;
	double vy;
};

struct vector_aceleracion	// Acceleration vector
{
	double ax;
	double ay;
};

struct vector_fuerza		// Force vector
{
	double fx;
	double fy;
};


struct vector_velocidad SumaVelocidad (struct vector_velocidad, struct vector_velocidad);

struct punto Velocidad2Posicion(struct punto, struct vector_velocidad, float );

struct vector_velocidad Aceleracion2Velocidad(struct vector_velocidad, struct vector_aceleracion, float );

struct vector_fuerza SumaFuerzas( struct vector_fuerza, struct vector_fuerza*, int , struct vector_fuerza*, int );

struct vector_aceleracion Fuerza2Aceleracion( struct vector_fuerza, double);

struct vector_fuerza CalculaReaccionNormalCentroSegmento ( double , double, double );

struct vector_fuerza CalculaReaccionNormalExtremoSegmento ( struct punto, struct punto, /*double angulo_segmento, */ double, double );

struct vector_velocidad AnulaVelocidadNormalASegmento( struct vector_velocidad, double); 

//struct vector_velocidad AnulaVelocidadEntradaNormalASegmento( struct vector_velocidad , double );

struct vector_velocidad VelAngular2VelLineal ( struct punto, struct punto, double, double );

double VelAng2Angulo ( double, double, double );

double CalculaVelGiroSobreSegmento ( struct vector_velocidad, double, double );

struct vector_velocidad AnulaVelocidadTangencialARecta( struct vector_velocidad, struct punto, struct punto);

struct vector_velocidad VelocidadSobreRecta ( double, struct punto, struct punto );

double modulo_vector_velocidad(struct vector_velocidad);

double modulo_vector_velocidad_cuadrado(struct vector_velocidad);

