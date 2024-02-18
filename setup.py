from distutils.core import setup, Extension

simulator_extensions = Extension(
    'simulator',
    sources=[
        'simulator_extensions/module.c',
        'simulator_extensions/combinations.c',
        'simulator_extensions/groups.c',
        'simulator_extensions/possibilities.c',
        'simulator_extensions/possibility.c',
        'simulator_extensions/simulator.c',
    ],
    extra_compile_args=["-g"]
)

setup(
    name='simulator',
    version='1.0',
    description='Simulator package',
    ext_modules=[simulator_extensions]
)
