all: main.o handlers.o helpers.c vehicle.o line.o b_tree.o sorting.o
	gcc  main.o handlers.o helpers.c vehicle.o line.o b_tree.o sorting.o -o programaTrab -lm
main.o: main.c
	gcc main.c -c
handlers.o: handlers.c
	gcc handlers.c -c
helpers.o: helpers.c
	gcc helpers.c -c
vehicle.o: vehicle.c
	gcc vehicle.c -c
line.o: line.c
	gcc line.c -c
b_tree.o: b_tree.c
	gcc b_tree.c -c
sorting.o: sorting.c
	gcc sorting.c -c
clean:
	rm -rf programaTrab *.o
run:
	./programaTrab