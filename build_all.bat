rem And the rest work with 7.1
set mssdk=c:\mssdk\7.1
py -2.7-32 setup.py -q build
@if errorlevel 1 goto failed
py -2.7 setup.py -q build
@if errorlevel 1 goto failed
rem py3k
py -3.3-32 setup3.py -q build
@if errorlevel 1 goto failed
py -3.3 setup3.py -q build
@if errorlevel 1 goto failed
py -3.4-32 setup3.py -q build
@if errorlevel 1 goto failed
py -3.4 setup3.py -q build
@if errorlevel 1 goto failed
py -3.5-32 setup3.py -q build
@if errorlevel 1 goto failed
py -3.5 setup3.py -q build
@if errorlevel 1 goto failed

goto xit
:failed
@echo Oops - failed!
goto xit
:xit