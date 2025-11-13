Inverted : Main.o Create_DataBase.o Validate.o Operations.o Display_and_Search.o Save_DataBase.o Update_DataBase.o
	gcc -o $@ $^

Main.o : Main.c
	gcc -c Main.c -o Main.o

Create_DataBase.o : Create_DataBase.c
	gcc -c Create_DataBase.c -o Create_DataBase.o

Validate.o : Validate.c
	gcc -c Validate.c -o Validate.o

Operations.o : Operations.c
	gcc -c Operations.c -o Operations.o

Display_and_Search.o : Display_and_Search.c
	gcc -c Display_and_Search.c -o Display_and_Search.o

Save_DataBase.o : Save_DataBase.c
	gcc -c Save_DataBase.c -o Save_DataBase.o

Update_DataBase.o : Update_DataBase.c
	gcc -c Update_DataBase.c -o Update_DataBase.o

clean :
	rm -f *.o Inverted
