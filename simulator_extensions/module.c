/**
 * module.c
 *
 * This file contains the module definition of the `simulator` module for Python.
 *
 * @author  201st-Luka
 * @date    21 october 2023
 */


#include <python3.11/Python.h>

#include "possibility.h"
#include "possibilities.h"
#include "combinations.h"
#include "simulator.h"
#include "groups.h"

/**
 * @var     simulatormodule
 *
 * @brief   simulator module definition structure
 */
static struct PyModuleDef simulatormodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "simulator",                                  /**< Name of the module */
    .m_doc = "C implementations of simulator functions",    /**< Documentation of the module */
    .m_size = -1,
};

/**
 * @fn      PyMODINIT_FUNC PyInit_simulator(void)
 *
 * @brief   Module initialisation function
 *
 * @return  PyObject:   Returns the module if it is loaded correctly
 *          NULL:       An error occurred
 */
PyMODINIT_FUNC PyInit_simulator(void) {
    // creating the module pointer
    PyObject *module;
    // check if all types are ready to use, if not NULL is returned
    if (
            PyType_Ready(&CombinationsType) < 0
            || PyType_Ready(&SimulatorType) < 0
            || PyType_Ready(&PossibilityType) < 0
            || PyType_Ready(&PossibilitiesType) < 0
            || PyType_Ready(&GroupsType) < 0
    ) return NULL;

    // creating the module
    module = PyModule_Create(&simulatormodule);
    if (module == NULL)
        return NULL;

    // incrementing the references of the types
    Py_INCREF(&CombinationsType);
    Py_INCREF(&SimulatorType);
    Py_INCREF(&PossibilityType);
    Py_INCREF(&PossibilitiesType);
    Py_INCREF(&GroupsType);

    // adding the classes to the module
    PyModule_AddObject(module, "Combinations", (PyObject*) &CombinationsType);
    PyModule_AddObject(module, "Simulator", (PyObject*) &SimulatorType);
    PyModule_AddObject(module, "Possibility", (PyObject*) &PossibilityType);
    PyModule_AddObject(module, "Possibilities", (PyObject*) &PossibilitiesType);
    PyModule_AddObject(module, "Groups", (PyObject*) &GroupsType);

    // returning the built module
    return module;
}
