# -*- Makefile -*-

voronoi1: voronoi1.o DCEL.o
    gcc DCEL.o voronoi1.o -o voronoi1

vornoi1.o: vornoi1.c structs.h
    gcc -o voronoi1.c

DCEL.o: DCEL.c structs.h
    gcc -o DCEL.c

