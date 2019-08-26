option = -lblas -llapack

all: berry.o eigen.o

berry.o: berry_main.o loadInput.o utils.o initialize.o
	$(CXX) berry_main.o loadInput.o utils.o initialize.o $(option) -o berry.o

eigen.o: eigen_main.o
	$(CXX) eigen_main.o $(option) -o eigen.o

.cpp.o:
	$(CXX) -c $<

loadInput.o: variables_ext.h

berry_main.o: variables.h initialize.h
