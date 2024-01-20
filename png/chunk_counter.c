#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include "png_parser.h"

const char *FILE_NAME = "image.png";

typedef struct {
  unsigned length;
} PNG_Chunk;

int main() {
  FILE *f = fopen(FILE_NAME, "rb");
  if (f == NULL) {
    fprintf(stderr, "Couldn't open %s.\n", FILE_NAME);
    return EXIT_FAILURE;
  }

  if (check_png_file_signature(f)) {
    fprintf(stderr, "%s is not a valid png file.\n", FILE_NAME);
    fclose(f);
    return EXIT_FAILURE;
  }

  /* Read the first chunk */
  char chunk_type[5] = {0, 0, 0, 0, 0};
  unsigned int chunk_length = 0;
  while (1) {
    /* Skipping over data and CRC value*/
    if (fseek(f, chunk_length + 4, SEEK_CUR)) {
      fprintf(stderr, "Error skipping over data.\n");
      fclose(f);
      return EXIT_FAILURE;
    }

    /* Unfortunately, network byte order screws us */
    if (fread(&chunk_length, sizeof(unsigned int), 1, f) < 1) {
      fprintf(stderr, "Error reading chunk length.\n");
      fclose(f);
      return EXIT_FAILURE;
    }
    chunk_length = ntohl(chunk_length);

    if (fread(chunk_type, sizeof(char), 4, f) < 4) {
      fprintf(stderr, "Error reading chunk type.\n");
      fclose(f);
      return EXIT_FAILURE;
    }

    printf("Chunk type: %s\n", chunk_type);
    printf("Chunk length: %i\n", chunk_length);
  }

  fclose(f);
  return 0;
}
