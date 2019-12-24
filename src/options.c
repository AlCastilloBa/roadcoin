
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
	bool fullscreen_ok, screen_x_resolution_ok, screen_y_resolution_ok;

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
		printf("Falta screen_x_resolution Se toma el valor por defecto.\n");
		opciones_cargadas.screen_x_resolution=OPTIONS_DEFAULT_FULLSCREEN;
	}
	if (screen_y_resolution_ok == false) 		
	{
		printf("Falta screen_y_resolution Se toma el valor por defecto.\n");
		opciones_cargadas.screen_x_resolution=OPTIONS_DEFAULT_FULLSCREEN;
	}



	#ifdef DEBUG_INFO
	printf("Opciones cargadas. Continuamos.\n" );
	#endif
	return opciones_cargadas;
}





void GuardarArchivoOpciones ( struct opciones opciones_a_guardar )
{
	// (TODO) Pendiente de hacer
	// Abrir archivo como escritura, sobreescribir todo el contenido



	// Escribir las lineas una a una


}
