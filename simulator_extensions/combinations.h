/**
 * combinations.h
 *
 * This file contains the prototypes for the `Combinations` object
 */


#ifndef SIMULATOR_EXTENSIONS_COMBINATIONS_H
#define SIMULATOR_EXTENSIONS_COMBINATIONS_H


#include <python3.11/Python.h>
#include <stdlib.h>


/**
 * @typedef Combinations
 *
 * @brief   `Combinations` object
 */
typedef struct {
    PyObject_HEAD
    unsigned int s, p, d, f;
} Combinations;


/**
 * @fn      PyObject* Combinations_New(PyTypeObject* type, PyObject* args, PyObject* kwargs)
 *
 * @brief   The `__new__` method of the `Combinations` object
 *
 * @param   type the PyTypeObject of the `Combinations` object
 * @param   args the argument tuple that was passed to the function
 * @param   kwargs key word arguments dictionary that was passed to the function
 *
 * @return  PyObject*:  Pointer to the created `Combinations` object
 */
PyObject* Combinations_New(PyTypeObject* type, PyObject* args, PyObject* kwargs);

/**
 * @fn      int Combinations_Init(Combinations *self, PyObject *args, PyObject *kwargs)
 *
 * @brief   The `__init__` method of the `Combinations` object
 *
 * @param   self the pointer to a `Combinations` instance
 * @param   args the argument tuple that was passed to the function
 * @param   kwargs key word arguments dictionary that was passed to the function
 *
 * @return  int:    The return value is 0 and indicates success
 */
int Combinations_Init(Combinations *self, PyObject *args, PyObject *kwargs);

/**
 * @fn      void Combinations_Dealloc(Combinations *self)
 *
 * @brief   The `__del__` method of the `Combinations` object
 *
 * @param   self the pointer to a `Combinations` instance
 */
void Combinations_Dealloc(Combinations *self);

/**
 * @fn      PyObject *Combinations_GetS(Combinations *self, void *closure)
 *
 * @brief   Getter of s
 *
 * @param   self the pointer to a `Combinations` instance
 * @param   closure NULL
 *
 * @return PyLong_FromSize_t:   self->s
 */
PyObject *Combinations_GetS(Combinations *self, void *closure);

/**
 * @fn      PyObject *Combinations_GetP(Combinations *self, void *closure)
 *
 * @brief   Getter of P
 *
 * @param   self the pointer to a `Combinations` instance
 * @param   closure NULL
 *
 * @return PyLong_FromSize_t:   self->p
 */
PyObject *Combinations_GetP(Combinations *self, void *closure);

/**
 * @fn      PyObject *Combinations_GetD(Combinations *self, void *closure)
 *
 * @brief   Getter of d
 *
 * @param   self the pointer to a `Combinations` instance
 * @param   closure NULL
 *
 * @return PyLong_FromSize_t:   self->d
 */
PyObject *Combinations_GetD(Combinations *self, void *closure);

/**
 * @fn      PyObject *Combinations_GetF(Combinations *self, void *closure)
 *
 * @brief   Getter of f
 *
 * @param   self the pointer to a `Combinations` instance
 * @param   closure NULL
 *
 * @return PyLong_FromSize_t:   self->f
 */
PyObject *Combinations_GetF(Combinations *self, void *closure);


/**
 * @var     Combinations_getset
 *
 * @brief   Getters (and setters) of the `Combinations` object
 */
extern PyGetSetDef Combinations_getset[];

/**
 * @var     CombinationsType
 *
 * @brief   PyTypeObject of the `Combinations` object
 */
extern PyTypeObject CombinationsType;


#endif //SIMULATOR_EXTENSIONS_COMBINATIONS_H
