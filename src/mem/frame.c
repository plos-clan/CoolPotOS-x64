#include "frame.h"
#include "klog.h"
#include "hhdm.h"
#include "limine.h"

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
        .id = LIMINE_MEMMAP_REQUEST,
        .revision = 0,
};

FrameAllocator frame_allocator;

void init_frame() {
    uint64_t memory_size = 0;
    struct limine_memmap_response *memory_map = memmap_request.response;

    for (uint64_t i = 0; i < memory_map->entry_count; i++) {
        struct limine_memmap_entry *region = memory_map->entries[i];
        logkf("region start %llx, size: %lld\n", region->base, region->length);
        uint64_t end = region->base + region->length;
        if (end > memory_size) memory_size = end;
    }

    size_t bitmap_size = (memory_size / 4096 + 7) / 8;
    logkf("Bitmap size %lld\n", bitmap_size);
    uint64_t bitmap_address = 0;

    for (uint64_t i = 0; i < memory_map->entry_count; i++) {
        struct limine_memmap_entry *region = memory_map->entries[i];
        if (region->type == LIMINE_MEMMAP_USABLE && region->length >= bitmap_size) {
            bitmap_address = region->base;
            break;
        }
    }

    if (!bitmap_address) return;
    logkf("Bitmap address 0x%llx\n", bitmap_address);

    void *bitmap_buffer = phys_to_virt(bitmap_address);
    Bitmap *bitmap = &frame_allocator.bitmap;
    bitmap_init(bitmap, bitmap_buffer, bitmap_size);

    size_t origin_frames = 0;
    for (uint64_t i = 0; i < memory_map->entry_count; i++) {
        struct limine_memmap_entry *region = memory_map->entries[i];
        if (region->type == LIMINE_MEMMAP_USABLE) {
            size_t start_frame = region->base / 4096;
            size_t frame_count = region->length / 4096;
            origin_frames += frame_count;
            bitmap_set_range(bitmap, start_frame, start_frame + frame_count, true);
        }
    }

    size_t bitmap_frame_start = bitmap_address / 4096;
    size_t bitmap_frame_count = (bitmap_size + 4095) / 4096;
    bitmap_set_range(bitmap, bitmap_frame_start, bitmap_frame_start + bitmap_frame_count, false);

    frame_allocator.origin_frames = origin_frames;
    frame_allocator.usable_frames = origin_frames - bitmap_frame_count;

    logkf("Available memory: %lld MiB", (origin_frames / 256));
}

uint64_t alloc_frames(size_t count) {
    Bitmap *bitmap = &frame_allocator.bitmap;
    size_t frame_index = bitmap_find_range(bitmap, count, true);

    if (frame_index == (size_t) - 1) return 0;
    bitmap_set_range(bitmap, frame_index, frame_index + count, false);
    frame_allocator.usable_frames -= count;

    return frame_index * 4096;
}
