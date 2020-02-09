LINA = lina_cal.o lina.o lina_aux.o lina_init.o
OBJECTS = main.o fraction.o mexception.o matrix.o $(LINA)
MATRIX = matrix.h matrix.tpp matrix2.tpp block.h block.tpp block2.tpp

Lina.exe: $(OBJECTS)
	g++ $(OBJECTS) -o Lina.exe
	
main.o: mexception.h lina.h fraction.h $(MATRIX)

mexception.o: mexception.h

fraction.o:  fraction.h mexception.h

$(LINA): mexception.h lina.h fraction.h


.PHONY: clean

clean:
	-rm $(OBJECTS)



