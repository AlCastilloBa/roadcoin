#include "physics.h"
#include "camera.h"



//void CalculaCamara(	double pos_real_moneda_x, int* pos_pant_moneda_x,
//			double pos_real_moneda_y, int* pos_pant_moneda_y )
//{
//	//DE MOMENTO ESTO, FALTA IMPLEMENTAR LOS CALCULOS DE LA CAMARA
//	*pos_pant_moneda_x = (int)pos_real_moneda_x;
//	*pos_pant_moneda_y = (int)pos_real_moneda_y;
//
//}

struct posicion_camara CalculaCamara( struct posicion pos_real )
{
	struct posicion_camara pos_cam;
	//DE MOMENTO ESTO, FALTA IMPLEMENTAR LOS CALCULOS DE LA CAMARA
	pos_cam.x = (int)pos_real.x;
	pos_cam.y = (int)pos_real.y;
	return pos_cam;
}
