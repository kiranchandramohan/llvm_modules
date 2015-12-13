all: clean LocalOpts.so test

#CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags all) -g -O0
CXX = g++
CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) -g -O0

%.so: %.o
	g++ -dynamiclib -flat_namespace -shared $^ -o $@

test : strength algebraic constfold

#algebraic2 algebraic3

algebraic1 :algebraic1.o
	clang -O0 -emit-llvm -c algebraic1.c
	opt -mem2reg algebraic1.o -o algebraic1_r.o
	opt -p -load ./LocalOpts.so -local-opts algebraic1_r.o -o out1

algebraic2 : algebraic2.o
	clang -O0 -emit-llvm -c algebraic2.c
	opt -mem2reg algebraic2.o -o algebraic2_r.o
	opt -p -load ./LocalOpts.so -local-opts algebraic2_r.o -o out2

algebraic3 : algebraic3.o
	clang -O0 -emit-llvm -c algebraic3.c
	opt -mem2reg algebraic3.o -o algebraic3_r.o
	opt -p -load ./LocalOpts.so -local-opts algebraic3_r.o -o out3
	
algebraic4 : algebraic4.o
	clang -O0 -emit-llvm -c algebraic4.c
	opt -mem2reg algebraic4.o -o algebraic4_r.o
	opt -p -load ./LocalOpts.so -local-opts algebraic4_r.o -o out4
	
algebraic5 : algebraic5.o
	clang -O0 -emit-llvm -c algebraic5.c
	opt -mem2reg algebraic5.o -o algebraic5_r.o
	opt -p -load ./LocalOpts.so -local-opts algebraic5_r.o -o out5
	
strength : strength.o
	clang -O0 -emit-llvm -c strength.c
	opt -mem2reg strength.o -o strength_r.o
	opt -p -load ./LocalOpts.so -local-opts strength_r.o -o out_str
	
algebraic : algebraic.o
	clang -O0 -emit-llvm -c algebraic.c
	opt -mem2reg algebraic.o -o algebraic_r.o
	opt -p -load ./LocalOpts.so -local-opts algebraic_r.o -o out_alg
	
constfold : constfold.o
	clang -O0 -emit-llvm -c constfold.c
	opt -mem2reg constfold.o -o constfold_r.o
	opt -p -load ./LocalOpts.so -local-opts constfold_r.o -o out_cf
	
clean:
	rm -f *.o *~ *.so
	rm -f out1 out2 out3 out4 out5 out_str
