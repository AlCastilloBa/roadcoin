#include <math.h>
#include <stdbool.h>

#include "maps.h"


#define PI 3.1416

struct punto GiraPunto ( struct punto centro_giro, struct punto punto_inicial, double angulo)
{
	// Matriz de giro 2D
	// Si el giro fuese respecto al origen:
	//	[ x' ]   [ cos a   - sin a ] [ x ]
	//	[ y' ] = [ sin a     cos a ] [ y ]
	// Como no es respecto al origen:
	//	[ x' - xo ]   [ cos a   - sin a ] [ x - xo ]	((Revisar))
	//	[ y' - xo ] = [ sin a     cos a ] [ y - yo ]
	struct punto punto_final;
	double coseno, seno;

	coseno = cos(angulo*(2 * PI / 360));
	seno = sin(angulo*(2 * PI / 360));

	punto_final.x = ( coseno*(punto_inicial.x-centro_giro.x) - seno*(punto_inicial.y-centro_giro.y)) + centro_giro.x;
	punto_final.y = ( seno*(punto_inicial.x-centro_giro.x) + coseno*(punto_inicial.y-centro_giro.y)) + centro_giro.y;

	return punto_final;
}

void GiraMapaCompleto( struct segmento* inicial, struct segmento* girado, struct punto centro_giro, int num_segmentos, double angulo )
{
	int segm;
	// VERSION LENTA PARA PROBAR, SE PUEDE OPIMIZAR (reduciendo numero de veces que se calcula sin y cos)
	for ( segm = 0 ; segm < num_segmentos ; segm++ )
	{
		girado[segm].start = GiraPunto( centro_giro , inicial[segm].start, angulo );
		girado[segm].end   = GiraPunto( centro_giro , inicial[segm].end  , angulo );
	}

}


double ProductoEscalar2D (struct punto A, struct punto B)
{
	// Nota función puede sufrir "cancelaciones catastróficas"
	// En caso de problemas, usar "Kahan summation algorithm"
	// A*B = |A| |B| cos(a)
	double resultado;
	resultado = A.x * B.x + A.y * B.y;
	return resultado;
} 


double ProductoEscalarPerpendicular2D (struct punto A, struct punto B)
{
	// Nota: no confundir con el producto escalar
	// A*B = |A| |B| sin(a)
	double resultado;
	resultado = A.x * B.y - A.y * B.x;
	return resultado;
}

bool ProyeccionEstaEnSegmento( struct punto P, struct punto A, struct punto B)
{
	// Esta función indica si la proyección de P en la recta definida por AB (que es P'), está dentro
	// del segmento entre A y B.

	/*Consider the vectors e1 = (v2, v1) and e2 = (pv1).
	    If p' matches with v1 (this means both have the same location), then
	    DP(e1, e2) = 0. e1 is then perpendicular to e2.
	    On the other side, if p' matches with v2 (thus p' is the same as v2), then
	    DP(e1, e2) = DP(e1, e1) = e1.x^2 + e1.y^2.
	    If p' lies somewhere between v1 and v2, then for the dot-product of e1 and e2 applies:
	    0 <= DP(e1,e2) <= e1.x^2 + e1.y^2.
	*/

	double AreaRectangulo, ProductoEscalar;
	struct punto e1, e2;

	// Vector e1 es A B.	
	e1.x = B.x - A.x;	e1.y = B.y - A.y;
	// Vector e2 es P A.
	e2.x = P.x - A.x;	e2.y = P.y - A.y;
	
	AreaRectangulo = ProductoEscalar2D(e1,e1);
	ProductoEscalar = ProductoEscalar2D(e1,e2);

	return (ProductoEscalar > 0 || ProductoEscalar < AreaRectangulo);
}

bool PuntoEstaEnSegmento (struct punto P, struct punto A, struct punto B)
{
	// Esta función indica si un punto P está en el segmento definido por AB.

	int dx1, dy1;
	double epsilon;
	struct punto e1, e2;
	
	// Calculamos "epsilon", debido a que la igualdad no será exacta, debido a imprecisiones por la coma flotante.
	// instead of checking against zero, some epsilon value has to be used. Because the size of the pdp value depends
	// on the length of the vectors, no static value can be used. One approach is to compare the pdp/area value to 
	// the fraction of another area which also depends on the length of the line e1=(v1, v2), e.g. the area 
	// of the square with side e1 which is computed below 
	epsilon = 0.003 * (dx1 * dx1 + dy1 * dy1);

	e1.x = A.x - P.x;	e1.y = A.y - P.y;
	e2.x = B.x - P.x;	e1.y = B.y - P.y;

	return ( ProductoEscalarPerpendicular2D(e1, e2) < epsilon );
	
}

struct punto PuntoProyectadoSobreRecta( struct punto P, struct punto A, struct punto B)
{
	// Esta función calcula las coordenadas de un punto p_prima que es la proyección de P sobre la recta
	// definida por A y B.

	struct punto e1, e2, p_prima;
	double valorProdEscalar, long_e1_cuad;

	e1.x = B.x - A.x ;	e1.y = B.y - A.y;
	e2.x = P.x - A.x ;	e2.y = P.y - A.y;

	valorProdEscalar = ProductoEscalar2D( e1, e2 );

	long_e1_cuad = e1.x * e1.x + e1.y * e1.y;

	p_prima.x = A.x + (valorProdEscalar * e1.x) / long_e1_cuad;
	p_prima.y = A.y + (valorProdEscalar * e1.y) / long_e1_cuad;

	return p_prima;
}

double LongitudVector_Cuadrado( struct punto v, struct punto w)
{
	// Calcula el cuadrado del modulo de un vector
	return (w.x-v.x)*(w.x-v.x) + (w.y-v.y)*(w.y-v.y);
}


double DistanciaPuntoRecta_Cuadrado( struct punto P, struct punto A, struct punto B)
{
	// Esta función calcula la distancia entre un punto P y una recta definida por A B.
	struct punto P_proyectado;

	P_proyectado = PuntoProyectadoSobreRecta( P, A, B);
	return LongitudVector_Cuadrado(P, P_proyectado);
}

//--------------------------------------------------------------------------------------------------

bool ProyeccionCirculoTocaSegmento( struct punto centro_circulo, double radio_circulo, struct punto A, struct punto B)
{
	// Esta función indica si la proyección de un circulo (definido por centro y radio) en la recta definida por AB,
	// está dentro del segmento entre A y B.
	// Nota: ver funcion ProyeccionEstaEnSegmento para explicaciones

	
	double AreaRectangulo, ProductoEscalar;
	struct punto e1, e2;

	// Vector e1 es A B.	
	e1.x = B.x - A.x;	e1.y = B.y - A.y;
	// Vector e2 es P A.
	e2.x = centro_circulo.x - A.x;	e2.y = centro_circulo.y - A.y;
	
	AreaRectangulo = ProductoEscalar2D(e1,e1);
	ProductoEscalar = ProductoEscalar2D(e1,e2);

	return (ProductoEscalar > 0 - radio_circulo || ProductoEscalar < (AreaRectangulo+radio_circulo));
}


bool CirculoTocaSegmento( struct punto centro_circulo, double radio_circulo, struct punto A, struct punto B)
{
	if ( ProyeccionCirculoTocaSegmento( centro_circulo, radio_circulo, A, B) == false )
	{
		return false;
	}
	if (DistanciaPuntoRecta_Cuadrado( centro_circulo, A, B) < (radio_circulo*radio_circulo) )
	{
		return true;
	}
	else
	{
		return false;
	}
}


