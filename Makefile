LDLIBS=-lGLEW -framework OpenGL -lglfw3
all: triangles
clean:
	rm -f *.o triangle
