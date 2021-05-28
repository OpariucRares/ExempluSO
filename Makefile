
all:
	gcc src/main.c -o bin/main -Wall -Wextra -Wpedantic
	gcc src/worker1.c -o bin/worker1 -Wall -Wextra -Wpedantic
	gcc src/worker2.c -o bin/worker2 -Wall -Wextra -Wpedantic



clean:
	rm MyFifo
	rm MyOtherFifo
	rm -rf bin/*
