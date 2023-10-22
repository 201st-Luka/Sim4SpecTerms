/**
 * possibilities.c
 *
 * This file contains the prototypes of the `Possibilities` object
 */


#ifndef SIMULATOR_EXTENSIONS_POSSIBILITIES_H
#define SIMULATOR_EXTENSIONS_POSSIBILITIES_H


#include <python3.11/Python.h>

#include "possibility.h"


/**
 * @typedef Possibilities
 *
 * @brief   Possibilities object used to calculate the permutations
 */
typedef struct {
    PyObject_HEAD
    Possibility *s, *p, *d, *f;
} Possibilities;


/**
 * @fn      PyObject *Possibilities_New(PyTypeObject *type, PyObject *args, PyObject *kwargs)
 *
 * @brief   The `__new__` method of the `Possibilities` object
 *
 * @param   type the PyTypeObject of the `Possibilities` object
 * @param   args the argument tuple that was passed to the function
 * @param   kwargs key word arguments dictionary that was passed to the function
 *
 * @return  The new `Possibilities` instance
 */
PyObject *Possibilities_New(PyTypeObject *type, PyObject *args, PyObject *kwargs);

/**
 * @fn      int Possibilities_Init(Possibilities *self, PyObject *args, PyObject *kwargs)
 *
 * @brief   The `__init__` methods of the `Possibilities` object
 *
 * @param   self pointer to a `Possibilities` instance
 * @param   args the argument tuple that was passed to the function
 * @param   kwargs key word arguments dictionary that was passed to the function
 *
 * @return  int:    returns 0 if the sub objects could be created successfully else 1
 */
int Possibilities_Init(Possibilities *self, PyObject *args, PyObject *kwargs);

/**
 * @fn      void Possibilities_Dealloc(Possibilities *self)
 *
 * @brief   The `__del__` method of the `Possibilities` object
 *
 * @param   self pointer to a `Possibilities` object
 */
void Possibilities_Dealloc(Possibilities *self);

/**
 * @fn      PyObject *Possibilities_GetS(Possibilities *self, void *closure)
 *
 * @brief   Getter of s
 *
 * @param   self pointer to a `Possibilities` object
 * @param   closure pointer to additional data (NULL)
 *
 * @return  Possibility:    self->s returns the s possibilities
 */
PyObject *Possibilities_GetS(Possibilities *self, void *closure);

/**
 * @fn      PyObject *Possibilities_GetS(Possibilities *self, void *closure)
 *
 * @brief   Getter of p
 *
 * @param   self pointer to a `Possibilities` object
 * @param   closure pointer to additional data (NULL)
 *
 * @return  Possibility:    self->p returns the s possibilities
 */
PyObject *Possibilities_GetP(Possibilities *self, void *closure);

/**
 * @fn      PyObject *Possibilities_GetS(Possibilities *self, void *closure)
 *
 * @brief   Getter of d
 *
 * @param   self pointer to a `Possibilities` object
 * @param   closure pointer to additional data (NULL)
 *
 * @return  Possibility:    self->d returns the s possibilities
 */
PyObject *Possibilities_GetD(Possibilities *self, void *closure);

/**
 * @fn      PyObject *Possibilities_GetS(Possibilities *self, void *closure)
 *
 * @brief   Getter of f
 *
 * @param   self pointer to a `Possibilities` object
 * @param   closure pointer to additional data (NULL)
 *
 * @return  Possibility:    self->f returns the s possibilities
 */
PyObject *Possibilities_GetF(Possibilities *self, void *closure);


/**
 * @var     Possibilities_getset
 *
 * @brief   Getter and setter methods of the `Possibilities` object
 */
extern PyGetSetDef Possibilities_getset[];

/**
 * @var     PossibilitiesType
 *
 * @brief   PyTypeObject of the `Possibilities` object
 */
extern PyTypeObject PossibilitiesType;


#endif //SIMULATOR_EXTENSIONS_POSSIBILITIES_H
