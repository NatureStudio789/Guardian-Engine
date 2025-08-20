set /p IDE=Please Input Your IDE:

call "Dependencies/Binaries/Premake/premake5.exe" --file=Guardian-Engine.lua %IDE%
pause