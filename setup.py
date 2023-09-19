from distutils.core import setup, Extension

simulator_extensions = Extension(
    'simulator',
    sources=['simulator_extensions/module.c']
)

setup(
    name='simulator',
    version='1.0',
    description='Simulator package',
    ext_modules=[simulator_extensions]
)
