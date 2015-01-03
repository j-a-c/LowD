CXX=g++
I686=i686-w64-mingw32.static-

CXXFLAGS= -O2 -std=c++11
UNIXLIBS= -lGLEW -lglfw3 -lGLU -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lXinerama -lXcursor
I686LIBS= -DGLEW_STATIC -I/usr/include/GL -L/usr/lib -lglew32 -lglu32 -lglfw3 -lopengl32 -lgdi32 --static -mwindows

SRC= main.cpp


lowd: linux windows

linux:
	$(CXX) $(SRC) $(CXXFLAGS) $(UNIXLIBS) -o bin/lowd 
	#	Optimization and gprof
	#	g++ -pg -O2 cube.cpp -lGLU -lGLEW -lglfw -o cube

windows:
	$(I686)$(CXX) $(SRC) $(CXXFLAGS) $(I686LIBS) -o bin/lowd.exe
	#`'i686-w64-mingw32.static-pkg-config' glew --cflags --libs` 
	#`'i686-w64-mingw32.static-pkg-config' glfw3 --cflags --libs`
