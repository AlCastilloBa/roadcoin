
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "maps.h"


#define DEBUG_INFO

struct mapa CargarMapaDesdeArchivo( char *nombre_archivo )
{
	struct mapa mapa_a_cargar;
	char linea_leida[200];
	bool nombre_ok, num_segmentos_ok, modo_giro_mapa_ok, punto_giro_ok, angulo_max_ok, pos_inicial_ok, gravedad_ok;
	bool segmento_ok[LIMITE_SEGMENTOS];
	bool vector_segmentos_inicializado =false;
	FILE *archivo;
	int linea=0;
	int segmento_actual, i;

	archivo = fopen( nombre_archivo, "r" );
	if (archivo == NULL)
	{
		printf("Error: No se puede abrir el archivo %s \n", nombre_archivo );
		exit(-1);
	}


	while ( (fgets(linea_leida, 200, archivo)) != NULL )
	{
		linea++;
		#ifdef DEBUG_INFO
		printf("Linea %d leida\n", linea );
		#endif
		if ( (linea_leida[0] == '#') || (linea_leida[0] == '\n') || (strlen(linea_leida)==0) )	//lo de ver si está vacia no funciona
		{
			//Es un comentario, no hacer nada
			#ifdef DEBUG_INFO
			printf("Linea %d es un comentario\n", linea );
			#endif
		}
		else
		{
			if (strstr(linea_leida, "nombre") != NULL )
			{
				sscanf(linea_leida, "nombre=%s", mapa_a_cargar.NombreMapa);
				nombre_ok = true;
				#ifdef DEBUG_INFO
				printf("Linea %d, Nombre mapa = %s\n", linea, mapa_a_cargar.NombreMapa);
				#endif
			}
			else if (strstr(linea_leida, "num_segmentos") != NULL )
			{
				sscanf(linea_leida, "num_segmentos=%d", &(mapa_a_cargar.NumeroSegmentos));
				num_segmentos_ok = true;
				#ifdef DEBUG_INFO
				printf("Linea %d, Numero segmentos = %d\n", linea, mapa_a_cargar.NumeroSegmentos);
				#endif
				// Reserva el vector de segmentos
				if ( vector_segmentos_inicializado ==  true )
				{
					printf( "Error: ¿Dos declaraciones de numero de segmentos?\n");
					exit(-1);
				}
				else
				{
					mapa_a_cargar.Mapa = calloc(mapa_a_cargar.NumeroSegmentos, sizeof(struct segmento));
					vector_segmentos_inicializado = true;
					#ifdef DEBUG_INFO
					printf("Memoria reservada para %d segmentos\n", mapa_a_cargar.NumeroSegmentos);
					#endif
				}
			}
			else if (strstr(linea_leida, "modo_giro_mapa") != NULL )
			{
				sscanf(linea_leida, "modo_giro_mapa=%d", &(mapa_a_cargar.TipoGiro));
				modo_giro_mapa_ok = true;
				#ifdef DEBUG_INFO
				printf("Linea %d, Modo Giro Mapa = %d\n", linea, mapa_a_cargar.TipoGiro);
				#endif
			}
			else if (strstr(linea_leida, "punto_giro") != NULL )
			{
				if ( sscanf(linea_leida, "punto_giro=(%lf,%lf)", &(mapa_a_cargar.PuntoGiroFijo.x), &(mapa_a_cargar.PuntoGiroFijo.y) ) == 2 )
				{
					punto_giro_ok = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, punto_giro --> x=%f, y=%f \n", linea, mapa_a_cargar.PuntoGiroFijo.x, mapa_a_cargar.PuntoGiroFijo.y );
					#endif
				}
				else
				{	
					printf("Linea %d, punto_giro --> No se han podido leer todos los valores.\n", linea);
					exit(-1);
				}
			}
			else if (strstr(linea_leida, "angulo_max") != NULL )
			{
				if ( sscanf(linea_leida, "angulo_max=%f", &(mapa_a_cargar.AnguloMax) ) == 1 )
				{
					angulo_max_ok = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, angulo_max=%f\n", linea, mapa_a_cargar.AnguloMax );
					#endif
				}
				else
				{	
					printf("Linea %d, angulo_max --> No se han podido leer el valor.\n", linea);
					exit(-1);
				}
			}
			else if (strstr(linea_leida, "pos_inicial_moneda") != NULL )
			{
				if ( sscanf(linea_leida, "pos_inicial_moneda=(%lf,%lf)", &(mapa_a_cargar.PuntoInicialMoneda.x), &(mapa_a_cargar.PuntoInicialMoneda.y) ) == 2 )
				{
					pos_inicial_ok = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, pos_inicial_moneda --> x=%f, y=%f \n", linea, mapa_a_cargar.PuntoInicialMoneda.x, mapa_a_cargar.PuntoInicialMoneda.y );
					#endif
				}
				else
				{	
					printf("Linea %d, pos_inicial_moneda --> No se han podido leer todos los valores.\n", linea);
					exit(-1);
				}
			}
			else if (strstr(linea_leida, "gravedad") != NULL )
			{
				if ( sscanf(linea_leida, "gravedad=%f", &(mapa_a_cargar.Gravedad) ) == 1 )
				{
					gravedad_ok = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, gravedad=%f\n", linea, mapa_a_cargar.Gravedad );
					#endif
				}
				else
				{	
					printf("Linea %d, gravedad --> No se han podido leer el valor.\n", linea);
					exit(-1);
				}
			}
			else if (strstr(linea_leida, "segmento") != NULL )
			{
				if (vector_segmentos_inicializado==false)
				{
					printf("La declaracion de los segmentos debe ir después de especificar el numero de segmentos\n");
					exit(-1);
				}
				else
				{
					sscanf(linea_leida, "segmento[%d]",&segmento_actual); // Leemos primero el segmento actual, para poder hacer el direccionamiento en las siguientes instrucciones
					if ( sscanf(linea_leida, "segmento[%d]=((%lf,%lf),(%lf,%lf),%d)", 	&segmento_actual, 
														&(mapa_a_cargar.Mapa[segmento_actual].start.x),
														&(mapa_a_cargar.Mapa[segmento_actual].start.y),
														&(mapa_a_cargar.Mapa[segmento_actual].end.x),
														&(mapa_a_cargar.Mapa[segmento_actual].end.y),
														&(mapa_a_cargar.Mapa[segmento_actual].type)            ) == 6 )
					{
						segmento_ok[segmento_actual] = true;
						#ifdef DEBUG_INFO
						printf("Linea %d, segmento %d --> Inicio x=%f, y=%f; Fin x=%f, y=%f; tipo=%d \n", 	linea, 
																	segmento_actual,
																	mapa_a_cargar.Mapa[segmento_actual].start.x,
																	mapa_a_cargar.Mapa[segmento_actual].start.y,
																	mapa_a_cargar.Mapa[segmento_actual].end.x,
																	mapa_a_cargar.Mapa[segmento_actual].end.y,
																	mapa_a_cargar.Mapa[segmento_actual].type );

						#endif
					}
					else
					{	
						printf("Linea %d, segmento --> No se han podido leer todos los valores.\n", linea);
						exit(-1);
					}


				}
			}
			else
			{
				printf("Linea %d, expresion no reconocida, se ignora.\n", linea);
			}

		}

	}

	fclose(archivo);


	// Verificamos que lo tengamos todo (que al archivo no le faltasen datos)
	if (nombre_ok == false) 		printf("Falta el nombre del mapa.\n");
	if (num_segmentos_ok == false) 		printf("Falta el número de segmentos.\n");	
	if (modo_giro_mapa_ok == false) 	printf("Falta el modo de giro.\n");
	if (mapa_a_cargar.TipoGiro == punto_fijo &&  punto_giro_ok == false)
					 	printf("Falta el punto de giro.\n");	
	if (angulo_max_ok == false) 		printf("Falta el angulo máximo.\n");
	if (pos_inicial_ok == false) 		printf("Falta el la posición inicial.\n");
	if (gravedad_ok == false) 		printf("Falta la gravedad.\n");

	if (nombre_ok==false || num_segmentos_ok==false || modo_giro_mapa_ok==false || punto_giro_ok==false || angulo_max_ok==false || pos_inicial_ok==false || gravedad_ok==false)
	{
		printf("Archivo de mapa incompleto\n");
		exit(-1);
	}
	for (i=0; i<mapa_a_cargar.NumeroSegmentos; i++)
	{
		if (segmento_ok[i]=false)
		{
			printf("Segmento %d no definido\n");
			exit(-1);
		}
	}
	
	#ifdef DEBUG_INFO
	printf("Mapa %s cargado. Continuamos.\n", nombre_archivo );
	#endif
	return mapa_a_cargar;
}
