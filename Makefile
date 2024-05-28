CPPFLAGS = -Wall -Werror -Wextra -std=c++11 -Wno-deprecated-declarations

all:
	g++ $(CPPFLAGS) monitors.cpp -o monitors.out
	g++ $(CPPFLAGS) semaphors.cpp -o semaphors.out

clean:
	rm -rf *.out

rebuild:
	clean 
	all