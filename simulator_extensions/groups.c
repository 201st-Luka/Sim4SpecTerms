/**
 * groups.c
 *
 * This file contains the implementations of the grouping functions.
 */


#include <python3.11/Python.h>
#include <stdlib.h>

#include "groups.h"
#include "simulator.h"


// marco to calculate the absolute value of a number
#define ABS(expression) (((expression) >= 0) ? (expression) : ((expression) * (-1)))


/**
 * @fn      static short find_max_ml(Simulator *simulator)
 *
 * @brief   Static function that searches the maximal ml value in the simulator->compressed structure
 *
 * @param   simulator a pointer to a `Simulator` object
 *
 * @return  short:  The biggest ml value or -1 if no row left
 */
static short find_max_ml(Simulator *simulator) {
    short max_ml = -1;
    for (unsigned int i = 0; i < simulator->compressed->rowsCount / 2; ++i) {
        if (simulator->compressed->rows[i].count) {
            if (simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].count) {
                if (simulator->compressed->rows[i].ml > simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].ml) {
                    if (simulator->compressed->rows[i].ml > max_ml)
                        max_ml = simulator->compressed->rows[i].ml;
                } else if (simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].ml > max_ml)
                    max_ml = simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].ml;
            } else if (simulator->compressed->rows[i].ml > max_ml)
                max_ml = simulator->compressed->rows[i].ml;
        } else if (simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].count
                   && simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].ml > max_ml)
            max_ml = simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].ml;
    }

    return max_ml;
}

/**
 * @fn      static float find_max_ms_with_ml(Simulator *simulator, short abs_ml)
 *
 * @brief   Static function that searches the maximal ml value in the simulator->compressed structure
 *
 * @param   simulator a pointer to a `Simulator` object
 * @param   abs_ml the ml value that is associated with the ms value
 *
 * @return  short:  The biggest ms value with matching ml value or -1 if no row left
 */
static float find_max_ms_with_ml(Simulator *simulator, short abs_ml) {
    float max_ms = -1;

    for (unsigned int i = 0; i < simulator->compressed->rowsCount / 2; ++i) {
        if (simulator->compressed->rows[i].count
            && simulator->compressed->rows[i].ml == abs_ml) {
            if (simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].count
                && simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].ml == abs_ml) {
                if (simulator->compressed->rows[i].ms > simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].ms) {
                    if (simulator->compressed->rows[i].ms > max_ms)
                        max_ms = simulator->compressed->rows[i].ms;
                } else if (simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].ms > max_ms)
                    max_ms = simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].ms;
            } else if (simulator->compressed->rows[i].ms > max_ms)
                max_ms = simulator->compressed->rows[i].ms;
        } else if(simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].count
                  && simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].ml == abs_ml
                  && simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].ms > max_ms)
            max_ms = simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].ms;
    }

    return max_ms;
}

/**
 * @fn      static Terms *create_terms(unsigned short abs_ml, float abs_ms)
 *
 * @brief   Creator of `Terms`
 *
 * @param   abs_ml absolute value of ml
 * @param   abs_ms absolute value of ns
 *
 * @return  Terms*: Pointer to the created `Terms`
 */
static Terms *create_terms(unsigned short abs_ml, float abs_ms) {
    // allocate memory for the Terms structure and return NULL if the allocation failed
    Terms *terms = (Terms*) malloc(sizeof(Terms));
    if (terms == NULL)
        return NULL;

    // setting the term character
    char *x;
    switch (abs_ml) {
        case 0:
            x = "S";
            break;
        case 1:
            x = "P";
            break;
        case 2:
            x = "D";
            break;
        default:
            x = "F";
            break;
    }

    // calculate the values that are written around the term letter
    unsigned short sup = (unsigned short) (2 * abs_ms) + 1;
    float min_sub = ABS(abs_ml - abs_ms), max_sub = abs_ml + abs_ms;

    // allocating memory for the terms and return NULL on failure
    terms->term_count = (unsigned short) (max_sub - min_sub) + 1;
    terms->terms = (Term*) malloc(sizeof(Term) * terms->term_count);
    if (terms->terms == NULL) {
        free(terms);
        return NULL;
    }

    // building the different terms
    for (unsigned short i = 0; i < terms->term_count; ++i) {
        terms->terms[i].sup = sup;
        terms->terms[i].x = x;
        terms->terms[i].sub = min_sub + i;
    }

    return terms;
}

/**
 * @fn      static int create_group(Groups *groups, Simulator *simulator, unsigned int combs)
 *
 * @brief   Creates a group and adds it to the `groups`
 *
 * @param   groups pointer to a `Groups` instance
 * @param   simulator pointer to a `Simulator` instance
 *
 * @return  0 if success else a value from 1 to 5
 */
static int create_group(Groups *groups, Simulator *simulator) {
    // finding the maximal ml value of the simulator and returning 1 if nothing was found
    short max_ml = find_max_ml(simulator);
    if (max_ml == -1)
        return 1;

    // finding the maximal ms value that has the previously calculated ml value and return 2 if nothing was found
    float max_ms = find_max_ms_with_ml(simulator, max_ml);
    if (max_ms == -1.0)
        return 2;

    if (groups->groups == NULL) {
        // if groups->groups == NULL, no group was created and memory must be allocated (returns 3 on failure)
        groups->groups = (Group*) malloc(sizeof(Group));
        groups->group_count = 1;
        if (groups->groups == NULL)
            return 3;

        // setting the first group's values
        groups->groups[0].count = 1;
        groups->groups[0].id = 1;
        groups->groups[0].abs_ml = max_ml;
        groups->groups[0].abs_ms = max_ms;
        groups->groups[0].terms = create_terms(max_ml, max_ms);

        // return 5 if the term creation failed
        if (groups->groups[0].terms == NULL)
            return 5;

    } else {
        // searching the ml and ms values in the groups
        short found = 0;
        for (unsigned int i = 0; i < groups->group_count && !found; ++i) {
            if (groups->groups[i].abs_ml == max_ml
                    && groups->groups[i].abs_ms == max_ms) {
                // if the ml and ms values are found, the group count increments
                ++groups->groups[i].count;
                found = 1;
            }
        }

        if (!found) {
            // if the ml and ms values are not found, the groups are re-allocated and 4 is returned on failure
            Group *tmp_ptr = (Group*) realloc(groups->groups, sizeof(Group) * ++groups->group_count);
            if (tmp_ptr == NULL)
                return 4;

            groups->groups = tmp_ptr;

            // setting the group values for the created group
            groups->groups[groups->group_count - 1].count = 1;
            groups->groups[groups->group_count - 1].id = groups->group_count;
            groups->groups[groups->group_count - 1].abs_ml = max_ml;
            groups->groups[groups->group_count - 1].abs_ms = max_ms;
            groups->groups[groups->group_count - 1].terms = create_terms(max_ml, max_ms);

            // returning 5 if the term creation failed
            if (groups->groups[groups->group_count - 1].terms == NULL)
                return 5;
        }
    }

    // decrementing the count of used rows in the compressed simulator
    for (short i_ml = -max_ml; i_ml <= max_ml; ++i_ml) {
        // from -max_ml to max_ml
        for (short i_ms = (short) (-max_ms * 2); i_ms <= (short) (max_ms * 2); ++i_ms) {
            // from -max_ms to max_ms (float iteration is awful so the float is converted to a short)
            for (unsigned short i = 0; i < simulator->compressed->rowsCount / 2; ++i) {
                // for each compressed simulator row
                if (simulator->compressed->rows[i].count
                        && simulator->compressed->rows[i].ml == i_ml
                        && simulator->compressed->rows[i].ms == ((float) i_ms) / 2) {
                    // if the values match, the row counter decrements by 1
                    --simulator->compressed->rows[i].count;
                    break;
                } else if (simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].count
                           && simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].ml == i_ml
                           && simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].ms == ((float) i_ms) / 2) {
                    // if the previous `if` found nothing, this if checks from the end of the array (to use less
                    // iterations and reducing the theoretical complexity of the algorithm
                    --simulator->compressed->rows[simulator->compressed->rowsCount - 1 - i].count;
                    break;
                }
            }
        }
    }

    return 0;
}

/**
 * @fn      static PyObject *group_to_tuple(Group *group)
 *
 * @brief   Converter that converts a group to a tuple of values
 *
 * @param   group a pointer to a `Group`
 *
 * @return  PyObject*:  A tuple containing the group
 */
static PyObject *group_to_tuple(Group *group) {
    // creating tuple object for as return value and for the terms
    PyObject *tuple = PyTuple_New(5), *terms = PyTuple_New(group->terms->term_count);

    // setting the term items in the terms tuple
    for (unsigned short i = 0; i < group->terms->term_count; ++i) {
        PyObject *term = PyTuple_New(3);
        PyTuple_SET_ITEM(term, 0, PyLong_FromSsize_t((unsigned int) group->terms->terms[i].sup));
        PyTuple_SET_ITEM(term, 1, PyUnicode_FromString(group->terms->terms[i].x));
        PyTuple_SET_ITEM(term, 2, PyFloat_FromDouble((double) group->terms->terms[i].sub));

        PyTuple_SET_ITEM(terms, i, term);
    }

    // setting the group items in the final tuple
    PyTuple_SET_ITEM(tuple, 0, PyLong_FromSize_t(group->id));
    PyTuple_SET_ITEM(tuple, 1, PyLong_FromSsize_t((int) group->abs_ml));
    PyTuple_SET_ITEM(tuple, 2, PyFloat_FromDouble((double) group->abs_ms));
    PyTuple_SET_ITEM(tuple, 3, PyLong_FromSize_t(group->count));
    PyTuple_SET_ITEM(tuple, 4, terms);

    // incrementing the reference for the Python reference counter
    Py_INCREF(tuple);

    // returning the result
    return tuple;
}


PyObject *Groups_New(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
    Groups *self = (Groups*) type->tp_alloc(type, 0);
    if (self != NULL)
        self->groups = NULL;
    else {
        PyErr_SetString(PyExc_MemoryError, "Not enough free memory left.");
    }

    return (PyObject*) self;
}

int Groups_Init(Groups *self, PyObject *args, PyObject *kwargs) {
    Simulator *simulator = NULL;
    PyArg_ParseTuple(args, "O!", &SimulatorType, &simulator);
    if (simulator == NULL)
        return 1;

    self->group_count = 0;

    int result;
    while ((result = create_group(self, simulator)) == 0);


    if (result) {
        return 0;
    } else {
        PyErr_SetString(PyExc_Exception, "Something went wrong while calculating the groups.");
        return 1;
    }
}

void Groups_Dealloc(Groups *self) {
    if (self->groups != NULL) {
        for (unsigned int i = 0; i < self->group_count; ++i) {
            free(self->groups[i].terms->terms);
            free(self->groups[i].terms);
        }

        free(self->groups);
    }

    Py_TYPE(self)->tp_free((PyObject*) self);
}

PyObject *Groups_GetItem(Groups *self, PyObject *key) {
    if (PyLong_Check(key)) {
        return group_to_tuple(&(self->groups[PyLong_AsSize_t(key) % self->group_count]));
    } else if (PySlice_Check(key)) {
        Py_ssize_t start, stop, step, slicelength;

        if (PySlice_GetIndicesEx(
                key,
                self->group_count,
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
            PyList_SET_ITEM(result, i / step, group_to_tuple(&(self->groups[i])));
        }

        return result;
    } else {
        PyErr_SetString(PyExc_TypeError, "Key must be int or slice");
        return NULL;
    }
}

PyObject *Groups_Iter(Groups *self) {
    self->iter = 0;

    Py_INCREF(self);

    return (PyObject*) self;
}

PyObject *Groups_IterNext(Groups *iter) {
    if (iter->iter < iter->group_count)
        return group_to_tuple(&(iter->groups[iter->iter++]));

    PyErr_SetNone(PyExc_StopIteration);
    return NULL;
}

Py_ssize_t Groups_Len(Groups *self) {
    return (Py_ssize_t) self->group_count;
}


PySequenceMethods Groups_sequence_methods = {
    .sq_length = (lenfunc) Groups_Len,
};

PyMappingMethods Groups_mapping_methods = {
    .mp_length = 0,
    .mp_subscript = (binaryfunc) Groups_GetItem,
};

PyTypeObject GroupsType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "simulator.Groups",
    .tp_basicsize = sizeof(Groups),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor) Groups_Dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Groups object",
    .tp_new = Groups_New,
    .tp_init = (initproc) Groups_Init,
    .tp_iter = (getiterfunc) Groups_Iter,
    .tp_iternext = (iternextfunc) Groups_IterNext,
    .tp_as_mapping = &Groups_mapping_methods,
    .tp_as_sequence = &Groups_sequence_methods,
};
