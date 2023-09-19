#include <python3.11/Python.h>

#include "combinations.c"
#include "possibilities_gen.c"
#include "possibilities_transform.c"


static PyMethodDef SimulatorMethods[] = {
    {"combinations", combinations, METH_VARARGS, "Calculate combinations."},
    {"possibilities", possibilities_gen, METH_VARARGS, "Generate possibilities."},
    {"tf_poss", transform_possibilities, METH_VARARGS, "Transform possibilities."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef simulatormodule = {
    PyModuleDef_HEAD_INIT,
    "simulator",
    "C implementations of helper functions",
    -1,
    SimulatorMethods
};

// Module initialization entry point
PyMODINIT_FUNC PyInit_simulator(void) {
    return PyModule_Create(&simulatormodule);
}
