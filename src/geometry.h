#define PI 3.1416

// Tipos de interseccion circulo - segmento
enum tipo_interseccion_circulo_segmento 
{ 	
	sin_interseccion=0,			// 0 - El circulo no toca el segmento en ningun sitio.
	interseccion_extremo_start=1,		// 1 - El circulo toca el segmento en el extremo A.
	interseccion_central=2,			// 2 - El cirtulo toca el segmento en un punto central cualquiera (entre los extremos).
	interseccion_extremo_end=3		// 3 - El circulo toca el segmento en el extremo B.
};


struct punto GiraPunto ( struct punto, struct punto, double);

void GiraMapaCompleto( struct segmento*, struct segmento*, struct punto, int, double  );

float AnguloSegmento (struct segmento );

double ProductoEscalar2D (struct punto, struct punto);

double ProductoEscalarPerpendicular2D (struct punto, struct punto);

bool ProyeccionEstaEnSegmento( struct punto, struct punto, struct punto);

bool PuntoEstaEnSegmento (struct punto, struct punto, struct punto);

struct punto PuntoProyectadoSobreRecta( struct punto, struct punto, struct punto);

double LongitudVector_Cuadrado( struct punto v, struct punto w);

double LongitudVector( struct punto v, struct punto w);

double DistanciaPuntoRecta_Cuadrado( struct punto , struct punto , struct punto );

//bool ProyeccionCirculoTocaSegmento( struct punto, double , struct punto, struct punto);

bool CirculoTocaSegmentoExcluyendoExtremos( struct punto , double , struct punto, struct punto);

enum tipo_interseccion_circulo_segmento CirculoTocaSegmento( struct punto, double, struct punto, struct punto);

bool onSegment(struct punto, struct punto, struct punto);

int orientation(struct punto, struct punto, struct punto);

bool doIntersect(struct punto, struct punto, struct punto, struct punto);

struct punto CalculaPosTangenteACentroSegmento ( struct punto, struct punto, struct punto, double);

struct punto CalculaPosTangenteAExtremoSegmento ( struct punto, struct punto, double);
