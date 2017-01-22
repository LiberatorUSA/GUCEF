/*	Public domain	*/

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC const M_VectorOps4 mVecOps4_FPU;
extern DECLSPEC M_Vector4 M_VectorZero4_FPU(void);
extern DECLSPEC M_Vector4 M_VectorGet4_FPU(M_Real, M_Real, M_Real, M_Real);
extern DECLSPEC void M_VectorSet4_FPU(M_Vector4 *, M_Real, M_Real, M_Real, M_Real);
extern DECLSPEC void M_VectorCopy4_FPU(M_Vector4 *, const M_Vector4 *);
extern DECLSPEC M_Vector4 M_VectorFlip4_FPU(M_Vector4);
extern DECLSPEC M_Vector4 M_VectorFlip4p_FPU(const M_Vector4 *);
extern DECLSPEC M_Real M_VectorLen4_FPU(M_Vector4);
extern DECLSPEC M_Real M_VectorLen4p_FPU(const M_Vector4 *);
extern DECLSPEC M_Real M_VectorDot4_FPU(M_Vector4, M_Vector4);
extern DECLSPEC M_Real M_VectorDot4p_FPU(const M_Vector4 *, const M_Vector4 *);
extern DECLSPEC M_Real M_VectorDistance4_FPU(M_Vector4, M_Vector4);
extern DECLSPEC M_Real M_VectorDistance4p_FPU(const M_Vector4 *, const M_Vector4 *);
extern DECLSPEC M_Vector4 M_VectorNorm4_FPU(M_Vector4);
extern DECLSPEC M_Vector4 M_VectorNorm4p_FPU(const M_Vector4 *);
extern DECLSPEC void M_VectorNorm4v_FPU(M_Vector4 *);
extern DECLSPEC M_Vector4 M_VectorScale4_FPU(M_Vector4, M_Real);
extern DECLSPEC M_Vector4 M_VectorScale4p_FPU(const M_Vector4 *, M_Real);
extern DECLSPEC void M_VectorScale4v_FPU(M_Vector4 *, M_Real);
extern DECLSPEC M_Vector4 M_VectorAdd4_FPU(M_Vector4, M_Vector4);
extern DECLSPEC M_Vector4 M_VectorAdd4p_FPU(const M_Vector4 *, const M_Vector4 *);
extern DECLSPEC void M_VectorAdd4v_FPU(M_Vector4 *, const M_Vector4 *);
extern DECLSPEC M_Vector4 M_VectorSum4_FPU(const M_Vector4 *, Uint);
extern DECLSPEC M_Vector4 M_VectorSub4_FPU(M_Vector4, M_Vector4);
extern DECLSPEC M_Vector4 M_VectorSub4p_FPU(const M_Vector4 *, const M_Vector4 *);
extern DECLSPEC void M_VectorSub4v_FPU(M_Vector4 *, const M_Vector4 *);
extern DECLSPEC M_Vector4 M_VectorAvg4_FPU(M_Vector4, M_Vector4);
extern DECLSPEC M_Vector4 M_VectorAvg4p_FPU(const M_Vector4 *, const M_Vector4 *);
extern DECLSPEC void M_VectorVecAngle4_FPU(M_Vector4, M_Vector4, M_Real *, M_Real *, M_Real *);
extern DECLSPEC M_Vector4 M_VectorLERP4_FPU(M_Vector4, M_Vector4, M_Real);
extern DECLSPEC M_Vector4 M_VectorLERP4p_FPU(M_Vector4 *, M_Vector4 *, M_Real);
extern DECLSPEC M_Vector4 M_VectorElemPow4_FPU(M_Vector4, M_Real);
__END_DECLS
/* Close generated block */
