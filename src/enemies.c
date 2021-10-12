#include <stdio.h>
#include <string.h>
#include <math.h>

#include "maps.h"
#include "enemies.h"
#include "graphics.h"
#include "physics.h"
#include "geometry.h"

// Si definido DEBUG_INFO, mostrar textos de informacion por la terminal (hace el programa más lento)
// If DEBUG_INFO is defined, information texts will be shown via terminal (but the program will run slower)
// #define DEBUG_INFO



void Inicializar_Tipos_Enemigos_En_Mapa( struct tipos_enemigos_usados_en_mapa* lista_tipos)
{
	int contador_tipos;

	for ( contador_tipos=0; contador_tipos < MAX_TIPOS_ENEMIGOS_POR_MAPA; contador_tipos++ )
	{
		lista_tipos[contador_tipos].tipo_utilizado = false;
		lista_tipos[contador_tipos].ID_unico_tipo_enemigo = 0;
	}

}


int Clasificar_Tipos_Unicos_Enemigos( struct tipos_enemigos_usados_en_mapa *lista_tipos, struct enemigo_ruta *lista_enemigos, int numero_enemigos )
{
	// Esta función recorre los enemigos en memoria, y los clasifica por tipos, dando un numero unico.
	// La función devuelve el numero de tipos de enemigos encontrados.

	// This function reads enemies stored in memory and classifies them accordint to types, assigning them an unique ID number for each type.
	// This function returns the amount of enemy types found.

	int contador_tipos, contador_enemigos, contador_busqueda_tipos;
	bool tipo_ya_estaba_utilizado;

	contador_tipos = 0;

	for ( contador_enemigos=0; contador_enemigos < numero_enemigos; contador_enemigos++ )
	{
		if ( lista_enemigos[contador_enemigos].definido_OK == true )
		{
			if ( contador_tipos == 0)
			{
				// Es el primer tipo encontrado --- This is the first type found
				lista_tipos[contador_tipos].tipo_utilizado = true;
				lista_tipos[contador_tipos].ID_unico_tipo_enemigo = contador_tipos;
				strcpy(lista_tipos[contador_tipos].nombre_tipo, lista_enemigos[contador_enemigos].nombre_tipo);
				lista_enemigos[contador_enemigos].ID_unico_tipo_enemigo = contador_tipos;
				contador_tipos++;
			}
			else
			{
				// Como no es el primer tipo encontrado, tenemos que ver si se repiten
				// Since this is no longer the first type found, we must check if this type is already classified
				tipo_ya_estaba_utilizado = false;
				for ( contador_busqueda_tipos = 0; contador_busqueda_tipos < contador_tipos; contador_busqueda_tipos++ )
				{
					if ( strcmp( lista_enemigos[contador_enemigos].nombre_tipo , lista_tipos[contador_busqueda_tipos].nombre_tipo ) == 0 ) /*Igual*/ 
					{
						
						lista_enemigos[contador_enemigos].ID_unico_tipo_enemigo = contador_busqueda_tipos;
						tipo_ya_estaba_utilizado = true;
						break;
					}
				}
				if ( !tipo_ya_estaba_utilizado )
				{
					// Lo clasificamos como nuevo tipo
					// We classify as a new type
					lista_tipos[contador_tipos].tipo_utilizado = true;
					lista_tipos[contador_tipos].ID_unico_tipo_enemigo = contador_tipos;
					strcpy(lista_tipos[contador_tipos].nombre_tipo, lista_enemigos[contador_enemigos].nombre_tipo);
					lista_enemigos[contador_enemigos].ID_unico_tipo_enemigo = contador_tipos;
					contador_tipos++;
				}
			}
		}
		else
		{
			// Si el enemigo actual no está definido, ninguno de los siguientes deberia estarlo
			// Entonces salimos de la funcion
			// If the current enemy is not defined, then none of the following types should be defined either.
			// Then we exit the function
			return contador_tipos;
		}
	}
	return contador_tipos;
}


void Lista_Tipos_Enemigos_Unicos( struct tipos_enemigos_usados_en_mapa *lista_tipos )		// List of unique enemy types
{
	int contador_tipos;

	printf("\n");
	printf("---------------------------------------------\n");
	printf("Lista de tipos de enemigos utilizados\n");
	printf("---------------------------------------------\n");

	for ( contador_tipos = 0; contador_tipos < MAX_TIPOS_ENEMIGOS_POR_MAPA; contador_tipos++ )
	{
		if ( lista_tipos[contador_tipos].tipo_utilizado )
		{
			printf("Tipo %d: ID=%d, nombre=%s \n", contador_tipos, lista_tipos[contador_tipos].ID_unico_tipo_enemigo, lista_tipos[contador_tipos].nombre_tipo );
		}
	}
	printf("---------------------------------------------\n");
	printf("\n");
}



void Leer_Ficheros_Definicion_Tipos_Enemigos( struct tipos_enemigos_usados_en_mapa *lista_tipos )	// Read enemy types definition files
{
	int contador_tipos, linea;
	FILE *archivo_tipo_enemigo;
	char nombre_archivo_enemigo[255];
	char linea_leida[200];
	bool sprite_sheet_ok=false, num_anim_sprites_ok=false, radio_ok=false, sprite_size_ok=false, frames_per_animation_ok=false, move_speed_ok=false, rot_speed_ok=false;

	for ( contador_tipos = 0; contador_tipos < MAX_TIPOS_ENEMIGOS_POR_MAPA; contador_tipos++ )
	{
		if ( lista_tipos[contador_tipos].tipo_utilizado )
		{
			strcpy( nombre_archivo_enemigo , "enemies/");
			strcat( nombre_archivo_enemigo , lista_tipos[contador_tipos].nombre_tipo );
			#ifdef DEBUG_INFO
			printf("Vamos a leer el archivo %s para el tipo de enemigo con ID=%d\n", nombre_archivo_enemigo, contador_tipos );
			#endif
			archivo_tipo_enemigo = fopen( nombre_archivo_enemigo, "r" );
			if (archivo_tipo_enemigo == NULL)
			{
				printf("Error: No se puede abrir el archivo de tipo enemigo %s \n", nombre_archivo_enemigo );
				exit(-1);
			}

			#ifdef DEBUG_INFO
			printf("Leyendo archivo de tipo enemigo numero %d, nombre %s \n", contador_tipos, nombre_archivo_enemigo );
			#endif

			linea = 0;

			while ( (fgets(linea_leida, 200, archivo_tipo_enemigo)) != NULL )
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
					if (strstr(linea_leida, "sprite_sheet") != NULL )
					{
						char cadena_aux[200]; char* token;
						strcpy( cadena_aux, linea_leida);
						token = strtok( cadena_aux, "=" ); token = strtok( NULL, "=" );
						strcpy( lista_tipos[contador_tipos].RutaSpriteSheet, token);
						Eliminar_NewLine_En_FinalCadena( lista_tipos[contador_tipos].RutaSpriteSheet ); 	// Nuevo 19/4/2020
						sprite_sheet_ok = true;
						#ifdef DEBUG_INFO
						printf("Linea %d, sprite_sheet = %s\n", linea, lista_tipos[contador_tipos].RutaSpriteSheet );
						#endif
					}
					else if (strstr(linea_leida, "num_anim_sprites") != NULL )
					{
						if ( sscanf(linea_leida, "num_anim_sprites=%d", &(lista_tipos[contador_tipos].num_anim_sprites) ) == 1 )
						{
							num_anim_sprites_ok = true;
							#ifdef DEBUG_INFO
							printf("Linea %d, num_anim_sprites=%d\n", linea, lista_tipos[contador_tipos].num_anim_sprites );
							#endif
						}
						else
						{	
							printf("Linea %d, num_anim_sprites --> No se han podido leer el valor.\n", linea);
							exit(-1);
						}

					}
					else if (strstr(linea_leida, "radio") != NULL )
					{
						if ( sscanf(linea_leida, "radio=%d", &(lista_tipos[contador_tipos].radio) ) == 1 )
						{
							radio_ok = true;
							#ifdef DEBUG_INFO
							printf("Linea %d, radio=%d\n", linea, lista_tipos[contador_tipos].radio );
							#endif
						}
						else
						{	
							printf("Linea %d, radio --> No se han podido leer el valor.\n", linea);
							exit(-1);
						}
					}
					else if (strstr(linea_leida, "sprite_size") != NULL )
					{
						if ( sscanf(linea_leida, "sprite_size=%d", &(lista_tipos[contador_tipos].sprite_size) ) == 1 )
						{
							sprite_size_ok = true;
							#ifdef DEBUG_INFO
							printf("Linea %d, sprite_size=%d\n", linea, lista_tipos[contador_tipos].sprite_size );
							#endif
						}
						else
						{	
							printf("Linea %d, sprite_size --> No se han podido leer el valor.\n", linea);
							exit(-1);
						}
					}
					else if (strstr(linea_leida, "frames_per_animation") != NULL )
					{
						if ( sscanf(linea_leida, "frames_per_animation=%d", &(lista_tipos[contador_tipos].frames_per_animation) ) == 1 )
						{
							frames_per_animation_ok = true;
							#ifdef DEBUG_INFO
							printf("Linea %d, frames_per_animation=%d\n", linea, lista_tipos[contador_tipos].frames_per_animation );
							#endif
						}
						else
						{	
							printf("Linea %d, frames_per_animation --> No se han podido leer el valor.\n", linea);
							exit(-1);
						}
					}
					else if (strstr(linea_leida, "move_speed") != NULL )
					{
						if ( sscanf(linea_leida, "move_speed=%d", &(lista_tipos[contador_tipos].move_speed) ) == 1 )
						{
							move_speed_ok = true;
							#ifdef DEBUG_INFO
							printf("Linea %d, move_speed=%d\n", linea, lista_tipos[contador_tipos].move_speed );
							#endif
						}
						else
						{	
							printf("Linea %d, move_speed --> No se han podido leer el valor.\n", linea);
							exit(-1);
						}
					}
					else if (strstr(linea_leida, "rot_speed") != NULL )
					{
						if ( sscanf(linea_leida, "rot_speed=%d", &(lista_tipos[contador_tipos].rot_speed) ) == 1 )
						{
							rot_speed_ok = true;
							#ifdef DEBUG_INFO
							printf("Linea %d, rot_speed=%d\n", linea, lista_tipos[contador_tipos].rot_speed );
							#endif
						}
						else
						{	
							printf("Linea %d, rot_speed --> No se han podido leer el valor.\n", linea);
							exit(-1);
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
			fclose( archivo_tipo_enemigo ); archivo_tipo_enemigo=NULL;
			
			////////////////////////////////////////////////////////////////////////////////////////////////
			// Verificamos que lo tengamos todo (que al archivo no le faltasen datos)
			// Check if we have all the required data (and the file lacks no essential data)
			if (sprite_sheet_ok == false)
			{
				printf("En fichero %s, falta la ruta del sprite sheet del enemigo.\n", nombre_archivo_enemigo);
			}
			if (num_anim_sprites_ok == false)
			{
				printf("En fichero %s, falta el numero de sprites de animacion del enemigo.\n", nombre_archivo_enemigo);
			}
			if (radio_ok == false)
			{
				printf("En fichero %s, falta el radio (tamaño enemigo) del enemigo.\n", nombre_archivo_enemigo);
			}
			if (sprite_size_ok == false)
			{
				printf("En fichero %s, falta el sprite_size (tamaño sprite) del enemigo.\n", nombre_archivo_enemigo);
			}
			if (frames_per_animation_ok == false)
			{
				printf("En fichero %s, falta el numero de frames por cada imagen de animacion del enemigo.\n", nombre_archivo_enemigo);
			}
			if (move_speed_ok == false)
			{
				printf("En fichero %s, falta el la velocidad lineal de movimiento del enemigo.\n", nombre_archivo_enemigo);
			}
			if (rot_speed_ok == false)
			{
				printf("En fichero %s, falta el la velocidad de rotacion del enemigo.\n", nombre_archivo_enemigo);
			}
			if (sprite_sheet_ok==false || num_anim_sprites_ok==false || radio_ok==false || sprite_size_ok==false || frames_per_animation_ok==false ||
				move_speed_ok==false || rot_speed_ok==false )
			{
				printf("Error: Archivo de definicion de enemigo %s esta incompleto\n", nombre_archivo_enemigo);
				exit(-1);
			}
		#ifdef DEBUG_INFO
		printf("Fichero de enemigo %s leido correctamente.\n", nombre_archivo_enemigo);
		#endif
		}
	}
	#ifdef DEBUG_INFO
	printf("Todos los ficheros de definicion de enemigos de este mapa han sido leidos. Continuamos.\n" );
	#endif
}


bool Inicializar_Texturas_Tipos_Enemigos( struct tipos_enemigos_usados_en_mapa *lista_tipos )		// Initialize enemy types textures
{
	int contador_tipos;
	int contador_sprites_animacion;
	bool exito=true;

	#ifdef DEBUG_INFO
	printf("Entrando en funcion Inicializar_Texturas_Tipos_Enemigos.\n" );
	#endif

	for ( contador_tipos = 0; contador_tipos < MAX_TIPOS_ENEMIGOS_POR_MAPA; contador_tipos++ )
	{
		if ( lista_tipos[contador_tipos].tipo_utilizado )
		{
			// Cargar textura de hoja de sprites --- Load sprite sheet texture
			lista_tipos[contador_tipos].sprite_sheet = CargaTextura( lista_tipos[contador_tipos].RutaSpriteSheet, 
							&(lista_tipos[contador_tipos].dim_sprite_sheet_X),
							&(lista_tipos[contador_tipos].dim_sprite_sheet_Y),
							true );
			if ( lista_tipos[contador_tipos].sprite_sheet == NULL )
			{
				printf( "Error: no se ha podido cargar la imagen del sprite sheet del enemigo %s, ruta %s\n", lista_tipos[contador_tipos].nombre_tipo ,lista_tipos[contador_tipos].RutaSpriteSheet ); 
				exito = false;
			}
			// Verificar tamaño de la imagen con respecto al resto de parametros de la animacion
			// Check image size with respect to the animation parameters
			if (  (lista_tipos[contador_tipos].dim_sprite_sheet_Y != lista_tipos[contador_tipos].sprite_size) || 
				(lista_tipos[contador_tipos].dim_sprite_sheet_X != lista_tipos[contador_tipos].sprite_size * lista_tipos[contador_tipos].num_anim_sprites) )
			{
				printf( "Error: enemigo %s, la imagen del sprite sheet (%s) mide %d x %d, pero deberia medir %d x %d \n", lista_tipos[contador_tipos].nombre_tipo ,
																	lista_tipos[contador_tipos].RutaSpriteSheet,
																	lista_tipos[contador_tipos].dim_sprite_sheet_X,
																	lista_tipos[contador_tipos].dim_sprite_sheet_Y,
																	lista_tipos[contador_tipos].sprite_size * lista_tipos[contador_tipos].num_anim_sprites,
																	lista_tipos[contador_tipos].sprite_size  ); 
				exito = false;	
			}

			// Calcular cuadros de los sprites de la animacion
			// Calculate animation squares
			for ( contador_sprites_animacion = 0 ; contador_sprites_animacion < lista_tipos[contador_tipos].num_anim_sprites ;  contador_sprites_animacion++ )
			{
				lista_tipos[contador_tipos].cuadro_sprite[contador_sprites_animacion].x = lista_tipos[contador_tipos].sprite_size * contador_sprites_animacion;
				lista_tipos[contador_tipos].cuadro_sprite[contador_sprites_animacion].y = 0;
				lista_tipos[contador_tipos].cuadro_sprite[contador_sprites_animacion].w = lista_tipos[contador_tipos].sprite_size;
				lista_tipos[contador_tipos].cuadro_sprite[contador_sprites_animacion].h = lista_tipos[contador_tipos].sprite_size;
			}
		}
	}
	#ifdef DEBUG_INFO
	printf("Texturas de tipos de enemigos inicializadas.\n");
	#endif

	return exito;
}

void Liberar_Memoria_Tipos_Enemigos( struct tipos_enemigos_usados_en_mapa *lista_tipos)		// Free memory of enemy types
{
	int contador_tipos;

	for ( contador_tipos = 0; contador_tipos < MAX_TIPOS_ENEMIGOS_POR_MAPA; contador_tipos++ )
	{
		// Liberar memoria de texturas de tipos de enemigos --- Free texture memory for enemy types
		SDL_DestroyTexture( lista_tipos[contador_tipos].sprite_sheet );  lista_tipos[contador_tipos].sprite_sheet = NULL;
		// Marcar como no definido (por precaucion, realmente no sería necesario) --- Mark as not defined (should be unnecesary)
		lista_tipos[contador_tipos].tipo_utilizado = false;
	}

}


bool Verificar_Datos_Enemigos( struct enemigo_ruta *lista_enemigos, int numero_enemigos )	// Verify enemy data
{
	int contador_enemigos, contador_waypoints;
	bool sin_errores = true;

	for (contador_enemigos = 0; contador_enemigos < numero_enemigos ; contador_enemigos++ )
	{
		if ( lista_enemigos[contador_enemigos].num_waypoints <= 0 )
		{
			printf( "Error: Enemigo %d, numero de waypoints (%d) es incorrecto.\n", contador_enemigos, lista_enemigos[contador_enemigos].num_waypoints );
			sin_errores = false;
		}

		// Verificar waypoints correctamente definidos --- Check if waypoints are correctly defined
		for (contador_waypoints = 0; contador_waypoints < lista_enemigos[contador_enemigos].num_waypoints ; contador_waypoints++ )
		{
			if ( lista_enemigos[contador_enemigos].waypoints[contador_waypoints].definido == false )
			{
				printf( "Error: Enemigo %d, waypoints %d no definido, pero numero de waypoints indicado deberia ser %d.\n", contador_enemigos, contador_waypoints, lista_enemigos[contador_enemigos].num_waypoints );
				sin_errores = false;
			}
		}

		if ( lista_enemigos[contador_enemigos].speed_multiplier < 0 )
		{
			printf( "Error: Enemigo %d, numero de waypoints (%d) es incorrecto.\n", contador_enemigos, lista_enemigos[contador_enemigos].num_waypoints );
			sin_errores = false;
		}
	}
	return sin_errores;
}




void Calcular_Numero_Waypoints_Enemigos( struct enemigo_ruta *lista_enemigos, int numero_enemigos )		// Calculate number of enemy waypoints
{
	int contador_enemigos, contador_waypoints;

	for (contador_enemigos = 0; contador_enemigos < numero_enemigos ; contador_enemigos++ )
	{
		for (contador_waypoints = 0; contador_waypoints < MAX_WAYPOINTS_POR_ENEMIGO ; contador_waypoints++ )
		{
			if ( lista_enemigos[contador_enemigos].waypoints[contador_waypoints].definido == false )
			{
				break;
			}
		}
		lista_enemigos[contador_enemigos].num_waypoints = contador_waypoints ;
		#ifdef DEBUG_INFO
		printf("Enemigo %d, num_waypoints=%d\n", contador_enemigos, contador_waypoints);
		#endif
	}
}



void Calcula_Nueva_Posicion_Enemigos( int numero_enemigos, 				// Calculate new position of enemies
					struct enemigo_ruta *lista_enemigos, 
					struct tipos_enemigos_usados_en_mapa *lista_tipos,
					struct punto *posic_actual_enemigos, 
					float* angulo_enemigos, 
					int* waypoint_destino,
					float tiempo_imagen )
{
	int contador_enemigos;
	float angulo_destino;
	struct vector_velocidad velocidad_avance;

	for (contador_enemigos = 0; contador_enemigos < numero_enemigos ; contador_enemigos++ )
	{
		#ifdef DEBUG_INFO
		printf("Actualizando posicion de enemigo ruta numero %d\n", contador_enemigos);
		printf("Waypoint destino = %d\n", waypoint_destino[contador_enemigos] );
		#endif
		// Calcular angulo de destino (en grados) --- Calculate destination angle (in degrees)
		angulo_destino = WrapAngle_0_360 (  atan2( lista_enemigos[contador_enemigos].waypoints[waypoint_destino[contador_enemigos]].waypoint.y - posic_actual_enemigos[contador_enemigos].y , 
					lista_enemigos[contador_enemigos].waypoints[waypoint_destino[contador_enemigos]].waypoint.x - posic_actual_enemigos[contador_enemigos].x) * 
					180/PI
					);
		#ifdef DEBUG_INFO
		printf("Angulo destino %f\n", angulo_destino);
		#endif
		// Calcular tolerancia (TODO esto se puede optimizar, no es necesario calcularlo siempre) --- Calculate tolerance
		lista_enemigos[contador_enemigos].tolerancia_wp_pos_cuad = lista_tipos[lista_enemigos[contador_enemigos].ID_unico_tipo_enemigo].move_speed *
									lista_enemigos[contador_enemigos].speed_multiplier * tiempo_imagen ;
		lista_enemigos[contador_enemigos].tolerancia_wp_pos_cuad *= lista_enemigos[contador_enemigos].tolerancia_wp_pos_cuad; /*Cuadrado*/

		lista_enemigos[contador_enemigos].tolerancia_wp_ang = lista_tipos[lista_enemigos[contador_enemigos].ID_unico_tipo_enemigo].rot_speed *
									lista_enemigos[contador_enemigos].speed_multiplier * tiempo_imagen ;

		#ifdef DEBUG_INFO
		printf("Tol_wp_pos_cuad=%f, Tol_wp_ang=%f \n", lista_enemigos[contador_enemigos].tolerancia_wp_pos_cuad, lista_enemigos[contador_enemigos].tolerancia_wp_ang );
		#endif

		#ifdef DEBUG_INFO		
		printf("Diferencia angulos=%f\n", fabs( angulo_enemigos[contador_enemigos] - angulo_destino ) );
		#endif
		// ¿La rotacion es necesaria? --- Does the enemy need to rotate?
		if ( fabs( angulo_enemigos[contador_enemigos] - angulo_destino ) >= lista_enemigos[contador_enemigos].tolerancia_wp_ang )
		{
			// Rotacion necesaria --- The enemy must rotate
			#ifdef DEBUG_INFO
			printf("Rotacion necesaria\n");
			#endif

			if ( WrapAngle_0_360( angulo_enemigos[contador_enemigos] - angulo_destino ) > 180 )
			{
				// Rotacion en sentido horario (incrementar angulo) --- Clockwise rotation
				angulo_enemigos[contador_enemigos] += lista_enemigos[contador_enemigos].tolerancia_wp_ang;
			}
			else
			{
				// Rotacion en sentido anti-horario (decrementar angulo) --- Counterclockwise rotation
				angulo_enemigos[contador_enemigos] -= lista_enemigos[contador_enemigos].tolerancia_wp_ang;
			}
			angulo_enemigos[contador_enemigos] =  WrapAngle_0_360 ( angulo_enemigos[contador_enemigos] );
		}
		else
		{
			#ifdef DEBUG_INFO
			printf("Rotacion no necesaria\n");
			#endif
			// Rotacion no necesaria, procedemos a moverlo linealmente --- Enemy does not need to rotate, it must move
			angulo_enemigos[contador_enemigos] = angulo_destino;

			velocidad_avance = VelocidadSobreRecta ( /*Modulo*/ lista_tipos[lista_enemigos[contador_enemigos].ID_unico_tipo_enemigo].move_speed * lista_enemigos[contador_enemigos].speed_multiplier,
								 /*Punto1*/ posic_actual_enemigos[contador_enemigos],
 								 /*Punto2*/ lista_enemigos[contador_enemigos].waypoints[waypoint_destino[contador_enemigos]].waypoint );

			posic_actual_enemigos[contador_enemigos] = Velocidad2Posicion( posic_actual_enemigos[contador_enemigos], 
											velocidad_avance, 
											tiempo_imagen );

			// ¿Se ha alcanzado el waypoint? --- Did the enemy reached the waypoint?
			if ( LongitudVector_Cuadrado( posic_actual_enemigos[contador_enemigos] , lista_enemigos[contador_enemigos].waypoints[waypoint_destino[contador_enemigos]].waypoint )<= lista_enemigos[contador_enemigos].tolerancia_wp_pos_cuad )
			{
				// Waypoint ha sido alcanzado --- Waypoint reached
				posic_actual_enemigos[contador_enemigos] = lista_enemigos[contador_enemigos].waypoints[waypoint_destino[contador_enemigos]].waypoint;
				// Pasamos a ir al siguiente waypoint --- The enemy will seek the next waypoint
				waypoint_destino[contador_enemigos] += 1;
				if ( waypoint_destino[contador_enemigos] >= lista_enemigos[contador_enemigos].num_waypoints )
				{
					waypoint_destino[contador_enemigos] = 0;
				}
				
			}
		}
	} // Siguiente enemigo --- Next enemy

}

