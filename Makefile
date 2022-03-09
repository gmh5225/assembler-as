CFLAGS=-g

SRC=$(wildcard src/*.cpp)
OBJS=$(addprefix build/,$(patsubst %.cpp, %.o, $(SRC)))

all: check as

.PHONY: check
check:
	if [ ! -d ./build ] ; then mkdir -p build; fi
	if [ ! -d ./build/src ] ; then mkdir -p build/src; fi

as: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o build/as
	
build/src/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY: clean	
clean:
	rm build/as; \
	rm build/src/*.o
	
