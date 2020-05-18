#include "physics.h"
#include "maps.h"
#include "camera.h"

#include <stdio.h>
#include <stdlib.h>



struct punto CalculaCamara( 		enum modo_camara modo, int resolucion_x_pantalla, int resolucion_y_pantalla,
					int desplazamiento_manual_usuario_x, int desplazamiento_manual_usuario_y,
					struct punto pos_real_moneda, 
					int num_segmentos, 
					struct segmento* segmentos_pos_real,  struct segmento* segmentos_pos_pantalla,
					int pos_real_fondo_giratorio_izquierda, int pos_real_fondo_giratorio_arriba, int pos_real_fondo_giratorio_derecha, int pos_real_fondo_giratorio_abajo, 
					int* pos_cam_fondo_giratorio_izquierda, int* pos_cam_fondo_giratorio_arriba, int* pos_cam_fondo_giratorio_derecha, int* pos_cam_fondo_giratorio_abajo,
					int num_pinball_bumpers,
					struct punto* pinball_bumpers_pos_real, struct punto* pinball_bumpers_pos_pantalla,
					int num_zonas_aceleracion_circular,
					struct punto* zonas_acel_circ_pos_real, struct punto* zonas_acel_circ_pos_pantalla  ) 
{
	// Nota: 	posición real --> posición en coordenadas del juego, antes de calcular la posición en la pantala
	// 		posición en pantalla --> coordenadas de la pantalla, tras calcular la posición

	// Note:	real position --> position in game coordinates, before calculating position on screen
	//		position on screen --> screen coordinates, after calculating position
	struct punto posicion_moneda_en_pantalla;
	int desplazamiento_x, desplazamiento_y;		// Offset x, Offset y
	int segm;
	int bumper;
	int zona_acel_circ;

	// Calculamos el desplazamiento --- Calculate offset
	switch ( modo )
	{
		case camara_fija_en_origen:
			desplazamiento_x = 0;
			desplazamiento_y = 0;
			break;
		case camara_sigue_moneda:
			// La cámara estará posicionada en el centro de la pantalla --- Camera will be positione on screen center
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
	
	// Calculamos la posición de la moneda en la pantalla --- Calculate coin position on screen
	posicion_moneda_en_pantalla.x = (int) pos_real_moneda.x - desplazamiento_x;
	posicion_moneda_en_pantalla.y = (int) pos_real_moneda.y - desplazamiento_y;
	// Calculamos la posición de los segmentos en la pantalla --- Calculate line segments position on screen
	for ( segm = 0 ; segm < num_segmentos ; segm++ )
	{
		segmentos_pos_pantalla[segm].start.x = segmentos_pos_real[segm].start.x - desplazamiento_x;
		segmentos_pos_pantalla[segm].start.y = segmentos_pos_real[segm].start.y - desplazamiento_y;
		segmentos_pos_pantalla[segm].end.x = segmentos_pos_real[segm].end.x - desplazamiento_x;
		segmentos_pos_pantalla[segm].end.y = segmentos_pos_real[segm].end.y - desplazamiento_y;		
	}
	// Calculamos la posición del fondo giratorio en la pantalla --- Calculate rotating background position on screen
	*pos_cam_fondo_giratorio_izquierda = pos_real_fondo_giratorio_izquierda - desplazamiento_x;
	*pos_cam_fondo_giratorio_arriba = pos_real_fondo_giratorio_arriba - desplazamiento_y;
	*pos_cam_fondo_giratorio_derecha = pos_real_fondo_giratorio_derecha - desplazamiento_x;
	*pos_cam_fondo_giratorio_abajo = pos_real_fondo_giratorio_abajo - desplazamiento_y;
	// Calculamos la posicion de los pinball bumpers en la pantalla --- Calculating pinball bumpers positions on screen
	for ( bumper = 0 ; bumper < num_pinball_bumpers ; bumper ++ )
	{
		pinball_bumpers_pos_pantalla[bumper].x = pinball_bumpers_pos_real[bumper].x - desplazamiento_x;
		pinball_bumpers_pos_pantalla[bumper].y = pinball_bumpers_pos_real[bumper].y - desplazamiento_y;
	}
	// Calculamos la posición de las zonas circulares de aceleración --- Calculating position of round acceleration zones on screen
	for ( zona_acel_circ = 0 ; zona_acel_circ < num_zonas_aceleracion_circular ; zona_acel_circ ++ )
	{
		zonas_acel_circ_pos_pantalla[zona_acel_circ].x = zonas_acel_circ_pos_real[zona_acel_circ].x - desplazamiento_x;
		zonas_acel_circ_pos_pantalla[zona_acel_circ].y = zonas_acel_circ_pos_real[zona_acel_circ].y - desplazamiento_y;
	}


	return posicion_moneda_en_pantalla;
}





