//
// Created by luka on 20/10/23.
//

#ifndef SIMULATOR_EXTENSIONS_GROUPS_H
#define SIMULATOR_EXTENSIONS_GROUPS_H


#include <python3.11/Python.h>
#include <stdlib.h>


typedef struct {
    unsigned short sup;
    char *x;
    float sub;
} Term;

typedef struct {
    unsigned short term_count;
    Term *terms;
} Terms;

typedef struct {
    short abs_ml;
    float abs_ms;
    unsigned int id, count;
    Terms *terms;
} Group;

typedef struct {
    PyObject_HEAD
    Group *groups;
    unsigned int group_count, iter;
} Groups;


PyObject *Groups_New(PyTypeObject *type, PyObject *args, PyObject *kwargs);

int Groups_Init(Groups *self, PyObject *args, PyObject *kwargs);

void Groups_Dealloc(Groups *self);

PyObject *Groups_GetItem(Groups *self, PyObject *key);

PyObject *Groups_Iter(Groups *self);

PyObject *Groups_IterNext(Groups *iter);

Py_ssize_t Groups_Len(Groups *self);

extern PySequenceMethods Groups_sequence_methods;

extern PyMappingMethods Groups_mapping_methods;

extern PyTypeObject GroupsType;


#endif //SIMULATOR_EXTENSIONS_GROUPS_H
