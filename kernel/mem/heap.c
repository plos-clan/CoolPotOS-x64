#include "heap.h"
#include "klog.h"

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
};

void init_heap() {
    struct limine_hhdm_response *hhdm_response = hhdm_request.response;
    uint64_t physical_memory_offset = hhdm_response->offset;
    logkf("physical memory offset: %llx\n", physical_memory_offset);

    struct limine_memmap_response *memmap_response = memmap_request.response;
    for (uint64_t i = 0; i < memmap_response->entry_count; i++) {
        struct limine_memmap_entry *entry = memmap_response->entries[i];
        logkf("base: %x, length: %x, type: %d\n", entry->base, entry->length, entry->type);
    }

    heap_init((uint8_t * )(physical_memory_offset + 0x3c0f000), 0x400000);
}
