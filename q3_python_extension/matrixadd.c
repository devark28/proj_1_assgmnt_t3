#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject *matrixadd_add(PyObject *self, PyObject *args) {
    PyObject *matrix_a, *matrix_b;

    if (!PyArg_ParseTuple(args, "OO", &matrix_a, &matrix_b))
        return NULL;

    Py_ssize_t rows = PyList_Size(matrix_a);
    if (rows != PyList_Size(matrix_b)) {
        PyErr_SetString(PyExc_ValueError, "matrices must have the same number of rows");
        return NULL;
    }

    PyObject *result = PyList_New(rows);
    if (!result)
        return NULL;

    for (Py_ssize_t i = 0; i < rows; i++) {
        PyObject *row_a = PyList_GetItem(matrix_a, i);
        PyObject *row_b = PyList_GetItem(matrix_b, i);
        Py_ssize_t cols = PyList_Size(row_a);

        if (cols != PyList_Size(row_b)) {
            Py_DECREF(result);
            PyErr_SetString(PyExc_ValueError, "matrices must have the same number of columns");
            return NULL;
        }

        PyObject *row_r = PyList_New(cols);
        for (Py_ssize_t j = 0; j < cols; j++) {
            long a = PyLong_AsLong(PyList_GET_ITEM(row_a, j));
            long b = PyLong_AsLong(PyList_GET_ITEM(row_b, j));
            PyList_SET_ITEM(row_r, j, PyLong_FromLong(a + b));
        }

        PyList_SET_ITEM(result, i, row_r);
    }

    return result;
}

static PyMethodDef MatrixAddMethods[] = {
    {"add", matrixadd_add, METH_VARARGS, "Add two matrices given as lists of lists of ints."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef matrixaddmodule = {
    PyModuleDef_HEAD_INIT,
    "matrixadd",
    "Matrix addition implemented with the CPython C API.",
    -1,
    MatrixAddMethods
};

PyMODINIT_FUNC PyInit_matrixadd(void) {
    return PyModule_Create(&matrixaddmodule);
}
