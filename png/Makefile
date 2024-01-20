all: main.c 
	gcc --std=c89 main.c -o parse

counter: chunk_counter.c png_parser.c
	gcc --std=c89 chunk_counter.c png_parser.c -o $@
