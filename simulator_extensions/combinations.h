//
// Created by luka on 20/10/23.
//

#ifndef SIMULATOR_EXTENSIONS_COMBINATIONS_H
#define SIMULATOR_EXTENSIONS_COMBINATIONS_H


#include <python3.11/Python.h>
#include <stdlib.h>


typedef struct {
    PyObject_HEAD
    unsigned int s, p, d, f;
} Combinations;


PyObject* Combinations_New(PyTypeObject* type, PyObject* args, PyObject* kwargs);

int Combinations_Init(Combinations *self, PyObject *args, PyObject *kwargs);

void Combinations_Dealloc(Combinations *self);

PyObject *Combinations_GetS(Combinations *self, void *closure);

PyObject *Combinations_GetP(Combinations *self, void *closure);

PyObject *Combinations_GetD(Combinations *self, void *closure);

PyObject *Combinations_GetF(Combinations *self, void *closure);

extern PyGetSetDef Combinations_getset[];

extern PyTypeObject CombinationsType;


#endif //SIMULATOR_EXTENSIONS_COMBINATIONS_H
