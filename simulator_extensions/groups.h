/**
 * groups.h
 *
 * This file contains the prototypes of the grouping functions.
 */


#ifndef SIMULATOR_EXTENSIONS_GROUPS_H
#define SIMULATOR_EXTENSIONS_GROUPS_H


#include <python3.11/Python.h>
#include <stdlib.h>


/**
 * @typedef Terms
 *
 * @brief   A spectroscopic term
 */
typedef struct {
    unsigned short sup;
    char *x;
    float sub;
} Term;

/**
 * @typedef Terms
 *
 * @brief   Terms list
 */
typedef struct {
    unsigned short term_count;
    Term *terms;
} Terms;

/**
 * @typedef Group
 *
 * @brief   A spectroscopic group
 */
typedef struct {
    short abs_ml;
    float abs_ms;
    unsigned int id, count;
    Terms *terms;
} Group;

/**
 * @typedef Groups
 *
 * @brief   `Groups` object
 */
typedef struct {
    PyObject_HEAD
    Group *groups;
    unsigned int group_count, iter;
} Groups;

/**
 * @fn      PyObject *Groups_New(PyTypeObject *type, PyObject *args, PyObject *kwargs)
 *
 * @brief   The `__new__` method of the `Groups` object
 *
 * @param   type the PyTypeObject of the `Groups` object
 * @param   args the argument tuple that was passed to the function
 * @param   kwargs key word arguments dictionary that was passed to the function
 *
 * @return PyObject*:   The pointer to the created `Groups` instance
 */
PyObject *Groups_New(PyTypeObject *type, PyObject *args, PyObject *kwargs);

/**
 * @fn      int Groups_Init(Groups *self, PyObject *args, PyObject *kwargs)
 *
 * @param   self pointer to a `Groups` instance
 * @param   args the argument tuple that was passed to the function
 * @param   kwargs key word arguments dictionary that was passed to the function
 *
 * @return  int:    0 is returned if succeeded else 1
 */
int Groups_Init(Groups *self, PyObject *args, PyObject *kwargs);

/**
 * @fn      void Groups_Dealloc(Groups *self)
 *
 * @brief   The `__del__` method of the `Groups` object
 *
 * @param   self pointer to a `Groups` instance
 */
void Groups_Dealloc(Groups *self);

/**
 * @fn      PyObject *Groups_GetItem(Groups *self, PyObject *key)
 *
 * @brief   The `__getitem__` method of the `Groups` object
 *
 * @param   self pointer to a `Groups` instance
 * @param   key index or `Slice` object of the items
 *
 * @return  PyObject*:  The group at the index or a list that contains the slice of the items at indexes
 */
PyObject *Groups_GetItem(Groups *self, PyObject *key);

/**
 * @fn      PyObject *Groups_Iter(Groups *self)
 *
 * @brief   The `__iter__` method of the `Groups` object
 *
 * @param   self pointer to a `Groups` instance
 *
 * @return  PyObject*:  The `Groups` object with `self->iter` at 0
 */
PyObject *Groups_Iter(Groups *self);

/**
 * @fn      PyObject *Groups_IterNext(Groups *iter)
 *
 * @brief   The `__next__` method of the `Groups` object
 *
 * @param   iter pointer to a `Groups` instance
 *
 * @return  PyObject*:  The next item/term
 */
PyObject *Groups_IterNext(Groups *iter);

/**
 * @fn      Py_ssize_t Groups_Len(Groups *self)
 *
 * @brief   The `__len__` method of the `Groups` object
 *
 * @param   self a pointer to a `Groups` instance
 *
 * @return  Py_ssize_t: The length of the instance
 */
Py_ssize_t Groups_Len(Groups *self);

/**
 * @var     Groups_sequence_methods
 *
 * @brief   Sequence methods of the `Groups` object
 */
extern PySequenceMethods Groups_sequence_methods;

/**
 * @var     Groups_mapping_methods
 *
 * @brief   Mapping methods of the `Groups` object
 */
extern PyMappingMethods Groups_mapping_methods;

/**
 * @var     GroupsType
 *
 * @brief   PyTypeObject of the `Groups` object
 */
extern PyTypeObject GroupsType;


#endif //SIMULATOR_EXTENSIONS_GROUPS_H
