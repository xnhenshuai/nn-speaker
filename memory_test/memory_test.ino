#include <Arduino.h>
#include "esp_heap_caps.h"

void printMemoryStatus(const char* title) {
    Serial.println(title);

    Serial.printf("Heap total size: %u\n", heap_caps_get_total_size(MALLOC_CAP_8BIT));
    Serial.printf("Heap free size : %u\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));

    Serial.printf("Internal free: %u\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
    Serial.printf("DMA free: %u\n", heap_caps_get_free_size(MALLOC_CAP_DMA));
    Serial.printf("PSRAM free: %u\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));

    Serial.printf("Largest block: %u\n", heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    delay(2000);

    printMemoryStatus("=== Before Allocation ===");

    uint8_t* internal_buf = (uint8_t*) heap_caps_malloc(16 * 1024, MALLOC_CAP_INTERNAL);
    uint8_t* dma_buf      = (uint8_t*) heap_caps_malloc(8 * 1024, MALLOC_CAP_DMA);
    uint8_t* psram_buf    = (uint8_t*) heap_caps_malloc(64 * 1024, MALLOC_CAP_SPIRAM);

    if (internal_buf) Serial.println("Internal OK");
    if (dma_buf) Serial.println("DMA OK");
    if (psram_buf) Serial.println("PSRAM OK");

    printMemoryStatus("=== After Allocation ===");
}

void loop() {}