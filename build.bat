cd /D "%~dp0"
set DIRTYJOE_OUTPUT=%~dp0\%1_release_%2
set DIRTYJOE_INTDIR=%~dp0\int\%1_release_%2

mkdir %DIRTYJOE_OUTPUT%
mkdir %DIRTYJOE_OUTPUT%\scripts

copy dirtyJOE\PyJOE\scripts\*.py %DIRTYJOE_OUTPUT%\scripts\

if "%1" == "totalcmd" copy dirtyJOE\totalcmd\pluginst.inf %DIRTYJOE_OUTPUT%\

hhc HelpFile\dirtyjoe.hhp
move HelpFile\dirtyjoe.chm %DIRTYJOE_OUTPUT%\

set PYTHONIOENCODING=utf8
python JavaVMSpec\jvmspec_html_to_text.py JavaVMSpec\jvms-6.html > spec.input

msbuild -m JavaVMSpec\jvm_spec_gen\jvm_spec_gen.sln -p:configuration=release -p:platform=win32 -p:intdir=%DIRTYJOE_INTDIR%\jvm_spec\
JavaVMSpec\jvm_spec_gen\Release\jvm_spec_gen.exe spec.input
move jvmspec.dat %DIRTYJOE_OUTPUT%\
del jvmspec.bin
del spec.input

msbuild -m -t:"%1:clean;build" -p:configuration=release -p:platform=%2 -p:outdir=%DIRTYJOE_OUTPUT%\ -p:intdir=%DIRTYJOE_INTDIR%\%1\
msbuild -m -t:"PyJOE:clean;build" -p:"configuration=release 25" -p:platform=%2 -p:outdir=%DIRTYJOE_OUTPUT%\ -p:intdir=%DIRTYJOE_INTDIR%\25\
msbuild -m -t:"PyJOE:clean;build" -p:"configuration=release 26" -p:platform=%2 -p:outdir=%DIRTYJOE_OUTPUT%\ -p:intdir=%DIRTYJOE_INTDIR%\26\
msbuild -m -t:"PyJOE:clean;build" -p:"configuration=release 27" -p:platform=%2 -p:outdir=%DIRTYJOE_OUTPUT%\ -p:intdir=%DIRTYJOE_INTDIR%\27\