#include <limine.h>
#include "gop.h"
#include "kprint.h"
#include "terminal.h"
#include "hhdm.h"
#include "frame.h"
#include "heap.h"
#include "krlibc.h"
#include "acpi.h"

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(2)

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER

void kmain(void) {
    init_gop();
    init_serial();
    init_hhdm();
    init_frame();
    init_heap();
    init_terminal();

    printk("CoolPorOS %s (Limine Bootloader) on an x86_64\n",KERNEL_NAME);
    kinfo("Video: %d x %d",framebuffer->width,framebuffer->height);

    acpi_setup();

    for (;;) __asm__ ("hlt");
}
