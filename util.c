#include "util.h"

void memswap(void *p1, void *p2, size_t size) {
        void *tp = malloc(size);
        memcpy(tp, p1, size);
        memcpy(p1, p2, size);
        memcpy(p2, tp, size);
        free(tp);
}

void *memdup(void *p, size_t size) {
	void *d = malloc(size);
	memcpy(d, p, size);
	return d;
}
