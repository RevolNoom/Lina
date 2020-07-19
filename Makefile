LINA_O  = lina.o lina_init.o lina_cal.o lina_aux.o 
LINA_CPP = lina.cpp lina_init.cpp lina_cal.cpp lina_aux.cpp
OBJECTS = main.o fraction.o mexception.o  $(LINA_O)
MATRIX = matrix.h matrix.tpp matrix2.tpp block.h block.tpp block2.tpp mexception.o
G++ = g++ -std=c++17

Lina.exe: $(OBJECTS)
	$(G++)  $(OBJECTS) -o Lina.exe
	
main.o: $(LINA_O) fraction.o
	$(G++) -c main.cpp 

mexception.o: mexception.h

fraction.o:  fraction.h mexception.o

$(LINA_O): lina.h lina_util.h lina_util.tpp  $(LINA_CPP) $(MATRIX)
	$(G++) -c $(LINA_CPP)


.PHONY: clean tidy

clean:
	-rm $(OBJECTS)

tidy:
	$(G++)  $(OBJECTS) -o Lina.exe && make clean



