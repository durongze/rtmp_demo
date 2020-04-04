call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"
mkdir dyzbuild
pushd dyzbuild
for /f %%i in ('dir /s /b "*.dll"') do (del %%i )
for /f %%i in ('dir /s /b "*.lib"') do (del %%i )
cmake -G "Visual Studio 14 2015 Win64" ..
msbuild librtmp\rtmp.lib.vcxproj
msbuild rtmppublish.vcxproj
msbuild rtmpdump.vcxproj
msbuild rtmpgw.vcxproj
msbuild rtmpsrv.vcxproj
msbuild rtmpsuck.vcxproj
for /f %%i in ('dir /s /b "*.lib"') do (copy %%i .\)
for /f %%i in ('dir /s /b "*.dll"') do (copy %%i .\)
popd
