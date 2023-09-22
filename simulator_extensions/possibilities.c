#ifndef __POSSIBILITIES__
#define __POSSIBILITIES__


#include <python3.11/Python.h>
#include <python3.11/longobject.h>
#include <python3.11/pyerrors.h>
#include <python3.11/tupleobject.h>

#include "possibility.c"
#include "combinations.c"


typedef struct {
    PyObject_HEAD
    Possibility *s, *p, *d, *f;
} Possibilities;

static PyObject *Possibilities_New(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
    Possibilities *self = (Possibilities*) type->tp_alloc(type, 0);

    self->s = (Possibility*) PossibilityType.tp_new(&PossibilityType, args, kwargs);
    self->p = (Possibility*) PossibilityType.tp_new(&PossibilityType, args, kwargs);
    self->d = (Possibility*) PossibilityType.tp_new(&PossibilityType, args, kwargs);
    self->f = (Possibility*) PossibilityType.tp_new(&PossibilityType, args, kwargs);

    return (PyObject*) self;
}

static int Possibilities_Init(Possibilities *self, PyObject *args, PyObject *kwargs) {
    PyObject *electrons,
        *args_s = PyTuple_New(3),
        *args_p = PyTuple_New(3),
        *args_d = PyTuple_New(3),
        *args_f = PyTuple_New(3);
    Combinations *combinations;
    unsigned int s, p, d, f;
    if (PyArg_ParseTuple(args, "OO", &electrons, &combinations)) {
        PyArg_ParseTuple(electrons, "iiii", &s, &p, &d, &f);
    } else {
        PyErr_SetString(PyExc_TypeError, "`tuple(int, int, int, int), Combinations()´ must be provided.");
            return 1;
    }

    PyTuple_SET_ITEM(args_s, 0, PyLong_FromSize_t(s));
    PyTuple_SET_ITEM(args_s, 1, PyLong_FromSize_t(2));
    PyTuple_SET_ITEM(args_s, 2, PyLong_FromSize_t(combinations->s));

    PyTuple_SET_ITEM(args_p, 0, PyLong_FromSize_t(p));
    PyTuple_SET_ITEM(args_p, 1, PyLong_FromSize_t(6));
    PyTuple_SET_ITEM(args_p, 2, PyLong_FromSize_t(combinations->p));

    PyTuple_SET_ITEM(args_d, 0, PyLong_FromSize_t(d));
    PyTuple_SET_ITEM(args_d, 1, PyLong_FromSize_t(10));
    PyTuple_SET_ITEM(args_d, 2, PyLong_FromSize_t(combinations->d));

    PyTuple_SET_ITEM(args_f, 0, PyLong_FromSize_t(f));
    PyTuple_SET_ITEM(args_f, 1, PyLong_FromSize_t(14));
    PyTuple_SET_ITEM(args_f, 2, PyLong_FromSize_t(combinations->f));

    PossibilityType.tp_init((PyObject*) self->s, args_s, kwargs);
    PossibilityType.tp_init((PyObject*) self->p, args_p, kwargs);
    PossibilityType.tp_init((PyObject*) self->d, args_d, kwargs);
    PossibilityType.tp_init((PyObject*) self->f, args_f, kwargs);

    return 0;
}

static void Possibilities_Dealloc(Possibilities *self) {
    printf("POSSIBILITIES DEALLOC start\n");
    printf("POSSIBILITIES DEALLOC - s start\n");
    Py_XDECREF(self->s);
    printf("POSSIBILITIES DEALLOC - s end\n");
    Py_XDECREF(self->p);
    Py_XDECREF(self->d);
    Py_XDECREF(self->f);
    Py_TYPE(self)->tp_free((PyObject*) self);
    printf("POSSIBILITIES DEALLOC end\n");
}

static PyObject *Possibilities_GetS(Possibilities *self, void *closure) {
    Py_INCREF(self->s);
    return (PyObject*) self->s;
}

static PyObject *Possibilities_GetP(Possibilities *self, void *closure) {
    Py_INCREF(self->p);
    return (PyObject*) self->p;
}

static PyObject *Possibilities_GetD(Possibilities *self, void *closure) {
    Py_INCREF(self->d);
    return (PyObject*) self->d;
}

static PyObject *Possibilities_GetF(Possibilities *self, void *closure) {
    Py_INCREF(self->f);
    return (PyObject*) self->f;
}


static PyGetSetDef Possibilities_getset[] = {
    {"s", (getter) Possibilities_GetS, NULL, "possibilities of the orbiral S", NULL},
    {"p", (getter) Possibilities_GetP, NULL, "possibilities of the orbiral P", NULL},
    {"d", (getter) Possibilities_GetD, NULL, "possibilities of the orbiral D", NULL},
    {"f", (getter) Possibilities_GetF, NULL, "possibilities of the orbiral F", NULL},
    {NULL}
};

PyTypeObject PossibilitiesType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "simulator.Possibilities",
    .tp_basicsize = sizeof(Possibilities),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor) Possibilities_Dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Possibilities object",
    .tp_new = Possibilities_New,
    .tp_init = (initproc) Possibilities_Init,
    .tp_getset = Possibilities_getset,
};


#endif // __POSSIBILITIES__