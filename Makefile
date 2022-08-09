option = -lblas -llapack

target_berry = berry_main.o loadInput.o utils.o initialize.o interpretEquation.o lapack.o
target_eigen = eigen_main.o loadInput.o utils.o initialize.o interpretEquation.o lapack.o

target_eigen_HWR = eigen_HWR_main.o loadInput.o utils.o initialize.o loadInput_HWR.o interpretEquation.o lapack.o
target_berry_HWR = berry_HWR_main.o loadInput.o utils.o initialize.o loadInput_HWR.o interpretEquation.o lapack.o

all: berry.o eigen.o eigen_HWR.o berry_HWR.o

berry.o: $(target_berry)
	$(CXX) $(target_berry) $(option) -o berry.o

eigen.o: $(target_eigen)
	$(CXX) $(target_eigen) $(option) -o eigen.o

eigen_HWR.o: $(target_eigen_HWR)
	$(CXX) $(target_eigen_HWR) $(option) -o eigen_HWR.o

berry_HWR.o: $(target_berry_HWR)
	$(CXX) $(target_berry_HWR) $(option) -o berry_HWR.o

.cpp.o:
	$(CXX) -c $<

loadInput.o: variables_ext.hpp
interpretEquation.o: variables_ext.hpp
berry_main.o: variables.hpp initialize.hpp interpretEquation.hpp utils.hpp lapack.hpp
eigen_main.o: variables.hpp initialize.hpp interpretEquation.hpp utils.hpp lapack.hpp
eigen_HWR_main.o: variables.hpp initialize.hpp utils.hpp lapack.hpp loadInput_HWR.hpp interpretEquation.o
berry_HWR_main.o: variables.hpp initialize.hpp utils.hpp lapack.hpp loadInput_HWR.hpp interpretEquation.o
