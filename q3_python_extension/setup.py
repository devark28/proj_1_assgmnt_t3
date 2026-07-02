from setuptools import setup, Extension

setup(
    name="matrixadd",
    version="1.0",
    ext_modules=[Extension("matrixadd", sources=["matrixadd.c"])]
)
