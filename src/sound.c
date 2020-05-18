#include <SDL_mixer.h>


void Volumen_Chunk_Escala( Mix_Chunk* sonido, double valor, double maximo, double minimo)		// Set Sound "Chunk" Volume according to a scale
{
	// Esta función ajusta el volumen de un sonido particular, escalando una magnitud entre dos valores.
	// 	Valor: magnitud de la que dependerá el volumen del sonido
	//	maximo: valor de la magnitud a la que le corresponde el volumen máximo --> MIX_MAX_VOLUME(128)
	//	minimo: valor de la magnutod a la que le corresponde el volumen minimo --> 0
	// Nota: si la magnitud es superior al volumen maximo, el volumen lo ajustaremos al valor maximo (MIX_MAX_VOLUME).
	// 	si la magnitud es inferior al valor minimo, el volumen será cero

	// This function sets the volume of a given "mix chunk", scalling the magnitude between two values:
	//	Value: magnitude wich the volume will depend on
	//	maximum: maximum magnitude value that will be mapped to the maximum volume --> MIX_MAX_VOLUME(128)
	//	minimum: minimum magnitude value that will be mapped to the minimum volume --> 0
	// Note: if the magnitude is greater than the maximum volume, the volume will be set to the maximum volume allowed (MIX_MAX_VOLUME)
	//	if the magnitud is lower than the minimum volme, the volume will be zero.
	
	int volumen;
	if ( valor <= minimo )
	{
		volumen = 0;
	}
	else if (valor >= maximo )
	{
		volumen = MIX_MAX_VOLUME;
	}
	else /* minimo < valor < maximo ) */
	{
		volumen = ((MIX_MAX_VOLUME /* -0 */) * (valor - minimo )) / (maximo - minimo) ;
	}

	Mix_VolumeChunk( sonido , volumen );
}
