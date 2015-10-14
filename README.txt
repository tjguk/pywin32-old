This is a fork of the pywin32 package with a view to understand and refining
the somewhat involved build & install process.

The particular goals are:

* Assume 2.7 & 3.3+ with the corresponding compilers / SDKs

* No change to the pywin32 code; only the distutils / pip support

* Break out pywin32-core, pythonwin, pywin32-com as separate PyPI packages with suitable interdependencies
