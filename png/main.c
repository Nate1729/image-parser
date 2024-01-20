#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *FILE_NAME = "img/Icon14_09.png";
const unsigned char PNG_BYTE_SIGNATURE[] = {0x89, 0x50, 0x4e, 0x47,
                                            0x0d, 0x0a, 0x1a, 0x0a};

/* Check if the file at `f` has the correct
 * png byte signature.
 * RETURNS
 *  0 -> Success
 *  _ -> Failure
 *  */
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

typedef struct {
  unsigned int length;
  char chunk_type[4];
  char crc[4];
  char *data;
} Chunk;

int parse_chunk(Chunk *chunk, FILE *f);

int main() {
  FILE *f = fopen(FILE_NAME, "rb");
  if (f == NULL) {
    fprintf(stderr, "Couldn't open \"%s\"\n", FILE_NAME);
    return EXIT_FAILURE;
  }

  if (sizeof(unsigned int) != 4) {
    fprintf(stderr, "Exiting early! Unsigned int error\n");
    return EXIT_FAILURE;
  }

  if (check_png_file_signature(f) != 0) {
    fprintf(stderr, "File signature is incorrect for png\n.");
    return EXIT_FAILURE;
  }

  Chunk chunk;
  parse_chunk(&chunk, f);

  char chunk_type[5];
  memcpy(chunk_type, chunk.chunk_type, 4);
  chunk_type[4] = '\0';
  printf("Chunk type: %s\n", chunk_type);

  fclose(f);
  return EXIT_SUCCESS;
}

int parse_chunk(Chunk *chunk, FILE *f) {
  char chunk_type[4], crc[4];
  unsigned int length;
  if (fread(&length, sizeof(unsigned int), 1, f) < 1) {
    fprintf(stderr, "Error reading chunk length.\n");
    return -1;
  };
  length = ntohl(length);
  chunk->length = length;

  if (fread(chunk_type, sizeof(char), 4, f) < 4) {
    fprintf(stderr, "Error reading chunk type.\n");
    return -1;
  }
  memcpy(chunk->chunk_type, chunk_type, 4);

  char *data = malloc(sizeof(char) * length);
  printf("Chunk size is %i.\n", length);
  if (fread(data, sizeof(char), length, f) < length) {
    fprintf(stderr, "Error reading chunk data.\n");
    free(data);
    return -1;
  }
  return 0;
}
