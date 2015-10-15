# -*- coding: utf-8 -*-
import os, sys
import distutils.command.install

from .._compat import *
from .. import logging
log = logging.logger(__package__)

class my_install(distutils.command.install.install):
    def run(self):
        distutils.command.install.install.run(self)
        # Custom script we run at the end of installing - this is the same script
        # run by bdist_wininst
        # This child process won't be able to install the system DLLs until our
        # process has terminated (as distutils imports win32api!), so we must use
        # some 'no wait' executor - spawn seems fine!  We pass the PID of this
        # process so the child will wait for us.
        # XXX - hmm - a closer look at distutils shows it only uses win32api
        # if _winreg fails - and this never should.  Need to revisit this!
        # If self.root has a value, it means we are being "installed" into
        # some other directory than Python itself (eg, into a temp directory
        # for bdist_wininst to use) - in which case we must *not* run our
        # installer
        if not self.dry_run and not self.root:
            # We must run the script we just installed into Scripts, as it
            # may have had 2to3 run over it.
            filename = os.path.join(self.prefix, "Scripts", "pywin32_postinstall.py")
            if not os.path.isfile(filename):
                raise RuntimeError("Can't find '%s'" % (filename,))
            log.info("Executing post install script...")
            # What executable to use?  This one I guess.
            os.spawnl(os.P_NOWAIT, sys.executable,
                      sys.executable, filename,
                      "-quiet", "-wait", str(os.getpid()), "-install")

