CFLAGS=-g -I.

SRC=$(wildcard src/*.cpp)
OBJS=$(addprefix build/,$(patsubst %.cpp, %.o, $(SRC)))

ELF_SRC=$(wildcard libelf/*.cpp)
ELF_OBJS=$(addprefix build/,$(patsubst %.cpp, %.o, $(ELF_SRC)))

all: check lex libelf as lib_test

.PHONY: check
check:
	if [ ! -d ./build ] ; then mkdir -p build; fi
	if [ ! -d ./build/libelf ]; then mkdir -p build/libelf; fi
	if [ ! -d ./build/src ] ; then mkdir -p build/src; fi
	
.PHONY: lex
lex:
	minilex x86.lex ./src

##
## libelf
##	
libelf: $(ELF_OBJS)
	ar -cvq build/libelf.a $(ELF_OBJS)
	
build/libelf/%.o: libelf/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

##
## The assembler
##
as: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) build/libelf.a -o build/as
	
build/src/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@
	
##
## Other stuff
##
.PHONY: lib_test
lib_test:
	as lib_test.asm -o build/lib_test.o
	
.PHONY: test
test:
	./test.sh

.PHONY: clean	
clean:
	rm build/as; \
	rm build/*.o; \
	rm build/*.a; \
	rm build/src/*.o; \
	rm build/libelf/*.o;
	
