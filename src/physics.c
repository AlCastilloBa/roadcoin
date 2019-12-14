#include <math.h>
#include <stdlib.h>


#include "maps.h"
#include "physics.h"
#include "geometry.h"

// Si definido DEBUG_INFO, mostrar textos de informacion por la terminal (hace el programa más lento)
//#define DEBUG_INFO

#ifdef DEBUG_INFO
	#include <stdio.h>
#endif


struct vector_velocidad SumaVelocidad (struct vector_velocidad vel1, struct vector_velocidad vel2)
{
	// Suma componente a componente
	struct vector_velocidad resultado;
	resultado.vx = vel1.vx + vel2.vx;
	resultado.vy = vel1.vy + vel2.vy;
	return resultado;
}

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

struct vector_fuerza CalculaReaccionNormalExtremoSegmento ( struct punto centro_circulo, struct punto extremo_segmento, /*double angulo_segmento, */ double gravedad, double masa )
{
	double angulo_radio, modulo_normal;
	struct vector_fuerza normal;
	angulo_radio = atan2( centro_circulo.y - extremo_segmento.y , centro_circulo.x - extremo_segmento.x );	

	modulo_normal = abs( masa*gravedad*cos( PI/2 - angulo_radio ) );

	if ( (centro_circulo.x >= extremo_segmento.x ) && (centro_circulo.y >= extremo_segmento.y) )	//Mayor, Mayor
	{
		normal.fx = abs(modulo_normal*sin(angulo_radio));	// Positivo
		normal.fy = abs(modulo_normal*cos(angulo_radio));	// Positivo
	}
	else if ( (centro_circulo.x >= extremo_segmento.x ) && (centro_circulo.y < extremo_segmento.y) )	// Mayor, Menor
	{
		normal.fx = abs(modulo_normal*sin(angulo_radio));		// Positivo
		normal.fy = -(abs(modulo_normal*cos(angulo_radio)));		// Negativo
	}
	else if ( (centro_circulo.x < extremo_segmento.x ) && (centro_circulo.y < extremo_segmento.y) )		// Menor, Menor 
	{
		normal.fx = -(abs(modulo_normal*sin(angulo_radio)));	// Negativo
		normal.fy = -(abs(modulo_normal*cos(angulo_radio)));	// Negativo
	}
	else if ( (centro_circulo.x < extremo_segmento.x ) && (centro_circulo.y >= extremo_segmento.y) )	// Menor, Mayor 
	{
		normal.fx = -(abs(modulo_normal*sin(angulo_radio)));	// Negativo
		normal.fy = abs(modulo_normal*cos(angulo_radio));	// Positivo
	}
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
	//if (angulo_segmento>=0)
	//{
		vector_unitario_normal.x = sin(angulo_segmento);	// Resultado debe ser positivo
		vector_unitario_normal.y = -cos(angulo_segmento);	// Resultado debe ser negativo
	//	#ifdef DEBUG_INFO
	//	// Comprobacion de los signos
	//	if ((vector_unitario_normal.x<0) || (vector_unitario_normal.y > 0))
	//	{
	//		printf("Aviso: Funcion AnulaVelocidadNormalASegmento no se comporta segun lo esperado\n");
	//		printf("nx = %f; ny = %f \n", vector_unitario_normal.x, vector_unitario_normal.y);
	//	}
	//	#endif
	//}
	//else
	//{
	//	vector_unitario_normal.x = sin(angulo_segmento);	// Resultado debe ser negativo
	//	vector_unitario_normal.y = -cos(angulo_segmento);	// Resultado debe ser negativo
	//	#ifdef DEBUG_INFO
	//	// Comprobacion de los signos
	//	if ((vector_unitario_normal.x>0) || (vector_unitario_normal.y > 0))
	//	{
	//		printf("Aviso: Funcion AnulaVelocidadNormalASegmento no se comporta segun lo esperado\n");
	//		printf("nx = %f; ny = %f \n", vector_unitario_normal.x, vector_unitario_normal.y);
	//	}
	//	#endif
	//}
	prod_escalar_v_n = ProductoEscalar2D (vector_velocidad_inicial, vector_unitario_normal);
	
	velocidad_tangencial.vx = velocidad_inicial.vx - prod_escalar_v_n * vector_unitario_normal.x;
	velocidad_tangencial.vy = velocidad_inicial.vy - prod_escalar_v_n * vector_unitario_normal.y;


	#ifdef DEBUG_INFO
	printf("En funcion AnulaVelocidadNormalASegmento: prodesc = %f,  \n nx = %f; ny = %f, \n vtx=%f, vty=%f \n",  prod_escalar_v_n, vector_unitario_normal.x, vector_unitario_normal.y, velocidad_tangencial.vx, velocidad_tangencial.vy );
	#endif
	return velocidad_tangencial;
}


struct vector_velocidad VelAngular2VelLineal ( struct punto centro_giro, struct punto pos_real, double angulo_girado, double delta_tiempo )
{
	// Nota: angulo en grados, delta_tiempo en segundos

	//vel_lineal = ( omega * radio) * vector unitario normal al radio

	struct vector_velocidad vel_lineal;
	struct punto vector_unitario_normal;
	double angulo_radio, radio, omega;

	angulo_radio = atan2( pos_real.y - centro_giro.y , pos_real.x - centro_giro.x );	
	vector_unitario_normal.x = sin(angulo_radio);	// Resultado debe ser positivo
	vector_unitario_normal.y = -cos(angulo_radio);	// Resultado debe ser negativo
	radio = LongitudVector(pos_real, centro_giro);

	omega = (angulo_girado * PI / 180) / delta_tiempo;	// (TODO) Si delta_tiempo es pequeño, omega es enorme

	vel_lineal.vx = omega * radio * vector_unitario_normal.x;
	vel_lineal.vy = omega * radio * vector_unitario_normal.y;

	return vel_lineal;
}


double VelAng2Angulo ( double angulo_inicial, double omega, double delta_tiempo )
{
	// phi_final = phi_inicial + omega * delta_tiempo
	//     phi en rad
	//     omega en rad/s
	//     delta_tiempo en s
	return angulo_inicial + omega * delta_tiempo;

}

double CalculaVelGiroSobreSegmento ( struct vector_velocidad velocidad_inicial, double angulo_segmento, double radio_moneda )
{
	// omega = Velocidad tangencial / radio moneda
	// (TODO) Optimizacion, esto se calcula en VelAngular2VelLineal, se pueden reducir calculos

	struct punto vector_unitario_normal, vector_velocidad_inicial;
	struct vector_velocidad velocidad_tangencial;
	double prod_escalar_v_n;

	//Convertimos velocidad desde struct vector_velocidad a struct punto
	vector_velocidad_inicial.x = velocidad_inicial.vx;
	vector_velocidad_inicial.y = velocidad_inicial.vy;

	vector_unitario_normal.x = sin(angulo_segmento);	// Resultado debe ser positivo
	vector_unitario_normal.y = -cos(angulo_segmento);	// Resultado debe ser negativo

	prod_escalar_v_n = ProductoEscalar2D (vector_velocidad_inicial, vector_unitario_normal);
	
	velocidad_tangencial.vx = velocidad_inicial.vx - prod_escalar_v_n * vector_unitario_normal.x;
	velocidad_tangencial.vy = velocidad_inicial.vy - prod_escalar_v_n * vector_unitario_normal.y;

	if (velocidad_tangencial.vx >= 0)	// Giro hacia la derecha, omega positivo
	{
		return (sqrt( velocidad_tangencial.vx*velocidad_tangencial.vx + velocidad_tangencial.vy*velocidad_tangencial.vy )) / radio_moneda;
	}
	if (velocidad_tangencial.vx < 0)	// Giro hacia la izquierda, omega negativo
	{
		return -(sqrt( velocidad_tangencial.vx*velocidad_tangencial.vx + velocidad_tangencial.vy*velocidad_tangencial.vy )) / radio_moneda;
	}
}

