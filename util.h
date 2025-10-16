#ifndef MAX
#define MAX(A, B)               ((A) > (B) ? (A) : (B))
#endif

#ifndef MIN
#define MIN(A, B)               ((A) < (B) ? (A) : (B))
#endif

#ifndef CLAMP
#define CLAMP(V,L,U) (V < L ? L : V > U ? U : V)
#endif

#ifndef CLAMP255
#define CLAMP255(V) CLAMP(V,0,255)
#endif

#undef explicit_bzero
void explicit_bzero(void *, size_t);
