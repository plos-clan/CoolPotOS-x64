#include "terminal.h"
#include "gop.h"
#include "klog.h"

void init_terminal() {
    TerminalDisplay display = {
        .width = framebuffer->width,
        .height = framebuffer->height,
        .address = framebuffer->address
    };

    terminal_init(&display, 12.0, malloc, free, NULL);
}
