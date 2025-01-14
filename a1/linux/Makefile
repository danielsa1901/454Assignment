LDFLAGS = -L. -lglfw -lGL -ldl
CXXFLAGS = -g -Wall -Wno-write-strings -Wno-parentheses -DLINUX

vpath %.cpp ../src
vpath %.c   ../src/glad/src
vpath %.o   ../obj

OBJS = mc.o world.o drawSegs.o strokefont.o fgStroke.o gpuProgram.o linalg.o glad.o

EXEC = mc

all:    $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS) $(LDFLAGS) 

glad.o: ../src/glad/src/glad.c

clean:
	rm -f *~ $(EXEC) $(OBJS) Makefile.bak

depend:	
	makedepend -Y ../src/*.h ../src/*.cpp 2> /dev/null
	@echo ""
	@echo "Now edit the Makefile to remove the ../src/ prefix of all of the dependences for the .o files"
	@echo ""

# DO NOT DELETE

circle.o: ../src/linalg.h ../src/drawSegs.h ../src/headers.h
circle.o: ../src/glad/include/glad/glad.h
circle.o: ../src/glad/include/KHR/khrplatform.h ../src/gpuProgram.h
circle.o: ../src/seq.h
city.o: ../src/headers.h ../src/glad/include/glad/glad.h
city.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
city.o: ../src/drawSegs.h ../src/gpuProgram.h ../src/seq.h
drawSegs.o: ../src/headers.h ../src/glad/include/glad/glad.h
drawSegs.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
drawSegs.o: ../src/gpuProgram.h ../src/seq.h
fgStroke.o: ../src/headers.h ../src/glad/include/glad/glad.h
fgStroke.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
gpuProgram.o: ../src/headers.h ../src/glad/include/glad/glad.h
gpuProgram.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
gpuProgram.o: ../src/seq.h
headers.o: ../src/glad/include/glad/glad.h
headers.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
missile.o: ../src/linalg.h ../src/gpuProgram.h ../src/headers.h
missile.o: ../src/glad/include/glad/glad.h
missile.o: ../src/glad/include/KHR/khrplatform.h ../src/seq.h
missile.o: ../src/drawSegs.h
silo.o: ../src/headers.h ../src/glad/include/glad/glad.h
silo.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
silo.o: ../src/drawSegs.h ../src/gpuProgram.h ../src/seq.h
strokefont.o: ../src/headers.h ../src/glad/include/glad/glad.h
strokefont.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
strokefont.o: ../src/gpuProgram.h ../src/seq.h
world.o: ../src/seq.h ../src/linalg.h ../src/missile.h
world.o: ../src/gpuProgram.h ../src/headers.h
world.o: ../src/glad/include/glad/glad.h
world.o: ../src/glad/include/KHR/khrplatform.h ../src/drawSegs.h
world.o: ../src/silo.h ../src/city.h ../src/circle.h
drawSegs.o: ../src/drawSegs.h ../src/headers.h
drawSegs.o: ../src/glad/include/glad/glad.h
drawSegs.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
drawSegs.o: ../src/gpuProgram.h ../src/seq.h
fgStroke.o: ../src/fgStroke.h ../src/headers.h
fgStroke.o: ../src/glad/include/glad/glad.h
fgStroke.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
gpuProgram.o: ../src/gpuProgram.h ../src/headers.h
gpuProgram.o: ../src/glad/include/glad/glad.h
gpuProgram.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
gpuProgram.o: ../src/seq.h
linalg.o: ../src/linalg.h
mc.o: ../src/headers.h ../src/glad/include/glad/glad.h
mc.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
mc.o: ../src/world.h ../src/seq.h ../src/missile.h ../src/gpuProgram.h
mc.o: ../src/drawSegs.h ../src/silo.h ../src/city.h ../src/circle.h
mc.o: ../src/strokefont.h
strokefont.o: ../src/strokefont.h ../src/headers.h
strokefont.o: ../src/glad/include/glad/glad.h
strokefont.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
strokefont.o: ../src/gpuProgram.h ../src/seq.h ../src/fgStroke.h
world.o: ../src/headers.h ../src/glad/include/glad/glad.h
world.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
world.o: ../src/world.h ../src/seq.h ../src/missile.h
world.o: ../src/gpuProgram.h ../src/drawSegs.h ../src/silo.h
world.o: ../src/city.h ../src/circle.h
