# Author:neo
# created:2017-11-16
# Target: assembled RW with MF
# Dependency: GUN-GSL

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import numpy
import os, sys
import os.path
from distutils.sysconfig import *


#### Python include
py_inc = [get_python_inc()]

#### NumPy include
np_inc = [numpy.get_include()]

ext_modules = [
    Extension("RWEMF",["RWEMF.pyx","c_RW.cpp","c_MF.cpp"],
    include_dirs=['/home/neo/.local/gsl/include']+py_inc+np_inc,
    library_dirs=['/home/neo/.local/gsl/lib'],
    libraries=['gsl','gslcblas'],
    runtime_library_dirs=['/home/neo/.local/gsl/lib'],
    language='c++')
]

setup(
    name = "RWEMF random walk enhanced by matrix factorization",
    cmdclass = {'build_ext': build_ext},
    ext_modules = ext_modules
)

# python setup.py build_ext --inplace