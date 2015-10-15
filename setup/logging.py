# -*- coding: utf-8 -*-
from __future__ import absolute_import
import os, sys
import logging
import logging.handlers

LOGGING_FILENAME = "pywin32.log"
LOGGING_FILEPATH = LOGGING_FILENAME

level = logging.DEBUG
formatter = logging.Formatter("%(asctime)s - %(name)s - %(funcName)s - %(levelname)s - %(message)s")

handler = logging.FileHandler(
    LOGGING_FILEPATH,
    mode="a",
    encoding="utf-8"
)
handler.setLevel(level)
handler.setFormatter(formatter)

stderr_handler = logging.StreamHandler()
stderr_handler.setLevel(level)
stderr_handler.setFormatter(formatter)

def logger(name):
    _logger = logging.getLogger("pywin32.%s" % name)
    _logger.setLevel(level)
    _logger.addHandler(handler)
    _logger.addHandler(stderr_handler)
    return _logger
