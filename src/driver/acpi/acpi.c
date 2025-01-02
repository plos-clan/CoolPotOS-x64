#include "acpi.h"
#include "krlibc.h"
#include "kprint.h"
#include "limine.h"

__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rsdp_request = {
        .id = LIMINE_RSDP_REQUEST,
        .revision = 0
};

void acpi_setup(){
    struct limine_rsdp_response *response = rsdp_request.response;
    RSDP *rsdp = (RSDP *)response->address;
    if(rsdp == NULL) {
        kwarn("Cannot find acpi rsdp table.");
        return;
    }
    XSDT *xsdt = (XSDT*)rsdp->xsdt_address;
    if(xsdt == NULL){
        kwarn("Cannot find acpi xsdt table.");
        return;
    }
}

