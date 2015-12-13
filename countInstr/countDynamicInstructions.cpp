#define DEBUG_TYPE "countDynamicInstructions"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
#include <string>
using namespace std;
using namespace llvm;
namespace {
  // Hello - The first implementation, without getAnalysisUsage.
  struct CountDynamicInstructions : public ModulePass {
    static char ID; // Pass identification, replacement for typeid
    static map<string, int> counter;
    CountDynamicInstructions() : ModulePass(ID) {}
    virtual bool runOnModule(Module &m) {
      for (Module::iterator func = m.begin(); m.end() != func; ++func) {
        for (Function::iterator block = func->begin(); func->end() != block; ++block) {
          map<string, int> counter;
          bool main_exit = false;
          for (BasicBlock::iterator inst = block->begin(); block->end() != inst; ++inst) {
            counter[inst->getOpcodeName()]++;
            main_exit = main_exit || (!strcmp("main", func->getName().str().c_str()) && ReturnInst::classof(inst));
          }
          IRBuilder<> builder(block->getTerminator());
          for (map<string, int>::iterator opCount = counter.begin(); counter.end() != opCount; ++opCount) {
            Constant* c = m.getOrInsertFunction("_Z12handleOpcodePci",
                FunctionType::get(builder.getVoidTy(),
                  PointerType::getInt8PtrTy(m.getContext()),
                  IntegerType::get(m.getContext(),32)));
            Function* handle_opcode = static_cast<Function*>(c);
            builder.CreateCall2(handle_opcode, builder.CreateGlobalStringPtr(opCount->first.c_str()), builder.getInt32(opCount->second));
          }
          if (main_exit) {
            Constant *c = m.getOrInsertFunction("_Z21printOpcodeStatisticsv", FunctionType::get(builder.getVoidTy(), false));
            Function *print_stats = static_cast<Function*>(c);
            builder.CreateCall(print_stats);
          }
        }
      }
      return false;
    }
  };
}
char CountDynamicInstructions::ID = 0;
static RegisterPass<CountDynamicInstructions> X("countDynamicInstructions", "Count Dynamic Instructions Pass");
map<string, int> CountDynamicInstructions::counter;
