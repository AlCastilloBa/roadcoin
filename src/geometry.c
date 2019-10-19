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
	// VERSION LENTA PARA PROBAR, SE DEBE OPIMIZAR
	for ( segm = 0 ; segm < num_segmentos ; segm++ )
	{
		girado[segm].start = GiraPunto( centro_giro , inicial[segm].start, angulo );
		girado[segm].end   = GiraPunto( centro_giro , inicial[segm].end  , angulo );
	}

}
