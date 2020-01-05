//#include "physics.h"
//#include "maps.h"


struct posicion_camara
{
	int x;
	int y;
};


enum modo_camara
{
	camara_fija_en_origen,		// Camera fixed in initial position
	camara_sigue_moneda,		// Camera following the coin
	camara_movil_por_usuario	// Camera can be moved by the user
};


//void CalculaCamara(double , int*, double , int*  );

struct posicion_camara CalculaCamara( struct punto );

struct punto CalculaCamara2( enum modo_camara, int, int, int, int, struct punto, int, struct segmento*,  struct segmento*, int, int, int, int, int*, int*, int*, int* );

