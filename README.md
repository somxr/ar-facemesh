Download opencv from this link:
https://github.com/opencv/opencv/releases/download/4.5.4/opencv-4.5.4-vc14_vc15.exe
This is a self-extracting executable, so run it and extract it anywhere.

Open CMakeLists.txt.
Rename "Test" to the project name of your choice on these line:
- project(Test)
- set(EXEC Test)
Save.

Open CMake-GUI.
Point the source code folder to this folder.
For "Where to build the binaries", point it to a folder called "build".
Click Configure.
Scroll down to find OpenCV_DIR.
Point OpenCV_DIR to the build folder of where you extracted opencv (e.g.: C:/dev/opencv/build)
Click Configure then Generate.
Click on Open Project.

You should have all three libs op