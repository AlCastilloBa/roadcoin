#define PI 3.1416

// Tipos de interseccion circulo - segmento
enum tipo_interseccion_circulo_segmento 
{ 	
	sin_interseccion=0,			// 0 - El circulo no toca el segmento en ningun sitio.
	interseccion_extremo_start=1,		// 1 - El circulo toca el segmento en el extremo A.
	interseccion_central=2,			// 2 - El cirtulo toca el segmento en un punto central cualquiera (entre los extremos).
	interseccion_extremo_end=3,		// 3 - El circulo toca el segmento en el extremo B.
	interseccion_atravesado=4		// 4 - El circulo ha atravesado el segmento sin tocarlo
};


struct punto GiraPunto ( struct punto, struct punto, double);

void GiraMapaCompleto( struct segmento*, struct segmento*, struct punto, int, double  );

void CopiaSegmentosSinGiro( struct segmento*, struct segmento*, int );

void GiraBumpers( struct pinball_bumper*, struct punto*, struct punto, int, double );

void GiraZonasAcelCirc( struct zona_aceleracion_circular*, struct punto*, struct punto, int, double );

void CopiaBumpersSinGiro( struct pinball_bumper*, struct punto*, int );

void CopiaZonasAcelCircSinGiro( struct zona_aceleracion_circular*, struct punto*, int );

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

double WrapAngle_0_2pi (double);

double WrapAngle_0_360 (double);

double WrapAngle_mpi_pi (double);

double WrapAngle_m180_180 (double );

float AnguloRecta (struct punto, struct punto );

struct punto CalculaPosTangenteCirculoCirculo ( struct punto, struct punto, double, double);

double ProductoVectorial2D_ComponenteZ( struct punto, struct punto );

bool MismoLado(struct punto, struct punto, struct punto, struct punto);

bool PuntoDentroDeTriangulo(struct punto, struct punto, struct punto,struct punto);

double AnguloRadEntreDosSegmentos ( struct segmento, struct segmento );




