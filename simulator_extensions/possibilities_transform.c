#include <python3.11/Python.h>
#include <python3.11/pystrcmp.h>
#include <python3.11/unicodeobject.h>


PyObject* transform_possibilities(PyObject* self, PyObject* args) {
    PyObject *possibilities;
    int orbitals;

    if (!PyArg_ParseTuple(args, "Oi", &possibilities, &orbitals))
        return NULL;
    
    if (!PyTuple_Check(possibilities)) {
        PyErr_SetString(PyExc_TypeError, "Input must be a tuple.");
        return NULL;
    }
    
    Py_ssize_t length = PyTuple_Size(possibilities);
    
    PyObject *new_poss = PyTuple_New(length);
    
    for (unsigned int i = 0; i < length; ++i) {
        PyObject *item = PyTuple_GetItem(possibilities, i);
        
        if (PyLong_Check(item)) {
            unsigned short orbi = orbitals / 2;
            int value = PyLong_AsLong(item);
            if (value == -1 && PyErr_Occurred()) {
                
                PyErr_Clear();
                return NULL;
            }
            
            PyObject *result = PyTuple_New(orbi);
            
            for (unsigned short o = 1; o <= orbi; ++o) {
                switch (value % 4) {
                    case 0:
                        PyTuple_SET_ITEM(result, orbi - o, Py_BuildValue("s", ""));
                        break;
                    case 1:
                        PyTuple_SET_ITEM(result, orbi - o, Py_BuildValue("s", "&#x21c2"));
                        break;
                    case 2:
                        PyTuple_SET_ITEM(result, orbi - o, Py_BuildValue("s", "&#x21bf"));
                        break;
                    case 3:
                        PyTuple_SET_ITEM(result, orbi - o, Py_BuildValue("s", "&#x296e"));
                        break;
                    default:
                        return NULL;
                }
                value /= 4;
            }
            
            PyTuple_SET_ITEM(new_poss, i, result);
        } else {
            PyErr_SetString(PyExc_TypeError, "Elements in the tuple must be integers.");
            return NULL;
        }
    }
  
    return new_poss;
}