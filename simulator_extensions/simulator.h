/**
 * simulator.h
 *
 * This file contains the prototypes of the `Simulator` object.
 */


#ifndef SIMULATOR_EXTENSIONS_SIMULATOR_H
#define SIMULATOR_EXTENSIONS_SIMULATOR_H


#include <python3.11/Python.h>
#include <stdlib.h>

#include "combinations.h"
#include "possibilities.h"


/**
 * @typedef CompressedSimulatorRow
 *
 * @brief   Row of `CompressedSimulator`
 */
typedef struct {
    short ml;
    float ms;
    unsigned short count;
} CompressedSimulatorRow;

/**
 * @typedef CompressedSimulator
 *
 * @brief   Structure to compress the `Simulator` object and speed up processes
 */
typedef struct {
    CompressedSimulatorRow *rows;
    unsigned int rowsCount;
} CompressedSimulator;

/**
 * @typedef SimulatorRow
 *
 * @brief   Row of `Simulator`
 */
typedef struct {
    unsigned int orbitals;
    short ml;
    float ms;
} SimulatorRow;

/**
 * @typedef Simulator
 *
 * @brief   Simulator object
 */
typedef struct {
    PyObject_HEAD
    unsigned int s, p, d, f, iter;
    Combinations *combinations;
    Possibilities *possibilities;
    SimulatorRow *rows;
    CompressedSimulator *compressed;
} Simulator;


/**
 * @fn      PyObject *Simulator_New(PyTypeObject *type, PyObject *args, PyObject *kwargs)
 *
 * @brief   The `__new__` method of the `Simulator` object
 *
 * @param   type the PyTypeObject of the `Simulator` object
 * @param   args the argument tuple that was passed to the function
 * @param   kwargs key word arguments dictionary that was passed to the function
 *
 * @return PyObject*:   The pointer to the created `Simulator` instance
 */
PyObject *Simulator_New(PyTypeObject *type, PyObject *args, PyObject *kwargs);

/**
 * @fn      int Simulator_Init(Simulator *self, PyObject *args, PyObject *kwargs)
 *
 * @param   self pointer to a `Simulator` instance
 * @param   args the argument tuple that was passed to the function
 * @param   kwargs key word arguments dictionary that was passed to the function
 *
 * @return  int:    0 is returned if succeeded else 1
 */
int Simulator_Init(Simulator *self, PyObject *args, PyObject *kwargs);

/**
 * @fn      void Groups_Dealloc(Simulator *self)
 *
 * @brief   The `__del__` method of the `Simulator` object
 *
 * @param   self pointer to a `Simulator` instance
 */
void Simulator_Dealloc(Simulator *self);

/**
 * @fn      PyObject *Simulator_GetCombinations(Simulator *self, void *closure)
 *
 * @brief   Getter of the `Combinations` instance of the simulator
 *
 * @param   self pointer to a `Simulator` instance
 * @param   closure additional data (NULL)
 *
 * @return  PyObject*:  `Combinations` instance of the simulator
 */
PyObject *Simulator_GetCombinations(Simulator *self, void *closure);

/**
 * @fn      PyObject *Simulator_GetPossibilities(Simulator *self, void *closure)
 *
 * @brief   Getter of the `Possibilities` instance of the simulator
 *
 * @param   self pointer to a `Simulator` instance
 * @param   closure additional data (NULL)
 *
 * @return  PyObject*:  `Possibilities` instance of the simulator
 */
PyObject  *Simulator_GetPossibilities(Simulator* self, void *closure);

/**
 * @fn      PyObject *Simulator_Iter(Groups *self)
 *
 * @brief   The `__iter__` method of the `Simulator` object
 *
 * @param   self pointer to a `Simulator` instance
 *
 * @return  PyObject*:  The `Simulator` object with `self->iter` at 0
 */
PyObject *Simulator_Iter(Simulator *self);

/**
 * @fn      PyObject *Simulator_IterNext(Simulator *iter)
 *
 * @brief   The `__next__` method of the `Simulator` object
 *
 * @param   iter pointer to a `Simulator` instance
 *
 * @return  PyObject*:  The next item/row
 */
PyObject *Simulator_IterNext(Simulator *iter);

/**
 * @fn      PyObject *Simulator_GetItem(Simulator *self, PyObject *key)
 *
 * @brief   The `__getitem__` method of the `Simulator` object
 *
 * @param   self pointer to a `Simulator` instance
 * @param   key index or `Slice` object of the items
 *
 * @return  PyObject*:  The row at the index or a list that contains the slice of the items at indexes
 */
PyObject *Simulator_GetItem(Simulator *self, PyObject *key);

/**
 * @fn      Py_ssize_t Simulator_Len(Simulator *self)
 *
 * @brief   The `__len__` method of the `Simulator` object
 *
 * @param   self a pointer to a `Simulator` instance
 *
 * @return  Py_ssize_t: The length of the instance
 */
Py_ssize_t Simulator_Len(Simulator *self);


/**
 * @var     Simulator_getset
 *
 * @brief   Getters and setters of the `Simulator` object
 */
extern PyGetSetDef Simulator_getset[];

/**
 * @var     Simulator_sequence_methods
 *
 * @brief   Sequence methods of the `Simulator` object
 */
extern PySequenceMethods Simulator_sequence_methods;

/**
 * @var     Simulator_mapping_methods
 *
 * @brief   Mapping methods of the `Simulator` object
 */
extern PyMappingMethods Simulator_mapping_methods;

/**
 * @var     SimulatorType
 *
 * @brief   PyTypeObject of the `Simulator` object
 */
extern PyTypeObject SimulatorType;


#endif //SIMULATOR_EXTENSIONS_SIMULATOR_H
