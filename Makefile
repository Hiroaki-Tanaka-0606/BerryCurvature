option = -lblas -llapack

target_berry = berry_main.o loadInput.o utils.o initialize.o interpretEquation.o lapack.o
target_eigen = eigen_main.o loadInput.o utils.o initialize.o interpretEquation.o lapack.o

all: berry.o eigen.o

berry.o: $(target_berry)
	$(CXX) $(target_berry) $(option) -o berry.o

eigen.o: $(target_eigen)
	$(CXX) $(target_eigen) $(option) -o eigen.o

.cpp.o:
	$(CXX) -c $<

loadInput.o: variables_ext.hpp
interpretEquation.o: variables_ext.hpp
berry_main.o: variables.hpp initialize.hpp interpretEquation.hpp utils.hpp lapack.hpp
eigen_main.o: variables.hpp initialize.hpp interpretEquation.hpp utils.hpp lapack.hpp
