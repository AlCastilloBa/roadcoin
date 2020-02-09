
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "maps.h"


 #define DEBUG_INFO

struct mapa CargarMapaDesdeArchivo( char *nombre_archivo )
{
	struct mapa mapa_a_cargar;
	struct punto punto_auxiliar;
	char linea_leida[200];
	bool nombre_ok=false, num_segmentos_ok=false, modo_giro_mapa_ok=false, punto_giro_ok=false, angulo_max_ok=false, pos_inicial_ok=false, gravedad_ok=false, imagen_moneda_ok=false, imagen_fondo_ok=false; 
	bool ruta_musica_ok=false;
	bool escala_presente=false, num_pinball_bumpers_presente=false;
	float escala;
	bool fondo_giratorio_ok=false, imagen_fondo_giratorio_ok=false, pos_fondo_giratorio_ok=false, centro_giro_fondo_giratorio_ok=false ;
	bool segmento_ok[LIMITE_SEGMENTOS];
	bool bumper_ok[LIMITE_BUMPERS];
	bool vector_segmentos_inicializado =false, vector_bumpers_inicializado=false;
	FILE *archivo;
	int linea=0;
	int segmento_actual, i, otro_segmento_actual, bumper_actual;

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
							// Valor no válido, se toma el valor por defecto
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
			else if (strstr(linea_leida, "num_bumpers") != NULL )	//Nuevo 9/2/2020
			{
				sscanf(linea_leida, "num_bumpers=%d", &(mapa_a_cargar.NumeroPinballBumpers));
				num_pinball_bumpers_presente = true;
				#ifdef DEBUG_INFO
				printf("Linea %d, Numero bumpers = %d\n", linea, mapa_a_cargar.NumeroPinballBumpers);
				#endif
				// Reserva el vector de bumpers
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
					sscanf(linea_leida, "bumper[%d]",&bumper_actual); // Leemos primero el bumper actual, para poder hacer el direccionamiento en las siguientes instrucciones
					if ( sscanf(linea_leida, "bumper[%d]=((%lf,%lf),%f,%f)", 		&bumper_actual, 
														&(mapa_a_cargar.Bumpers[bumper_actual].centro.x),
														&(mapa_a_cargar.Bumpers[bumper_actual].centro.y),
														&(mapa_a_cargar.Bumpers[bumper_actual].radio),
														&(mapa_a_cargar.Bumpers[bumper_actual].velocidad_salida)            ) == 5 )
					{
						bumper_ok[bumper_actual] = true;
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
			///////////////////////////////////////////////////////////
			// Seguir añadiendo nuevas opciones aqui
			//////////////////////////////////////////////////////////
			else
			{
				printf("Linea %d, expresion no reconocida, se ignora.\n", linea);
			}

		}

	}

	fclose(archivo);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Verificamos que lo tengamos todo (que al archivo no le faltasen datos)
	if (nombre_ok == false) 		printf("Falta el nombre del mapa.\n");
	if (num_segmentos_ok == false) 		printf("Falta el número de segmentos.\n");	
	if (modo_giro_mapa_ok == false) 	printf("Falta el modo de giro.\n");
	if (mapa_a_cargar.TipoGiro == punto_fijo &&  punto_giro_ok == false)
					 	printf("Falta el punto de giro (y el mapa gira alrededor de un punto fijo).\n");	
	if (angulo_max_ok == false) 		printf("Falta el angulo máximo.\n");
	if (pos_inicial_ok == false) 		printf("Falta el la posición inicial.\n");
	if (gravedad_ok == false) 		printf("Falta la gravedad.\n");
	if (imagen_moneda_ok == false)		printf("Falta el la ruta de la imagen de la moneda.\n");
	if (imagen_fondo_ok == false)		printf("Falta el la ruta de la imagen del fondo.\n");

	if (fondo_giratorio_ok == false )	printf("Falta especificar si hay o no un fondo giratorio.\n");
	if (mapa_a_cargar.HayFondoGiratorio == true && imagen_fondo_giratorio_ok == false )
						printf("Falta el la ruta de la imagen del fondo giratorio (y hay fondo giratorio).\n");
	if (mapa_a_cargar.HayFondoGiratorio == true && pos_fondo_giratorio_ok == false )
						printf("Falta la posición del fondo giratorio (y hay fondo giratorio).\n");
	if (mapa_a_cargar.HayFondoGiratorio == true && centro_giro_fondo_giratorio_ok == false )
						printf("Falta la posición del centro de giro del fondo giratorio (y hay fondo giratorio).\n");
	if ( ruta_musica_ok == false )
						printf("Falta especificar la ruta de la musica.\n");


	if (nombre_ok==false || num_segmentos_ok==false || modo_giro_mapa_ok==false || (mapa_a_cargar.TipoGiro == punto_fijo && punto_giro_ok==false ) || 
		angulo_max_ok==false || pos_inicial_ok==false || gravedad_ok==false || imagen_moneda_ok==false || imagen_fondo_ok==false || 
		fondo_giratorio_ok==false || (mapa_a_cargar.HayFondoGiratorio == true && imagen_fondo_giratorio_ok == false) || (mapa_a_cargar.HayFondoGiratorio == true && pos_fondo_giratorio_ok == false ) || (mapa_a_cargar.HayFondoGiratorio == true && centro_giro_fondo_giratorio_ok == false ) ||
		ruta_musica_ok == false )
	{
		printf("Archivo de mapa incompleto\n");
		exit(-1);
	}
	for (i=0; i<mapa_a_cargar.NumeroSegmentos; i++)
	{
		if (segmento_ok[i]==false)
		{
			printf("Segmento %d no definido\n");
			exit(-1);
		}
	}
	if ( num_pinball_bumpers_presente )
	{
		for (i=0; i<mapa_a_cargar.NumeroPinballBumpers; i++)
		{
			if (bumper_ok[i]==false)
			{
				printf("Bumper %d no definido\n");
				exit(-1);
			}
		}		
	}
	else
	{
		mapa_a_cargar.NumeroPinballBumpers=0;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Para simplificar los calculos futuros, vamos a hacer que el punto de inicio (start) siempre esté a la izquierda.
	for (segmento_actual=0; segmento_actual<mapa_a_cargar.NumeroSegmentos; segmento_actual++)
	{
		if ( mapa_a_cargar.Mapa[segmento_actual].start.x > mapa_a_cargar.Mapa[segmento_actual].end.x )
		{
			// Intercambiamos start y end
			punto_auxiliar = mapa_a_cargar.Mapa[segmento_actual].end;
			mapa_a_cargar.Mapa[segmento_actual].end = mapa_a_cargar.Mapa[segmento_actual].start;
			mapa_a_cargar.Mapa[segmento_actual].start = punto_auxiliar;
			#ifdef DEBUG_INFO
			printf("Se han intercambiado start y end en segmento %d\n", segmento_actual);
			#endif
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//  Detectamos adyacencias
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
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Aplicación de escala al mapa
	if ( escala_presente )
	{
		// Segmentos
		for (segmento_actual=0; segmento_actual<mapa_a_cargar.NumeroSegmentos; segmento_actual++)
		{
			mapa_a_cargar.Mapa[segmento_actual].start.x = mapa_a_cargar.Mapa[segmento_actual].start.x * escala;
			mapa_a_cargar.Mapa[segmento_actual].start.y = mapa_a_cargar.Mapa[segmento_actual].start.y * escala;
			mapa_a_cargar.Mapa[segmento_actual].end.x = mapa_a_cargar.Mapa[segmento_actual].end.x * escala;
			mapa_a_cargar.Mapa[segmento_actual].end.y = mapa_a_cargar.Mapa[segmento_actual].end.y * escala;
		}
		// Posicion inicial moneda
		mapa_a_cargar.PuntoInicialMoneda.x = mapa_a_cargar.PuntoInicialMoneda.x * escala;
		mapa_a_cargar.PuntoInicialMoneda.y = mapa_a_cargar.PuntoInicialMoneda.y * escala;
		// Centro de giro
		mapa_a_cargar.PuntoGiroFijo.x = mapa_a_cargar.PuntoGiroFijo.x * escala;
		mapa_a_cargar.PuntoGiroFijo.y = mapa_a_cargar.PuntoGiroFijo.y * escala;
		// Fondo giratorio
		mapa_a_cargar.CentroGiroFondoGiratorio.x = mapa_a_cargar.CentroGiroFondoGiratorio.x * escala;
		mapa_a_cargar.CentroGiroFondoGiratorio.y = mapa_a_cargar.CentroGiroFondoGiratorio.y * escala;
		mapa_a_cargar.Pos_x_izquierda_fondo_giratorio = mapa_a_cargar.Pos_x_izquierda_fondo_giratorio * escala;
		mapa_a_cargar.Pos_y_arriba_fondo_giratorio = mapa_a_cargar.Pos_y_arriba_fondo_giratorio * escala;
		mapa_a_cargar.Pos_x_derecha_fondo_giratorio = mapa_a_cargar.Pos_x_derecha_fondo_giratorio * escala;
		mapa_a_cargar.Pos_y_abajo_fondo_giratorio = mapa_a_cargar.Pos_y_abajo_fondo_giratorio * escala;
	}

	#ifdef DEBUG_INFO
	printf("Mapa %s cargado. Continuamos.\n", nombre_archivo );
	#endif
	return mapa_a_cargar;
}
