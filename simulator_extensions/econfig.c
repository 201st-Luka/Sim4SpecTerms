#include <python3.11/Python.h>


PyObject *compose_econfig(PyObject *self, PyObject *args) {
    
    if (!PyTuple_Check(possibilities) || PyTuple_Check(combinations)) {
        PyErr_SetString(PyExc_TypeError, "Input must be a tuples.");
        return NULL;
    }
    
    PyObject *econfig = PyTuple_New(
        PyLong_AsSize_t(PyTuple_GetItem((PyObject*) possibilities, 0)) *
        PyLong_AsSize_t(PyTuple_GetItem((PyObject*) possibilities, 1)) *
        PyLong_AsSize_t(PyTuple_GetItem((PyObject*) possibilities, 2)) *
        PyLong_AsSize_t(PyTuple_GetItem((PyObject*) possibilities, 3))
    );
    
    return econfig;
}