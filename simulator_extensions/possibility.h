//
// Created by luka on 20/10/23.
//

#ifndef SIMULATOR_EXTENSIONS_POSSIBILITY_H
#define SIMULATOR_EXTENSIONS_POSSIBILITY_H


#include <python3.11/Python.h>
#include <stdlib.h>
#include <stdio.h>


#define ARROW_UP   "\u21bF"
#define ARROW_DOWN "\u21C2"
#define ARROW_BOTH "\u296E"


typedef struct {
    PyObject_HEAD
    unsigned int combinations, electrons, max_electrons, iter;
    unsigned short *poss;
} Possibility;

PyObject *Possibility_New(PyTypeObject *type, PyObject *args, PyObject *kwargs);

int Possibility_Init(Possibility *self, PyObject *args, PyObject *kwargs);

void Possibility_Dealloc(Possibility *self);

PyObject *Possibility_Iter(Possibility *self);

PyObject *int_to_arrow(unsigned short sub_value);

PyObject *Possibility_IterNext(Possibility *iter);

PyObject *Possibility_GetItem(Possibility *self, Py_ssize_t index);

extern PySequenceMethods Possibility_sequence_methods;

extern PyTypeObject PossibilityType;


#endif //SIMULATOR_EXTENSIONS_POSSIBILITY_H
