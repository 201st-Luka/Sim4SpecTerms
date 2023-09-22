#ifndef __SIMULATOR__
#define __SIMULATOR__


#include <python3.11/Python.h>
#include <python3.11/object.h>
#include <python3.11/tupleobject.h>
#include <stdlib.h>

#include "combinations.c"
#include "possibilities.c"


typedef struct {
    unsigned int orbitals;
    short ml;
    float ms;
    unsigned int group;
} SimulatorRow;


typedef struct {
    PyObject_HEAD
    unsigned int s, p, d, f;
    Combinations *combinations;
    Possibilities *possibilities;
    SimulatorRow *rows;
} Simulator;

static PyObject* Simulator_New(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
    Simulator *self = (Simulator*) type->tp_alloc(type, 0);
    if (self != NULL) {        
        if ((self->combinations = (Combinations*) CombinationsType.tp_new(&CombinationsType, args, kwargs)) == NULL) {
            Py_DECREF(self);
            Py_TYPE(self)->tp_free((PyObject*) self);
            self = NULL;
        } else if ((self->possibilities = (Possibilities*) PossibilitiesType.tp_new(&PossibilityType, args, kwargs)) == NULL) {
            Py_DECREF(self);
            Py_TYPE(self)->tp_free((PyObject*) self);
            self = NULL;
        } else {
            self->s = 0;
            self->p = 0;
            self->d = 0;
            self->f = 0;
        }
    }
    return (PyObject*) self;
}

static int Simulator_Init(Simulator *self, PyObject *args, PyObject *kwargs) {
    PyArg_ParseTuple(args, "iiii", &self->s, &self->p, &self->d, &self->f);
    
    CombinationsType.tp_init((PyObject*) self->combinations, args, kwargs);
    PyObject *args_tuple = PyTuple_New(2);
    PyTuple_SET_ITEM(args_tuple, 0, args);
    PyTuple_SET_ITEM(args_tuple, 1, (PyObject*) self->combinations);
    PossibilitiesType.tp_init((PyObject*) self->possibilities, args_tuple, kwargs);

    return 0;
}

static void Simulator_Dealloc(Simulator *self) {
    printf("SIMULATOR DEALLOC start\n");
    if (self->combinations != NULL) {
        Py_XDECREF(self->combinations);
    }
    if (self->possibilities != NULL) {
    printf("SIMULATOR DEALLOC here\n");
        Py_XDECREF(self->possibilities);
    }
    Py_TYPE(self)->tp_free((PyObject*) self);
    printf("SIMULATOR DEALLOC ok\n");
}

static PyObject *Simulator_GetCombinations(Simulator *self, void *closure) {
    Py_INCREF(self->combinations);
    return (PyObject*) self->combinations;
}

static PyObject  *Simulator_GetPossibilities(Simulator* self, void *closure) {
    Py_INCREF(self->possibilities);
    return (PyObject*) self->possibilities;
}

static PyGetSetDef Simulator_getset[] = {
    {"combinations", (getter) Simulator_GetCombinations, NULL, "combinations of the orbirals", NULL},
    {"possibilities", (getter) Simulator_GetPossibilities, NULL, "possibilities of the orbirals", NULL},
    {NULL}
};

PyTypeObject SimulatorType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "simulator.Simulator",
    .tp_basicsize = sizeof(Simulator),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor) Simulator_Dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Simulator object",
    .tp_new = Simulator_New,
    .tp_init = (initproc) Simulator_Init,
    .tp_getset = Simulator_getset,
};


#endif //__SIMULATOR__