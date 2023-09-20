#include <python3.11/Python.h>

#include "combinations.c"


static struct PyModuleDef simulatormodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "simulator",
    .m_doc = "C implementations of helper functions",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit_simulator(void) {
    PyObject *module;
    if (PyType_Ready(&CombinationsType) < 0)
        return NULL;

    module = PyModule_Create(&simulatormodule);
    if (module == NULL)
        return NULL;

    Py_INCREF(&CombinationsType);
    PyModule_AddObject(module, "Combinations", (PyObject*) &CombinationsType);

    return module;
}
