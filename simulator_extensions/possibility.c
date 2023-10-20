#ifndef __POSSIBILITY__
#define __POSSIBILITY__


#include <python3.11/Python.h>
#include <stdlib.h>
#include <stdio.h>

#include "possibility.h"


#define ARROW_UP   "\u21bF"
#define ARROW_DOWN "\u21C2"
#define ARROW_BOTH "\u296E"


static void permute(unsigned short *result,
                    unsigned short perm,
                    unsigned int ones,
                    unsigned int length,
                    unsigned int i,
                    unsigned int *result_len) {
    if (ones <= 0) {
        result[*result_len] = perm << (length - i);
        (*result_len)++;
    } else if (i < length) {
        permute(result, (perm << 1) + 1, ones - 1, length, i + 1, result_len);
        if ((length - i) > ones) permute(result, perm << 1, ones, length, i + 1, result_len);
    }
}

static void generate_permutation(unsigned int ones, unsigned int length, unsigned short *result) {
    unsigned int result_len = 0;
    permute(result, 0, ones, length, 0, &result_len);
}

static PyObject *int_to_arrow_tuple(unsigned short value, unsigned int len) {
    PyObject *tuple = PyTuple_New(len);
    for (unsigned short i = 1; i <= len; ++i) {
        PyTuple_SET_ITEM(tuple, len - i, int_to_arrow(value % 4));
        value /= 4;
    }

    return tuple;
}


PyObject *Possibility_New(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
    Possibility *self = (Possibility*) type->tp_alloc(type, 0);
    return (PyObject*) self;
}

int Possibility_Init(Possibility *self, PyObject *args, PyObject *kwargs) {
    if (!PyArg_ParseTuple(args, "iii", &self->electrons, &self->max_electrons, &self->combinations)) {
        PyErr_SetString(PyExc_ValueError, "Invalid argument format");
        Py_DECREF(self);
        return 1;
    }

    self->poss = (unsigned short*) malloc(sizeof(unsigned short) * self->combinations);
    if (self->poss == NULL) {
        PyErr_NoMemory();
        Py_XDECREF((PyObject*) self);
        return 1;
    }

    generate_permutation(self->electrons, self->max_electrons, self->poss);
    return 0;
}

void Possibility_Dealloc(Possibility *self) {
    free(self->poss);
    Py_TYPE(self)->tp_free((PyObject*) self);
}

PyObject *Possibility_Iter(Possibility *self) {
    self->iter = 0;

    Py_INCREF(self);

    return (PyObject*) self;
}

PyObject *int_to_arrow(unsigned short sub_value) {
    switch (sub_value) {
    case 0:
        return Py_BuildValue("s", "");
    case 1:
        return Py_BuildValue("s", ARROW_DOWN);
    case 2:
        return Py_BuildValue("s", ARROW_UP);
    case 3:
        return Py_BuildValue("s", ARROW_BOTH);
    }

    return NULL;
}

PyObject *Possibility_IterNext(Possibility *iter) {
    if (iter->iter < iter->combinations) {
        PyObject *tuple = int_to_arrow_tuple(iter->poss[iter->iter++], iter->max_electrons / 2);
        Py_INCREF(tuple);
        return tuple;
    } else {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }
}

PyObject *Possibility_GetItem(Possibility *self, Py_ssize_t index) {
    if (index >= self->combinations) {
        PyErr_SetString(PyExc_IndexError, "Index out of bounds");
        return NULL;
    }
    
    PyObject *tuple = int_to_arrow_tuple(self->poss[index], self->max_electrons / 2);
    Py_INCREF(tuple);
    return tuple;
}


PySequenceMethods Possibility_sequence_methods = {
    .sq_length = 0,
    .sq_item = (ssizeargfunc) Possibility_GetItem,
};

PyTypeObject PossibilityType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "simulator.Possibility",
    .tp_basicsize = sizeof(Possibility),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor) Possibility_Dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Possibility object",
    .tp_new = Possibility_New,
    .tp_init = (initproc) Possibility_Init,
    .tp_iter = (getiterfunc) Possibility_Iter,
    .tp_iternext = (iternextfunc) Possibility_IterNext,
    .tp_as_sequence = &Possibility_sequence_methods,
};


#endif // __POSSIBILITY__