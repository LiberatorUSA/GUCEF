/*	Public domain	*/

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC void M_PolygonInit(M_Polygon *);
extern DECLSPEC void M_PolygonFree(M_Polygon *);
extern DECLSPEC M_Polygon M_PolygonFromPts(Uint, const M_Vector2 *);
extern DECLSPEC M_Polygon M_PolygonFromLines(Uint, const M_Line2 *);
extern DECLSPEC M_Polygon M_PolygonFromPointSet2(const M_PointSet2 *);
extern DECLSPEC M_Polygon M_PolygonFromPointSet2i(const M_PointSet2i *);
extern DECLSPEC M_PointSet2 M_PolygonToPointSet2(const M_Polygon *);
extern DECLSPEC M_PointSet2i M_PolygonToPointSet2i(const M_Polygon *, M_Real, M_Real);
extern DECLSPEC M_Polygon M_PolygonRead(AG_DataSource *);
extern DECLSPEC void M_PolygonWrite(AG_DataSource *, const M_Polygon *);
extern DECLSPEC int M_PolygonCopy(M_Polygon *, const M_Polygon *);
extern DECLSPEC void M_PolygonScale(M_Polygon *, M_Real, M_Real);
extern DECLSPEC void M_PolygonOffset(M_Polygon *, M_Real, M_Real);
extern DECLSPEC int M_PolygonAddLine(M_Polygon *, M_Line2);
extern DECLSPEC int M_PolygonDelVertex(M_Polygon *, int);
extern DECLSPEC int M_PointInPolygon(const M_Polygon *, M_Vector2);
extern DECLSPEC int M_PolygonIsConvex(const M_Polygon *);
/*
 * Add a vertex to a polygon.
 * Return vertex index on success, -1 on failure.
 */

static __inline__ int
M_PolygonAddVertex(M_Polygon *P, M_Vector2 v)
{
	M_Vector2 *vNew;

	if ((vNew = (M_Vector2 *)AG_TryRealloc(P->v, (P->n+1)*sizeof(M_Vector2))) == NULL) {
		return (-1);
	}
	P->v = vNew;
	P->v[P->n] = v;
	return (P->n++);
}
__END_DECLS
/* Close generated block */
