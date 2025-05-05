all: clean sqlib.h exec_gen

exec: all
	./exec_gen `whoami` 1> exec.sql

exec_gen: sqlib.h exec_gen.cpp
	g++ -o exec_gen exec_gen.cpp

clean:
	rm -f exec_gen

.PHONY: all clean
