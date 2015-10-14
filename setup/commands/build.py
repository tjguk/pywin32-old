import distutils.command.build

from .. import logging
log = logging.logger(__package__)

class my_build(distutils.command.build.build):
    def run(self):
        distutils.command.build.build.run(self)
        # write a pywin32.version.txt.
        ver_fname = os.path.join(gettempdir(), "pywin32.version.txt")
        try:
            f = open(ver_fname, "w")
            f.write("%s\n" % build_id)
            f.close()
        except EnvironmentError as why:
            log.error("Failed to open '%s': %s" % (ver_fname, why))

