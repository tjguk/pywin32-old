import distutils.command.install_data

from .. import logging
log = logging.logger(__package__)

class my_install_data(distutils.command.install_data.install_data):
    """A custom install_data command, which will install it's files
    into the standard directories (normally lib/site-packages).
    """
    def finalize_options(self):
        if self.install_dir is None:
            installobj = self.distribution.get_command_obj('install')
            self.install_dir = installobj.install_lib
        log.info('Installing data files to %s' % self.install_dir)
        install_data.finalize_options(self)

    def copy_file(self, src, dest):
        dest, copied = install_data.copy_file(self, src, dest)
        return dest, copied

