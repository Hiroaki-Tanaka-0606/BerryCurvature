option = -lblas -llapack

target_berry = berry_main.o loadInput.o utils.o initialize.o interpretEquation.o
all: berry.o eigen.o

berry.o: $(target_berry)
	$(CXX) $(target_berry) $(option) -o berry.o

eigen.o: eigen_main.o
	$(CXX) eigen_main.o $(option) -o eigen.o

.cpp.o:
	$(CXX) -c $<

loadInput.o: variables_ext.hpp
interpretEquation.o: variables_ext.hpp
berry_main.o: variables.hpp initialize.hpp interpretEquation.hpp utils.hpp
