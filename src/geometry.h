



struct punto GiraPunto ( struct punto, struct punto, double);

void GiraMapaCompleto( struct segmento*, struct segmento*, struct punto, int, double  );

double ProductoEscalar2D (struct punto, struct punto);

double ProductoEscalarPerpendicular2D (struct punto, struct punto);

bool ProyeccionEstaEnSegmento( struct punto, struct punto, struct punto);

bool PuntoEstaEnSegmento (struct punto, struct punto, struct punto);

struct punto PuntoProyectadoSobreRecta( struct punto, struct punto, struct punto);

double LongitudVector_Cuadrado( struct punto v, struct punto w);

double DistanciaPuntoRecta_Cuadrado( struct punto , struct punto , struct punto );

bool ProyeccionCirculoTocaSegmento( struct punto, double , struct punto, struct punto);

bool CirculoTocaSegmento( struct punto, double, struct punto, struct punto);
