#pragma once

#include "ctype.h"

struct ACPISDTHeader {
    char Signature[4];
    uint32_t Length;
    uint8_t Revision;
    uint8_t Checksum;
    char OEMID[6];
    char OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;
};

typedef struct {
    char signature[8];    // 签名
    uint8_t checksum;     // 校验和
    char oem_id[6];       // OEM ID
    uint8_t revision;     // 版本
    uint32_t rsdt_address; // V1: RSDT 地址 (32-bit)
    uint32_t length;      // 结构体长度
    uint64_t xsdt_address; // V2: XSDT 地址 (64-bit)
    uint8_t extended_checksum; // 扩展校验和
    uint8_t reserved[3];  // 保留字段
} RSDP;

typedef struct {
    struct ACPISDTHeader h;
    uint64_t *PointerToOtherSDT;
} XSDT;

void acpi_setup();
