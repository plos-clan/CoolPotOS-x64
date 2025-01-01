#include <stddef.h>
#include "limine.h"
#include "gop.h"

struct limine_framebuffer *framebuffer = NULL;

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
        .id = LIMINE_FRAMEBUFFER_REQUEST,
        .revision = 0
};

void init_gop() {
    if (framebuffer_request.response == NULL
        || framebuffer_request.response->framebuffer_count < 1) {
        for (;;) __asm__ ("hlt");
    }

    framebuffer = framebuffer_request.response->framebuffers[0];
    gop_clear();
}

void gop_clear() {
    uint64_t stride = framebuffer->pitch / 4;

    for (size_t y = 0; y < framebuffer->height; y++) {
        for (size_t x = 0; x < framebuffer->width; x++) {
            *((uint32_t *) framebuffer->address + (y * stride + x)) = 0xffffff;
        }
    }
}

