@echo off
cls

REM Check if CMake is installed
where cmake >nul 2>nul
IF %ERRORLEVEL% NEQ 0 (
    echo CMake is not installed. Please install CMake from https://cmake.org/download/ or use Chocolatey:
    echo choco install cmake
    exit /b
)

REM Create and navigate to the build directory
echo Creating build directory...
mkdir build
cd build

REM Run cmake, then make the files (Windows uses MSBuild or NMake, depending on setup)
echo Running CMake and building the project...
cmake ..
cmake --build . --config Release

REM Install the files
echo Installing header files and library...
copy ..\*.hpp C:\Program Files\YourLibrary\include\
copy build\Release\libAI.dll C:\Program Files\YourLibrary\lib\
echo Done

REM Finish
echo Installation complete!
pause
