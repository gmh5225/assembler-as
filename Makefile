CFLAGS=-g

SRC=$(wildcard src/*.cpp)
OBJS=$(addprefix build/,$(patsubst %.cpp, %.o, $(SRC)))

all: check lex as lib_test

.PHONY: check
check:
	if [ ! -d ./build ] ; then mkdir -p build; fi
	if [ ! -d ./build/src ] ; then mkdir -p build/src; fi
	
.PHONY: lex
lex:
	minilex x86.lex ./src

as: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o build/as
	
build/src/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@
	
.PHONY: lib_test
lib_test:
	as lib_test.asm -o build/lib_test.o

.PHONY: clean	
clean:
	rm build/as; \
	rm build/*.o; \
	rm build/src/*.o
	
