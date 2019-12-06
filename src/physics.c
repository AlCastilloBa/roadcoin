#include <math.h>


#include "maps.h"
#include "physics.h"
#include "geometry.h"

// Si definido DEBUG_INFO, mostrar textos de informacion por la terminal (hace el programa más lento)
//#define DEBUG_INFO

#ifdef DEBUG_INFO
	#include <stdio.h>
#endif


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


struct vector_fuerza SumaFuerzas( struct vector_fuerza gravedad , struct vector_fuerza* normales_segmentos, int numero_segmentos )
{
	// Nota: (TODO) Añadir más tipos de fuerzas
	int i;
	struct vector_fuerza resultante;
	resultante.fx = gravedad.fx;
	resultante.fy = gravedad.fy;
	for ( i=0 ; i< numero_segmentos ; i++ )
	{
		resultante.fx += normales_segmentos[i].fx;
		resultante.fy += normales_segmentos[i].fy;
	}
	return resultante;
}

struct vector_aceleracion Fuerza2Aceleracion( struct vector_fuerza fuerza, double masa )
{
	struct vector_aceleracion aceleracion;
	aceleracion.ax = fuerza.fx / masa;
	aceleracion.ay = fuerza.fy / masa;
	return aceleracion;
}

struct vector_fuerza CalculaReaccionNormalCentroSegmento ( double angulo_segmento, double gravedad, double masa )
{
	// Esta función devuelve un vector de la fuerza normal de apoyo sobre el centro de un segmento.
	// Nota: no es válida cuando el contacto es con los extremos del semgento
	double coseno, seno, modulo_normal;
	struct vector_fuerza normal;
	coseno = cos(angulo_segmento);
	seno = sin(angulo_segmento);

	modulo_normal = masa*gravedad*coseno;
	normal.fx = modulo_normal*seno;
	normal.fy = -(modulo_normal*coseno);

	return normal;
}

struct vector_velocidad AnulaVelocidadNormalASegmento( struct vector_velocidad velocidad_inicial, double angulo_segmento)
{
	struct punto vector_unitario_normal, vector_velocidad_inicial;
	struct vector_velocidad velocidad_tangencial;
	double prod_escalar_v_n;

	//Convertimos velocidad desde struct vector_velocidad a struct punto
	vector_velocidad_inicial.x = velocidad_inicial.vx;
	vector_velocidad_inicial.y = velocidad_inicial.vy;

	//Creamos un vector unitario normal al segmento, dado el angulo
	if (angulo_segmento>=0)
	{
		vector_unitario_normal.x = sin(angulo_segmento);	// Resultado debe ser positivo
		vector_unitario_normal.y = -cos(angulo_segmento);	// Resultado debe ser negativo
		#ifdef DEBUG_INFO
		// Comprobacion de los signos
		if ((vector_unitario_normal.x<0) || (vector_unitario_normal.y > 0))
		{
			printf("Aviso: Funcion AnulaVelocidadNormalASegmento no se comporta segun lo esperado\n");
			printf("nx = %f; ny = %f \n", vector_unitario_normal.x, vector_unitario_normal.y);
		}
		#endif
	}
	else
	{
		vector_unitario_normal.x = sin(angulo_segmento);	// Resultado debe ser negativo
		vector_unitario_normal.y = -cos(angulo_segmento);	// Resultado debe ser negativo
		#ifdef DEBUG_INFO
		// Comprobacion de los signos
		if ((vector_unitario_normal.x>0) || (vector_unitario_normal.y > 0))
		{
			printf("Aviso: Funcion AnulaVelocidadNormalASegmento no se comporta segun lo esperado\n");
			printf("nx = %f; ny = %f \n", vector_unitario_normal.x, vector_unitario_normal.y);
		}
		#endif
	}
	prod_escalar_v_n = ProductoEscalar2D (vector_velocidad_inicial, vector_unitario_normal);
	
	velocidad_tangencial.vx = velocidad_inicial.vx - prod_escalar_v_n * vector_unitario_normal.x;
	velocidad_tangencial.vy = velocidad_inicial.vy - prod_escalar_v_n * vector_unitario_normal.y;


	#ifdef DEBUG_INFO
	printf("En funcion AnulaVelocidadNormalASegmento: prodesc = %f,  \n nx = %f; ny = %f, \n vtx=%f, vty=%f \n",  prod_escalar_v_n, vector_unitario_normal.x, vector_unitario_normal.y, velocidad_tangencial.vx, velocidad_tangencial.vy );
	#endif
	return velocidad_tangencial;
}


