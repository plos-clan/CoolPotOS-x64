#include "ctype.h"
#include "krlibc.h"

typedef struct {
    size_t *buffer;
    size_t length;
} Bitmap;

#define BITS_PER_WORD (sizeof(size_t) * 8)

void bitmap_init(Bitmap *bitmap, size_t *buffer, size_t size);

bool bitmap_get(const Bitmap *bitmap, size_t index);

void bitmap_set(Bitmap *bitmap, size_t index, bool value);

void bitmap_set_range(Bitmap *bitmap, size_t start, size_t end, bool value);

size_t bitmap_find_range(const Bitmap *bitmap, size_t length, bool value);
