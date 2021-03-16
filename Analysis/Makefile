all: Dos.o

target_Dos = Dos_main.o

Dos.o: $(target_Dos)
	$(CXX) $(target_Dos) -o Dos.o

.cpp.o:
	$(CXX) -c $<
