#ifndef __POSSIBILITIES__
#define __POSSIBILITIES__


#include <python3.11/Python.h>
#include <python3.11/longobject.h>
#include <assert.h>
#include <stdlib.h>





typedef struct {
    PyObject_HEAD
    unsigned short **poss_s, **poss_p, **poss_d, **poss_f;
} Possibilities;




#endif // __POSSIBILITIES__