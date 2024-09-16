::----------------------------------------------------------------
:: START
::----------------------------------------------------------------

@echo off

pushd ..

::----------------------------------------------------------------
:: VARIABLES
::----------------------------------------------------------------

@set path_bin_r_memory=     bin\debug-x64
@set path_include_r_common= modules\r-common\include

if not exist %path_bin_r_memory% (
    mkdir %path_bin_r_memory%
)


::----------------------------------------------------------------
:: ARGUMENTS
::----------------------------------------------------------------

@set cl_flags=        /c ^
                      /Zi

@set cl_output=       /Fo:%path_bin_r_memory%\RMemory.obj ^
                      /Fd:%path_bin_r_memory%\RWin32.pdb


@set cl_includes=     /I src                ^
                      /I include            ^
                      /I internal           ^
                      /I %path_include_r_common%

@set cl_source=       src\r-memory.cpp

::----------------------------------------------------------------
:: COMPILE
::----------------------------------------------------------------

call cl.exe       ^
    %cl_flags%    ^
    %cl_output%   ^
    %cl_includes% ^
    %cl_source%

::----------------------------------------------------------------
:: LINK
::----------------------------------------------------------------

call lib.exe /out:bin\debug-x64\RMemory.lib bin\debug-x64\RMemory.obj

::----------------------------------------------------------------
:: END
::----------------------------------------------------------------

popd