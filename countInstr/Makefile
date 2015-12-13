all: countDynamicInstructions.so countStaticInstructions.so

CFLAGS = -O2
CXX = clang++
CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) -g -O0

%.so: %.o
	$(CXX) -dynamiclib -shared $^ -o $@

%.bc: %.c
	clang $(CFLAGS) -emit-llvm -c $^ -o $@

%.bco: %.bc
	opt -load ./countDynamicInstructions.so -countDynamicInstructions $^ -o $@

%.bci: %.bco instrumentHelp
	llvm-link instrumentHelp.bc $< -o $@

%.obj: %.bci
	llc -filetype=obj $^ -o=$@

instrumentHelp : instrumentHelp.cpp
	clang $(CFLAGS) -emit-llvm -o instrumentHelp.bc -c instrumentHelp.cpp

%.dy: %.obj
	$(CXX) $(CFLAGS) -o $@ $^
	./$@

%.st: %.bc
	opt -load ./countStaticInstructions.so -countStaticInstructions $^ -o simple.bcout

clean:
	rm -f *.st *.dy *.o *.bcout *~ *.bc *.ll out  simple.profile  simple.profile.s mxm_instrumented simple_instrumented
