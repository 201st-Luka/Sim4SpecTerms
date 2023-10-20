//
// Created by luka on 20/10/23.
//

#ifndef SIMULATOR_EXTENSIONS_POSSIBILITIES_H
#define SIMULATOR_EXTENSIONS_POSSIBILITIES_H


#include <python3.11/Python.h>

#include "possibility.h"
#include "combinations.c"


typedef struct {
    PyObject_HEAD
    Possibility *s, *p, *d, *f;
} Possibilities;

PyObject *Possibilities_New(PyTypeObject *type, PyObject *args, PyObject *kwargs);

int Possibilities_Init(Possibilities *self, PyObject *args, PyObject *kwargs);

void Possibilities_Dealloc(Possibilities *self);

PyObject *Possibilities_GetS(Possibilities *self, void *closure);

PyObject *Possibilities_GetP(Possibilities *self, void *closure);

PyObject *Possibilities_GetD(Possibilities *self, void *closure);

PyObject *Possibilities_GetF(Possibilities *self, void *closure);

extern PyGetSetDef Possibilities_getset[];

extern PyTypeObject PossibilitiesType;


#endif //SIMULATOR_EXTENSIONS_POSSIBILITIES_H
