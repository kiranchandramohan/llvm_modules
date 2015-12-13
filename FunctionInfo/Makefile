all: FunctionInfo.so

#CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags all) -g -O0
CXX = g++
CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) -g -O0

%.so: %.o
	$(CXX) -dynamiclib -shared $^ -o $@

clean:
	rm -f *.o *~ *.so
