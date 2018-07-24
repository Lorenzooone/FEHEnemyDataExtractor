OBJ = main.o HSDArc.o enemyDataExtractor.o

FEHEnemyDataExtractor: $(OBJ)
	g++ -o FEHEnemyDataExtractor $(OBJ) -static

-include dependencies

.PHONY: depend
depend:
	g++ -MM *.cpp > dependencies

