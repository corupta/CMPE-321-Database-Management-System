INCDIR =	-I"."

storageManager:	Scheme.o System.o RWHandler.o main.o SystemCatalog.o
		g++ -std=c++11 Scheme.o System.o RWHandler.o main.o SystemCatalog.o -o storageManager
		@echo "Makefile finished successfully. Created program: storageManager"

Scheme.o:	./Scheme.h
		g++ -std=c++11 -c $(INCDIR) ./Scheme.cpp

System.o:	./System.h
		g++ -std=c++11 -c $(INCDIR) ./System.cpp

RWHandler.o:	./RWHandler.h
		g++ -std=c++11 -c $(INCDIR) ./RWHandler.cpp

main.o:	./System.h
		g++ -std=c++11 -c $(INCDIR) ./main.cpp

SystemCatalog.o:	./SystemCatalog.h
		g++ -std=c++11 -c $(INCDIR) ./SystemCatalog.cpp

