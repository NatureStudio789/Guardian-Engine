set /p IDE=Please Input Your IDE:

call "Dependencies/Binaries/Premake/premake5.exe" %IDE%
PAUSE