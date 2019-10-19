



struct punto GiraPunto ( struct punto, struct punto, double);

void GiraMapaCompleto( struct segmento*, struct segmento*, struct punto, int, double  );





struct punto ProyeccionPuntoSobreSegmento ( struct punto punto_a_proyectar, struct segmento segmento_sobre_el_que_se_proyecta);

bool InterferenciaCircSegmento( struct punto centro_circ, double radio_circ, struct segmento segmento_actual );
