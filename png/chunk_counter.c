#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "png_parser.h"

const char *FILE_NAME = "image.png";

typedef struct {
  unsigned int length;
  char type[4];
} ChunkHeader;

/* Read the length and type of the current chunk.
 * Edits the `ChunkHeader` in-place.
 * Returns 0 for success and 1 for failure.
 * */
int read_chunk_header(FILE *f, ChunkHeader *header) {
  unsigned int raw_length;
  if (fread(&raw_length, sizeof(unsigned int), 1, f) < 1) {
    fprintf(stderr, "Error reading chunk header.\n");
    return 1;
  }
  header->length = ntohl(raw_length);

  if (fread(header->type, sizeof(char), 4, f) < 4) {
    fprintf(stderr, "Erro reading chunk header.\n");
    return 1;
  }

  return 0;
}

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

  ChunkHeader chunk_header;
  if (read_chunk_header(f, &chunk_header)) {
    fprintf(stderr, "Had problem reading chunk error -- Exiting early.\n");
    fclose(f);
    return EXIT_FAILURE;
  }

  unsigned chunk_count = 1;
  while (convert_char_to_chunk_type(chunk_header.type) != CHUNK_TYPE_IEND) {
    /* Debug stuff
    fprintf(stdout, "Chunk type: %.4s\n", chunk_header.type);
    fprintf(stdout, "Chunk length: %i\n", chunk_header.length);
    */

    if (fseek(f, chunk_header.length + 4, SEEK_CUR)) {
      fprintf(stderr, "Error skipping over data.\n");
      fclose(f);
      return EXIT_FAILURE;
    }

    if (read_chunk_header(f, &chunk_header)) {
      fprintf(stderr, "Had problem reading chunk error -- Exiting early.\n");
      fclose(f);
      return EXIT_FAILURE;
    }

    chunk_count += 1;
  }

  fclose(f);

  printf("Total Chunks: %i\n", chunk_count);
  return 0;
}
