This is an AR face mask application. It is inspired by popular social media AR "filters" that augment things like makeup, tattoos, and face masks on the user's face. It uses OpenGL for graphics, OpenCV for computer vision tools, and DLib for machine learning and face detection algorithms.  

To build the project:

Download opencv from this link:
https://github.com/opencv/opencv/releases/download/4.5.4/opencv-4.5.4-vc14_vc15.exe
This is a self-extracting executable, so run it and extract it anywhere.

Open CMakeLists.txt.
Rename "Test" to the project name of your choice on these lines:
- project(Test)
- set(EXEC Test)
Save

Open CMake-GUI.
Point the source code folder to this folder.
For "Where to build the binaries", point it to a folder called "build".
Click Configure.
Scroll down to find OpenCV_DIR.
Point OpenCV_DIR to the build folder of where you extracted opencv (e.g.: C:/dev/opencv/build)
Click Configure then Generate.
Click on Open Project.

You should have a project sln file with all three libraries included 

Copy the following files from src to the root of your 'build' folder:
- Textures 
- Shaders
- shape_predictor_68_face_landmarks.dat 

Open the project and run it in Release mode, since Debug mode is too slow