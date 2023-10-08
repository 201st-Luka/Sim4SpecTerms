#ifndef __GROUPS__
#define __GROUPS__


#include <python3.11/Python.h>
#include <stdlib.h>

#include "simulator.c"


typedef struct {
    short abs_ml;
    float abs_ms;
    unsigned int id, count;
    char *term;
} Group;

typedef struct {
    PyObject_HEAD
    Group *groups;
    unsigned int group_count, iter;
} Groups;


static PyObject *Groups_New(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
    Groups *self = (Groups*) type->tp_alloc(type, 0);
    if (self != NULL)
        self->groups = NULL;
    else {
        PyErr_SetString(PyExc_MemoryError, "Not enough free memory left.");
    }

    return (PyObject*) self;
}

static short find_max_ml_no_group(Simulator *simulator, unsigned int combs) {
    short max_ml = -1;
    for (unsigned int i = 0; i < combs / 2; ++i) {
        if (!simulator->rows[i].group) {
            if (!simulator->rows[combs - 1 - i].group) {
                if (simulator->rows[i].ml > simulator->rows[combs - 1 - i].ml) {
                    if (simulator->rows[i].ml > max_ml)
                        max_ml = simulator->rows[i].ml;
                } else if (simulator->rows[combs - 1 - i].ml > max_ml)
                    max_ml = simulator->rows[combs - 1 - i].ml;
            } else if (simulator->rows[i].ml > max_ml)
                max_ml = simulator->rows[i].ml;
        } else if (!simulator->rows[combs - 1 - i].group && simulator->rows[combs - 1 - i].ml > max_ml)
            max_ml = simulator->rows[combs - 1 - i].ml;
    }

//    for (unsigned int i = 0; i < combs; ++i) {
//        if (!simulator->rows[i].group && simulator->rows[i].ml > max_ml)
//            max_ml = simulator->rows[i].ml;
//    }

    return max_ml;
}

static float find_max_ms_no_group_with_ml(Simulator *simulator, unsigned int combs, short abs_ml) {
    float max_ms = -1;

    for (unsigned int i = 0; i < combs / 2; ++i) {
        if (!simulator->rows[i].group
                && simulator->rows[i].ml == abs_ml) {
            if (!simulator->rows[combs - 1 - i].group
                    && simulator->rows[combs - 1 - i].ml == abs_ml) {
                if (simulator->rows[i].ms > simulator->rows[combs - 1 - i].ms) {
                    if (simulator->rows[i].ms > max_ms)
                        max_ms = simulator->rows[i].ms;
                } else if (simulator->rows[combs - 1 - i].ms > max_ms)
                    max_ms = simulator->rows[combs - 1 - i].ms;
            } else if (simulator->rows[i].ms > max_ms)
                max_ms = simulator->rows[i].ms;
        } else if(!simulator->rows[combs - 1 - i].group
                && simulator->rows[combs - 1 - i].ml == abs_ml
                && simulator->rows[combs - 1 - i].ms > max_ms)
            max_ms = simulator->rows[combs - 1 - i].ms;
    }

//    for (unsigned int i = 0; i < combs; ++i) {
//        if (!simulator->rows[i].group && simulator->rows[i].ml == abs_ml && simulator->rows[i].ms > max_ms)
//            max_ms = simulator->rows[i].ms;
//    }

    return max_ms;
}

static int create_group(Groups *groups, Simulator *simulator, unsigned int combs) {
    short max_ml = find_max_ml_no_group(simulator, combs);
    if (max_ml == -1)
        return 1;

    float max_ms = find_max_ms_no_group_with_ml(simulator, combs, max_ml);
    if (max_ms == -1.0)
        return 2;

    unsigned int group_id;
    if (groups->groups == NULL) {
        groups->groups = (Group*) malloc(sizeof(Group));
        groups->group_count = 1;

        if (groups->groups == NULL)
            return 3;

        groups->groups[0].count = 1;
        groups->groups[0].id = 1;
        groups->groups[0].abs_ml = max_ml;
        groups->groups[0].abs_ms = max_ms;
//        groups->groups[0].term = "None";

        group_id = 1;
    } else {
        short found = 0;
        for (unsigned int i = 0; i < groups->group_count && !found; ++i) {
            if (groups->groups[i].abs_ml == max_ml
                    && groups->groups[i].abs_ms == max_ms) {
                ++groups->groups[i].count;
                found = 1;
                group_id = i + 1;
            }
        }

        if (!found) {
            Group *tmp_ptr = (Group*) realloc(groups->groups, sizeof(Group) * ++groups->group_count);
            if (tmp_ptr == NULL)
                return 4;

            groups->groups = tmp_ptr;

            groups->groups[groups->group_count - 1].count = 1;
            groups->groups[groups->group_count - 1].id = groups->group_count;
            groups->groups[groups->group_count - 1].abs_ml = max_ml;
            groups->groups[groups->group_count - 1].abs_ms = max_ms;
//            groups->groups[groups->group_count - 1].term = "None";

            group_id = groups->group_count;
        }
    }

    for (short i_ml = -max_ml; i_ml <= max_ml; ++i_ml) {
        for (short i_ms = (short) (-max_ms * 2); i_ms <= (short) (max_ms * 2); ++i_ms) {
            for (unsigned short i = 0; i < combs; ++i) {
                if (!simulator->rows[i].group
                        && simulator->rows[i].ml == i_ml
                        && simulator->rows[i].ms == ((float) i_ms) / 2) {
                    simulator->rows[i].group = group_id;
                    break;
                }
            }
        }
    }
    return 0;
}

static int Groups_Init(Groups *self, PyObject *args, PyObject *kwargs) {
    Simulator *simulator;
    PyArg_ParseTuple(args, "O!", &SimulatorType, &simulator);

    self->group_count = 0;

    unsigned int combs = simulator->combinations->s
            * simulator->combinations->p
            * simulator->combinations->d
            * simulator->combinations->f;

    int result;
    while ((result = create_group(self, simulator, combs)) == 0);


    if (result) {
        return 0;
    } else {
        PyErr_SetString(PyExc_Exception, "Something went wrong while calculating the groups.");
        return 1;
    }
}

static void Groups_Dealloc(Groups *self) {
    if (self->groups != NULL)
        free(self->groups);

    Py_TYPE(self)->tp_free((PyObject*) self);
}


static PyObject *group_to_tuple(Group *group) {
    PyObject *tuple = PyTuple_New(5);

    PyTuple_SET_ITEM(tuple, 0, PyLong_FromSize_t(group->id));
    PyTuple_SET_ITEM(tuple, 1, PyLong_FromSsize_t((int) group->abs_ml));
    PyTuple_SET_ITEM(tuple, 2, PyFloat_FromDouble((double) group->abs_ms));
    PyTuple_SET_ITEM(tuple, 3, PyLong_FromSize_t(group->count));
//    PyTuple_SET_ITEM(tuple, 4, PyUnicode_FromString(group->term));
    PyTuple_SET_ITEM(tuple, 4, PyUnicode_FromString(""));

    Py_INCREF(tuple);

    return tuple;
}


static PyObject *Groups_GetItem(Groups *self, PyObject *key) {
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

static PyObject *Groups_Iter(Groups *self) {
    self->iter = 0;

    Py_INCREF(self);

    return (PyObject*) self;
}

static PyObject *Groups_IterNext(Groups *iter) {
    if (iter->iter < iter->group_count)
        return group_to_tuple(&(iter->groups[iter->iter++]));

    PyErr_SetNone(PyExc_StopIteration);
    return NULL;
}

static Py_ssize_t Groups_Len(Groups *self) {
    return (Py_ssize_t) self->group_count;
}


static PySequenceMethods Groups_sequence_methods = {
        .sq_length = (lenfunc) Groups_Len,
};

static PyMappingMethods Groups_mapping_methods = {
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


#endif // __GROUPS__