LINA_O  = lina.o lina_init.o lina_cal.o lina_aux.o
LINA_CPP = lina.cpp lina_init.cpp lina_cal.cpp lina_aux.cpp
OBJECTS = main.o fraction.o mexception.o matrix.o $(LINA)
MATRIX = matrix.h matrix.tpp matrix2.tpp block.h block.tpp block2.tpp mexception.h
G++ = g++ -std=c++17

Lina.exe: $(OBJECTS)
	$(G++) $(OBJECTS) -o Lina.exe
	
main.o: mexception.h lina.h fraction.h $(MATRIX)

mexception.o: mexception.h

fraction.o:  fraction.h mexception.h

$(LINA_O): $(LINA_CPP) $(MATRIX)
	$(G++) $(LINA_CPP)


.PHONY: clean

clean:
	-rm $(OBJECTS)



