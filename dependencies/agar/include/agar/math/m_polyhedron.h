/*	Public domain	*/

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC void M_PolyhedronInit(M_Polyhedron *);
extern DECLSPEC void M_PolyhedronFree(M_Polyhedron *);
extern DECLSPEC int M_PolyhedronRead(AG_DataSource *, M_Polyhedron *);
extern DECLSPEC void M_PolyhedronWrite(AG_DataSource *, const M_Polyhedron *);
extern DECLSPEC M_Polyhedron M_PolyhedronFromPts(Uint, const M_Vector3 *);
extern DECLSPEC Uint M_PolyhedronAddVertex(M_Polyhedron *, M_Vector3);
extern DECLSPEC void M_PolyhedronDelVertex(M_Polyhedron *, Uint);
extern DECLSPEC Uint M_PolyhedronAddEdge(M_Polyhedron *, int, int);
extern DECLSPEC void M_PolyhedronDelEdge(M_Polyhedron *, Uint);
extern DECLSPEC Uint M_PolyhedronAddFacet(M_Polyhedron *, Uint, const Uint *);
extern DECLSPEC void M_PolyhedronDelFacet(M_Polyhedron *, Uint);
__END_DECLS
/* Close generated block */
