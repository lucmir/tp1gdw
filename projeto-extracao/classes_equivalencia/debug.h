#ifdef DEBUGP
#undef DEBUGP
#undef TDEBUGP
#endif

#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#ifdef DEBUG
#define DEBUGP(args...) do { fprintf(stderr, "%s:%d ", __FILE__, __LINE__); fprintf(stderr, ## args); } while(0)
#define TDEBUGP(args...) do { for (uint __tdit__ = 0; __tdit__ < state.depth; ++__tdit__) fprintf(stderr, " ");\
fprintf(stderr, "%s:%d ", __FILE__, __LINE__); fprintf(stderr, ## args);  } while(0)
#else
#define DEBUGP(args...)
#define TDEBUGP(args...)
#endif
