CC=g++
CFLAGS=-W -Wall -ansi -pedantic -std=c++0x
LDFLAGS=

all: taskGenerator simEDF EDF_study

taskGenerator: taskGenerator.o mainTG.o
	$(CC) -o dist/taskGenerator bin/taskGenerator.o bin/mainTG.o $(LDFLAGS)

taskGenerator.o: taskGenerator/taskGenerator.cpp
	$(CC) -o bin/taskGenerator.o -c taskGenerator/taskGenerator.cpp $(CFLAGS)

mainTG.o: taskGenerator/main.cpp taskGenerator/taskGenerator.h
	$(CC) -o bin/mainTG.o -c taskGenerator/main.cpp $(CFLAGS)

simEDF: Job.o Task.o simEDF.o mainSE.o
	$(CC) -o dist/simEDF bin/Job.o bin/Task.o bin/simEDF.o bin/mainSE.o $(LDFLAGS)

Job.o: simEDF/Job.cpp
	$(CC) -o bin/Job.o -c simEDF/Job.cpp $(CFLAGS)

Task.o: simEDF/Task.cpp simEDF/Job.h
	$(CC) -o bin/Task.o -c simEDF/Task.cpp $(CFLAGS)

simEDF.o: simEDF/simEDF.cpp simEDF/Job.h simEDF/Task.h
	$(CC) -o bin/simEDF.o -c simEDF/simEDF.cpp $(CFLAGS)

mainSE.o: simEDF/main.cpp simEDF/simEDF.h simEDF/Job.h simEDF/Task.h
	$(CC) -o bin/mainSE.o -c simEDF/main.cpp $(CFLAGS)

EDF_study: mainES.o
	$(CC) -o dist/EDF_study bin/mainES.o $(LDFLAGS)

mainES.o: EDF_study/main.cpp
	$(CC) -o bin/mainES.o -c EDF_study/main.cpp $(CFLAGS)

clean:
	rm -rf bin/*.o

mrproper: clean
	rm dist/*
