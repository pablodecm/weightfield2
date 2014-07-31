#!/bin/sh

CFLAGS=`root-config --cflags`
RCLIBS=`root-config --libs `
MYLIBS="Potentials.o Field.o PotenDict.o FieldDict.o"
GC="g++ -g -std=c++0x -Wall -fPIC"


$GC $CFLAGS -c Potentials.cxx
rootcint -f PotenDict.C -c -p -I./ Potentials.h
$GC `root-config --cflags` -c PotenDict.C -o PotenDict.o

$GC $CFLAGS -c Field.cxx
rootcint -f FieldDict.C -c -p -I./ Field.h
$GC `root-config --cflags` -c FieldDict.C -o FieldDict.o


$GC $CFLAGS -c wfnogui.cpp
g++ -g -m64 wfnogui.o $MYLIBS $RCLIBS -lTreePlayer -o wfnogui

