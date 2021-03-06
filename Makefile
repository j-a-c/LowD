CXX=g++
I686=i686-w64-mingw32.static

CXXFLAGS= -O2 -std=c++11

UNIXLIBS= -lGLEW -lglfw3 -lGLU -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lXinerama -lXcursor -I/usr/include/freetype2 -lfreetype
I686LIBS= -DGLEW_STATIC -I/usr/include/GL -L/usr/lib -lGLEW -lglu32 -lglfw3 -lopengl32 -lgdi32 -I/usr/local/mxe/usr/$(I686)/include/freetype2 -lharfbuzz -lcairo -lgobject-2.0 -lfontconfig -lusp10 -lmsimg32 -lgdi32 -lpixman-1 -L/usr/local/mxe/usr/$(I686)/lib/../lib -lffi -lexpat -lfreetype -lbz2 -lpng16 -lz -lharfbuzz_too -lglib-2.0 -lws2_32 -lole32 -lwinmm -lshlwapi -lpcre -lintl -liconv --static -mwindows


SRC= *.cpp biomes/*.cpp blocks/*.cpp


lowd: linux windows

linux:
	$(CXX) $(SRC) $(CXXFLAGS) $(UNIXLIBS) -o bin/lowd 
	#	Optimization and gprof
	#	g++ -pg -O2 cube.cpp -lGLU -lGLEW -lglfw -o cube

windows:
	$(I686)-$(CXX) $(SRC) $(CXXFLAGS) $(I686LIBS) -o bin/lowd.exe
	#`'i686-w64-mingw32.static-pkg-config' glew --cflags --libs` 
	#`'i686-w64-mingw32.static-pkg-config' glfw3 --cflags --libs`
