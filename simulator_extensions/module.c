#include <python3.11/Python.h>

#include "combinations.c"
#include "simulator.c"
#include "possibility.c"
#include "possibilities.c"
#include "group.c"
#include "groups.c"


static struct PyModuleDef simulatormodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "simulator",
    .m_doc = "C implementations of helper functions",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit_simulator(void) {
    PyObject *module;
    if (
            PyType_Ready(&CombinationsType) < 0
            || PyType_Ready(&SimulatorType) < 0
            || PyType_Ready(&PossibilityType) < 0
            || PyType_Ready(&PossibilitiesType) < 0
    ) return NULL;

    module = PyModule_Create(&simulatormodule);
    if (module == NULL)
        return NULL;

    Py_INCREF(&CombinationsType);
    Py_INCREF(&SimulatorType);
    Py_INCREF(&PossibilityType);
    Py_INCREF(&PossibilitiesType);
    PyModule_AddObject(module, "Combinations", (PyObject*) &CombinationsType);
    PyModule_AddObject(module, "Simulator", (PyObject*) &SimulatorType);
    PyModule_AddObject(module, "Possibility", (PyObject*) &PossibilityType);
    PyModule_AddObject(module, "Possibilities", (PyObject*) &PossibilitiesType);

    return module;
}
