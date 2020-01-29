OBJECTS = main.o fraction.o mexception.o matrix.o lina.o lina_aux.o
MATRIX = matrix.h matrix.tpp matrix2.tpp block.h block.tpp block2.tpp

Lina.exe: $(OBJECTS)
	g++ $(OBJECTS) -o Lina.exe
	
main.o: mexception.h lina.h fraction.h $(MATRIX)

matrix.o: matrix.h

mexception.o: mexception.h

fraction.o:  fraction.h mexception.h

lina.o lina_aux.o: mexception.h lina.h fraction.h

.PHONY: clean

clean:
	-rm $(OBJECTS)



