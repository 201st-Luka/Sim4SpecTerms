//
// Created by luka on 20/10/23.
//

#ifndef SIMULATOR_EXTENSIONS_SIMULATOR_H
#define SIMULATOR_EXTENSIONS_SIMULATOR_H


#include <python3.11/Python.h>
#include <stdlib.h>

#include "combinations.h"
#include "possibilities.h"


#define S_ORBITAL 2
#define P_ORBITAL 6
#define D_ORBITAL 10
#define F_ORBITAL 14
#define TOTAL_ORBITALS 32


typedef struct {
    short ml;
    float ms;
    unsigned short count;
} CompressedSimulatorRow;

typedef struct {
    CompressedSimulatorRow *rows;
    unsigned int rowsCount;
} CompressedSimulator;

typedef struct {
    unsigned int orbitals;
    short ml;
    float ms;
} SimulatorRow;

typedef struct {
    PyObject_HEAD
    unsigned int s, p, d, f, iter;
    Combinations *combinations;
    Possibilities *possibilities;
    SimulatorRow *rows;
    CompressedSimulator *compressed;
} Simulator;


PyObject *Simulator_New(PyTypeObject *type, PyObject *args, PyObject *kwargs);

int Simulator_Init(Simulator *self, PyObject *args, PyObject *kwargs);

void Simulator_Dealloc(Simulator *self);

PyObject *Simulator_GetCombinations(Simulator *self, void *closure);

PyObject  *Simulator_GetPossibilities(Simulator* self, void *closure);

PyObject *Simulator_Iter(Simulator *self);

PyObject *Simulator_IterNext(Simulator *iter);

PyObject *Simulator_GetItem(Simulator *self, PyObject *key);

Py_ssize_t Simulator_Len(Simulator *self);


extern PyGetSetDef Simulator_getset[];

extern PySequenceMethods Simulator_sequence_methods;

extern PyMappingMethods Simulator_mapping_methods;

extern PyTypeObject SimulatorType;


#endif //SIMULATOR_EXTENSIONS_SIMULATOR_H
