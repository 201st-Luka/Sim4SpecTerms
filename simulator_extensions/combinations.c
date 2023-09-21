#ifndef __COMBINATIONS__
#define __COMBINATIONS__


#include <python3.11/Python.h>
#include <stdlib.h>


static unsigned long int factorial (unsigned int n) {
    long int result = 1;
    for (unsigned int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

static unsigned int combination(unsigned int n, unsigned int max_electrons) {
    return (factorial(max_electrons) / (factorial(n) * factorial(max_electrons - n)));
}


typedef struct {
    PyObject_HEAD
    unsigned int s, p, d, f;
} Combinations;

static PyObject* Combinations_New(PyTypeObject* type, PyObject* args, PyObject* kwargs) {
    Combinations* self = (Combinations*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->s = 0;
        self->p = 0;
        self->d = 0;
        self->f = 0;
    }
    return (PyObject*)self;
}

static int Combinations_Init(Combinations *self, PyObject *args, PyObject *kwargs) {
    unsigned int s, p, d, f;
    PyArg_ParseTuple(args, "iiii", &s, &p, &d, &f);

    self->s = combination(s, 2);
    self->p = combination(p, 6);
    self->d = combination(d, 10);
    self->f = combination(f, 14);

    return 0;
}

static void Combinations_Dealloc(Combinations *self) {
    Py_TYPE(self)->tp_free((PyObject*) self);
}

static PyObject *Combinations_GetS(Combinations *self, void *closure) {
    return PyLong_FromSize_t(self->s);
}

static PyObject *Combinations_GetP(Combinations *self, void *closure) {
    return PyLong_FromSize_t(self->p);
}

static PyObject *Combinations_GetD(Combinations *self, void *closure) {
    return PyLong_FromSize_t(self->d);
}

static PyObject *Combinations_GetF(Combinations *self, void *closure) {
    return PyLong_FromSize_t(self->f);
}

static PyGetSetDef Combinations_getset[] = {
    {"s", (getter) Combinations_GetS, NULL, "combinations for s", NULL},
    {"p", (getter) Combinations_GetP, NULL, "combinations for p", NULL},
    {"d", (getter) Combinations_GetD, NULL, "combinations for d", NULL},
    {"f", (getter) Combinations_GetF, NULL, "combinations for f", NULL},
    {NULL}
};

PyTypeObject CombinationsType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "simulator.Combinations",
    .tp_basicsize = sizeof(Combinations),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor) Combinations_Dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Combinations object",
    .tp_new = Combinations_New,
    .tp_init = (initproc) Combinations_Init,
    .tp_getset = Combinations_getset,
};


#endif // __COMBINATIONS__