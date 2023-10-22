/**
 * possibilities.h
 *
 * This file contains the prototypes of the `Possibility` object.
 */


#ifndef SIMULATOR_EXTENSIONS_POSSIBILITY_H
#define SIMULATOR_EXTENSIONS_POSSIBILITY_H


#include <python3.11/Python.h>
#include <stdlib.h>
#include <stdio.h>


#define ARROW_UP   "\u21bF"
#define ARROW_DOWN "\u21C2"
#define ARROW_BOTH "\u296E"


/**
 * @typedef Possibility
 *
 * @brief   Possibility object
 */
typedef struct {
    PyObject_HEAD
    unsigned int combinations, electrons, max_electrons, iter;
    unsigned short *poss;
} Possibility;



/**
 * @fn      PyObject *Possibility_New(PyTypeObject *type, PyObject *args, PyObject *kwargs)
 *
 * @brief   The `__new__` method of the `Possibility` object
 *
 * @param   type the PyTypeObject of the `Possibility` object
 * @param   args the argument tuple that was passed to the function
 * @param   kwargs key word arguments dictionary that was passed to the function
 *
 * @return  The pointer to the new `Possibility` instance
 */
PyObject *Possibility_New(PyTypeObject *type, PyObject *args, PyObject *kwargs);

/**
 * @fn      int Possibility_Init(Possibility *self, PyObject *args, PyObject *kwargs)
 *
 * @brief   The `__init__` method of the `Possibility` object
 *
 * @param   self the pointer to a `Possibility` instance
 * @param   args the argument tuple that was passed to the function
 * @param   kwargs key word arguments dictionary that was passed to the function
 *
 * @return  int:    0 indicates successful initialisation of the instance else 1
 */
int Possibility_Init(Possibility *self, PyObject *args, PyObject *kwargs);

/**
 * @fn      void Possibility_Dealloc(Possibility *self)
 *
 * @brief   The `__del__` method of the `Possibility` object
 *
 * @param   self the pointer to a `Possibility` instance
 */
void Possibility_Dealloc(Possibility *self);

/**
 * @fn      PyObject *Possibility_Iter(Possibility *self)
 *
 * @brief   The `__iter__` method of the `Possibility` object
 *
 * @param   self pointer to a `Possibility` instance
 *
 * @return  PyObject*:  The `Possibility` object with `self->iter` at 0
 */
PyObject *Possibility_Iter(Possibility *self);

/**
 * @fn      PyObject *int_to_arrow(unsigned short sub_value)
 *
 * @brief   Helper function that converts 0, 1, 2, 3 to "no spin", "spin up", "spin down", "spin up and down" harpoon
 *          arrows
 *
 * @param   sub_value unsigned short 0, 1, 2, 3
 *
 * @return  The Python Unicode object of the string containing the arrow
 */
PyObject *int_to_arrow(unsigned short sub_value);

/**
 * @fn      PyObject *Possibility_IterNext(Possibility *iter)
 *
 * @brief   The `__next__` method of the `Possibility` object
 *
 * @param   iter pointer to a `Possibility` instance
 *
 * @return  PyObject*:  The next item/possibility
 */
PyObject *Possibility_IterNext(Possibility *iter);

/**
 * @fn      PyObject *Possibility_GetItem(Possibility *self, Py_ssize_t index)
 *
 * @brief   The `__getitem__` method of the `Possibility` object
 *
 * @param   self pointer to a `Possibility` instance
 * @param   key index or `Slice` object of the items
 *
 * @return  PyObject*:  The possibility at the index or a list that contains the slice of the items at indexes
 */
PyObject *Possibility_GetItem(Possibility *self, Py_ssize_t index);


/**
 * @var     Possibility_sequence_methods
 *
 * @brief   Sequence methods of the `Possibility` object
 */
extern PySequenceMethods Possibility_sequence_methods;

/**
 * @var     PossibilityType
 *
 * @brief   PyTypeObject of the `Possibility` object
 */
extern PyTypeObject PossibilityType;


#endif //SIMULATOR_EXTENSIONS_POSSIBILITY_H
