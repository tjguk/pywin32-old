from distutils.command.build_py import build_py
from distutils.command.build_scripts import build_scripts

from .build import my_build as build
from .build_ext import my_build_ext as build_ext
from .install import my_install as install
from .install_data import my_install_data as install_data
