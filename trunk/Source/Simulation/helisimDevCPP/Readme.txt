to get this to compile

1: copy files from glut-MingW-DEV-C++.zip
# glut.h - Save this file to " C:/dev-c++/include/gl/ "
# glut32.def - Save this file to " C:/dev-c++/lib/ "
# glut32.dll - Save this file to " C:/windows/command/ "

2: add these lines to project options->Parameters->linker
-lopengl32 -lglu32 -lglut32
libglut.a
glut32.lib
