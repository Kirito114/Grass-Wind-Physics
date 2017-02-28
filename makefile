#Makefile pour les courbes de Bézier
#R. RAFFIN, Dept Info, Arles

compilateur = g++
preproc_options = -c -Wall -std=c++11
link_options = -Wall
INCLUDE=-I D:\Users\Anthony\Documents\Libraries\C++\glew\include -I D:\Users\Anthony\Documents\Libraries\C++\freeglut\include -I D:\Users\Anthony\Documents\Libraries\C++\FreeImage\Dist\x32
LIB=-L D:\Users\Anthony\Documents\Libraries\C++\glew\lib -L D:\Users\Anthony\Documents\Libraries\C++\freeglut\lib -L D:\Users\Anthony\Documents\Libraries\C++\FreeImage\Dist\x32
libs = -lopengl32 -lglew32 -lfreeglut -lglu32 -lFreeImage -lwinmm -lgdi32

main = main

all: $(main)
	@echo "\ncompilation terminee"

$(main): point3.o BezierCurve.o fonctions.o glsl_vs.o glsl_fs.o glsl_program.o main.o
	@echo "--- compilation de $@ ..."
	$(compilateur)  $(link_options) $^ $(LIB) $(libs) -o $@
	@echo "--- compilation de $@ terminée"

point3.o: point3.cpp point3.h
	$(compilateur) $(preproc_options) $<
BezierCurve.o : BezierCurve.cpp BezierCurve.hpp
	$(compilateur) $(preproc_options) $<
main.o: main.cpp
	$(compilateur) $(preproc_options) $< $(INCLUDE)
glsl_vs.o : glsl_vs.cpp glsl_vs.h
	$(compilateur) $(preproc_options) $< $(INCLUDE)
glsl_fs.o : glsl_fs.cpp glsl_fs.h
	$(compilateur) $(preproc_options) $< $(INCLUDE)
glsl_program.o : glsl_program.cpp glsl_program.h
	$(compilateur) $(preproc_options) $< $(INCLUDE)
fonctions.o : fonctions.cpp fonctions.h
	$(compilateur) $(preproc_options) $< $(INCLUDE)
clean:
	rm -f *.o $(main) *~

archive:
	tar czvf $(main).tgz *.cpp *.h makefile
