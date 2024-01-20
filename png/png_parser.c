
#include "png_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned char PNG_BYTE_SIGNATURE[] = {0x89, 0x50, 0x4e, 0x47,
                                            0x0d, 0x0a, 0x1a, 0x0a};

int check_png_file_signature(FILE *f) {
  /* Move to the beginning of the file. */
  unsigned char buffer[8];
  int i;
  fseek(f, 0, SEEK_SET);
  if (fread(buffer, 1, 8, f) < 8) {
    fprintf(stderr, "Error: Issue reading file!\n");
    return -1;
  }

  return memcmp(buffer, PNG_BYTE_SIGNATURE, 8);
}

const char IHDR[] = {'I', 'H', 'D', 'R'};
const char tEXt[] = {'t', 'E', 'X', 't'};
const char iTXt[] = {'i', 'T', 'X', 't'};
const char PLTE[] = {'P', 'L', 'T', 'E'};
const char tRNS[] = {'t', 'R', 'N', 'S'};
const char IDAT[] = {'I', 'D', 'A', 'T'};
const char IEND[] = {'I', 'E', 'N', 'D'};

ChunkType convert_char_to_chunk_type(char *data) {
  if (!memcmp(data, IHDR, 4)) {
    return CHUNK_TYPE_IHDR;
  } else if (!memcmp(data, tEXt, 4)) {
    return CHUNK_TYPE_tEXt;
  } else if (!memcmp(data, iTXt, 4)) {
    return CHUNK_TYPE_iTXt;
  } else if (!memcmp(data, PLTE, 4)) {
    return CHUNK_TYPE_PLTE;
  } else if (!memcmp(data, tRNS, 4)) {
    return CHUNK_TYPE_tRNS;
  } else if (!memcmp(data, IDAT, 4)) {
    return CHUNK_TYPE_IDAT;
  } else if (!memcmp(data, IEND, 4)) {
    return CHUNK_TYPE_IEND;
  } else {
    return CHUNK_TYPE_UNKNOWN;
  }
}
