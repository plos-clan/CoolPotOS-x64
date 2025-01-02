#include "bitmap.h"

void bitmap_init(Bitmap *bitmap, size_t *buffer, size_t size) {
    bitmap->buffer = buffer;
    bitmap->length = size * 8;
    memset(buffer, 0, size);
}

bool bitmap_get(const Bitmap *bitmap, size_t index) {
    size_t word_index = index / BITS_PER_WORD;
    size_t bit_index = index % BITS_PER_WORD;
    return (bitmap->buffer[word_index] >> bit_index) & 1;
}

void bitmap_set(Bitmap *bitmap, size_t index, bool value) {
    size_t word_index = index / BITS_PER_WORD;
    size_t bit_index = index % BITS_PER_WORD;
    if (value) {
        bitmap->buffer[word_index] |= ((size_t) 1 << bit_index);
    } else {
        bitmap->buffer[word_index] &= ~((size_t) 1 << bit_index);
    }
}

void bitmap_set_range(Bitmap *bitmap, size_t start, size_t end, bool value) {
    if (start >= end || start >= bitmap->length) {
        return;
    }

    size_t start_word = (start + BITS_PER_WORD - 1) / BITS_PER_WORD;
    size_t end_word = end / BITS_PER_WORD;

    for (size_t i = start; i < start_word * BITS_PER_WORD && i < end; i++) {
        bitmap_set(bitmap, i, value);
    }

    if (start_word > end_word) {
        return;
    }

    if (start_word <= end_word) {
        size_t fill_value = value ? (size_t) - 1 : 0;
        for (size_t i = start_word; i < end_word; i++) {
            bitmap->buffer[i] = fill_value;
        }
    }

    for (size_t i = end_word * BITS_PER_WORD; i < end; i++) {
        bitmap_set(bitmap, i, value);
    }
}

size_t bitmap_find_range(const Bitmap *bitmap, size_t length, bool value) {
    size_t count = 0;
    size_t start_index = 0;
    size_t byte_match = value ? (size_t) - 1 : 0;

    for (size_t word_idx = 0; word_idx < bitmap->length / BITS_PER_WORD; word_idx++) {
        size_t word = bitmap->buffer[word_idx];

        if (word == !byte_match) {
            count = 0;
        } else if (word == byte_match) {
            if (length < BITS_PER_WORD) {
                return word_idx * BITS_PER_WORD;
            }
            if (count == 0) {
                start_index = word_idx * BITS_PER_WORD;
            }
            count += BITS_PER_WORD;
            if (count >= length) {
                return start_index;
            }
        } else {
            for (size_t bit = 0; bit < BITS_PER_WORD; bit++) {
                bool bit_value = (word >> bit) & 1;
                if (bit_value == value) {
                    if (count == 0) {
                        start_index = word_idx * BITS_PER_WORD + bit;
                    }
                    count++;
                    if (count == length) {
                        return start_index;
                    }
                } else {
                    count = 0;
                }
            }
        }
    }

    return (size_t) - 1;
}
