#include <limine.h>
#include "gop.h"
#include "kprint.h"
#include "terminal.h"
#include "heap.h"

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3)

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER

void kmain(void) {
    init_gop();
    init_serial();
    init_heap();
    init_terminal();

    kinfo("Hello, World!");
    kdebug("Log some debug info!");

    for (;;) __asm__ ("hlt");
}
