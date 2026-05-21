#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE  (1024 * 1024)  // 1 MiB
#define BLOCK_COUNT 50

int main(void)
{
    uint8_t* blocks[BLOCK_COUNT];

    for (int i = 0; i < BLOCK_COUNT; i++) {
        blocks[i] = malloc(BLOCK_SIZE);
        if (blocks[i] == NULL) {
            fprintf(stderr, "block %d: allocation failed\n", i);
            return 1;
        }
        memset(blocks[i], 0, BLOCK_SIZE);
        printf("block %02d allocated and zeroed (%d MiB total)\n", i + 1, i + 1);
    }

    printf("all %d blocks allocated successfully\n", BLOCK_COUNT);

    for (int i = 0; i < BLOCK_COUNT; i++)
        free(blocks[i]);

    return 0;
}
