#ifndef __SIMULATOR__
#define __SIMULATOR__


#include <python3.11/Python.h>
#include <stdlib.h>

#include "simulator.h"


#define S_ORBITAL 2
#define P_ORBITAL 6
#define D_ORBITAL 10
#define F_ORBITAL 14
#define TOTAL_ORBITALS 32


static int generate_rows(Simulator* self) {
    unsigned int all_combs = self->combinations->s * self->combinations->p * self->combinations->d * self->combinations->f,
            d_combs = self->combinations->f * self->combinations->d,
            p_combs = d_combs * self->combinations->p;

    for (unsigned int i = 0; i < all_combs; i++) {
        self->rows[i].orbitals = (((((self->possibilities->s->poss[i / p_combs] << P_ORBITAL)
                                     | self->possibilities->p->poss[(i % p_combs) / d_combs]) << D_ORBITAL)
                                   | self->possibilities->d->poss[(i % d_combs) / self->combinations->f]) << F_ORBITAL)
                                 | self->possibilities->f->poss[i % self->combinations->f];
        self->rows[i].ms = (float)((self->rows[i].orbitals & (1 << 31)) >> 31) - (float)((self->rows[i].orbitals & (1 << 30)) >> 30);
        self->rows[i].ml = 0;
        for (int pdf = (P_ORBITAL + D_ORBITAL + F_ORBITAL) / 2 - 1; pdf >= (D_ORBITAL + F_ORBITAL) / 2; --pdf) {
            self->rows[i].ms += (float)((self->rows[i].orbitals & (1 << (pdf * 2 + 1))) >> (pdf * 2 + 1)) - (float)((self->rows[i].orbitals & (1 << (pdf * 2))) >> (pdf * 2));
            self->rows[i].ml += (((self->rows[i].orbitals & (1 << (pdf * 2 + 1))) >> (pdf * 2 + 1)) + ((self->rows[i].orbitals & (1 << (pdf * 2))) >> (pdf * 2))) * (-(pdf - (D_ORBITAL + F_ORBITAL) / 2) + 1);
        }
        for (int df = (D_ORBITAL + F_ORBITAL) / 2 - 1; df >= F_ORBITAL / 2; --df) {
            self->rows[i].ms += (float)((self->rows[i].orbitals & (1 << (df * 2 + 1))) >> (df * 2 + 1)) - (float)((self->rows[i].orbitals & (1 << (df * 2))) >> (df * 2));
            self->rows[i].ml += (((self->rows[i].orbitals & (1 << (df * 2 + 1))) >> (df * 2 + 1)) + ((self->rows[i].orbitals & (1 << (df * 2))) >> (df * 2))) * (-(df - F_ORBITAL / 2) + 2);
        }
        for (int f = F_ORBITAL / 2 - 1; f >= 0 / 2; --f) {
            self->rows[i].ms += (float)((self->rows[i].orbitals & (1 << (f * 2 + 1))) >> (f * 2 + 1)) - (float)((self->rows[i].orbitals & (1 << (f * 2))) >> (f * 2));
            self->rows[i].ml += (((self->rows[i].orbitals & (1 << (f * 2 + 1))) >> (f * 2 + 1)) + ((self->rows[i].orbitals & (1 << (f * 2))) >> (f * 2))) * (-f + 3);
        }
        self->rows[i].ms /= 2;

        if (self->compressed->rows == NULL) {
            self->compressed->rows = (CompressedSimulatorRow*) malloc(sizeof(CompressedSimulatorRow));
            if (self->compressed->rows == NULL) {
                return 1;
            } else {
                self->compressed->rowsCount = 1;
                self->compressed->rows[0].ms = self->rows[i].ms;
                self->compressed->rows[0].ml = self->rows[i].ml;
                self->compressed->rows[0].count = 1;
            }

        } else {
            short found = 0;
            for (unsigned int j = 0; j < self->compressed->rowsCount; ++j) {
                if (self->compressed->rows[j].ms == self->rows[i].ms
                    && self->compressed->rows[j].ml == self->rows[i].ml) {
                    ++self->compressed->rows[j].count;
                    found = 1;
                    break;
                }
            }

            if (!found) {
                CompressedSimulatorRow *tmp = (CompressedSimulatorRow*)
                        realloc(self->compressed->rows, sizeof(CompressedSimulatorRow) * ++self->compressed->rowsCount);
                if (tmp == NULL) {
                    return 2;
                } else {
                    self->compressed->rows = tmp;
                    self->compressed->rows[self->compressed->rowsCount - 1].ms = self->rows[i].ms;
                    self->compressed->rows[self->compressed->rowsCount - 1].ml = self->rows[i].ml;
                    self->compressed->rows[self->compressed->rowsCount - 1].count = 1;
                }
            }
        }
    }

    return 0;
}

static PyObject *row_to_arrow_tuple(SimulatorRow *row) {
    PyObject *tuple = PyTuple_New((TOTAL_ORBITALS / 2) + 2);
    unsigned int value = row->orbitals;
    for (unsigned short i = 1; i <= TOTAL_ORBITALS / 2; ++i) {
        PyTuple_SET_ITEM(tuple, TOTAL_ORBITALS / 2 - i, int_to_arrow(value % 4));
        value /= 4;
    }
    PyTuple_SET_ITEM(tuple, TOTAL_ORBITALS / 2, PyLong_FromLong((long) row->ml));
    PyTuple_SET_ITEM(tuple, TOTAL_ORBITALS / 2 + 1, PyFloat_FromDouble((double) row->ms));

    return tuple;
}


PyObject *Simulator_New(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
    Simulator *self = (Simulator*) type->tp_alloc(type, 0);
    if (self != NULL) {
        if ((self->combinations = (Combinations*) CombinationsType.tp_new(&CombinationsType, args, kwargs)) == NULL) {
            Py_DECREF(self);
            Py_TYPE(self)->tp_free((PyObject*) self);
            self = NULL;
        } else if ((self->possibilities = (Possibilities*) PossibilitiesType.tp_new(&PossibilitiesType, args, kwargs)) == NULL) {
            Py_DECREF(self);
            Py_TYPE(self)->tp_free((PyObject*) self);
            self = NULL;
        } else if ((self->compressed = (CompressedSimulator*) malloc(sizeof(CompressedSimulator))) == NULL) {
            Py_DECREF(self);
            Py_TYPE(self)->tp_free((PyObject*) self);
            self = NULL;
        } else {
            self->s = 0;
            self->p = 0;
            self->d = 0;
            self->f = 0;
            self->compressed->rows = NULL;
            self->compressed->rowsCount = 0;
        }

    }
    return (PyObject*) self;
}

int Simulator_Init(Simulator *self, PyObject *args, PyObject *kwargs) {
    PyArg_ParseTuple(args, "iiii", &self->s, &self->p, &self->d, &self->f);

    CombinationsType.tp_init((PyObject*) self->combinations, args, kwargs);
    PyObject *args_tuple = PyTuple_New(2);
    PyTuple_SET_ITEM(args_tuple, 0, args);
    PyTuple_SET_ITEM(args_tuple, 1, (PyObject*) self->combinations);
    PossibilitiesType.tp_init((PyObject*) self->possibilities, args_tuple, kwargs);

    self->rows = (SimulatorRow*) malloc(
        sizeof(SimulatorRow) * self->combinations->s * self->combinations->p * self->combinations->d * self->combinations->f
    );
    if (self->rows == NULL) {
        Py_XDECREF(self);
        PyErr_NoMemory();
        return 1;
    }

    if (generate_rows(self)) {
        Py_XDECREF(self);
        PyErr_NoMemory();
        return 1;
    }

    return 0;
}

void Simulator_Dealloc(Simulator *self) {
    if (self->combinations != NULL)
        Py_XDECREF(self->combinations);

    if (self->possibilities != NULL)
        Py_XDECREF(self->possibilities);

    if (self->rows != NULL)
        free(self->rows);

    if (self->compressed != NULL) {
        if (self->compressed->rows != NULL)
            free(self->compressed->rows);
        free(self->compressed);
    }

    Py_TYPE(self)->tp_free((PyObject*) self);
}

PyObject *Simulator_GetCombinations(Simulator *self, void *closure) {
    Py_INCREF(self->combinations);
    return (PyObject*) self->combinations;
}

PyObject  *Simulator_GetPossibilities(Simulator* self, void *closure) {
    Py_INCREF(self->possibilities);
    return (PyObject*) self->possibilities;
}

PyObject *Simulator_Iter(Simulator *self) {
    self->iter = 0;

    Py_INCREF(self);

    return (PyObject*) self;
}

PyObject *Simulator_IterNext(Simulator *iter) {
    if (iter->iter < iter->combinations->s * iter->combinations->p * iter->combinations->d * iter->combinations->f) {
        PyObject *tuple = row_to_arrow_tuple(&iter->rows[iter->iter]);
        Py_INCREF(tuple);
        iter->iter++;
        return tuple;
    } else {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }
}

PyObject *Simulator_GetItem(Simulator *self, PyObject *key) {
    if (PyLong_Check(key)) {
        return row_to_arrow_tuple(
            &self->rows[PyLong_AsSize_t(key) % self->combinations->s * self->combinations->p * self->combinations->d * self->combinations->f]
        );
    } else if (PySlice_Check(key)) {
        Py_ssize_t start, stop, step, slicelength;

        if (PySlice_GetIndicesEx(
                key,
                self->combinations->s * self->combinations->p * self->combinations->d * self->combinations->f,
                &start,
                &stop,
                &step,
                &slicelength
        ) < 0) {
            PyErr_SetString(PyExc_ValueError, "Invalid slice");
            return NULL;
        }

        PyObject *result = PyList_New(slicelength);
        if (result == NULL) {
            PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for result");
            return NULL;
        }

        for (unsigned int i = start; i < stop; i += step) {
            PyList_SET_ITEM(result, (i - start) / step, row_to_arrow_tuple(&self->rows[i]));
        }
        return result;
    } else {
        PyErr_SetString(PyExc_TypeError, "Key must be int or slice");
        return NULL;
    }
}

Py_ssize_t Simulator_Len(Simulator *self) {
    return (Py_ssize_t) (self->combinations->s * self->combinations->p * self->combinations->d * self->combinations->f);
}


PyGetSetDef Simulator_getset[] = {
        {"combinations", (getter) Simulator_GetCombinations, NULL, "combinations of the orbirals", NULL},
        {"possibilities", (getter) Simulator_GetPossibilities, NULL, "possibilities of the orbirals", NULL},
        {NULL}
};

PySequenceMethods Simulator_sequence_methods = {
    .sq_length = (lenfunc) Simulator_Len,
};

PyMappingMethods Simulator_mapping_methods = {
    .mp_length = 0,
    .mp_subscript = (binaryfunc) Simulator_GetItem,
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
    .tp_iter = (getiterfunc) Simulator_Iter,
    .tp_iternext = (iternextfunc) Simulator_IterNext,
    .tp_getset = Simulator_getset,
    .tp_as_mapping = &Simulator_mapping_methods,
    .tp_as_sequence = &Simulator_sequence_methods,
};


#endif //__SIMULATOR__