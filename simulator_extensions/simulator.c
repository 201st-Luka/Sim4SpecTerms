#ifndef __SIMULATOR__
#define __SIMULATOR__


#include <python3.11/Python.h>
#include <python3.11/tupleobject.h>
#include <stdlib.h>

#include "combinations.c"
#include "possibilities.c"


typedef struct {
    PyObject_HEAD
    unsigned int s, p, d, f;
    Combinations *combinations;
    Possibilities *possibilities;
} Simulator;

static PyObject* Simulator_New(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
    Simulator *self = (Simulator*) type->tp_alloc(type, 0);
    if (self != NULL) {        
        self->combinations = (Combinations*) CombinationsType.tp_new(&CombinationsType, args, kwargs);
        if (self->combinations != NULL) {
            self->s = 0;
            self->p = 0;
            self->d = 0;
            self->f = 0;
        } else {
            Py_TYPE(self)->tp_free((PyObject*) self);
            self = NULL;
        }
    }
    return (PyObject*) self;
}

static int Simulator_Init(Simulator *self, PyObject *args, PyObject *kwargs) {
    PyArg_ParseTuple(args, "iiii", &self->s, &self->p, &self->d, &self->f);
    
    PyObject *t_args = PyTuple_New(4), *t_kwargs = PyTuple_New(0);
    
    PyTuple_SET_ITEM(t_args, 0, PyLong_FromSize_t(self->s));
    PyTuple_SET_ITEM(t_args, 1, PyLong_FromSize_t(self->p));
    PyTuple_SET_ITEM(t_args, 2, PyLong_FromSize_t(self->d));
    PyTuple_SET_ITEM(t_args, 3, PyLong_FromSize_t(self->f));
    
    CombinationsType.tp_init((PyObject*) self->combinations, args, kwargs);

    return 0;
}

static void Simulator_Dealloc(Simulator *self) {
    Py_XDECREF(self->combinations);
    Py_TYPE(self)->tp_free((PyObject*) self);
}

static PyObject *Simulator_GetCombinations(Simulator *self, void *closure) {
    Py_INCREF(self->combinations);
    return (PyObject*) self->combinations;
}

static PyGetSetDef Simulator_getset[] = {
    {"combinations", (getter) Simulator_GetCombinations, NULL, "combinations of the orbirals", NULL},
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