png_parser.o: png_parser.h png_parser.c
	gcc --std=c89 -c png_parser.c -o $@

parse: main.c png_parser.o
	gcc --std=c89 main.c png_parser.o -o $@

counter: chunk_counter.c png_parser.c
	gcc --std=c89 chunk_counter.c png_parser.c -o $@
