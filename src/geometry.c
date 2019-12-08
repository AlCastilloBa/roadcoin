#include <math.h>
#include <stdbool.h>

#include "maps.h"
#include "geometry.h"


//#define PI 3.1416

#define MIN(a,b) (((a)<(b))?(a):(b))		// Nota: no validas con operadores ++, -- , etc
#define MAX(a,b) (((a)>(b))?(a):(b))


struct punto GiraPunto ( struct punto centro_giro, struct punto punto_inicial, double angulo)
{
	// Matriz de giro 2D
	// Si el giro fuese respecto al origen:
	//	[ x' ]   [ cos a   - sin a ] [ x ]
	//	[ y' ] = [ sin a     cos a ] [ y ]
	// Como no es respecto al origen:
	//	[ x' - xo ]   [ cos a   - sin a ] [ x - xo ]	((Revisar))
	//	[ y' - yo ] = [ sin a     cos a ] [ y - yo ]
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

float AnguloSegmento (struct segmento segm )
{
	// Nota: Criterio de signos. Angulo positivo si va de X+ a Y+
	// Nota: (TODO) Pendiente de probar.
	return atan2( segm.end.y - segm.start.y , segm.end.x - segm.start.x );
}


//////////////////////////////////////////////////////////////////////////////////////////////////

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

	return (ProductoEscalar > 0 && ProductoEscalar < AreaRectangulo);
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

double LongitudVector( struct punto v, struct punto w)
{
	// Calcula el modulo de un vector
	// Es lenta, por tener una raiz cuadrada. Intentar evitar y usar LongitudVector_Cuadrado en su lugar cuando sea posible.
	return sqrt(  (w.x-v.x)*(w.x-v.x) + (w.y-v.y)*(w.y-v.y)  );
}


double DistanciaPuntoRecta_Cuadrado( struct punto P, struct punto A, struct punto B)
{
	// Esta función calcula la distancia entre un punto P y una recta definida por A B.
	struct punto P_proyectado;

	P_proyectado = PuntoProyectadoSobreRecta( P, A, B);
	return LongitudVector_Cuadrado(P, P_proyectado);
}

//--------------------------------------------------------------------------------------------------

enum tipo_interseccion_circulo_segmento  CirculoTocaSegmento( struct punto centro_circulo, double radio_circulo, struct punto A, struct punto B)
{
	//Resultado:
	// 0 - El circulo no toca el segmento en ningun sitio.
	// 1 - El circulo toca el segmento en el extremo A.
	// 2 - El cirtulo toca el segmento en un punto central cualquiera (entre los extremos).
	// 3 - El circulo toca el segmento en el extremo B.
	if ( ProyeccionEstaEnSegmento( centro_circulo, A, B) == true )
	{
		if (DistanciaPuntoRecta_Cuadrado( centro_circulo, A, B) < (radio_circulo*radio_circulo) )
		{
			return interseccion_central;	// En este caso, toca en punto central
		}
		else
		{
			return sin_interseccion;	// No toca
		}
	}
	if (LongitudVector_Cuadrado(A, centro_circulo)  <= (radio_circulo*radio_circulo) )
	{
		return interseccion_extremo_start;
	}
	if (LongitudVector_Cuadrado(B, centro_circulo)  <= (radio_circulo*radio_circulo) )
	{
		return interseccion_extremo_end;
	}
	return sin_interseccion;
}

bool CirculoTocaSegmentoExcluyendoExtremos( struct punto centro_circulo, double radio_circulo, struct punto A, struct punto B)
{
	//Verificamos si el circulo toca el segmento, solo considerando si la proyeccion del centro esta en el segmento
	if ( ProyeccionEstaEnSegmento( centro_circulo, A, B) == false )
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

//------------------------------------------------------------------------------------------------

// Las siguientes funciones están sacadas (y adaptadas) de la siguiente página:
// https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(struct punto p, struct punto q, struct punto r)
{
    if (q.x <= MAX(p.x, r.x) && q.x >= MIN(p.x, r.x) &&
        q.y <= MAX(p.y, r.y) && q.y >= MIN(p.y, r.y))
       return true;
    return false;
}


// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(struct punto p, struct punto q, struct punto r)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}


// The main function that returns true if line segment 'p1q1' 
// and 'p2q2' intersect. 
bool doIntersect(struct punto p1, struct punto q1, struct punto p2, struct punto q2) 
{ 
    // Find the four orientations needed for general and 
    // special cases 
    int o1 = orientation(p1, q1, p2); 
    int o2 = orientation(p1, q1, q2); 
    int o3 = orientation(p2, q2, p1); 
    int o4 = orientation(p2, q2, q1); 
  
    // General case 
    if (o1 != o2 && o3 != o4) 
        return true; 
  
    // Special Cases 
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
    if (o1 == 0 && onSegment(p1, p2, q1)) return true; 
  
    // p1, q1 and q2 are colinear and q2 lies on segment p1q1 
    if (o2 == 0 && onSegment(p1, q2, q1)) return true; 
  
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
    if (o3 == 0 && onSegment(p2, p1, q2)) return true; 
  
     // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
    if (o4 == 0 && onSegment(p2, q1, q2)) return true; 
  
    return false; // Doesn't fall in any of the above cases 
} 



//-----------------------------------------------------------------------------------------------------

struct punto CalculaPosTangenteACentroSegmento ( struct punto A, struct punto B, struct punto centro_circulo, double radio)
{ 
	// Calcula la posición tangente al centro del segmento

	struct punto nuevo_centro_tangente, proyeccion_centro_sobre_recta;
	double distancia_centro_recta;

	proyeccion_centro_sobre_recta = PuntoProyectadoSobreRecta( centro_circulo, A, B);
	distancia_centro_recta = sqrt( DistanciaPuntoRecta_Cuadrado( centro_circulo, A, B) );
	
	nuevo_centro_tangente.x = proyeccion_centro_sobre_recta.x + ((centro_circulo.x - proyeccion_centro_sobre_recta.x )/distancia_centro_recta)* radio;
	nuevo_centro_tangente.y = proyeccion_centro_sobre_recta.y + ((centro_circulo.y - proyeccion_centro_sobre_recta.y )/distancia_centro_recta)* radio;

	return nuevo_centro_tangente;
}


struct punto CalculaPosTangenteAExtremoSegmento ( struct punto extremo, struct punto centro_circulo, double radio)
{ 
	// Calcula la posición tangente al extremo del segmento

	struct punto nuevo_centro_tangente;
	double distancia_centrocirculo_extremosegmento;

	distancia_centrocirculo_extremosegmento = LongitudVector( extremo, centro_circulo);
	
	nuevo_centro_tangente.x = extremo.x + ((centro_circulo.x - extremo.x )/distancia_centrocirculo_extremosegmento)* radio;
	nuevo_centro_tangente.y = extremo.y + ((centro_circulo.y - extremo.y )/distancia_centrocirculo_extremosegmento)* radio;

	return nuevo_centro_tangente;
}


