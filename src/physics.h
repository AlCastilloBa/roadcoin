
// Nota: como "posicion" se usa "punto", definido en "maps.h".

struct vector_velocidad
{
	double vx;
	double vy;
};

struct vector_aceleracion
{
	double ax;
	double ay;
};

struct vector_fuerza
{
	double fx;
	double fy;
};


struct vector_velocidad SumaVelocidad (struct vector_velocidad vel1, struct vector_velocidad vel2);

struct punto Velocidad2Posicion(struct punto, struct vector_velocidad, float );

struct vector_velocidad Aceleracion2Velocidad(struct vector_velocidad, struct vector_aceleracion, float );

struct vector_fuerza SumaFuerzas( struct vector_fuerza, struct vector_fuerza*, int );

struct vector_aceleracion Fuerza2Aceleracion( struct vector_fuerza, double);

struct vector_fuerza CalculaReaccionNormalCentroSegmento ( double , double, double );

struct vector_velocidad AnulaVelocidadNormalASegmento( struct vector_velocidad, double); 
