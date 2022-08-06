all: Dos.o Berry_EDC.o

target_Dos = Dos_main.o
target_Berry_EDC = Berry_EDC_main.o

Dos.o: $(target_Dos)
	$(CXX) $(target_Dos) -o Dos.o

Berry_EDC.o : $(target_Berry_EDC)
	$(CXX) $(target_Berry_EDC) -o Berry_EDC.o

.cpp.o:
	$(CXX) -c $<
