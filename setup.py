from distutils.core import setup, Extension

combinations_module = Extension('simulator', sources=['simulator_extensions/combinations.c'])

setup(
    name='simulator',
    version='1.0',
    description='Simulator package',
    ext_modules=[combinations_module]
)
