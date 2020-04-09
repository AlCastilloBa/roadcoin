function generar_circulo ( contador_inicial, centro_x, centro_y, radio, angulo_resolucion )
	angulo=0;
	contador=contador_inicial;
	while (angulo<360)
		punto_inicial_x =  centro_x + radio*cos(pi/180*(angulo));
		punto_inicial_y =  centro_y + radio*sin(pi/180*(angulo));	
		punto_final_x =  centro_x + radio*cos(pi/180*(angulo+angulo_resolucion));
		punto_final_y =  centro_y + radio*sin(pi/180*(angulo+angulo_resolucion));			
		printf("segmento[%d]=((%f,%f),(%f,%f),0,0)\n",contador, punto_inicial_x, punto_inicial_y, punto_final_x, punto_final_y);
		angulo = angulo +  angulo_resolucion;
		contador = contador + 1;
	endwhile
endfunction
