#ifndef MAX
#define MAX(A, B)               ((A) > (B) ? (A) : (B))
#endif

#undef explicit_bzero
void explicit_bzero(void *, size_t);
