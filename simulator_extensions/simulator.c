/**
 * simulator.c
 *
 * This file contains the implementation of the `Simulator` object.
 */


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


/**
 * @fn      static int generate_rows(Simulator* self)
 *
 * @brief   Helper function to generate the simulator rows and compressed simulator
 *
 * @param   self pointer to a `Simulator` instance
 *
 * @return  int:    0 if succeeded else 1
 */
static int generate_rows(Simulator* self) {
    // calculating the total number of combinations, the number of combinations for d and p
    unsigned int all_combs = self->combinations->s * self->combinations->p * self->combinations->d * self->combinations->f,
            d_combs = self->combinations->f * self->combinations->d,
            p_combs = d_combs * self->combinations->p;

    for (unsigned int i = 0; i < all_combs; i++) {
        // merging the different permutations to one permutation
        self->rows[i].orbitals = (((((self->possibilities->s->poss[i / p_combs] << P_ORBITAL)
                                     | self->possibilities->p->poss[(i % p_combs) / d_combs]) << D_ORBITAL)
                                   | self->possibilities->d->poss[(i % d_combs) / self->combinations->f]) << F_ORBITAL)
                                 | self->possibilities->f->poss[i % self->combinations->f];

        // calculating Ms value for s
        self->rows[i].ms = (float)((self->rows[i].orbitals & (1 << 31)) >> 31) - (float)((self->rows[i].orbitals & (1 << 30)) >> 30);
        // Ml value for s is always 0
        self->rows[i].ml = 0;

        for (int pdf = (P_ORBITAL + D_ORBITAL + F_ORBITAL) / 2 - 1; pdf >= (D_ORBITAL + F_ORBITAL) / 2; --pdf) {
            // calculating Ms and Ml values for p
            self->rows[i].ms += (float)((self->rows[i].orbitals & (1 << (pdf * 2 + 1))) >> (pdf * 2 + 1)) - (float)((self->rows[i].orbitals & (1 << (pdf * 2))) >> (pdf * 2));
            self->rows[i].ml += (((self->rows[i].orbitals & (1 << (pdf * 2 + 1))) >> (pdf * 2 + 1)) + ((self->rows[i].orbitals & (1 << (pdf * 2))) >> (pdf * 2))) * (-(pdf - (D_ORBITAL + F_ORBITAL) / 2) + 1);
        }

        for (int df = (D_ORBITAL + F_ORBITAL) / 2 - 1; df >= F_ORBITAL / 2; --df) {
            // calculating Ms and Ml values for d
            self->rows[i].ms += (float)((self->rows[i].orbitals & (1 << (df * 2 + 1))) >> (df * 2 + 1)) - (float)((self->rows[i].orbitals & (1 << (df * 2))) >> (df * 2));
            self->rows[i].ml += (((self->rows[i].orbitals & (1 << (df * 2 + 1))) >> (df * 2 + 1)) + ((self->rows[i].orbitals & (1 << (df * 2))) >> (df * 2))) * (-(df - F_ORBITAL / 2) + 2);
        }

        for (int f = F_ORBITAL / 2 - 1; f >= 0 / 2; --f) {
            // calculating Ms and Ml values for f
            self->rows[i].ms += (float)((self->rows[i].orbitals & (1 << (f * 2 + 1))) >> (f * 2 + 1)) - (float)((self->rows[i].orbitals & (1 << (f * 2))) >> (f * 2));
            self->rows[i].ml += (((self->rows[i].orbitals & (1 << (f * 2 + 1))) >> (f * 2 + 1)) + ((self->rows[i].orbitals & (1 << (f * 2))) >> (f * 2))) * (-f + 3);
        }

        // dividing Ms value by 2 because it was not done for each step (dividing by 2 at the end is cheaper and faster
        // than dividing each value by 2 separately
        self->rows[i].ms /= 2;

        if (self->compressed->rows == NULL) {
            // if no row compressing was done, memory will be allocated and 1 is returned on failure
            self->compressed->rows = (CompressedSimulatorRow*) malloc(sizeof(CompressedSimulatorRow));
            if (self->compressed->rows == NULL) {
                return 1;
            } else {
                // the element is inserted in the row compressing structure
                self->compressed->rowsCount = 1;
                self->compressed->rows[0].ms = self->rows[i].ms;
                self->compressed->rows[0].ml = self->rows[i].ml;
                self->compressed->rows[0].count = 1;
            }

        } else {
            // searching for the current Ml and Ms values
            short found = 0;
            for (unsigned int j = 0; j < self->compressed->rowsCount; ++j) {
                if (self->compressed->rows[j].ms == self->rows[i].ms
                        && self->compressed->rows[j].ml == self->rows[i].ml) {
                    // if they are found, the counter is incremented by 1
                    ++self->compressed->rows[j].count;
                    found = 1;
                    break;
                }
            }

            if (!found) {
                // if the values are not found, the row compressing structure is re-allocated for more memory and 2 is
                // returned on failure
                CompressedSimulatorRow *tmp = (CompressedSimulatorRow*)
                        realloc(self->compressed->rows, sizeof(CompressedSimulatorRow) * ++self->compressed->rowsCount);
                if (tmp == NULL) {
                    return 2;
                } else {
                    // the row Ms and Ml values are inserted in the row compressing structure
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

/**
 * @fn      static PyObject *row_to_arrow_tuple(SimulatorRow *row)
 *
 * @brief   Helper function that converts a `SimulatorRow` into a tuple of harpoon arrows
 *
 * @param   row the `SimulatorRow` that needs to be converted
 *
 * @return  PyObject*:  Tuple containing the harpoon arrows and Ml and Ms values
 */
static PyObject *row_to_arrow_tuple(SimulatorRow *row) {
    // creating a tuple that is returned
    PyObject *tuple = PyTuple_New((TOTAL_ORBITALS / 2) + 2);
    // copying the orbital value, so it can be modified without modifying the real value
    unsigned int value = row->orbitals;

    for (unsigned short i = 1; i <= TOTAL_ORBITALS / 2; ++i) {
        // converting 2 bits ore one orbital at a time to harpoon arrows
        PyTuple_SET_ITEM(tuple, TOTAL_ORBITALS / 2 - i, int_to_arrow(value % 4));
        value /= 4;
    }

    // adding the Ml and Ms values to the tuple
    PyTuple_SET_ITEM(tuple, TOTAL_ORBITALS / 2, PyLong_FromLong((long) row->ml));
    PyTuple_SET_ITEM(tuple, TOTAL_ORBITALS / 2 + 1, PyFloat_FromDouble((double) row->ms));

    return tuple;
}


PyObject *Simulator_New(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
    // creating a new simulator instance
    Simulator *self = (Simulator*) type->tp_alloc(type, 0);
    if (self != NULL) {
        // if the pointer is not NULL, the allocation was successful

        if ((self->combinations = (Combinations*) CombinationsType.tp_new(&CombinationsType, args, kwargs)) == NULL) {
            // creating a new combinations object and destroying the simulator if the creating failed
            Py_DECREF(self);
            Py_TYPE(self)->tp_free((PyObject*) self);
            self = NULL;
        } else if ((self->possibilities = (Possibilities*) PossibilitiesType.tp_new(&PossibilitiesType, args, kwargs)) == NULL) {
            // creating a new possibilities object and destroying the simulator if the creating failed
            Py_DECREF(self);
            Py_TYPE(self)->tp_free((PyObject*) self);
            self = NULL;
        } else if ((self->compressed = (CompressedSimulator*) malloc(sizeof(CompressedSimulator))) == NULL) {
            // creating a new compressed simulator object and destroying the simulator if the creating failed
            Py_DECREF(self);
            Py_TYPE(self)->tp_free((PyObject*) self);
            self = NULL;
        } else {
            // filling the empty fields with a value to prevent wrong behaviour
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
    // parsing the arguments
    PyArg_ParseTuple(args, "iiii", &self->s, &self->p, &self->d, &self->f);

    // initialising the sub objects of the simulator
    CombinationsType.tp_init((PyObject*) self->combinations, args, kwargs);
    // the possibilities object needs different arguments, so they have to be built
    PyObject *args_tuple = PyTuple_New(2);
    PyTuple_SET_ITEM(args_tuple, 0, args);
    PyTuple_SET_ITEM(args_tuple, 1, (PyObject*) self->combinations);
    PossibilitiesType.tp_init((PyObject*) self->possibilities, args_tuple, kwargs);

    // allocating memory for the simulator
    self->rows = (SimulatorRow*) malloc(
        sizeof(SimulatorRow) * self->combinations->s * self->combinations->p * self->combinations->d * self->combinations->f
    );
    if (self->rows == NULL) {
        // destroying the simulator on allocation failure
        Py_XDECREF(self);
        PyErr_NoMemory();
        return 1;
    }

    // generating the rows of the simulator
    if (generate_rows(self)) {
        // destroying the simulator if the allocation for a group failed
        Py_XDECREF(self);
        PyErr_NoMemory();
        return 1;
    }

    return 0;
}

void Simulator_Dealloc(Simulator *self) {
    // decrementing the references of combinations, possibilities, rows and compressed rows (they are going to be freed
    // by the Python interpreter if no reference is left over)
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

    // freeing the simulator object
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
        // continuing the iteration and creating a tuple of the row
        PyObject *tuple = row_to_arrow_tuple(&iter->rows[iter->iter]);
        // incrementing the reference of the row so the row is not instantly freed by the Python interpreter
        Py_INCREF(tuple);
        // incrementing the iteration status by 1
        iter->iter++;
        return tuple;
    } else {
        // raise StopIteration
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }
}

PyObject *Simulator_GetItem(Simulator *self, PyObject *key) {
    if (PyLong_Check(key)) {
        // if the key is a PyLong, the row can instantly be returned
        return row_to_arrow_tuple(
            &self->rows[PyLong_AsSize_t(key) % self->combinations->s * self->combinations->p * self->combinations->d * self->combinations->f]
        );
    } else if (PySlice_Check(key)) {
        // the key is a slice object

        // crating start, stop, step and slice length variables
        Py_ssize_t start, stop, step, slicelength;

        if (PySlice_GetIndicesEx(
                key,
                self->combinations->s * self->combinations->p * self->combinations->d * self->combinations->f,
                &start,
                &stop,
                &step,
                &slicelength
        ) < 0) {
            // the slice is not compatible with the simulator
            PyErr_SetString(PyExc_ValueError, "Invalid slice");
            return NULL;
        }

        // creating a list that is returned
        PyObject *result = PyList_New(slicelength);
        if (result == NULL) {
            // no memory available for the list allocation
            PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for result");
            return NULL;
        }

        for (unsigned int i = start; i < stop; i += step) {
            // inserting the rows in the list
            PyList_SET_ITEM(result, (i - start) / step, row_to_arrow_tuple(&self->rows[i]));
        }
        return result;
    } else {
        // the key is not a PyLong or PySlice
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