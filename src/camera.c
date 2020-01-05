#include "physics.h"
#include "maps.h"
#include "camera.h"

#include <stdio.h>
#include <stdlib.h>


//void CalculaCamara(	double pos_real_moneda_x, int* pos_pant_moneda_x,
//			double pos_real_moneda_y, int* pos_pant_moneda_y )
//{
//	//DE MOMENTO ESTO, FALTA IMPLEMENTAR LOS CALCULOS DE LA CAMARA
//	*pos_pant_moneda_x = (int)pos_real_moneda_x;
//	*pos_pant_moneda_y = (int)pos_real_moneda_y;
//
//}


struct posicion_camara CalculaCamara( struct punto pos_real )
{
	struct posicion_camara pos_cam;
	//DE MOMENTO ESTO, FALTA IMPLEMENTAR LOS CALCULOS DE LA CAMARA
	pos_cam.x = (int)pos_real.x;
	pos_cam.y = (int)pos_real.y;
	return pos_cam;
}


struct punto CalculaCamara2( 		enum modo_camara modo, int resolucion_x_pantalla, int resolucion_y_pantalla,
					int desplazamiento_manual_usuario_x, int desplazamiento_manual_usuario_y,
					struct punto pos_real_moneda, 
					int num_segmentos, 
					struct segmento* segmentos_pos_real,  struct segmento* segmentos_pos_pantalla,
					int pos_real_fondo_giratorio_izquierda, int pos_real_fondo_giratorio_arriba, int pos_real_fondo_giratorio_derecha, int pos_real_fondo_giratorio_abajo, 
					int* pos_cam_fondo_giratorio_izquierda, int* pos_cam_fondo_giratorio_arriba, int* pos_cam_fondo_giratorio_derecha, int* pos_cam_fondo_giratorio_abajo     ) 
{
	// Nota: 	posición real --> posición en coordenadas del juego, antes de calcular la posición en la pantala
	// 		posición en pantalla --> coordenadas de la pantalla, tras calcular la posición
	struct punto posicion_moneda_en_pantalla;
	int desplazamiento_x, desplazamiento_y;		// Offset x, Offset y
	int segm;

	// Calculamos el desplazamiento
	switch ( modo )
	{
		case camara_fija_en_origen:
			desplazamiento_x = 0;
			desplazamiento_y = 0;
			break;
		case camara_sigue_moneda:
			// La cámara estará posicionada en el centro de la pantalla
			desplazamiento_x = pos_real_moneda.x - (resolucion_x_pantalla/2);
			desplazamiento_y = pos_real_moneda.y - (resolucion_y_pantalla/2);
			break;
		case camara_movil_por_usuario:
			desplazamiento_x = desplazamiento_manual_usuario_x;
			desplazamiento_y = desplazamiento_manual_usuario_y;
			break;
		default:
			printf("En función CalculaCamara, error de programación, valor de modo no válido. Cerrando programa.\n");
			exit( -1 );
			break;
	}
	
	// Calculamos la posición de la moneda
	posicion_moneda_en_pantalla.x = (int) pos_real_moneda.x - desplazamiento_x;
	posicion_moneda_en_pantalla.y = (int) pos_real_moneda.y - desplazamiento_y;
	// Calculamos la posición de los segmentos en la pantalla
	for ( segm = 0 ; segm < num_segmentos ; segm++ )
	{
		segmentos_pos_pantalla[segm].start.x = segmentos_pos_real[segm].start.x - desplazamiento_x;
		segmentos_pos_pantalla[segm].start.y = segmentos_pos_real[segm].start.y - desplazamiento_y;
		segmentos_pos_pantalla[segm].end.x = segmentos_pos_real[segm].end.x - desplazamiento_x;
		segmentos_pos_pantalla[segm].end.y = segmentos_pos_real[segm].end.y - desplazamiento_y;		
	}
	// Calculamos la posición del fondo giratorio en la pantalla
	*pos_cam_fondo_giratorio_izquierda = pos_real_fondo_giratorio_izquierda - desplazamiento_x;
	*pos_cam_fondo_giratorio_arriba = pos_real_fondo_giratorio_arriba - desplazamiento_y;
	*pos_cam_fondo_giratorio_derecha = pos_real_fondo_giratorio_derecha - desplazamiento_x;
	*pos_cam_fondo_giratorio_abajo = pos_real_fondo_giratorio_abajo - desplazamiento_y;

	return posicion_moneda_en_pantalla;
}





