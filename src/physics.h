
struct posicion
{
	double x;
	double y;
};


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

struct posicion Velocidad2Posicion(struct posicion, struct vector_velocidad, float );

struct vector_velocidad Aceleracion2Velocidad(struct vector_velocidad, struct vector_aceleracion, float );
