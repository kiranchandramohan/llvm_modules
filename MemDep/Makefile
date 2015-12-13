all: memDepAnalysis.so

#CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags all) -g -O0
CFLAGS = -O0
CXX = g++
CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) -g -O0

%.so: %.o
	$(CXX) -dynamiclib -shared $^ -o $@

%.bc: %.c 
	clang $(CFLAGS) -emit-llvm -c $^ -o $@

clean:
	rm -f *.o *~ *.so
