#ifndef PNG_PARSER_H
#define PNG_PARSER_H

#include <stdio.h>

typedef struct {
  unsigned int width;
  unsigned int height;
  unsigned char bit_depth;
  unsigned char type;
  unsigned char compression_method;
  unsigned char filter_method;
  unsigned char interlace_method;
} ImageHeader;

/* Read the image header data from the file
 */
int read_image_header_data(ImageHeader *image_header, FILE *f);

/* Check if the file at `f` has the correct
 * png byte signature.
 * RETURNS
 *  0 -- Success
 *  else -- Failure
 *  */
int check_png_file_signature(FILE *f);

typedef enum {
  CHUNK_TYPE_UNKNOWN = -1,
  CHUNK_TYPE_IHDR,
  CHUNK_TYPE_tEXt,
  CHUNK_TYPE_iTXt,
  CHUNK_TYPE_PLTE,
  CHUNK_TYPE_tRNS,
  CHUNK_TYPE_IDAT,
  CHUNK_TYPE_IEND
} ChunkType;

ChunkType convert_char_to_chunk_type(char *data);

#endif /* PNG_PARSER_H */
