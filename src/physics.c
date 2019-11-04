
#include "maps.h"
#include "physics.h"


//struct vector_velocidad SumaVelocidad (struct vector_velocidad vel1, struct vector_velocidad vel2);

struct punto Velocidad2Posicion(struct punto pos_inicial, struct vector_velocidad vel, float delta_tiempo)
{
	// pos_final = pos_inicial + vel * tiempo
	struct punto pos_final;

	pos_final.x = pos_inicial.x + vel.vx * delta_tiempo;
	pos_final.y = pos_inicial.y + vel.vy * delta_tiempo;
	return pos_final;
}


struct vector_velocidad Aceleracion2Velocidad( struct vector_velocidad vinicial, struct vector_aceleracion acel, float delta_tiempo) 
{
	// vfinal = vinicial + acel * tiempo
	struct vector_velocidad vfinal;

	vfinal.vx = vinicial.vx + acel.ax * delta_tiempo;
	vfinal.vy = vinicial.vy + acel.ay * delta_tiempo;
	return vfinal;
}
