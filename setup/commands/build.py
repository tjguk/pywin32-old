import os, sys
import distutils.command.build
import tempfile

from .. import logging
log = logging.logger(__package__)
from .. import config

class my_build(distutils.command.build.build):
    def run(self):
        distutils.command.build.build.run(self)
        # write a pywin32.version.txt.
        ver_fname = os.path.join(tempfile.gettempdir(), "pywin32.version.txt")
        try:
            f = open(ver_fname, "w")
            f.write("%s\n" % config.BUILD_ID)
            f.close()
        except EnvironmentError as why:
            log.error("Failed to open '%s': %s" % (ver_fname, why))

