#include <math.h>
#include <stdbool.h>

#include "maps.h"
#include "geometry.h"


#define MIN(a,b) (((a)<(b))?(a):(b))		// Nota: no validas con operadores ++, -- , etc
#define MAX(a,b) (((a)>(b))?(a):(b))		// Note: not valid with operators ++, --, etc


struct punto GiraPunto ( struct punto centro_giro, struct punto punto_inicial, double angulo)	// Rotate point
{
	// Matriz de giro 2D --- 2D rotation matrix
	// Si el giro fuese respecto al origen: --- If we rotated around the origin
	//	[ x' ]   [ cos a   - sin a ] [ x ]
	//	[ y' ] = [ sin a     cos a ] [ y ]
	// Como no es respecto al origen: --- But since we do not rotate around the origin
	//	[ x' - xo ]   [ cos a   - sin a ] [ x - xo ]
	//	[ y' - yo ] = [ sin a     cos a ] [ y - yo ]
	struct punto punto_final;
	double coseno, seno;

	coseno = cos(angulo*(2 * PI / 360));
	seno = sin(angulo*(2 * PI / 360));

	punto_final.x = ( coseno*(punto_inicial.x-centro_giro.x) - seno*(punto_inicial.y-centro_giro.y)) + centro_giro.x;
	punto_final.y = ( seno*(punto_inicial.x-centro_giro.x) + coseno*(punto_inicial.y-centro_giro.y)) + centro_giro.y;

	return punto_final;
}

void GiraMapaCompleto( struct segmento* inicial, struct segmento* girado, struct punto centro_giro, int num_segmentos, double angulo )	// Rotate complete map
{
	int segm;
	// (TODO)VERSION LENTA PARA PROBAR, SE PUEDE OPIMIZAR (reduciendo numero de veces que se calcula sin y cos)
	// SLOW VERSION TO TEST, THIS CAN BE OPTIMIZED (reducing the number of times sin and cos are calculated)
	for ( segm = 0 ; segm < num_segmentos ; segm++ )
	{
		girado[segm].start = GiraPunto( centro_giro , inicial[segm].start, angulo );
		girado[segm].end   = GiraPunto( centro_giro , inicial[segm].end  , angulo );
	}

}

void CopiaSegmentosSinGiro( struct segmento* origen, struct segmento* destino, int num_segmentos )	// Copy segments without rotating
{
	// Para el caso sin rotación, simplemente copia los puntos sin aplicar un giro
	// For cases without rotation, copies points without rotating
	int segm;
	for ( segm = 0 ; segm < num_segmentos ; segm++ )
	{
		destino[segm].start = origen[segm].start;
		destino[segm].end   = origen[segm].end;
	}
}

void GiraBumpers( struct pinball_bumper* inicial, struct punto* girado, struct punto centro_giro, int num_bumpers, double angulo )	// Rotate bumpers
{
	int bumper;
	// (TODO)VERSION LENTA PARA PROBAR, SE PUEDE OPIMIZAR (reduciendo numero de veces que se calcula sin y cos)
	// SLOW VERSION TO TEST, THIS CAN BE OPTIMIZED (reducing the number of times sin and cos are calculated)
	if ( num_bumpers != 0 )
	{
		for ( bumper = 0 ; bumper < num_bumpers ; bumper++ )
		{
			girado[bumper] = GiraPunto( centro_giro , inicial[bumper].centro, angulo );
		}
	}
}

void GiraZonasAcelCirc( struct zona_aceleracion_circular* inicial, struct punto* girado, struct punto centro_giro, int num_zonas_acel, double angulo )	// Rotate round acceleration areas
{
	int zona_acel;
	// (TODO)VERSION LENTA PARA PROBAR, SE PUEDE OPIMIZAR (reduciendo numero de veces que se calcula sin y cos)
	// SLOW VERSION TO TEST, THIS CAN BE OPTIMIZED (reducing the number of times sin and cos are calculated)
	if ( num_zonas_acel != 0 )
	{
		for ( zona_acel = 0 ; zona_acel < num_zonas_acel ; zona_acel++ )
		{
			girado[zona_acel] = GiraPunto( centro_giro , inicial[zona_acel].centro, angulo );
		}
	}
}

void CopiaBumpersSinGiro( struct pinball_bumper* origen, struct punto* destino, int num_bumpers )	// Copy bumpers without rotating
{
	// Para el caso sin rotación, simplemente copia los puntos sin aplicar un giro
	// For cases without rotation, copies points without rotating
	int bumper;
	if ( num_bumpers != 0 )
	{
		for ( bumper = 0 ; bumper < num_bumpers ; bumper++ )
		{
			destino[bumper] = origen[bumper].centro;
		}
	}
}

void CopiaZonasAcelCircSinGiro( struct zona_aceleracion_circular* origen, struct punto* destino, int num_zonas_acel )	// Copy round acceleration zones without rotating
{
	// Para el caso sin rotación, simplemente copia los puntos sin aplicar un giro
	// For cases without rotation, copies points without rotating
	int zona_acel;
	if ( num_zonas_acel != 0 )
	{
		for ( zona_acel = 0 ; zona_acel < num_zonas_acel ; zona_acel++ )
		{
			destino[zona_acel] = origen[zona_acel].centro;
		}
	}
}


void GiraEnemigosRuta( struct punto* inicial, struct punto* girado, struct punto centro_giro, int num_enemigos_ruta, double angulo )	// (17/9/2021) 
{
	int enemigo_ruta;
	// (TODO)VERSION LENTA PARA PROBAR, SE PUEDE OPIMIZAR (reduciendo numero de veces que se calcula sin y cos)
	// SLOW VERSION TO TEST, THIS CAN BE OPTIMIZED (reducing the number of times sin and cos are calculated)
	if ( num_enemigos_ruta != 0 )
	{
		for ( enemigo_ruta = 0 ; enemigo_ruta < num_enemigos_ruta ; enemigo_ruta++ )
		{
			girado[enemigo_ruta] = GiraPunto( centro_giro , inicial[enemigo_ruta], angulo );
		}
	}

}

void CopiaEnemigosRutaSinGiro( struct punto* origen, struct punto* destino, int num_enemigos_ruta )	// (17/9/2021) 
{
	// Para el caso sin rotación, simplemente copia los puntos sin aplicar un giro
	// For cases without rotation, copies points without rotating
	int enemigo_ruta;
	if ( num_enemigos_ruta != 0 )
	{
		for ( enemigo_ruta = 0 ; enemigo_ruta < num_enemigos_ruta ; enemigo_ruta++ )
		{
			destino[enemigo_ruta] = origen[enemigo_ruta];
		}
	}
}


void GiraWormholes( struct wormhole* inicial, struct wormhole* girado, struct punto centro_giro, int num_wormholes, double angulo ) // (27/9/2021)
{
	int wormhole;
	// (TODO)VERSION LENTA PARA PROBAR, SE PUEDE OPIMIZAR (reduciendo numero de veces que se calcula sin y cos)
	// SLOW VERSION TO TEST, THIS CAN BE OPTIMIZED (reducing the number of times sin and cos are calculated)
	if ( num_wormholes != 0 )
	{
		for ( wormhole = 0 ; wormhole < num_wormholes ; wormhole++ )
		{
			girado[wormhole].p1 = GiraPunto( centro_giro , inicial[wormhole].p1, angulo );
			girado[wormhole].p2 = GiraPunto( centro_giro , inicial[wormhole].p2, angulo );
		}
	}
}



void CopiaWormholesSinGiro( struct wormhole* origen, struct wormhole* destino, int num_wormholes )		// (27/9/2021)
{
	// Para el caso sin rotación, simplemente copia los puntos sin aplicar un giro
	// For cases without rotation, copies points without rotating
	int wormhole;
	if ( num_wormholes != 0 )
	{
		for ( wormhole = 0 ; wormhole < num_wormholes ; wormhole++ )
		{
			destino[wormhole].p1 = origen[wormhole].p1;
			destino[wormhole].p2 = origen[wormhole].p2;
		}
	}
}



float AnguloSegmento (struct segmento segm )	// Calculate segment angle
{
	// Nota: Criterio de signos. Angulo positivo si va de X+ a Y+
	// Note: Angle sign criteria. Angle positive if it goes from +X to Y+.
	return atan2( segm.end.y - segm.start.y , segm.end.x - segm.start.x );
}




//////////////////////////////////////////////////////////////////////////////////////////////////

double ProductoEscalar2D (struct punto A, struct punto B)	// 2D dot product
{
	// Nota función puede sufrir "cancelaciones catastróficas" --- Note: this function implementation can suffer "catastrophic cancellations"
	// En caso de problemas, usar "Kahan summation algorithm" --- In case problems appear, use "Kahan summation algorithm" instead
	// A*B = |A| |B| cos(a)
	double resultado;
	resultado = A.x * B.x + A.y * B.y;
	return resultado;
} 


double ProductoEscalarPerpendicular2D (struct punto A, struct punto B)	// 2D perpendicular dot product
{
	// Nota: no confundir con el producto escalar --- Note: do not mistake with dot product
	// A*B = |A| |B| sin(a)
	double resultado;
	resultado = A.x * B.y - A.y * B.x;
	return resultado;
}

bool ProyeccionEstaEnSegmento( struct punto P, struct punto A, struct punto B)		// Proyection Is In Segment?
{
	// Esta función indica si la proyección de P en la recta definida por AB (que es P'), está dentro
	// del segmento entre A y B.
	// This function tests if the projection of P on the line defined by A and B (this is P'), is inside
	// the segment between A and B.

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

bool PuntoEstaEnSegmento (struct punto P, struct punto A, struct punto B)	// Point is in segment?
{
	// Esta función indica si un punto P está en el segmento definido por AB.
	// This function checks if a point P is in a segment defined by AB.

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

struct punto PuntoProyectadoSobreRecta( struct punto P, struct punto A, struct punto B)		// Point projected on line
{
	// Esta función calcula las coordenadas de un punto p_prima que es la proyección de P sobre la recta
	// definida por A y B.
	// This function calculates the coordinates of a point p', that is the projection of P on the line defined by A and B.

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

double LongitudVector_Cuadrado( struct punto v, struct punto w)		// Vector magnitude squared
{
	// Calcula el cuadrado del modulo de un vector
	// Calculates the sqare of the magnitude of a vector
	return (w.x-v.x)*(w.x-v.x) + (w.y-v.y)*(w.y-v.y);
}

double LongitudVector( struct punto v, struct punto w)		// Vector magnitude
{
	// Calcula el modulo de un vector
	// Es lenta, por tener una raiz cuadrada. Intentar evitar y usar LongitudVector_Cuadrado en su lugar cuando sea posible.
	// Calculates the vector magnitude
	// This is slow (doe to the square root). Try to avoid using it and use the prevoius function instead, when possible.
	return sqrt(  (w.x-v.x)*(w.x-v.x) + (w.y-v.y)*(w.y-v.y)  );
}


double DistanciaPuntoRecta_Cuadrado( struct punto P, struct punto A, struct punto B)	// Distante point from line, squared
{
	// Esta función calcula la distancia entre un punto P y una recta definida por A B.
	// This function calculates the distance between a point P and a line defined by A and B.
	struct punto P_proyectado;

	P_proyectado = PuntoProyectadoSobreRecta( P, A, B);
	return LongitudVector_Cuadrado(P, P_proyectado);
}

//--------------------------------------------------------------------------------------------------

enum tipo_interseccion_circulo_segmento  CirculoTocaSegmento( struct punto centro_circulo, double radio_circulo, struct punto A, struct punto B)	// Interference type between circle and line segment
{
	//Resultado:
	// 0 - El circulo no toca el segmento en ningun sitio.
	// 1 - El circulo toca el segmento en el extremo A.
	// 2 - El cirtulo toca el segmento en un punto central cualquiera (entre los extremos).
	// 3 - El circulo toca el segmento en el extremo B.

	// Result:
	// 0 - Circle does not touche the line segment.
	// 1 - Circle touches the line segment in A point.
	// 2 - Circle touches the segment in a central point.
	// 3 - Circle touches the segment in B point.
	if ( ProyeccionEstaEnSegmento( centro_circulo, A, B) == true )
	{
		if (DistanciaPuntoRecta_Cuadrado( centro_circulo, A, B) < (radio_circulo*radio_circulo) )
		{
			return interseccion_central;	// En este caso, toca en punto central --- In this case, it touches in a central point
		}
		else
		{
			return sin_interseccion;	// No toca --- It does not touch
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

bool CirculoTocaSegmentoExcluyendoExtremos( struct punto centro_circulo, double radio_circulo, struct punto A, struct punto B)	// Circle touches Segment Excluding Ends
{
	// Verificamos si el circulo toca el segmento, solo considerando si la proyeccion del centro esta en el segmento
	// Check if the circle touches the line segment, but only if the projection of the circle center is on the line segment
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
// The following functions are taken (and adapted) from this website:
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

struct punto CalculaPosTangenteACentroSegmento ( struct punto A, struct punto B, struct punto centro_circulo, double radio)	// Calculate Position Tangent To Segment Center
{ 
	// Calcula la posición tangente al centro del segmento
	// Calculates position tangent to line segment center

	struct punto nuevo_centro_tangente, proyeccion_centro_sobre_recta;
	double distancia_centro_recta;

	proyeccion_centro_sobre_recta = PuntoProyectadoSobreRecta( centro_circulo, A, B);
	distancia_centro_recta = sqrt( DistanciaPuntoRecta_Cuadrado( centro_circulo, A, B) );
	
	nuevo_centro_tangente.x = proyeccion_centro_sobre_recta.x + ((centro_circulo.x - proyeccion_centro_sobre_recta.x )/distancia_centro_recta)* radio;
	nuevo_centro_tangente.y = proyeccion_centro_sobre_recta.y + ((centro_circulo.y - proyeccion_centro_sobre_recta.y )/distancia_centro_recta)* radio;

	return nuevo_centro_tangente;
}


struct punto CalculaPosTangenteAExtremoSegmento ( struct punto extremo, struct punto centro_circulo, double radio)	// Calculate Posiiton Tangent To Segment End
{ 
	// Calcula la posición tangente al extremo del segmento
	// Calculate tangent position to segment end

	struct punto nuevo_centro_tangente;
	double distancia_centrocirculo_extremosegmento;

	distancia_centrocirculo_extremosegmento = LongitudVector( extremo, centro_circulo);
	
	nuevo_centro_tangente.x = extremo.x + ((centro_circulo.x - extremo.x )/distancia_centrocirculo_extremosegmento)* radio;
	nuevo_centro_tangente.y = extremo.y + ((centro_circulo.y - extremo.y )/distancia_centrocirculo_extremosegmento)* radio;

	return nuevo_centro_tangente;
}


//--------------------------------------------------------------------------------------------

double WrapAngle_0_2pi (double angulo)
{
	// Entrada en radianes, salida en radianes entre 0 y 2*pi
	// Input in radians, output in radians betewwn 0 and 2*pi
	angulo = fmod(angulo,2*PI);
	if (angulo < 0)
        	angulo += 2*PI;
    	return angulo;
}

double WrapAngle_0_360 (double angulo)
{
	// Entrada en grados, salida en grados entre 0 y 360
	// Input in degrees, output in degrees between 0 and 360.
	angulo = fmod(angulo,360);
	if (angulo < 0)
        	angulo += 360;
    	return angulo;
}

double WrapAngle_mpi_pi ( double angulo)
{
	angulo = fmod(angulo + 180,360);
	if (angulo < 0)
		angulo += 360;
	return angulo - 180;
}

double WrapAngle_m180_180 (double angulo)
{
	angulo = fmod(angulo + 180,360);
	if (angulo < 0)
		angulo += 360;
	return angulo - 180;
}


///////////////////////////////////////////////
// Nuevo 5/2/2020
float AnguloRecta (struct punto p, struct punto q )		// Line angle
{
	// Similar a "AnguloSegmento", pero la recta está definida por dos puntos
	// Nota: Criterio de signos. Angulo positivo si va de X+ a Y+
	// Nota: (TODO) Pendiente de probar.

	// Similar to "AnguloSegmento" function, but the line is defined with two points.
	// Note: Sign criteria. Angle is positive if goes from X+ to Y+
	return atan2( q.y - p.y , q.x - p.x );
}

// Nuevo 9/2/2020
struct punto CalculaPosTangenteCirculoCirculo ( struct punto centro_circulo_A, struct punto centro_circulo_B, double radio_A, double radio_B)	// Calculate Position Tangent Circle to Circle
{ 
	// Esta función calcula la posición del centro del circulo A para que sea tangente al circulo B (siguiendo la linea recta que marcan actualmente).
	// Se considera que el circulo B es fijo, y estamos buscando la posición tangente al circulo B.

	// This function calculates the circle A center's position that makes circles A and B be tangent (following the line that currently joins their centers).
	// We suppose circle B is fixed, and we are looking for the position tangent to circle B.

	struct punto nuevo_centro_circulo_B;
	double distancia_centros;

	distancia_centros = LongitudVector( centro_circulo_A, centro_circulo_B);
	
	nuevo_centro_circulo_B.x = centro_circulo_B.x + ((centro_circulo_A.x - centro_circulo_B.x )/distancia_centros)* (radio_A + radio_B);
	nuevo_centro_circulo_B.y = centro_circulo_B.y + ((centro_circulo_A.y - centro_circulo_B.y )/distancia_centros)* (radio_A + radio_B);

	return nuevo_centro_circulo_B;
}

//////////////////////////////////////////////////////////////////////
// Nuevo 23/2/2020
// Los siguientes algoritmos están extraídos de la página:
// The following algorithms have been taken from this website:
// https://blackpawn.com/texts/pointinpoly/default.html

double ProductoVectorial2D_ComponenteZ( struct punto A, struct punto B )	// Z component of the cross product of two 2D vectors
{
	// Esta funcion realiza el producto vectorial de dos vectores 2D, y solo devuelve la componente Z
	// This function computes the cross product of two 2D vectors, and return its Z component
	return (A.x*B.y - A.y*B.x);
}

bool MismoLado(struct punto P1, struct punto P2, struct punto A, struct punto B)	// Same Side
{
	// Esta funcion indica si los puntos P1 y P2 están al mismo lado (o no) de la linea definida por los puntos A y B
	// This function checks if points P1 and P2 are on the same side (or not) of the line defined between points A and B.
	float prod_vect_1, prod_vect_2;
	struct punto B_A =  { B.x - A.x  , B.y - A.y  };
	struct punto P1_A = { P1.x - A.x , P1.y - A.y };
	struct punto P2_A = { P2.x - A.x , P2.y - A.y };

	prod_vect_1 = ProductoVectorial2D_ComponenteZ(B_A, P1_A);
	prod_vect_2 = ProductoVectorial2D_ComponenteZ(B_A, P2_A);

	if ( prod_vect_1 * prod_vect_2 >= 0 )	// Si prod_vect_1 y prod_vect_2 tienen el mismo signo.... --- If prod_vect_1 and prod_vect_2 have the same sign...
	{
		return true;
	}
	else // Si tienen signos distintos... --- If they have different signs
	{
		return false;
	}
}

bool PuntoDentroDeTriangulo(struct punto P, struct punto A, struct punto B,struct punto C)	// Point inside triangle
{
	// Verificar si un punto P está dentro de un triangulo definido por los vértices A,B,C
	// Check if a point P is inside a triangle defined through its vertices A,B,C
	if ( MismoLado(P,A, B,C) && MismoLado(P,B, A,C) && MismoLado(P,C, A,B) )
	{ 
		return true;
	}
	else
	{
		return false;
	}
}

double AnguloRadEntreDosSegmentos ( struct segmento segm_A, struct segmento segm_B )	// Angle (in radians) between two segments
{
	// Esta función devuelve el angulo (en radianes) que forman dos segmentos
	// Nuevo 18/4/2020
	// Basado en la formula: Prod_escalar(A,B) = |A|*|B|*cos(angulo)
	// prod_esc(A,B)*prod_esc(A,B) = |A|*|A|*|B|*|B|*cos(angulo)*cos(angulo)
	// cos(angulo) = raiz( prod_esc(A,B)*prod_esc(A,B)/|A|*|A|*|B|*|B|)
	struct punto vector_A, vector_B;
	struct punto origen = {0.0, 0.0};
	double prod_escalar, coseno_angulo;

	vector_A.x = segm_A.end.x - segm_A.start.x;	vector_A.y = segm_A.end.y - segm_A.start.y;
	vector_B.x = segm_B.end.x - segm_B.start.x;	vector_B.y = segm_B.end.y - segm_B.start.y;

	prod_escalar = ProductoEscalar2D( vector_A, vector_B );

	coseno_angulo = sqrt( ( prod_escalar*prod_escalar) / ( LongitudVector_Cuadrado( vector_A , origen ) * LongitudVector_Cuadrado( vector_B, origen ) ) );

	return acos( coseno_angulo );
}




