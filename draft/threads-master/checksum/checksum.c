#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t checksum(char *bytes, int length);

#define BLOCKS 4
#define BLOCK_SZ 1024 * 1024
int main() {
  char data[BLOCKS * BLOCK_SZ];

  FILE *file = fopen("data.bin", "r");
  if (file == NULL) {
    perror("Could not open file");
    return 1;
  }

  size_t sz;
  sz = fread(data, 1, sizeof(data), file);
  if (sz < sizeof(data)) {
    perror("Could not read");
    exit(1);
  }

  uint64_t chksum = 0;

  printf("Checksum: %" PRIx64 "\n", checksum(data, sizeof(data)));
}

uint64_t checksum(char *bytes, int length) {
  uint64_t chksum = 0;

  int i, blk;
  for (blk = 0; blk < BLOCKS; ++blk) {
    uint64_t blksum = 0;
    uint64_t *data = (uint64_t *)(bytes + blk * BLOCK_SZ);
    for (i = 0; i < BLOCK_SZ / 8; ++i) {
      int k;
      uint64_t tmp = *data++;
      for (k = 0; k < 1024; ++k) {
        blksum *= tmp++;
        blksum += tmp++;
        blksum *= tmp++;
        blksum ^= tmp++;
        blksum *= tmp++;
      }
    }
    chksum ^= blksum;
  }

  return chksum;
}
