centro_x = 464;
centro_y = 462;
angulo_resolucion=10;
cantidad_puntos_circulo = 360/angulo_resolucion;
radios = [ 26, 59, 93, 126, 160, 193, 227, 260, 294, 327, 361, 394, 428, 461 ];
#              0    1    2    3      4      5      6    7      8     9     10    11    12    13
for i = 0:13
	printf("# Circulo %d\n",i);
	generar_circulo(i*cantidad_puntos_circulo,centro_x,centro_y,radios(i+1),angulo_resolucion);
endfor