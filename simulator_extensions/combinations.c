#include <python3.11/Python.h>


static unsigned long int factorial (unsigned int n) {
    long int result = 1;
    for (unsigned int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

static unsigned int possibilities_for_combination(unsigned int n, unsigned int max_electrons) {
    return (factorial(max_electrons) / (factorial(n) * factorial(max_electrons - n)));
}


PyObject* combinations(PyObject* self, PyObject* args) {
    unsigned int electrons, max_electrons;
    if (!PyArg_ParseTuple(args, "ii", &electrons, &max_electrons))
        return NULL;
    
    return PyLong_FromSize_t(possibilities_for_combination(electrons, max_electrons));
}
