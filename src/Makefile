INCDIR =	-I"."

storageManager:	Helpers.o Scheme.o System.o RWHandler.o main.o SystemCatalog.o
		g++ -std=c++11 Helpers.o Scheme.o System.o RWHandler.o main.o SystemCatalog.o -o storageManager
		@echo "Makefile finished successfully. Created program: storageManager"

Helpers.o:	./Helpers.cpp ./Helpers.h
		g++ -std=c++11 -c $(INCDIR) ./Helpers.cpp

Scheme.o:	./Scheme.cpp ./Scheme.h
		g++ -std=c++11 -c $(INCDIR) ./Scheme.cpp

System.o:	./System.cpp ./System.h
		g++ -std=c++11 -c $(INCDIR) ./System.cpp

RWHandler.o:	./RWHandler.cpp ./RWHandler.h
		g++ -std=c++11 -c $(INCDIR) ./RWHandler.cpp

main.o:	./main.cpp ./System.h
		g++ -std=c++11 -c $(INCDIR) ./main.cpp

SystemCatalog.o:	./SystemCatalog.cpp ./SystemCatalog.h
		g++ -std=c++11 -c $(INCDIR) ./SystemCatalog.cpp

