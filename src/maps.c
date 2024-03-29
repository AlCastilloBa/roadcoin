
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "maps.h"
#include "enemies.h"
#include "geometry.h"



// #define DEBUG_INFO

struct mapa CargarMapaDesdeArchivo( char *nombre_archivo )	// load Map from file
{
	struct mapa mapa_a_cargar;
	struct punto punto_auxiliar;
	char linea_leida[200];
	bool nombre_ok=false, descripcion_ok=false, num_segmentos_ok=false, modo_giro_mapa_ok=false, punto_giro_ok=false, angulo_max_ok=false, pos_inicial_ok=false, gravedad_ok=false, imagen_moneda_ok=false, imagen_fondo_ok=false; 
	bool ruta_musica_ok=false, angulo_flippers_ok=false;
	bool escala_presente=false, num_pinball_bumpers_presente=false, cuenta_atras_presente=false, num_zonas_acel_circ_presente=false, no_rot_moneda_presente=false;
	float escala;
	bool fondo_giratorio_ok=false, imagen_fondo_giratorio_ok=false, pos_fondo_giratorio_ok=false, centro_giro_fondo_giratorio_ok=false ;
	bool imagen_segm_pared_presente=false, imagen_segm_meta_presente=false, imagen_segm_muerte_presente=false;		/* (TODO) PRUEBAS 17/5/2020 */
	//bool segmento_ok[LIMITE_SEGMENTOS];
	//bool bumper_ok[LIMITE_BUMPERS];
	//bool zona_acel_circ_ok[LIMITE_ZONAS_ACEL_CIRC];
	bool vector_segmentos_inicializado =false, vector_bumpers_inicializado=false, vector_zonas_acel_circ_inicializado=false;
	FILE *archivo;
	int linea=0;
	int segmento_actual, i, otro_segmento_actual, bumper_actual, zona_acel_circ_actual;

	// Nuevas variables para enemigos (11/9/2021) --- New variables for enemies
	bool num_enemigos_ruta_presente=false;
	bool vector_enemigos_ruta_inicializado=false;
	int enemigo_ruta_actual, waypoint_actual, j;

	// Nuevas variables para wormholes (26/9/2021) --- New variables for wormholes
	bool num_wormholes_presente=false;
	bool vector_wormholes_inicializado=false;
	int wormhole_actual;

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
			//Es un comentario, no hacer nada --- This is a remark, nothing has to be done
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
			if (strstr(linea_leida, "descripcion") != NULL )
			{
				char cadena_aux[200]; char* token;
				//sscanf(linea_leida, "descripcion=\"%s\"", mapa_a_cargar.DescripcionMapa);
				strcpy( cadena_aux, linea_leida);
				token = strtok( cadena_aux, "=" ); token = strtok( NULL, "=" );
				strcpy( mapa_a_cargar.DescripcionMapa, token);
				Eliminar_NewLine_En_FinalCadena(mapa_a_cargar.DescripcionMapa ); 	// Nuevo 19/4/2020
				descripcion_ok = true;
				#ifdef DEBUG_INFO
				printf("Linea %d, descripcion = %s\n", linea, mapa_a_cargar.DescripcionMapa);
				#endif
			}
			else if (strstr(linea_leida, "num_segmentos") != NULL )
			{
				sscanf(linea_leida, "num_segmentos=%d", &(mapa_a_cargar.NumeroSegmentos));
				num_segmentos_ok = true;
				#ifdef DEBUG_INFO
				printf("Linea %d, Numero segmentos = %d\n", linea, mapa_a_cargar.NumeroSegmentos);
				#endif
				// Reserva el vector de segmentos --- Allocate memory for array of segments
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
					// Inicializamos todos los segmento OK a false --- Initialize al "segments OK" to false
					for (i=0; i<mapa_a_cargar.NumeroSegmentos; i++)
					{
						//segmento_ok[i]=false; (TODO) BORRAR
						mapa_a_cargar.Mapa[i].definido_OK = false;	// Nuevo 17/5/2020 (TODO)
					}
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
			else if (strstr(linea_leida, "imagen_moneda") != NULL )
			{
				if ( sscanf(linea_leida, "imagen_moneda=%s", mapa_a_cargar.RutaImagenMoneda ) == 1 )
				{
					imagen_moneda_ok = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, imagen_moneda=%s\n", linea, mapa_a_cargar.RutaImagenMoneda );
					#endif
				}
				else
				{	
					printf("Linea %d, imagen_moneda --> No se han podido leer el valor.\n", linea);
					exit(-1);
				}
			}
			else if (strstr(linea_leida, "imagen_fondo") != NULL )
			{
				if ( sscanf(linea_leida, "imagen_fondo=%s", mapa_a_cargar.RutaImagenFondo ) == 1 )
				{
					imagen_fondo_ok = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, imagen_fondo=%s\n", linea, mapa_a_cargar.RutaImagenFondo );
					#endif
				}
				else
				{	
					printf("Linea %d, imagen_fondo --> No se han podido leer el valor.\n", linea);
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
					int temp_segmento_invisible;
					sscanf(linea_leida, "segmento[%d]",&segmento_actual); // Leemos primero el segmento actual, para poder hacer el direccionamiento en las siguientes instrucciones --- Read current segment first, in order to make the correct addressing on the following lines.
					if ( segmento_actual >= mapa_a_cargar.NumeroSegmentos )
					{
						printf("Se ha declarado un segmento con numero %d, que no cabe en el numero de segmentos declarado %d.\n", segmento_actual, mapa_a_cargar.NumeroSegmentos );
						printf("Nota: se empieza a contar desde el indice 0\n");
						exit(-1);
					}
					if ( sscanf(linea_leida, "segmento[%d]=((%lf,%lf),(%lf,%lf),%d, %d)", 	&segmento_actual, 
														&(mapa_a_cargar.Mapa[segmento_actual].start.x),
														&(mapa_a_cargar.Mapa[segmento_actual].start.y),
														&(mapa_a_cargar.Mapa[segmento_actual].end.x),
														&(mapa_a_cargar.Mapa[segmento_actual].end.y),
														&(mapa_a_cargar.Mapa[segmento_actual].type),            
														&(temp_segmento_invisible)					) == 7 )
					{
						//segmento_ok[segmento_actual] = true; (TODO) BORRAR
						mapa_a_cargar.Mapa[segmento_actual].definido_OK = true;	// Nuevo 17/5/2020 (TODO)
						#ifdef DEBUG_INFO
						printf("Linea %d, segmento %d --> Inicio x=%f, y=%f; Fin x=%f, y=%f; tipo=%d; invisible=%d \n", 	linea, 
																	segmento_actual,
																	mapa_a_cargar.Mapa[segmento_actual].start.x,
																	mapa_a_cargar.Mapa[segmento_actual].start.y,
																	mapa_a_cargar.Mapa[segmento_actual].end.x,
																	mapa_a_cargar.Mapa[segmento_actual].end.y,
																	mapa_a_cargar.Mapa[segmento_actual].type,
																	temp_segmento_invisible		 );

						#endif
						switch (temp_segmento_invisible)
						{
							case 0:
								mapa_a_cargar.Mapa[segmento_actual].invisible = false;
								#ifdef DEBUG_INFO
								printf("Linea %d, segmento invisible=false \n", linea);
								#endif
								break;
							case 1:
								mapa_a_cargar.Mapa[segmento_actual].invisible = true;
								#ifdef DEBUG_INFO
								printf("Linea %d, segmento invisible=true \n", linea);
								#endif
								break;
							default:
								// Valor no válido, se toma el valor por defecto --- Invalid value, using default value.
								mapa_a_cargar.Mapa[segmento_actual].invisible = false;
								#ifdef DEBUG_INFO
								printf("Linea %d, dato invisible tiene un valor no valido, se supone visible. \n", linea);
								#endif
								break;
						}

					}
					else
					{	
						printf("Linea %d, segmento --> No se han podido leer todos los valores.\n", linea);
						exit(-1);
					}


				}
			}
			else if (strstr(linea_leida, "fondo_giratorio") != NULL )
			{
				int dato_leido;
				if ( sscanf(linea_leida, "fondo_giratorio=%d", &dato_leido ) == 1 )
				{
					switch (dato_leido)
					{
						case 0:
							mapa_a_cargar.HayFondoGiratorio = false;
							fondo_giratorio_ok = true;
							#ifdef DEBUG_INFO
							printf("Linea %d, fondo_giratorio=false \n", linea);
							#endif
							break;
						case 1:
							mapa_a_cargar.HayFondoGiratorio = true;
							fondo_giratorio_ok = true;
							#ifdef DEBUG_INFO
							printf("Linea %d, fondo_giratorio=true \n", linea);
							#endif
							break;
						default:
							// Valor no válido, se toma el valor por defecto --- Invalid value, using default value.
							fondo_giratorio_ok = false;
							#ifdef DEBUG_INFO
							printf("Linea %d, fondo_giratorio tiene un valor no permitido. \n", linea);
							#endif
							break;
					}
				}
			}
			else if (strstr(linea_leida, "imagen_fnd_giratorio") != NULL )
			{
				if ( sscanf(linea_leida, "imagen_fnd_giratorio=%s", mapa_a_cargar.RutaImagenFondoGiratorio ) == 1 )
				{
					imagen_fondo_giratorio_ok = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, imagen_fnd_giratorio=%s\n", linea, mapa_a_cargar.RutaImagenFondoGiratorio );
					#endif
				}
				else
				{	
					printf("Linea %d, imagen_fnd_giratorio --> No se han podido leer el valor.\n", linea);
					exit(-1);
				}
			}
			else if (strstr(linea_leida, "pos_fnd_giratorio") != NULL )
			{	
				if ( sscanf(linea_leida, "pos_fnd_giratorio=((%d,%d),(%d,%d))", 	&(mapa_a_cargar.Pos_x_izquierda_fondo_giratorio),
													&(mapa_a_cargar.Pos_y_arriba_fondo_giratorio),
													&(mapa_a_cargar.Pos_x_derecha_fondo_giratorio),
													&(mapa_a_cargar.Pos_y_abajo_fondo_giratorio)           ) == 4 )
				{
					pos_fondo_giratorio_ok = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, pos_fnd_giratorio --> (%d, y=%d), (%d, %d) \n", 	linea, 
																mapa_a_cargar.Pos_x_izquierda_fondo_giratorio,
																mapa_a_cargar.Pos_y_arriba_fondo_giratorio,
																mapa_a_cargar.Pos_x_derecha_fondo_giratorio,
																mapa_a_cargar.Pos_y_abajo_fondo_giratorio );

					#endif
				}
				else
				{	
					printf("Linea %d, pos_fnd_giratorio --> No se han podido leer todos los valores.\n", linea);
					exit(-1);
				}
			}
			else if (strstr(linea_leida, "centro_giro_fnd_gir") != NULL )
			{
				if ( sscanf(linea_leida, "centro_giro_fnd_gir=(%lf,%lf)", &(mapa_a_cargar.CentroGiroFondoGiratorio.x), &(mapa_a_cargar.CentroGiroFondoGiratorio.y) ) == 2 )
				{
					centro_giro_fondo_giratorio_ok = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, centro_giro_fnd_gir --> x=%f, y=%f \n", linea, mapa_a_cargar.CentroGiroFondoGiratorio.x, mapa_a_cargar.CentroGiroFondoGiratorio.y );
					#endif
				}
				else
				{	
					printf("Linea %d, centro_giro_fnd_gir --> No se han podido leer todos los valores.\n", linea);
					exit(-1);
				}
			}
			else if (strstr(linea_leida, "escala") != NULL )
			{
				if ( sscanf(linea_leida, "escala=%f", &escala ) == 1 )
				{
					escala_presente = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, escala=%f\n", linea, escala );
					#endif
				}
				else
				{	
					printf("Linea %d, escala --> No se ha podido leer el valor.\n", linea);
					exit(-1);
				}
			}
			else if (strstr(linea_leida, "ruta_musica") != NULL )
			{
				if ( sscanf(linea_leida, "ruta_musica=%s", mapa_a_cargar.RutaMusica ) == 1 )
				{
					ruta_musica_ok = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, ruta_musica=%s\n", linea, mapa_a_cargar.RutaMusica );
					#endif
				}
				else
				{	
					printf("Linea %d, ruta_musica --> No se han podido leer el valor.\n", linea);
					exit(-1);
				}
			}
			//  ____                                      
			// | __ ) _   _ _ __ ___  _ __   ___ _ __ ___ 
			// |  _ \| | | | '_ ` _ \| '_ \ / _ \ '__/ __|
			// | |_) | |_| | | | | | | |_) |  __/ |  \__ \
			// |____/ \__,_|_| |_| |_| .__/ \___|_|  |___/
			//                       |_|                  
			else if (strstr(linea_leida, "num_bumpers") != NULL )
			{
				sscanf(linea_leida, "num_bumpers=%d", &(mapa_a_cargar.NumeroPinballBumpers));
				num_pinball_bumpers_presente = true;
				#ifdef DEBUG_INFO
				printf("Linea %d, Numero bumpers = %d\n", linea, mapa_a_cargar.NumeroPinballBumpers);
				#endif
				// Reserva el vector de bumpers --- Allocate memory for bumpers array
				if ( vector_bumpers_inicializado ==  true )
				{
					printf( "Error: ¿Dos declaraciones de numero de bumpers?\n");
					exit(-1);
				}
				else
				{
					mapa_a_cargar.Bumpers = calloc(mapa_a_cargar.NumeroPinballBumpers, sizeof(struct pinball_bumper));
					vector_bumpers_inicializado = true;
					#ifdef DEBUG_INFO
					printf("Memoria reservada para %d bumpers\n", mapa_a_cargar.NumeroPinballBumpers);
					#endif
					// Inicializamos todos los bumper OK a false --- Initialize all "bumper OK" to false
					for (i=0; i<mapa_a_cargar.NumeroPinballBumpers; i++)
					{
						// bumper_ok[i]=false; (TODO) BORRAR
						mapa_a_cargar.Bumpers[i].definido_OK = false;   // (TODO) Pruebas 17/5/2020
					}
				}
			}
			else if (strstr(linea_leida, "bumper") != NULL )
			{
				if (vector_bumpers_inicializado==false)
				{
					printf("La declaracion de los bumpers debe ir después de especificar el numero de bumpers\n");
					exit(-1);
				}
				else
				{
					sscanf(linea_leida, "bumper[%d]",&bumper_actual); // Leemos primero el bumper actual, para poder hacer el direccionamiento en las siguientes instrucciones --- Read current segment first, in order to make the correct addressing on the following lines.
					if ( bumper_actual >= mapa_a_cargar.NumeroPinballBumpers )
					{
						printf("Se ha declarado un bumper con numero %d, que no cabe en el numero de bumpers declarado %d.\n", bumper_actual, mapa_a_cargar.NumeroPinballBumpers );
						printf("Nota: se empieza a contar desde el indice 0\n");
						exit(-1);
					}
					if ( sscanf(linea_leida, "bumper[%d]=((%lf,%lf),%f,%f)", 		&bumper_actual, 
														&(mapa_a_cargar.Bumpers[bumper_actual].centro.x),
														&(mapa_a_cargar.Bumpers[bumper_actual].centro.y),
														&(mapa_a_cargar.Bumpers[bumper_actual].radio),
														&(mapa_a_cargar.Bumpers[bumper_actual].velocidad_salida)            ) == 5 )
					{
						// bumper_ok[bumper_actual] = true; (TODO) BORRAR
						mapa_a_cargar.Bumpers[bumper_actual].definido_OK = true;	// (TODO) Pruebas 17/5/2020
						#ifdef DEBUG_INFO
						printf("Linea %d, bumper %d --> Centro x=%f, y=%f; Radio=%f, Velocidad salida=%f \n", 	linea, 
																	bumper_actual,
																	mapa_a_cargar.Bumpers[bumper_actual].centro.x,
																	mapa_a_cargar.Bumpers[bumper_actual].centro.y,
																	mapa_a_cargar.Bumpers[bumper_actual].radio,
																	mapa_a_cargar.Bumpers[bumper_actual].velocidad_salida );

						#endif
					}
					else
					{	
						printf("Linea %d, bumper --> No se han podido leer todos los valores.\n", linea);
						exit(-1);
					}
				}
			}
			else if (strstr(linea_leida, "angulo_flippers") != NULL )
			{
				if ( sscanf(linea_leida, "angulo_flippers=%f", &(mapa_a_cargar.angulo_flippers) ) == 1 )
				{
					angulo_flippers_ok = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, angulo_flippers=%f\n", linea, mapa_a_cargar.angulo_flippers );
					#endif
				}
				else
				{	
					printf("Linea %d, angulo_flippers --> No se ha podido leer el valor.\n", linea);
					exit(-1);
				}
			}
			else if (strstr(linea_leida, "tiempo_cuenta_atras") != NULL )
			{
				if ( sscanf(linea_leida, "tiempo_cuenta_atras=%d", &(mapa_a_cargar.SegundosCuentaAtras) ) == 1 )
				{
					cuenta_atras_presente = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, tiempo_cuenta_atras = %d segundos.\n", linea, mapa_a_cargar.SegundosCuentaAtras );
					#endif
				}
				else
				{	
					printf("Linea %d, tiempo_cuenta_atras --> No se ha podido leer el valor.\n", linea);
					exit(-1);
				}
			}
			//   _____                                      _                     
			//  |__  /___  _ __   __ _ ___    __ _  ___ ___| | ___ _ __ __ _  ___ 
			//    / // _ \| '_ \ / _` / __|  / _` |/ __/ _ \ |/ _ \ '__/ _` |/ __|
			//   / /| (_) | | | | (_| \__ \ | (_| | (_|  __/ |  __/ | | (_| | (__ 
			//  /____\___/|_| |_|\__,_|___/  \__,_|\___\___|_|\___|_|  \__,_|\___|
			else if (strstr(linea_leida, "num_zonas_acel_circ") != NULL )
			{
				sscanf(linea_leida, "num_zonas_acel_circ=%d", &(mapa_a_cargar.NumeroZonasAceleracionCircular));
				num_zonas_acel_circ_presente = true;
				#ifdef DEBUG_INFO
				printf("Linea %d, num_zonas_acel_circ = %d\n", linea, mapa_a_cargar.NumeroZonasAceleracionCircular);
				#endif
				// Reserva el vector de zonas de aceleracion circular --- Allocate memory for array of round acceleration zones
				if ( vector_zonas_acel_circ_inicializado ==  true )
				{
					printf( "Error: ¿Dos declaraciones de zonas circulares de aceleración?\n");
					exit(-1);
				}
				else
				{
					mapa_a_cargar.ZonasAceleracionCircular = calloc(mapa_a_cargar.NumeroZonasAceleracionCircular, sizeof(struct zona_aceleracion_circular));
					vector_zonas_acel_circ_inicializado = true;
					#ifdef DEBUG_INFO
					printf("Memoria reservada para %d zonas de aceleracion circular\n", mapa_a_cargar.NumeroZonasAceleracionCircular);
					#endif
					// Inicializamos todos los zona acel circ OK a false --- Initialize all "round accel zones" to false
					for (i=0; i<mapa_a_cargar.NumeroZonasAceleracionCircular; i++)
					{
						// zona_acel_circ_ok[i]=false; (TODO) BORRAR
						mapa_a_cargar.ZonasAceleracionCircular[i].definido_OK = false;	// (TODO) Pruebas 17/5/2020
					}
				}
			}
			else if (strstr(linea_leida, "zona_acel_circ") != NULL )
			{
				if (vector_zonas_acel_circ_inicializado==false)
				{
					printf("La declaracion de las zonas circulares de aceleración debe ir después de especificar el numero de zonas\n");
					exit(-1);
				}
				else
				{
					int temp_zona_acel_circ_invisible;
					sscanf(linea_leida, "zona_acel_circ[%d]",&zona_acel_circ_actual); // Leemos primero el numero actual de zona de acel, para poder hacer el direccionamiento en las siguientes instrucciones --- Read current segment first, in order to make the correct addressing on the following lines.
					if ( zona_acel_circ_actual >= mapa_a_cargar.NumeroZonasAceleracionCircular )
					{
						printf("Se ha declarado una zona acel circular con numero %d, que no cabe en el numero de zonas de aceleracion circular declarado %d.\n", zona_acel_circ_actual, mapa_a_cargar.NumeroZonasAceleracionCircular );
						printf("Nota: se empieza a contar desde el indice 0\n");
						exit(-1);
					}
					if ( sscanf(linea_leida, "zona_acel_circ[%d]=((%lf,%lf),%f,%f,%f,%d)", 		&zona_acel_circ_actual, 
														&(mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].centro.x),
														&(mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].centro.y),
														&(mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].radio),
														&(mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].angulo),
														&(mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].aceleracion),
														&(temp_zona_acel_circ_invisible )            ) == 7 )
					{
						// zona_acel_circ_ok[zona_acel_circ_actual] = true;	( TODO)
						mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].definido_OK = true;	// (TODO) Pruebas 17/5/2020
						#ifdef DEBUG_INFO
						printf("Linea %d, zona_acel_circ %d --> Centro x=%f, y=%f; Radio=%f, Angulo=%f, Aceleracion=%f, Invisible=%d \n", 	linea, 
																	zona_acel_circ_actual,
																	mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].centro.x,
																	mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].centro.y,
																	mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].radio,
																	mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].angulo,
																	mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].aceleracion,
																	temp_zona_acel_circ_invisible );

						#endif
						switch (temp_zona_acel_circ_invisible)
						{
							case 0:
								mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].invisible = false;
								#ifdef DEBUG_INFO
								printf("Linea %d, zona acel circ invisible=false \n", linea);
								#endif
								break;
							case 1:
								mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].invisible = true;
								#ifdef DEBUG_INFO
								printf("Linea %d, zona acel circ invisible=true \n", linea);
								#endif
								break;
							default:
								// Valor no válido, se toma el valor por defecto
								mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].invisible = false;
								#ifdef DEBUG_INFO
								printf("Linea %d, dato invisible tiene un valor no valido, se supone visible. \n", linea);
								#endif
								break;
						}


					}
					else
					{	
						printf("Linea %d, zona_acel_circ --> No se han podido leer todos los valores.\n", linea);
						exit(-1);
					}
				}
			}
			else if (strstr(linea_leida, "no_rot_moneda") != NULL )
			{
				no_rot_moneda_presente = true;
				mapa_a_cargar.no_rot_moneda = true;
				#ifdef DEBUG_INFO
				printf("Linea %d, no_rot_moneda --> La moneda no rotara \n");
				#endif

			}


			// (TODO) Pruebas 17/5/2020
			else if (strstr(linea_leida, "imagen_segm_pared") != NULL )
			{
				if ( sscanf(linea_leida, "imagen_segm_pared=%s", mapa_a_cargar.RutaImagenSegmentoPared ) == 1 )
				{
					imagen_segm_pared_presente = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, imagen_segm_pared=%s\n", linea, mapa_a_cargar.RutaImagenSegmentoPared );
					#endif
				}
				else
				{	
					printf("Linea %d, imagen_segm_pared --> No se ha podido leer el valor.\n", linea);
					exit(-1);
				}
			}
			else if (strstr(linea_leida, "imagen_segm_meta") != NULL )
			{
				if ( sscanf(linea_leida, "imagen_segm_meta=%s", mapa_a_cargar.RutaImagenSegmentoMeta ) == 1 )
				{
					imagen_segm_meta_presente = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, imagen_segm_meta=%s\n", linea, mapa_a_cargar.RutaImagenSegmentoMeta );
					#endif
				}
				else
				{	
					printf("Linea %d, imagen_segm_meta --> No se ha podido leer el valor.\n", linea);
					exit(-1);
				}
			}
			else if (strstr(linea_leida, "imagen_segm_muerte") != NULL )
			{
				if ( sscanf(linea_leida, "imagen_segm_muerte=%s", mapa_a_cargar.RutaImagenSegmentoMuerte ) == 1 )
				{
					imagen_segm_muerte_presente = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, imagen_segm_muerte=%s\n", linea, mapa_a_cargar.RutaImagenSegmentoMuerte );
					#endif
				}
				else
				{	
					printf("Linea %d, imagen_segm_muerte --> No se ha podido leer el valor.\n", linea);
					exit(-1);
				}
			}

			//  _____                      _
			// | ____|_ __   ___ _ __ ___ (_) __ _  ___  ___
			// |  _| | '_ \ / _ \ '_ ` _ \| |/ _` |/ _ \/ __|
			// | |___| | | |  __/ | | | | | | (_| | (_) \__ \
			// |_____|_| |_|\___|_| |_| |_|_|\__, |\___/|___/
			//                               |___/
			// (11/9/2021)
			else if (strstr(linea_leida, "num_enemigos_ruta") != NULL )
			{
				sscanf(linea_leida, "num_enemigos_ruta=%d", &(mapa_a_cargar.NumeroEnemigosRuta));
				num_enemigos_ruta_presente = true;
				#ifdef DEBUG_INFO
				printf("Linea %d, num_enemigos_ruta = %d\n", linea, mapa_a_cargar.NumeroEnemigosRuta);
				#endif
				// Reserva el vector de enemigos --- Allocate memory for array of enemies
				if ( vector_enemigos_ruta_inicializado ==  true )
				{
					printf( "Error: ¿Dos declaraciones de enemigos (tipo ruta)?\n");
					exit(-1);
				}
				else
				{
					mapa_a_cargar.EnemigosRuta = calloc(mapa_a_cargar.NumeroEnemigosRuta, sizeof(struct enemigo_ruta));
					vector_enemigos_ruta_inicializado = true;
					#ifdef DEBUG_INFO
					printf("Memoria reservada para %d enemigos tipo ruta.\n", mapa_a_cargar.NumeroEnemigosRuta);
					#endif
					// Inicializamos todos los enemigo ruta OK a false --- Initialize all "path enemies" to false
					for (i=0; i<mapa_a_cargar.NumeroEnemigosRuta; i++)
					{
						mapa_a_cargar.EnemigosRuta[i].definido_OK = false;
						// Inicializamos todos los definidos de los waypoints a false --- Initialize all "waypoints defined" to false
						for(j=0; j<MAX_WAYPOINTS_POR_ENEMIGO; j++)
						{
							mapa_a_cargar.EnemigosRuta[i].waypoints[j].definido = false;
						}

					}
				}
			}
			else if (strstr(linea_leida, "enemigo_ruta") != NULL )
			{
				if (vector_enemigos_ruta_inicializado==false)
				{
					printf("La declaracion de los enemigos (tipo ruta) debe ir después de especificar el numero de enemigos\n");
					exit(-1);
				}
				else
				{
					sscanf(linea_leida, "enemigo_ruta[%d]",&enemigo_ruta_actual); // Leemos primero el numero actual de enemigo, para poder hacer el direccionamiento en las siguientes instrucciones --- Read current enemy number first, in order to make the correct addressing on the following lines.
					if ( enemigo_ruta_actual >= mapa_a_cargar.NumeroEnemigosRuta )
					{
						printf("Se ha declarado un enemigo ruta con numero %d, que no cabe en el numero de enemigos (tipo ruta) declarado %d.\n", enemigo_ruta_actual, mapa_a_cargar.NumeroEnemigosRuta );
						printf("Nota: se empieza a contar desde el indice 0\n");
						exit(-1);
					} 
					if ( sscanf(linea_leida, "enemigo_ruta[%d]=%f,%s", 	&enemigo_ruta_actual, 
												&(mapa_a_cargar.EnemigosRuta[enemigo_ruta_actual].speed_multiplier),
												mapa_a_cargar.EnemigosRuta[enemigo_ruta_actual].nombre_tipo       ) == 3 )
					{
						mapa_a_cargar.EnemigosRuta[enemigo_ruta_actual].definido_OK = true;
						#ifdef DEBUG_INFO
						printf("Linea %d, enemigo_ruta %d --> Tipo =%s, speed_multiplier=%f \n", 	linea, 
																enemigo_ruta_actual,
																mapa_a_cargar.EnemigosRuta[enemigo_ruta_actual].nombre_tipo,
																mapa_a_cargar.EnemigosRuta[enemigo_ruta_actual].speed_multiplier );

						#endif  
					}
					else
					{	
						printf("Linea %d, enemigo_ruta --> No se han podido leer todos los valores.\n", linea);
						exit(-1);
					}
				}
			}
			else if (strstr(linea_leida, "waypoint_enemigo") != NULL )
			{
				if (vector_enemigos_ruta_inicializado==false)
				{
					printf("La declaracion de los enemigos (tipo ruta) debe ir después de especificar algun waypoint\n");
					exit(-1);
				}
				else
				{
					sscanf(linea_leida, "waypoint_enemigo[%d,%d]",&enemigo_ruta_actual, &waypoint_actual); // Leemos primero el numero actual de enemigo y waypoint, para poder hacer el direccionamiento en las siguientes instrucciones --- Read current enemy and waypoint first, in order to make the correct addressing on the following lines.
					if ( enemigo_ruta_actual >= mapa_a_cargar.NumeroEnemigosRuta )
					{
						printf("Se ha declarado un waypoint de un enemigo %d, que no cabe en el numero de enemigos (tipo ruta) declarado %d.\n", enemigo_ruta_actual, mapa_a_cargar.NumeroEnemigosRuta );
						printf("Nota: se empieza a contar desde el indice 0\n");
						exit(-1);
					} 
					if ( waypoint_actual >= MAX_WAYPOINTS_POR_ENEMIGO )
					{
						printf("Se ha declarado un waypoint numero %d, que supera el numero maximo de waypoins por enemigo (%d).\n", waypoint_actual, MAX_WAYPOINTS_POR_ENEMIGO );

					}

					if ( sscanf(linea_leida, "waypoint_enemigo[%d,%d]=(%lf,%lf)", 	&enemigo_ruta_actual, &waypoint_actual,
												&(mapa_a_cargar.EnemigosRuta[enemigo_ruta_actual].waypoints[waypoint_actual].waypoint.x),
												&(mapa_a_cargar.EnemigosRuta[enemigo_ruta_actual].waypoints[waypoint_actual].waypoint.y)       ) == 4 )
					{
						mapa_a_cargar.EnemigosRuta[enemigo_ruta_actual].waypoints[waypoint_actual].definido = true;

						
						#ifdef DEBUG_INFO
						printf("Linea %d, waypoint %d de enemigo_ruta %d --> x=%lf, y=%lf \n", 	linea, 
																enemigo_ruta_actual, waypoint_actual,
																mapa_a_cargar.EnemigosRuta[enemigo_ruta_actual].waypoints[waypoint_actual].waypoint.x,
																mapa_a_cargar.EnemigosRuta[enemigo_ruta_actual].waypoints[waypoint_actual].waypoint.y );

						#endif  
					}
					else
					{	
						printf("Linea %d, waypoint_enemigo_ruta --> No se han podido leer todos los valores.\n", linea);
						exit(-1);
					}
				}
			}
			//  __        __                   _           _           
			//  \ \      / /__  _ __ _ __ ___ | |__   ___ | | ___  ___ 
			//   \ \ /\ / / _ \| '__| '_ ` _ \| '_ \ / _ \| |/ _ \/ __|
			//    \ V  V / (_) | |  | | | | | | | | | (_) | |  __/\__ \
			//     \_/\_/ \___/|_|  |_| |_| |_|_| |_|\___/|_|\___||___/
			//    
			// (26/9/2021)
			else if (strstr(linea_leida, "num_wormh") != NULL )
			{
				sscanf(linea_leida, "num_wormh=%d", &(mapa_a_cargar.NumeroWormholes));
				num_wormholes_presente = true;
				#ifdef DEBUG_INFO
				printf("Linea %d, num_wormh = %d\n", linea, mapa_a_cargar.NumeroWormholes);
				#endif
				// Reserva el vector de wormholes --- Allocate memory for array of worhmoles
				if ( vector_wormholes_inicializado ==  true )
				{
					printf( "Error: ¿Dos declaraciones de numero de wormholes?\n");
					exit(-1);
				}
				else
				{
					mapa_a_cargar.Wormholes = calloc(mapa_a_cargar.NumeroWormholes, sizeof(struct wormhole));
					vector_wormholes_inicializado = true;
					#ifdef DEBUG_INFO
					printf("Memoria reservada para %d wormholes.\n", mapa_a_cargar.NumeroWormholes);
					#endif
					// Inicializamos todos los wormholes definido OK a false --- Initialize all "wormholes defined OK" to false
					for (i=0; i<mapa_a_cargar.NumeroWormholes; i++)
					{
						mapa_a_cargar.Wormholes[i].definido_OK = false;
					}
				}
			}
			else if (strstr(linea_leida, "wormhole") != NULL )
			{
				if (vector_wormholes_inicializado==false)
				{
					printf("La declaracion de los wormholes debe ir después de especificar el numero de wormholes.\n");
					exit(-1);
				}
				else
				{
					sscanf(linea_leida, "wormhole[%d]",&wormhole_actual); // Leemos primero el numero actual de wormhole, para poder hacer el direccionamiento en las siguientes instrucciones --- Read current wormhole first, in order to make the correct addressing on the following lines.
					if ( wormhole_actual >= mapa_a_cargar.NumeroWormholes )
					{
						printf("Se ha declarado un wormhole con numero %d, que no cabe en el numero de wormholes declarado %d.\n", wormhole_actual, mapa_a_cargar.NumeroWormholes );
						printf("Nota: se empieza a contar desde el indice 0\n");
						exit(-1);
					} 
					if ( sscanf(linea_leida, "wormhole[%d]=(%f,(%lf,%lf),(%lf,%lf))", 	&wormhole_actual, 
														&(mapa_a_cargar.Wormholes[wormhole_actual].radio),
														&(mapa_a_cargar.Wormholes[wormhole_actual].p1.x),
														&(mapa_a_cargar.Wormholes[wormhole_actual].p1.y),
														&(mapa_a_cargar.Wormholes[wormhole_actual].p2.x),
														&(mapa_a_cargar.Wormholes[wormhole_actual].p2.y)
													) == 6 )
					{
						mapa_a_cargar.Wormholes[wormhole_actual].definido_OK = true;
						#ifdef DEBUG_INFO
						printf("Linea %d, wormhole %d --> Radio =%f, p1=(%lf,%lf), p2=(%lf,%lf) \n", 	linea, 
																wormhole_actual,
																mapa_a_cargar.Wormholes[wormhole_actual].radio,
																mapa_a_cargar.Wormholes[wormhole_actual].p1.x,
																mapa_a_cargar.Wormholes[wormhole_actual].p1.y,
																mapa_a_cargar.Wormholes[wormhole_actual].p2.x,
																mapa_a_cargar.Wormholes[wormhole_actual].p2.y      );

						#endif  
					}
					else
					{	
						printf("Linea %d, wormhole --> No se han podido leer todos los valores.\n", linea);
						exit(-1);
					}
				}
			}
			////////////////////////////////////////////////////////////////////////////////
			// Seguir añadiendo nuevas opciones aqui --- New options must be added here
			////////////////////////////////////////////////////////////////////////////////
			else
			{
				#ifdef DEBUG_INFO
				printf("Linea %d, expresion no reconocida, se ignora.\n", linea);
				#endif
			}

		}

	}

	fclose(archivo);

	////////////////////////////////////////////////////////////////////////////////////////////
	// Gestion del tiempo de cuenta atras --- Countdown time management
	if ( cuenta_atras_presente == false )
	{
		mapa_a_cargar.CuentaAtras = false;
		mapa_a_cargar.SegundosCuentaAtras = 0;
		#ifdef DEBUG_INFO
		printf("Tiempo de cuenta atrás no estaba definido en el mapa. No se tendrá en cuenta ninguna cuenta atrás. \n");
		#endif
	}
	else
	{
		mapa_a_cargar.CuentaAtras = true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	// Buscar si hay flippers --- Check if map contains flippers
	mapa_a_cargar.mapa_contiene_flippers = false;
	for (segmento_actual=0; segmento_actual<mapa_a_cargar.NumeroSegmentos; segmento_actual++)
	{
		if ( (mapa_a_cargar.Mapa[segmento_actual].type == pinball_flipper_L ) || (mapa_a_cargar.Mapa[segmento_actual].type == pinball_flipper_R ) )
		{
			mapa_a_cargar.mapa_contiene_flippers = true;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Verificamos que lo tengamos todo (que al archivo no le faltasen datos)
	// Check if we have all the required data (and the file lacks no essential data)
	if (nombre_ok == false)
	{
		printf("Falta el nombre del mapa.\n");
	}
	if (descripcion_ok == false )
	{
		printf("Falta la descripcion del mapa.\n");
	}
	if (num_segmentos_ok == false)
	{
		printf("Falta el número de segmentos.\n");
	}
	if (modo_giro_mapa_ok == false) 
	{
		printf("Falta el modo de giro.\n");
	}
	if (mapa_a_cargar.TipoGiro == punto_fijo &&  punto_giro_ok == false)
	{
		printf("Falta el punto de giro (y el mapa gira alrededor de un punto fijo).\n");	
	}
	if (angulo_max_ok == false) 
	{
		printf("Falta el angulo máximo.\n");
	}
	if (pos_inicial_ok == false)
	{		
		printf("Falta el la posición inicial.\n");
	}
	if (gravedad_ok == false) 
	{	
		printf("Falta la gravedad.\n");
	}
	if (imagen_moneda_ok == false)
	{
		printf("Falta el la ruta de la imagen de la moneda.\n");
	}
	if (imagen_fondo_ok == false)
	{
		printf("Falta el la ruta de la imagen del fondo.\n");
	}
	if (fondo_giratorio_ok == false )
	{
		printf("Falta especificar si hay o no un fondo giratorio.\n");
	}
	if (mapa_a_cargar.HayFondoGiratorio == true && imagen_fondo_giratorio_ok == false )
	{
		printf("Falta el la ruta de la imagen del fondo giratorio (y hay fondo giratorio).\n");
	}
	if (mapa_a_cargar.HayFondoGiratorio == true && pos_fondo_giratorio_ok == false )
	{
		printf("Falta la posición del fondo giratorio (y hay fondo giratorio).\n");
	}
	if (mapa_a_cargar.HayFondoGiratorio == true && centro_giro_fondo_giratorio_ok == false )
	{
		printf("Falta la posición del centro de giro del fondo giratorio (y hay fondo giratorio).\n");
	}
	if ( ruta_musica_ok == false )
	{
		printf("Falta especificar la ruta de la musica.\n");
	}
	if ( mapa_a_cargar.mapa_contiene_flippers == true && angulo_flippers_ok == false )
	{
		printf("Falta especificar el angulo de los flippers (el mapa contiene flippers).\n");
	}


	if (nombre_ok==false ||/* descripcion_ok==false || */ num_segmentos_ok==false || modo_giro_mapa_ok==false || (mapa_a_cargar.TipoGiro == punto_fijo && punto_giro_ok==false ) || 
		angulo_max_ok==false || pos_inicial_ok==false || gravedad_ok==false || imagen_moneda_ok==false || imagen_fondo_ok==false || 
		fondo_giratorio_ok==false || (mapa_a_cargar.HayFondoGiratorio == true && imagen_fondo_giratorio_ok == false) || (mapa_a_cargar.HayFondoGiratorio == true && pos_fondo_giratorio_ok == false ) || (mapa_a_cargar.HayFondoGiratorio == true && centro_giro_fondo_giratorio_ok == false ) ||
		ruta_musica_ok == false  || ( mapa_a_cargar.mapa_contiene_flippers == true && angulo_flippers_ok == false ) )
	{
		printf("Archivo de mapa incompleto\n");
		exit(-1);
	}

	for (i=0; i<mapa_a_cargar.NumeroSegmentos; i++)
	{
		if ( mapa_a_cargar.Mapa[i].definido_OK == false)	// 17/5/2020
		{
			printf("Segmento %d no definido\n",i);
			exit(-1);
		}
	}

	if ( num_pinball_bumpers_presente )
	{
		for (i=0; i<mapa_a_cargar.NumeroPinballBumpers; i++)
		{
			if (mapa_a_cargar.Bumpers[i].definido_OK == false)
			{
				printf("Bumper %d no definido\n", i);
				exit(-1);
			}
		}		
	}
	else
	{
		mapa_a_cargar.NumeroPinballBumpers=0;
	}

	if ( num_zonas_acel_circ_presente )	// 22/3/2020
	{
		for (i=0; i<mapa_a_cargar.NumeroZonasAceleracionCircular; i++)
		{
			if (mapa_a_cargar.ZonasAceleracionCircular[i].definido_OK == false)
			{
				printf("Zona circular de aceleración %d no definido\n", i);
				exit(-1);
			}
		}		
	}
	else
	{
		mapa_a_cargar.NumeroZonasAceleracionCircular=0;
	}

	if ( num_enemigos_ruta_presente )	// Nuevo, 17/9/2021, verificar datos de enemigos --- Check enemy data
	{
		for (i=0; i<mapa_a_cargar.NumeroEnemigosRuta; i++ )
		{
			if ( mapa_a_cargar.EnemigosRuta[i].definido_OK == false )
			{
				printf("Enemigo tipo ruta %d no definido\n", i);
				exit(-1);
			}
		}

		Calcular_Numero_Waypoints_Enemigos( mapa_a_cargar.EnemigosRuta, mapa_a_cargar.NumeroEnemigosRuta );

		if ( !Verificar_Datos_Enemigos( mapa_a_cargar.EnemigosRuta, mapa_a_cargar.NumeroEnemigosRuta )  )
		{
			printf("Datos de enemigos son incorrectos.\n");
			exit(-1);			
		}

	}
	else
	{
		mapa_a_cargar.NumeroEnemigosRuta = 0;
	}

	if ( num_wormholes_presente )		// Nuevo 26/9/2021, Verificar datos de wormholes --- Check wormhole data
	{
		for( i=0; i<mapa_a_cargar.NumeroWormholes; i++ )
		{
			if ( mapa_a_cargar.Wormholes[i].definido_OK == false )
			{
				printf("Wormholes %d no definido.\n", i);
				exit(-1);
			}
		}
	}
	else
	{
		mapa_a_cargar.NumeroWormholes = 0;
	}


	if ( (cuenta_atras_presente == true) && (mapa_a_cargar.SegundosCuentaAtras<=0) )
	{
		printf("Tiempo de cuenta atras no valido\n");
		exit(-1);
	}

	// Si las rutas de las imagenes de los segmentos no están definidas, tomamos los valores por defecto
	// If image paths for line segments are not defined, then the default paths will be used
	// (TODO) Pruebas 17/5/2020
	if (imagen_segm_pared_presente == false)
	{
		#ifdef DEBUG_INFO
		printf("imagen_segm_pared no definida, se toma la imagen por defecto %s\n", IMAGEN_SEGMENTO_POR_DEFECTO );
		#endif
		strcpy( mapa_a_cargar.RutaImagenSegmentoPared, IMAGEN_SEGMENTO_POR_DEFECTO );
	}
	if (imagen_segm_meta_presente == false )
	{
		#ifdef DEBUG_INFO
		printf("imagen_segm_meta no definida, se toma la imagen por defecto %s\n", IMAGEN_SEGMENTO_POR_DEFECTO );
		#endif
		strcpy( mapa_a_cargar.RutaImagenSegmentoMeta, IMAGEN_SEGMENTO_POR_DEFECTO );
	}
	if (imagen_segm_muerte_presente == false )
	{
		#ifdef DEBUG_INFO
		printf("imagen_segm_muerte no definida, se toma la imagen por defecto %s\n", IMAGEN_SEGMENTO_POR_DEFECTO );
		#endif
		strcpy( mapa_a_cargar.RutaImagenSegmentoMuerte, IMAGEN_SEGMENTO_POR_DEFECTO );
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Para simplificar los calculos futuros, vamos a hacer que el punto de inicio (start) siempre esté a la izquierda.
	// In order to make future calculations more simple, let's make that start point is always on the left.
	for (segmento_actual=0; segmento_actual<mapa_a_cargar.NumeroSegmentos; segmento_actual++)
	{
		if ( mapa_a_cargar.Mapa[segmento_actual].start.x > mapa_a_cargar.Mapa[segmento_actual].end.x )
		{
			// Intercambiamos start y end --- Swap start and end
			punto_auxiliar = mapa_a_cargar.Mapa[segmento_actual].end;
			mapa_a_cargar.Mapa[segmento_actual].end = mapa_a_cargar.Mapa[segmento_actual].start;
			mapa_a_cargar.Mapa[segmento_actual].start = punto_auxiliar;
			#ifdef DEBUG_INFO
			printf("Se han intercambiado start y end en segmento %d\n", segmento_actual);
			#endif
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//  Detectamos adyacencias --- Check for adjacencies
	for (segmento_actual=0; segmento_actual<mapa_a_cargar.NumeroSegmentos; segmento_actual++)
	{
		mapa_a_cargar.Mapa[segmento_actual].start_adyacente_a_otro = false;
		mapa_a_cargar.Mapa[segmento_actual].end_adyacente_a_otro = false;
		for (otro_segmento_actual=0; otro_segmento_actual<mapa_a_cargar.NumeroSegmentos; otro_segmento_actual++)
		{
			if ( segmento_actual == otro_segmento_actual )
			{
				continue;
			}
			if (  ( AnguloRadEntreDosSegmentos( mapa_a_cargar.Mapa[segmento_actual] , mapa_a_cargar.Mapa[otro_segmento_actual] )*180/PI ) < 45  ) // Pruebas nuevo 18/4/2020
			{
				// No tenemos en cuenta la adyacencia si los dos segmentos forman mas de cierto angulo.
				// The adjacency is not taken into account if the angle between both segments is grater than a threshold
				if ( ( mapa_a_cargar.Mapa[segmento_actual].start.x == mapa_a_cargar.Mapa[otro_segmento_actual].start.x ) && ( mapa_a_cargar.Mapa[segmento_actual].start.y == mapa_a_cargar.Mapa[otro_segmento_actual].start.y ) || 
					( mapa_a_cargar.Mapa[segmento_actual].start.x == mapa_a_cargar.Mapa[otro_segmento_actual].end.x ) && ( mapa_a_cargar.Mapa[segmento_actual].start.y == mapa_a_cargar.Mapa[otro_segmento_actual].end.y ) )
				{
					mapa_a_cargar.Mapa[segmento_actual].start_adyacente_a_otro = true;
					#ifdef DEBUG_INFO
					printf("Detectada adyacencia en extremo start en segmento %d, con segmento %d\n", segmento_actual, otro_segmento_actual);
					#endif
				}
				if ( ( mapa_a_cargar.Mapa[segmento_actual].end.x == mapa_a_cargar.Mapa[otro_segmento_actual].start.x ) && ( mapa_a_cargar.Mapa[segmento_actual].end.y == mapa_a_cargar.Mapa[otro_segmento_actual].start.y ) || 
					( mapa_a_cargar.Mapa[segmento_actual].end.x == mapa_a_cargar.Mapa[otro_segmento_actual].end.x ) && ( mapa_a_cargar.Mapa[segmento_actual].end.y == mapa_a_cargar.Mapa[otro_segmento_actual].end.y ) )
				{
					mapa_a_cargar.Mapa[segmento_actual].end_adyacente_a_otro = true;
					#ifdef DEBUG_INFO
					printf("Detectada adyacencia en extremo end en segmento %d, con segmento %d\n", segmento_actual, otro_segmento_actual);
					#endif
				}
			} // Pruebas nuevo 18/4/2020
			/*else
			{
				#ifdef DEBUG_INFO
				printf("No se verifica adyacencia entre segmentos %d y %d ya que forman un angulo de %f grados \n", segmento_actual, otro_segmento_actual, AnguloRadEntreDosSegmentos( mapa_a_cargar.Mapa[segmento_actual] , mapa_a_cargar.Mapa[otro_segmento_actual] )*180/PI);
				#endif
			}*/		
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Aplicación de escala al mapa --- Map scale
	if ( escala_presente )
	{
		// Segmentos --- Segments
		for (segmento_actual=0; segmento_actual<mapa_a_cargar.NumeroSegmentos; segmento_actual++)
		{
			mapa_a_cargar.Mapa[segmento_actual].start.x = mapa_a_cargar.Mapa[segmento_actual].start.x * escala;
			mapa_a_cargar.Mapa[segmento_actual].start.y = mapa_a_cargar.Mapa[segmento_actual].start.y * escala;
			mapa_a_cargar.Mapa[segmento_actual].end.x = mapa_a_cargar.Mapa[segmento_actual].end.x * escala;
			mapa_a_cargar.Mapa[segmento_actual].end.y = mapa_a_cargar.Mapa[segmento_actual].end.y * escala;
		}
		// Posicion inicial moneda --- Coin starting position
		mapa_a_cargar.PuntoInicialMoneda.x = mapa_a_cargar.PuntoInicialMoneda.x * escala;
		mapa_a_cargar.PuntoInicialMoneda.y = mapa_a_cargar.PuntoInicialMoneda.y * escala;
		// Centro de giro --- Rotation center
		mapa_a_cargar.PuntoGiroFijo.x = mapa_a_cargar.PuntoGiroFijo.x * escala;
		mapa_a_cargar.PuntoGiroFijo.y = mapa_a_cargar.PuntoGiroFijo.y * escala;
		// Fondo giratorio --- Rotating background
		mapa_a_cargar.CentroGiroFondoGiratorio.x = mapa_a_cargar.CentroGiroFondoGiratorio.x * escala;
		mapa_a_cargar.CentroGiroFondoGiratorio.y = mapa_a_cargar.CentroGiroFondoGiratorio.y * escala;
		mapa_a_cargar.Pos_x_izquierda_fondo_giratorio = mapa_a_cargar.Pos_x_izquierda_fondo_giratorio * escala;
		mapa_a_cargar.Pos_y_arriba_fondo_giratorio = mapa_a_cargar.Pos_y_arriba_fondo_giratorio * escala;
		mapa_a_cargar.Pos_x_derecha_fondo_giratorio = mapa_a_cargar.Pos_x_derecha_fondo_giratorio * escala;
		mapa_a_cargar.Pos_y_abajo_fondo_giratorio = mapa_a_cargar.Pos_y_abajo_fondo_giratorio * escala;
		// Bumpers
		if ( mapa_a_cargar.NumeroPinballBumpers != 0 )
		{
			for ( bumper_actual=0; bumper_actual<mapa_a_cargar.NumeroPinballBumpers; bumper_actual++)
			{
				mapa_a_cargar.Bumpers[bumper_actual].centro.x *= escala;
				mapa_a_cargar.Bumpers[bumper_actual].centro.y *= escala;
				mapa_a_cargar.Bumpers[bumper_actual].radio *= escala; 
				mapa_a_cargar.Bumpers[bumper_actual].velocidad_salida *= escala;
			}
		}
		// Zonas de aceleracion circular --- Round acceleration zones
		if ( mapa_a_cargar.NumeroZonasAceleracionCircular != 0 )
		{
			for ( zona_acel_circ_actual=0; zona_acel_circ_actual<mapa_a_cargar.NumeroZonasAceleracionCircular; zona_acel_circ_actual++)
			{
				mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].centro.x *= escala;
				mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].centro.y *= escala;
				mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].radio *= escala; 
				mapa_a_cargar.ZonasAceleracionCircular[zona_acel_circ_actual].aceleracion *= escala;
			}
		}
		// Enemigos tipo ruta --- Path type enemies
		if ( mapa_a_cargar.NumeroEnemigosRuta != 0 )
		{
			for ( enemigo_ruta_actual=0; enemigo_ruta_actual<mapa_a_cargar.NumeroEnemigosRuta; enemigo_ruta_actual++)
			{
				for ( waypoint_actual=0; waypoint_actual<MAX_WAYPOINTS_POR_ENEMIGO ; waypoint_actual++)
				{
					if ( mapa_a_cargar.EnemigosRuta[enemigo_ruta_actual].waypoints[waypoint_actual].definido )
					{
						mapa_a_cargar.EnemigosRuta[enemigo_ruta_actual].waypoints[waypoint_actual].waypoint.x *= escala;
						mapa_a_cargar.EnemigosRuta[enemigo_ruta_actual].waypoints[waypoint_actual].waypoint.y *= escala;
					}
				}
			}
		}
		// Wormholes (26/9/2021)
		if ( mapa_a_cargar.NumeroWormholes != 0 )
		{
			for ( wormhole_actual = 0; wormhole_actual<mapa_a_cargar.NumeroWormholes; wormhole_actual++ )
			{
				mapa_a_cargar.Wormholes[wormhole_actual].p1.x *= escala;
				mapa_a_cargar.Wormholes[wormhole_actual].p1.y *= escala;
				mapa_a_cargar.Wormholes[wormhole_actual].p2.x *= escala;
				mapa_a_cargar.Wormholes[wormhole_actual].p2.y *= escala;
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Ver si la moneda rota o no --- Check if the coin must spin
	mapa_a_cargar.no_rot_moneda =  no_rot_moneda_presente;

	#ifdef DEBUG_INFO
	printf("Mapa %s cargado. Continuamos.\n", nombre_archivo );
	#endif
	return mapa_a_cargar;
}


//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////

struct mapa LeerInfoArchivo( char *nombre_archivo )	// Read File Info
{
	// Esta funcion es una version resumida de la anterior, no carga toda la información, 
	// sino sólamente aquella que es necesaria para el menu de seleccion
	// This function is a simplified version of the previous one. It does not load all information,
	// only those information needed for the level selection menu.
	struct mapa mapa_a_leer;
	//struct punto punto_auxiliar;
	char linea_leida[200];
	bool nombre_ok=false, descripcion_ok=false, modo_giro_mapa_ok=false, angulo_max_ok=false, gravedad_ok=false ; 
	bool cuenta_atras_presente=false;
	FILE *archivo;
	int linea=0;

	mapa_a_leer.RutaImagenDescMenu_Presente = false;
	mapa_a_leer.CuentaAtras = false;

	archivo = fopen( nombre_archivo, "r" );
	if (archivo == NULL)
	{
		printf("Leer info archivo: Error: No se puede abrir el archivo %s \n", nombre_archivo );
		exit(-1);
	}
	while ( (fgets(linea_leida, 200, archivo)) != NULL )
	{
		linea++;
		#ifdef DEBUG_INFO
		printf("Leer info archivo: Linea %d leida\n", linea );
		#endif
		if ( (linea_leida[0] == '#') || (linea_leida[0] == '\n') || (strlen(linea_leida)==0) )	//lo de ver si está vacia no funciona
		{
			//Es un comentario, no hacer nada --- This is a remark, nothing has to be done
			#ifdef DEBUG_INFO
			printf("Leer info archivo: Linea %d es un comentario\n", linea );
			#endif
		}
		else
		{
			if (strstr(linea_leida, "nombre") != NULL )
			{
				sscanf(linea_leida, "nombre=%s", mapa_a_leer.NombreMapa);
				nombre_ok = true;
				#ifdef DEBUG_INFO
				printf("Leer info archivo: Linea %d, Nombre mapa = %s\n", linea, mapa_a_leer.NombreMapa);
				#endif
			}
			if (strstr(linea_leida, "descripcion") != NULL )
			{
				char cadena_aux[200]; char* token;
				//sscanf(linea_leida, "descripcion=\"%s\"", mapa_a_cargar.DescripcionMapa);
				strcpy( cadena_aux, linea_leida);
				token = strtok( cadena_aux, "=" ); token = strtok( NULL, "=" );
				strcpy( mapa_a_leer.DescripcionMapa, token);
				descripcion_ok = true;
				#ifdef DEBUG_INFO
				printf("Leer info archivo: Linea %d, descripcion = %s\n", linea, mapa_a_leer.DescripcionMapa);
				#endif
			}
			else if (strstr(linea_leida, "modo_giro_mapa") != NULL )
			{
				sscanf(linea_leida, "modo_giro_mapa=%d", &(mapa_a_leer.TipoGiro));
				modo_giro_mapa_ok = true;
				#ifdef DEBUG_INFO
				printf("Leer info archivo: Linea %d, Modo Giro Mapa = %d\n", linea, mapa_a_leer.TipoGiro);
				#endif
			}
			else if (strstr(linea_leida, "angulo_max") != NULL )
			{
				if ( sscanf(linea_leida, "angulo_max=%f", &(mapa_a_leer.AnguloMax) ) == 1 )
				{
					angulo_max_ok = true;
					#ifdef DEBUG_INFO
					printf("Leer info archivo: Linea %d, angulo_max=%f\n", linea, mapa_a_leer.AnguloMax );
					#endif
				}
				else
				{	
					printf("Leer info archivo: Linea %d, angulo_max --> No se han podido leer el valor.\n", linea);
					exit(-1);
				}
			}
			else if (strstr(linea_leida, "gravedad") != NULL )
			{
				if ( sscanf(linea_leida, "gravedad=%f", &(mapa_a_leer.Gravedad) ) == 1 )
				{
					gravedad_ok = true;
					#ifdef DEBUG_INFO
					printf("Leer info archivo: Linea %d, gravedad=%f\n", linea, mapa_a_leer.Gravedad );
					#endif
				}
				else
				{	
					printf("Leer info archivo: Linea %d, gravedad --> No se han podido leer el valor.\n", linea);
					exit(-1);
				}
			}
			else if (strstr(linea_leida, "tiempo_cuenta_atras") != NULL )
			{
				if ( sscanf(linea_leida, "tiempo_cuenta_atras=%d", &(mapa_a_leer.SegundosCuentaAtras) ) == 1 )
				{
					cuenta_atras_presente = true;
					mapa_a_leer.CuentaAtras = true;
					#ifdef DEBUG_INFO
					printf("Leer info archivo: Linea %d, tiempo_cuenta_atras=%d\n", linea, mapa_a_leer.SegundosCuentaAtras );
					#endif
				}
				else
				{	
					printf("Leer info archivo: Linea %d, tiempo_cuenta_atras --> No se ha podido leer el valor.\n", linea);
					exit(-1);
				}
			}
			else if (strstr(linea_leida, "imagen_desc_menu") != NULL )
			{
				if ( sscanf(linea_leida, "imagen_desc_menu=%s", mapa_a_leer.RutaImagenDescMenu ) == 1 )
				{
					mapa_a_leer.RutaImagenDescMenu_Presente = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, imagen_desc_menu=%s\n", linea, mapa_a_leer.RutaImagenDescMenu );
					#endif
				}
				else
				{	
					printf("Linea %d, imagen_desc_menu --> No se han podido leer el valor.\n", linea);
					exit(-1);
				}
			}
			//////////////////////////////////////////////////////////////////////////////
			// Seguir añadiendo nuevas opciones aqui --- New options must be added here
			//////////////////////////////////////////////////////////////////////////////
			else
			{
				#ifdef DEBUG_INFO
				printf("Linea %d, expresion no reconocida, se ignora.\n", linea);
				#endif
			}
		}
	}
	#ifdef DEBUG_INFO
	printf("Leer info archivo: Mapa %s leido. Continuamos.\n", nombre_archivo );
	#endif
	return mapa_a_leer;
}

//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////

struct ConjuntoMapas* LeerConjuntosMapasJuego ( int* argumento_numero_conjunto_mapas )			// Read Game Level Sets
{
	// Esta funcion lee e interpreta el fichero "/maps/levelsets_list", que incluye la lista de todos los conjuntos de mapas del juego
	// Modifica el numero de conjunto de mapas, reserva memoria dinamica para la lista de conjuntos de mapas
	// Modifica el numero del conjunto de mapas segun lo leido en el fichero

	// This function reads the file "/maps/levelsets_list", that includes the list of all game level sets.
	// Modifies the level set total number and allocates dynamic memory for all level sets
	// Modifies the number of map levels accordint to the information read on the file.
	struct ConjuntoMapas* array_conjunto_mapas = NULL;
	char linea_leida[200];
	int linea = 0;
	int numero_conjunto_mapas = 0;
	FILE *archivo;
	bool vector_conjuntos_mapas_inicializado = false;
	int conjunto_mapa_actual, i;
	// bool conjunto_mapa_ok[LIMITE_CONJUNTOS_MAPAS];


	archivo = fopen( "maps/levelsets_list", "r" );
	if (archivo == NULL)
	{
		printf("Leer conjunto mapas juego: Error: No se puede abrir el archivo \"maps/levelsets_list\" \n" );
		exit(-1);
	}
	while ( (fgets(linea_leida, 200, archivo)) != NULL )
	{
		linea++;
		#ifdef DEBUG_INFO
		printf("Leer conjunto mapas juego: Linea %d leida\n", linea );
		#endif
		if ( (linea_leida[0] == '#') || (linea_leida[0] == '\n') || (strlen(linea_leida)==0) )	//lo de ver si está vacia no funciona
		{
			//Es un comentario, no hacer nada --- This is a remark, nothing has to be done
			#ifdef DEBUG_INFO
			printf("Leer conjunto mapas juego: Linea %d es un comentario\n", linea );
			#endif
		}
		else
		{
			if (strstr(linea_leida, "num_levelsets") != NULL )
			{
				sscanf(linea_leida, "num_levelsets=%d", &(numero_conjunto_mapas));
				#ifdef DEBUG_INFO
				printf("Leer conjunto mapas juego: Linea %d, Numero conjuntos mapas = %d\n", linea, numero_conjunto_mapas);
				#endif
				// Reserva el vector de conjuntos de mapas --- Allocate memory for array of level sets
				if ( vector_conjuntos_mapas_inicializado ==  true )
				{
					printf( "Leer conjunto mapas juego: Error: ¿Dos declaraciones de numero de conjuntos de mapas?\n");
					exit(-1);
				}
				else
				{
					array_conjunto_mapas = calloc(numero_conjunto_mapas, sizeof(struct ConjuntoMapas));
					vector_conjuntos_mapas_inicializado = true;
					#ifdef DEBUG_INFO
					printf("Leer conjunto mapas juego: Memoria reservada para %d conjuntos de mapas\n", numero_conjunto_mapas);
					#endif
					// Inicializamos todos los "OK" a false --- Initialize all "OK" flags to false
					for (i=0; i<numero_conjunto_mapas; i++)
					{
						array_conjunto_mapas[i].dir_definido_OK = false;
						array_conjunto_mapas[i].desc_definido_OK = false;
						array_conjunto_mapas[i].imag_definido_OK = false;
					}
				}
			}
			else if (strstr(linea_leida, "directorio") != NULL )
			{
				if (vector_conjuntos_mapas_inicializado==false)
				{
					printf("Leer conjunto mapas juego: La declaracion de los directorios debe ir después de especificar el numero de conjuntos de mapas\n");
					exit(-1);
				}
				else
				{
					char cadena_aux[255]; char* token;
					sscanf(linea_leida, "directorio[%d]",&conjunto_mapa_actual); // Leemos primero el conjunto actual, para poder hacer el direccionamiento en las siguientes instrucciones --- Read current segment first, in order to make the correct addressing on the following lines.
					if ( conjunto_mapa_actual >= numero_conjunto_mapas)
					{
						printf("Leer conjunto mapas juego: Se ha declarado un conjunto de mapas %d, que no cabe en el numero de conjuntos de mapas declarado %d.\n", conjunto_mapa_actual, numero_conjunto_mapas );
						printf("Nota: se empieza a contar desde el indice 0\n");
						exit(-1);
					}

					strcpy( cadena_aux, linea_leida);
					token = strtok( cadena_aux, "=" ); token = strtok( NULL, "=" );
					strcpy( array_conjunto_mapas[conjunto_mapa_actual].Directorio, token);
					Eliminar_NewLine_En_FinalCadena( array_conjunto_mapas[conjunto_mapa_actual].Directorio );
					array_conjunto_mapas[conjunto_mapa_actual].dir_definido_OK = true;
					#ifdef DEBUG_INFO
					printf("Leer conjunto mapas juego: Linea %d, directorio[%d] = %s\n", linea, conjunto_mapa_actual, array_conjunto_mapas[conjunto_mapa_actual].Directorio);
					#endif
				}
			}
			else if (strstr(linea_leida, "descripcion") != NULL )
			{
				if (vector_conjuntos_mapas_inicializado==false)
				{
					printf("Leer conjunto mapas juego: La declaracion de las descripciones debe ir después de especificar el numero de conjuntos de mapas\n");
					exit(-1);
				}
				else
				{
					char cadena_aux[255]; char* token;
					sscanf(linea_leida, "descripcion[%d]",&conjunto_mapa_actual); // Leemos primero el conjunto actual, para poder hacer el direccionamiento en las siguientes instrucciones --- Read current segment first, in order to make the correct addressing on the following lines.
					if ( conjunto_mapa_actual >= numero_conjunto_mapas)
					{
						printf("Leer conjunto mapas juego: Se ha declarado un conjunto de mapas %d, que no cabe en el numero de conjuntos de mapas declarado %d.\n", conjunto_mapa_actual, numero_conjunto_mapas );
						printf("Nota: se empieza a contar desde el indice 0\n");
						exit(-1);
					}

					strcpy( cadena_aux, linea_leida);
					token = strtok( cadena_aux, "=" ); token = strtok( NULL, "=" );
					strcpy( array_conjunto_mapas[conjunto_mapa_actual].Descripcion, token);
					Eliminar_NewLine_En_FinalCadena( array_conjunto_mapas[conjunto_mapa_actual].Descripcion );
					array_conjunto_mapas[conjunto_mapa_actual].desc_definido_OK = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, descripcion[%d] = %s\n", linea, conjunto_mapa_actual, array_conjunto_mapas[conjunto_mapa_actual].Descripcion);
					#endif
				}
			}
			else if (strstr(linea_leida, "ruta_imagen") != NULL )
			{
				if (vector_conjuntos_mapas_inicializado==false)
				{
					printf("Leer conjunto mapas juego: La declaracion de las descripciones debe ir después de especificar el numero de conjuntos de mapas\n");
					exit(-1);
				}
				else
				{
					char cadena_aux[255]; char* token;
					sscanf(linea_leida, "ruta_imagen[%d]",&conjunto_mapa_actual); // Leemos primero el conjunto actual, para poder hacer el direccionamiento en las siguientes instrucciones --- Read current segment first, in order to make the correct addressing on the following lines.
					if ( conjunto_mapa_actual >= numero_conjunto_mapas)
					{
						printf("Leer conjunto mapas juego: Se ha declarado un conjunto de mapas %d, que no cabe en el numero de conjuntos de mapas declarado %d.\n", conjunto_mapa_actual, numero_conjunto_mapas );
						printf("Nota: se empieza a contar desde el indice 0\n");
						exit(-1);
					}
					if ( sscanf(linea_leida, "ruta_imagen[%d]=%s", &conjunto_mapa_actual, array_conjunto_mapas[conjunto_mapa_actual].RutaImagen ) == 2 )
					{
						array_conjunto_mapas[conjunto_mapa_actual].imag_definido_OK = true;
						#ifdef DEBUG_INFO
						printf("Linea %d, ruta_imagen[%d] = %s\n", linea, conjunto_mapa_actual, array_conjunto_mapas[conjunto_mapa_actual].RutaImagen);
						#endif
					}
					else
					{	
						printf("Linea %d, ruta_imagen --> No se han podido leer el valor.\n", linea);
						exit(-1);
					}

				}
			}
		}
	}
	// Verificamos que estan todos los datos que deben estar --- Check if all required data is present
	for (i=0; i<numero_conjunto_mapas; i++)
	{
		if (array_conjunto_mapas[i].dir_definido_OK==false)
		{
			printf("Directorio para conjunto de mapas %d no definido\n",i);
			exit(-1);
		}
		if (array_conjunto_mapas[i].desc_definido_OK==false)
		{
			printf("Directorio para conjunto de mapas %d no definido\n",i);
			exit(-1);
		}	
		if (array_conjunto_mapas[i].imag_definido_OK==false)
		{
			printf("Directorio para conjunto de mapas %d no definido\n",i);
			exit(-1);
		}
	}

	// Devolvemos los argumentos --- Return argments
	*argumento_numero_conjunto_mapas = numero_conjunto_mapas;
	return array_conjunto_mapas;	
}



struct InfoMapas* LeerInfoMapasDeConjunto( char* nombre_directorio, int* argumento_num_mapas_en_conjunto )	// Read level info from a level set
{
	// Esta funcion lee e interpreta el fichero "/maps/nombre_directorio/level_list", que incluye la lista de niveles del conjunto
	// Modifica el numero de niveles, reserva memoria dinamica para la lista de niveles
	// Además, lee la información de cada mapa

	// This function reads the file "/maps/directory name/level_list", that includes the level list from the level set
	// Modifies the level number, allocates dynamic memory for the level set
	// Also, it reads information for each level on the level set

	// char* rutas_mapas_leidas[255];
	struct InfoMapas* info_mapas_en_conjunto = NULL;
	char linea_leida[200];
	int linea = 0;
	int numero_mapas = 0;
	char ruta_lista[255];

	FILE *archivo;
	bool vector_mapas_dentro_de_conjunto_inicializado = false;
	int mapa_actual, i;

	sprintf( ruta_lista, "maps/%s/level_list" , nombre_directorio );

	archivo = fopen( ruta_lista, "r" );
	if (archivo == NULL)
	{
		printf("Leer conjunto mapas %s: Error: No se puede abrir el archivo \"%s\" \n", nombre_directorio, ruta_lista );
		exit(-1);
	}
	while ( (fgets(linea_leida, 200, archivo)) != NULL )
	{
		linea++;
		#ifdef DEBUG_INFO
		printf("Leer conjunto mapas %s: Linea %d leida\n", nombre_directorio, linea );
		#endif
		if ( (linea_leida[0] == '#') || (linea_leida[0] == '\n') || (strlen(linea_leida)==0) )	//lo de ver si está vacia no funciona
		{
			//Es un comentario, no hacer nada --- This is a remark, nothing has to be done
			#ifdef DEBUG_INFO
			printf("Leer conjunto mapas %s: Linea %d es un comentario\n", nombre_directorio, linea );
			#endif
		}
		else
		{
			if (strstr(linea_leida, "num_levels") != NULL )
			{
				sscanf(linea_leida, "num_levels=%d", &(numero_mapas));
				#ifdef DEBUG_INFO
				printf("Leer conjunto mapas %s: Linea %d, Numero mapas = %d\n", nombre_directorio, linea, numero_mapas);
				#endif
				// Reserva el vector de información de mapas --- Allocates mnemory for map information array
				if ( vector_mapas_dentro_de_conjunto_inicializado ==  true )
				{
					printf( "Leer conjunto mapas %s: Error: ¿Dos declaraciones de numero de conjuntos de mapas?\n", nombre_directorio);
					exit(-1);
				}
				else
				{
					info_mapas_en_conjunto = calloc(numero_mapas, sizeof(struct InfoMapas));
					vector_mapas_dentro_de_conjunto_inicializado = true;
					#ifdef DEBUG_INFO
					printf("Leer conjunto mapas %s: Memoria reservada para %d conjuntos de mapas\n", nombre_directorio, numero_mapas);
					#endif
					// Inicializamos todos los "OK" a false --- Initialize all "OK" flag to false
					for (i=0; i<numero_mapas; i++)
					{
						info_mapas_en_conjunto[i].mapa_definido_OK = false;
					}
				}
			}
			else if (strstr(linea_leida, "ruta_mapa") != NULL )
			{
				if (vector_mapas_dentro_de_conjunto_inicializado==false)
				{
					printf("Leer conjunto mapas %s: La declaracion de las rutas debe ir después de especificar el numero de mapas\n", nombre_directorio);
					exit(-1);
				}
				else
				{
					char cadena_aux[255]; char* token;
					sscanf(linea_leida, "ruta_mapa[%d]",&mapa_actual); // Leemos primero el conjunto actual, para poder hacer el direccionamiento en las siguientes instrucciones --- Read current segment first, in order to make the correct addressing on the following lines.
					if ( mapa_actual >= numero_mapas)
					{
						printf("Leer conjunto mapas %s: Se ha declarado un mapa %d, que no cabe en el numero de mapas declarado %d.\n", mapa_actual, numero_mapas );
						printf("Nota: se empieza a contar desde el indice 0\n");
						exit(-1);
					}
					if ( sscanf(linea_leida, "ruta_mapa[%d]=%s", &mapa_actual, info_mapas_en_conjunto[mapa_actual].RutaMapa ) == 2 )
					{
						info_mapas_en_conjunto[mapa_actual].mapa_definido_OK = true;
						#ifdef DEBUG_INFO
						printf("Leer conjunto mapas %s: Linea %d, ruta_mapa[%d] = %s\n", nombre_directorio, linea, mapa_actual, info_mapas_en_conjunto[mapa_actual].RutaMapa);
						#endif
					}
					else
					{	
						printf("Leer conjunto mapas %s: Linea %d, ruta_mapa --> No se ha podido leer el valor.\n", nombre_directorio, linea);
						exit(-1);
					}

				}
			}
		}
	}

	// Verificamos que estan todos los datos que deben estar --- Check if all required data is present
	for (i=0; i<numero_mapas; i++)
	{
		if (info_mapas_en_conjunto[i].mapa_definido_OK == false)
		{
			printf("Leer conjunto mapas %s: Ruta mapa  para conjunto de mapas %d no definido\n", nombre_directorio, i);
			exit(-1);
		}
	}

	// Leemos la informacion de cada mapa --- Read each level information
	for (i=0; i<numero_mapas; i++)
	{
		info_mapas_en_conjunto[i].DatosMapa = LeerInfoArchivo( info_mapas_en_conjunto[i].RutaMapa );
	}

	// Devolvemos los argumentos --- Return arguments
	*argumento_num_mapas_en_conjunto = numero_mapas;
	return info_mapas_en_conjunto;
}



void Eliminar_NewLine_En_FinalCadena (char* cadena_original)		// Remove "NewLine" from string's end
{
	char *pos;

	if ((pos = strchr( cadena_original, '\n' )) != NULL )
	{
		*pos = '\0';
	}
}


