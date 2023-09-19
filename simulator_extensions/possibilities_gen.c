#include <python3.11/Python.h>
#include <assert.h>
#include <python3.11/longobject.h>
#include <stdlib.h>


static void permute(unsigned short *result, unsigned short perm, unsigned int ones, unsigned int length, unsigned int i, unsigned int *result_len) {
  assert(result != NULL && ones <= length && i <= length && result_len != NULL);
  if (ones <= 0) {
    result[*result_len] = perm << (length - i);
    (*result_len)++;
  } else if (i < length) {
    permute(result, (perm << 1) + 1, ones - 1, length, i + 1, result_len);
    if ((length - i) > ones) permute(result, perm << 1, ones, length, i + 1, result_len);
  }
}

static void generate_permutation(unsigned int ones, unsigned int length, unsigned short *result) {
  assert(result != NULL && ones <= length);
  unsigned int result_len = 0;
  permute(result, 0, ones, length, 0, &result_len);
}

PyObject* possibilities_gen(PyObject* self, PyObject* args) {
    unsigned int combinations, ones, length;
    
    if (!PyArg_ParseTuple(args, "iii", &combinations, &ones, &length))
        return NULL;
    
    unsigned short* result = malloc(sizeof(short) * combinations);
    if (result == NULL)
        return NULL;
    
    generate_permutation(ones, length, result);
    PyObject* tuple = PyTuple_New(combinations);
    for (unsigned int i = 0; i < combinations; ++i) {
        PyObject* value = PyLong_FromSize_t(result[i]);
        if (value == NULL) {
            Py_DECREF(tuple);
            return NULL;  // Conversion to Python float failed
        }
        PyTuple_SET_ITEM(tuple, i, value);
    }
    free(result);
    
    return tuple;
}
