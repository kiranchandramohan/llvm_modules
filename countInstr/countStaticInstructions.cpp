// 15-745 S13 Assignment 1: countStaticInstructions.cpp
// Group: bovik, bovik2
////////////////////////////////////////////////////////////////////////////////

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"

#include <ostream>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

using namespace llvm;

namespace {

  class countStaticInstructions : public ModulePass {

    private :
      std::map<std::string,int> instCount ;
      size_t instruction_count ;

    public:
      static char ID;

      countStaticInstructions() : ModulePass(ID), instCount(), instruction_count(0) { }

      ~countStaticInstructions() { }

      // We don't modify the program, so we preserve all analyses
      virtual void getAnalysisUsage(AnalysisUsage &AU) const
      {
        AU.setPreservesAll();
      }

      virtual bool runOnFunction(Function &F)
      {
        for(Function::iterator fi = F.begin() ; fi != F.end() ; fi++)
        {
          BasicBlock& b = *fi ;
          for(BasicBlock::iterator bi = b.begin() ; bi != b.end() ; bi++) 
          {
            Instruction& inst = *bi ;
            instCount[inst.getOpcodeName()]++ ;
            instruction_count++ ;
          }
        }

        return false;
      }

      virtual bool runOnModule(Module& M)
      {
        for (Module::iterator MI = M.begin(), ME = M.end(); MI != ME; ++MI)
        {
          runOnFunction(*MI);
        }

        for(std::map<std::string, int>::iterator i = instCount.begin(); i != instCount.end(); ++i)
        {
          std::cout << i->first << "\t" << i->second << "\n";
        }

        std::cout<<"TOTAL\t"<<instruction_count<<"\n" ;

        return false;
      }
  };

  // LLVM uses the address of this static member to identify the pass, so the
  // initialization value is unimportant.
  char countStaticInstructions::ID = 0;
  RegisterPass<countStaticInstructions> X("countStaticInstructions", "15745: Function Information");
}
