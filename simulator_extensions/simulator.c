#ifndef __SIMULATOR__
#define __SIMULATOR__


#include <python3.11/Python.h>
#include <python3.11/longobject.h>
#include <python3.11/object.h>
#include <python3.11/pyerrors.h>
#include <python3.11/tupleobject.h>
#include <stdlib.h>

#include "combinations.c"
#include "possibilities.c"


#define S_ORBITAL 2
#define P_ORBITAL 6
#define D_ORBITAL 10
#define F_ORBITAL 14
#define TOTAL_ORBITALS 32


typedef struct {
    unsigned int orbitals;
    short ml;
    float ms;
    unsigned int group;
} SimulatorRow;


typedef struct {
    PyObject_HEAD
    unsigned int s, p, d, f, iter;
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
        } else if ((self->possibilities = (Possibilities*) PossibilitiesType.tp_new(&PossibilitiesType, args, kwargs)) == NULL) {
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

static void generate_rows(Simulator* self) {
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
        self->rows[i].group = 0;
        self->rows[i].ms /= 2;
    }
}

static int Simulator_Init(Simulator *self, PyObject *args, PyObject *kwargs) {
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

    generate_rows(self);

    return 0;
}

static void Simulator_Dealloc(Simulator *self) {
    if (self->combinations != NULL)
        Py_XDECREF(self->combinations);

    if (self->possibilities != NULL)
        Py_XDECREF(self->possibilities);

    if (self->rows != NULL)
        free(self->rows);

    Py_TYPE(self)->tp_free((PyObject*) self);
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

static PyObject *Simulator_Iter(Simulator *self) {
    self->iter = 0;

    Py_INCREF(self);

    return (PyObject*) self;
}

static PyObject *int_to_arrow_tuple2(SimulatorRow *row) {
    PyObject *tuple = PyTuple_New((TOTAL_ORBITALS / 2) + 2);
    unsigned int value = row->orbitals;
    for (unsigned short i = 1; i <= TOTAL_ORBITALS / 2; ++i) {
        PyTuple_SET_ITEM(tuple, TOTAL_ORBITALS / 2 - i, int_to_arrow(value % 4));
        value /= 4;
    }
    PyTuple_SET_ITEM(tuple, TOTAL_ORBITALS / 2, PyFloat_FromDouble((double) row->ms));
    PyTuple_SET_ITEM(tuple, TOTAL_ORBITALS / 2 + 1, PyLong_FromLong((long) row->ml));

    return tuple;
}

static PyObject *Simulator_IterNext(Simulator *iter) {
    if (iter->iter < iter->combinations->s * iter->combinations->p * iter->combinations->d * iter->combinations->f) {
        PyObject *tuple = int_to_arrow_tuple2(&iter->rows[iter->iter]);
        Py_INCREF(tuple);
        iter->iter++;
        return tuple;
    } else {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }
}

static PyObject *Simulator_GetItem(Simulator *self, PyObject *key) {
    if (PyLong_Check(key)) {
        return int_to_arrow_tuple2(
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
            PyList_SET_ITEM(result, i / step, int_to_arrow_tuple2(&self->rows[i]));
        }

        return result;
    } else {
        PyErr_SetString(PyExc_TypeError, "Key must be int or slice");
        return NULL;
    }
}

static PyMappingMethods Simulator_sequence_methods = {
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
    .tp_as_mapping = &Simulator_sequence_methods,
};


#endif //__SIMULATOR__