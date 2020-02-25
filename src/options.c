
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "options.h"


#define DEBUG_INFO

struct opciones CargarArchivoOpciones()
{
	struct opciones opciones_cargadas;

	char linea_leida[200];
	bool fullscreen_ok=false, screen_x_resolution_ok=false, screen_y_resolution_ok=false, mouse_sensitivity_ok=false, wireframe_ok=false, textured_objects_ok=false;
	bool music_enabled_ok=false, sound_enabled_ok=false, map_rot_makes_coin_fly_ok=false, limit_coin_speed_ok=false;

	FILE *archivo;
	int linea=0;
	int i;

	archivo = fopen( "settings", "r" );
	if (archivo == NULL)
	{
		printf("Error: No se puede abrir el archivo \"settings\" \n" );
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

			if (strstr(linea_leida, "fullscreen") != NULL )
			{
				int dato_leido;
				if ( sscanf(linea_leida, "fullscreen=%d", &dato_leido ) == 1 )
				{
					switch (dato_leido)
					{
						case 0:
							opciones_cargadas.fullscreen = false;
							fullscreen_ok = true;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, fullscreen=false \n", linea);
							#endif
							break;
						case 1:
							opciones_cargadas.fullscreen = true;
							fullscreen_ok = true;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, fullscreen=true \n", linea);
							#endif
							break;
						default:
							// Valor no válido, se toma el valor por defecto
							fullscreen_ok = false;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, fullscreen tiene un valor no permitido, se tomará el valor por defecto. \n", linea);
							#endif
							break;
					}
				}
				else
				{	
					#ifdef DEBUG_INFO
					printf("Linea %d, fullscreen --> No se han podido leer todos los valores. Se tomará el valor por defecto.\n", linea);
					#endif
				}
			}
			else if (strstr(linea_leida, "screen_x_resolution") != NULL )
			{
				if ( sscanf(linea_leida, "screen_x_resolution=%d", &(opciones_cargadas.screen_x_resolution) ) == 1 )
				{
					screen_x_resolution_ok = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, screen_x_resolution=%d\n", linea, opciones_cargadas.screen_x_resolution );
					#endif
				}
				else
				{	
					#ifdef DEBUG_INFO
					printf("Linea %d, screen_x_resolution --> No se han podido leer el valor. Se tomará el valor por defecto.\n", linea);
					#endif
				}
			}
			else if (strstr(linea_leida, "screen_y_resolution") != NULL )
			{
				if ( sscanf(linea_leida, "screen_y_resolution=%d", &(opciones_cargadas.screen_y_resolution) ) == 1 )
				{
					screen_y_resolution_ok = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, screen_y_resolution=%d\n", linea, opciones_cargadas.screen_y_resolution );
					#endif
				}
				else
				{	
					#ifdef DEBUG_INFO
					printf("Linea %d, screen_y_resolution --> No se han podido leer el valor. Se tomará el valor por defecto.\n", linea);
					#endif
				}
			}
			else if (strstr(linea_leida, "wireframe") != NULL )
			{
				int dato_leido;
				if ( sscanf(linea_leida, "wireframe=%d", &dato_leido ) == 1 )
				{
					switch (dato_leido)
					{
						case 0:
							opciones_cargadas.wireframe = false;
							wireframe_ok = true;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, wireframe=false \n", linea);
							#endif
							break;
						case 1:
							opciones_cargadas.wireframe = true;
							wireframe_ok = true;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, wireframe=true \n", linea);
							#endif
							break;
						default:
							// Valor no válido, se toma el valor por defecto
							wireframe_ok = false;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, wireframe tiene un valor no permitido, se tomará el valor por defecto. \n", linea);
							#endif
							break;
					}
				}
				else
				{	
					#ifdef DEBUG_INFO
					printf("Linea %d, wireframe --> No se han podido leer todos los valores. Se tomará el valor por defecto.\n", linea);
					#endif
				}
			}
			else if (strstr(linea_leida, "textured_lines") != NULL )
			{
				int dato_leido;
				if ( sscanf(linea_leida, "textured_lines=%d", &dato_leido ) == 1 )
				{
					switch (dato_leido)
					{
						case 0:
							opciones_cargadas.textured_objects = false;
							textured_objects_ok = true;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, textured_objects=false \n", linea);
							#endif
							break;
						case 1:
							opciones_cargadas.textured_objects = true;
							textured_objects_ok = true;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, textured_objects=true \n", linea);
							#endif
							break;
						default:
							// Valor no válido, se toma el valor por defecto
							textured_objects_ok = false;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, textured_objects tiene un valor no permitido, se tomará el valor por defecto. \n", linea);
							#endif
							break;
					}
				}
				else
				{	
					#ifdef DEBUG_INFO
					printf("Linea %d, textured_lines --> No se han podido leer todos los valores. Se tomará el valor por defecto.\n", linea);
					#endif
				}
			}
			else if (strstr(linea_leida, "mouse_sensitivity") != NULL )
			{
				if ( sscanf(linea_leida, "mouse_sensitivity=%f", &(opciones_cargadas.mouse_sensitivity) ) == 1 )
				{
					mouse_sensitivity_ok = true;
					#ifdef DEBUG_INFO
					printf("Linea %d, mouse_sensitivity=%f\n", linea, opciones_cargadas.mouse_sensitivity );
					#endif
				}
				else
				{	
					#ifdef DEBUG_INFO
					printf("Linea %d, mouse_sensitivity --> No se han podido leer el valor. Se tomará el valor por defecto.\n", linea);
					#endif
				}
			}
			else if (strstr(linea_leida, "music_enabled") != NULL )
			{
				int dato_leido;
				if ( sscanf(linea_leida, "music_enabled=%d", &dato_leido ) == 1 )
				{
					switch (dato_leido)
					{
						case 0:
							opciones_cargadas.music_enabled = false;
							music_enabled_ok = true;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, music_enabled=false \n", linea);
							#endif
							break;
						case 1:
							opciones_cargadas.music_enabled = true;
							music_enabled_ok = true;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, music_enabled=true \n", linea);
							#endif
							break;
						default:
							// Valor no válido, se toma el valor por defecto
							music_enabled_ok = false;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, music_enabled tiene un valor no permitido, se tomará el valor por defecto. \n", linea);
							#endif
							break;
					}
				}
				else
				{	
					#ifdef DEBUG_INFO
					printf("Linea %d, music_enabled --> No se han podido leer todos los valores. Se tomará el valor por defecto.\n", linea);
					#endif
				}
			}
			else if (strstr(linea_leida, "sound_enabled") != NULL )
			{
				int dato_leido;
				if ( sscanf(linea_leida, "sound_enabled=%d", &dato_leido ) == 1 )
				{
					switch (dato_leido)
					{
						case 0:
							opciones_cargadas.sound_enabled = false;
							sound_enabled_ok = true;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, sound_enabled=false \n", linea);
							#endif
							break;
						case 1:
							opciones_cargadas.sound_enabled = true;
							sound_enabled_ok = true;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, sound_enabled=true \n", linea);
							#endif
							break;
						default:
							// Valor no válido, se toma el valor por defecto
							sound_enabled_ok = false;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, sound_enabled tiene un valor no permitido, se tomará el valor por defecto. \n", linea);
							#endif
							break;
					}
				}
				else
				{	
					#ifdef DEBUG_INFO
					printf("Linea %d, sound_enabled --> No se han podido leer todos los valores. Se tomará el valor por defecto.\n", linea);
					#endif
				}
			}
			else if (strstr(linea_leida, "map_rot_makes_coin_fly") != NULL )
			{
				int dato_leido;
				if ( sscanf(linea_leida, "map_rot_makes_coin_fly=%d", &dato_leido ) == 1 )
				{
					switch (dato_leido)
					{
						case 0:
							opciones_cargadas.map_rot_makes_coin_fly = false;
							map_rot_makes_coin_fly_ok = true;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, map_rot_makes_coin_fly=false \n", linea);
							#endif
							break;
						case 1:
							opciones_cargadas.map_rot_makes_coin_fly = true;
							map_rot_makes_coin_fly_ok = true;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, map_rot_makes_coin_fly=true \n", linea);
							#endif
							break;
						default:
							// Valor no válido, se toma el valor por defecto
							map_rot_makes_coin_fly_ok = false;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, map_rot_makes_coin_fly tiene un valor no permitido, se tomará el valor por defecto. \n", linea);
							#endif
							break;
					}
				}
				else
				{	
					#ifdef DEBUG_INFO
					printf("Linea %d, map_rot_makes_coin_fly --> No se han podido leer todos los valores. Se tomará el valor por defecto.\n", linea);
					#endif
				}
			}
			else if (strstr(linea_leida, "limit_coin_speed") != NULL )
			{
				int dato_leido;
				if ( sscanf(linea_leida, "limit_coin_speed=%d", &dato_leido ) == 1 )
				{
					switch (dato_leido)
					{
						case 0:
							opciones_cargadas.limit_coin_speed = false;
							limit_coin_speed_ok = true;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, limit_coin_speed=false \n", linea);
							#endif
							break;
						case 1:
							opciones_cargadas.limit_coin_speed = true;
							limit_coin_speed_ok = true;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, limit_coin_speed=true \n", linea);
							#endif
							break;
						default:
							// Valor no válido, se toma el valor por defecto
							limit_coin_speed_ok = false;
							#ifdef DEBUG_INFO
							printf("Opciones: Linea %d, limit_coin_speed tiene un valor no permitido, se tomará el valor por defecto. \n", linea);
							#endif
							break;
					}
				}
				else
				{	
					#ifdef DEBUG_INFO
					printf("Linea %d, limit_coin_speed --> No se han podido leer todos los valores. Se tomará el valor por defecto.\n", linea);
					#endif
				}
			}
			/////////////////////////////////////////////////////////////////////
			// Seguir añadiendo opciones aqui
			/////////////////////////////////////////////////////////////////////
			else
			{
				printf("Linea %d, expresion no reconocida, se ignora.\n", linea);
			}

		}

	}

	fclose(archivo);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// En caso de que falte algo, se toman los valores por defecto.
	if (fullscreen_ok == false)
	{
		printf("Opciones: Falta fullscreen. Se toma el valor por defecto.\n");
		opciones_cargadas.fullscreen=OPTIONS_DEFAULT_FULLSCREEN;
		
	} 		
	if (screen_x_resolution_ok == false) 		
	{
		printf("Falta screen_x_resolution. Se toma el valor por defecto.\n");
		opciones_cargadas.screen_x_resolution=OPTIONS_DEFAULT_SCREEN_X_RESOLUTION;
	}
	if (screen_y_resolution_ok == false) 		
	{
		printf("Falta screen_y_resolution. Se toma el valor por defecto.\n");
		opciones_cargadas.screen_y_resolution=OPTIONS_DEFAULT_SCREEN_Y_RESOLUTION;
	}
	if (wireframe_ok == false)
	{
		printf("Opciones: Falta wireframe. Se toma el valor por defecto.\n");
		opciones_cargadas.wireframe=OPTIONS_DEFAULT_WIREFRAME;
	}
	if (textured_objects_ok == false)
	{
		printf("Opciones: Falta textured_objects. Se toma el valor por defecto.\n");
		opciones_cargadas.textured_objects=OPTIONS_DEFAULT_TEXTURED_OBJECTS;
	}
	if (mouse_sensitivity_ok == false)
	{
		printf("Falta mouse_sensitivity. Se toma el valor por defecto.\n");
		opciones_cargadas.mouse_sensitivity=OPTIONS_DEFAULT_MOUSE_SENSIVITY;
	}
	if (music_enabled_ok == false)
	{
		printf("Falta music_enabled. Se toma el valor por defecto.\n");
		opciones_cargadas.music_enabled=OPTIONS_DEFAULT_MUSIC_ENABLED;
	}
	if (sound_enabled_ok == false)
	{
		printf("Falta sound_enabled. Se toma el valor por defecto.\n");
		opciones_cargadas.sound_enabled=OPTIONS_DEFAULT_SOUND_ENABLED;
	}
	if (map_rot_makes_coin_fly_ok == false)
	{
		printf("Falta map_rot_makes_coin_fly. Se toma el valor por defecto.\n");
		opciones_cargadas.map_rot_makes_coin_fly=OPTIONS_DEFAULT_MAP_ROT_MAKES_COIN_FLY;
	}
	if (limit_coin_speed_ok == false)
	{
		printf("Falta limit_coin_speed. Se toma el valor por defecto.\n");
		opciones_cargadas.limit_coin_speed=OPTIONS_DEFAULT_LIMIT_COIN_SPEED;
	}

	#ifdef DEBUG_INFO
	printf("Opciones cargadas. Continuamos.\n" );
	#endif
	return opciones_cargadas;
}





void GuardarArchivoOpciones ( struct opciones opciones_a_guardar )
{
	FILE *archivo;
	// Abrir archivo como escritura, sobreescribir todo el contenido
	archivo = fopen( "settings", "w" );
	if (archivo == NULL)
	{
		printf("Error: No se puede abrir el archivo \"settings\" \n" );
		exit(-1);
	}

	// Escribir las lineas una a una
	if ( opciones_a_guardar.fullscreen )
		fprintf( archivo, "fullscreen=1\n" );
	else
		fprintf( archivo, "fullscreen=0\n" );

	fprintf( archivo, "screen_x_resolution=%d\n", opciones_a_guardar.screen_x_resolution );

	fprintf( archivo, "screen_y_resolution=%d\n", opciones_a_guardar.screen_y_resolution );

	if ( opciones_a_guardar.wireframe )
		fprintf( archivo, "wireframe=1\n" );
	else
		fprintf( archivo, "wireframe=0\n" );

	if ( opciones_a_guardar.textured_objects )
		fprintf( archivo, "textured_lines=1\n" );
	else
		fprintf( archivo, "textured_lines=0\n" );

	fprintf( archivo, "mouse_sensitivity=%f\n", opciones_a_guardar.mouse_sensitivity );

	if ( opciones_a_guardar.music_enabled )
		fprintf( archivo, "music_enabled=1\n" );
	else
		fprintf( archivo, "music_enabled=0\n" );

	if ( opciones_a_guardar.sound_enabled )
		fprintf( archivo, "sound_enabled=1\n" );
	else
		fprintf( archivo, "sound_enabled=0\n" );

	if ( opciones_a_guardar.map_rot_makes_coin_fly )
		fprintf( archivo, "map_rot_makes_coin_fly=1\n" );
	else
		fprintf( archivo, "map_rot_makes_coin_fly=0\n" );

	if ( opciones_a_guardar.limit_coin_speed )
		fprintf( archivo, "limit_coin_speed=1\n" );
	else
		fprintf( archivo, "limit_coin_speed=0\n" );


	fclose(archivo);
}
