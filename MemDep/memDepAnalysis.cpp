// 15-745 S13 Assignment 1: memDepAnalysis.cpp
// Group: bovik, bovik2
////////////////////////////////////////////////////////////////////////////////

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/LoopPass.h"

#include "llvm/Support/Debug.h"
//#include "llvm/Analysis/DebugInfo.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/MemoryLocation.h"
#include "llvm/Analysis/AliasAnalysis.h"


#include <ostream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>

using namespace llvm;

namespace {

  class memDepAnalysis : public FunctionPass {

    private :
      std::unordered_map<Loop*, std::vector<StoreInst*>> storeMap ;
      std::unordered_map<Loop*, std::vector<LoadInst*>> loadMap ;
      //DominatorTree *dT ;

    public:
      static char ID;

      memDepAnalysis() : FunctionPass(ID) {
      }

      ~memDepAnalysis() {
      }

      // We don't modify the program, so we preserve all analyses
      virtual void getAnalysisUsage(AnalysisUsage &AU) const
      {
        AU.setPreservesAll();
        //AU.addRequired<LoopMemDepAnalysis>();
        AU.addRequired<LoopInfoWrapperPass>();
        AU.addRequired<ScalarEvolution>();
        //AU.addPreserved<BasicAAWrapperPass>();
        //AU.addPreserved<AAResultsWrapperPass>();
        //AU.addPreserved<GlobalsAAWrapperPass>();
        //AU.addPreserved<ScalarEvolutionWrapperPass>();
        //AU.addPreserved<SCEVAAWrapperPass>();
        //AU.addRequired<DominatorTree>();
      }

      void populateLoopStoresAndLoads(Loop* curLoop)
      {
          int store_cntr = 0 ;
          int load_cntr = 0 ;
          std::vector<StoreInst*> storeVec ;
          std::vector<LoadInst*> loadVec ;
          for (auto *BB : curLoop->getBlocks()) {
              for (Instruction &I : *BB) {
                  StoreInst *SI = dyn_cast<StoreInst>(&I);
                  if(SI) {
                      store_cntr++ ;
                      storeVec.push_back(SI) ;
                  }
                  LoadInst *LI = dyn_cast<LoadInst>(&I);
                  if(LI) {
                      load_cntr++ ;
                      loadVec.push_back(LI) ;
                  }
              }
          }
          std::pair<Loop*,std::vector<StoreInst*>> mysp (curLoop,storeVec);
          storeMap.insert(mysp) ;
          std::pair<Loop*,std::vector<LoadInst*>> mylp (curLoop,loadVec);
          loadMap.insert(mylp) ;
          std::cout<<"Loop : load="<<load_cntr<<",store="<<store_cntr<<"\n" ;
      }


      AliasResult alias(ScalarEvolution& sE, const MemoryLocation &LocA, const MemoryLocation &LocB) {
          // If either of the memory references is empty, it doesn't matter what the
          // pointer values are. This allows the code below to ignore this special
          // case.
          if (LocA.Size == 0 || LocB.Size == 0)
              return NoAlias;

          // This is SCEVAAResult. Get the SCEVs!
          const SCEV *AS = sE.getSCEV(const_cast<Value *>(LocA.Ptr));
          const SCEV *BS = sE.getSCEV(const_cast<Value *>(LocB.Ptr));
          std::cout<<"--------- SCEV -----------\n" ;
          printSCEV(AS) ;
          printSCEV(BS) ;
          std::cout<<"--------------------------\n" ;

          // If they evaluate to the same expression, it's a MustAlias.
          if (AS == BS)
              return MustAlias;
          else
              return NoAlias;
      }


      bool interloopLoadStoreSame(ScalarEvolution& sE, std::vector<StoreInst*> &sV, std::vector<LoadInst*> &lV)
      {
          std::cout<<"SV size="<<sV.size()<<", LV size="<<lV.size()<<std::endl ;
          bool aliasInstrs = true ;
          for(auto i=sV.begin() ; i != sV.end() ; i++) {
              for(auto j=lV.begin() ; j != lV.end() ; j++) {
                  MemoryLocation st = MemoryLocation::get(*i) ;
                  MemoryLocation l = MemoryLocation::get(*j) ;
                  AliasResult ar = alias(sE, st,l) ;
                  if(ar == NoAlias) {
                      std::cout<<"Loads and Stores dont alias\n" ;
                      aliasInstrs = false ;
                  } else {
                      std::cout<<"Loads and Stores alias\n" ;
                  }
              }
          }

          return aliasInstrs ;
      }

      virtual bool runOnFunction(Function &F)
      {
          if(F.empty()) return false ;
          std::cout<<"Currently in Function "<<F.getName().str()<<std::endl ;
          LoopInfo& lI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo() ;
          ScalarEvolution& sE = getAnalysis<ScalarEvolution>() ;

          for(auto i=lI.begin() ; i != lI.end() ; i++) {
              auto *TripCount = sE.getBackedgeTakenCount(*i) ;
              printSCEV(TripCount) ;
          }

          for(auto i=lI.begin() ; i != lI.end() ; i++) {
              populateLoopStoresAndLoads(*i) ;
          }

        for(auto i=lI.begin() ; i != lI.end() ; i++) {
            for(auto j=lI.begin() ; j != lI.end() ; j++) {
                if(*i == *j)
                    continue ;

                if(interloopLoadStoreSame(sE, storeMap[*i],loadMap[*j])) {
                    std::cout<<"No need for barriers\n" ;
                }
            }
        }

        return false;
      }

      void printSCEV(const SCEV* scv) {
            //scv->dump() ;
            if(dyn_cast<SCEVConstant>(scv)) {
                auto *TC = dyn_cast<SCEVConstant>(scv) ;
                std::cout<<" Value = "<<TC->getValue()->getZExtValue()<<"\n" ;
            } else if(dyn_cast<SCEVNAryExpr>(scv)) {
                std::cout<<"SCEVNaryExpr\n" ;
                auto *TripCountC = dyn_cast<SCEVNAryExpr>(scv) ;
                size_t num_opnds = TripCountC->getNumOperands() ;
                std::cout<<"num_opnds = "<<num_opnds<<"\n" ;
                for(int i=0 ; i<num_opnds ; i++) {
                    const SCEV* s = TripCountC->getOperand(i) ;
                    printSCEV(s) ;
                }
            }
      }

      //virtual bool runOnModule(Module& M)
      //{
      //    for (auto MI = M.begin(), ME = M.end(); MI != ME; ++MI)
      //    {
      //        runOnFunction(*MI);
      //    }

      //    return false;
      //}
  };
  char memDepAnalysis::ID = 0;
  RegisterPass<memDepAnalysis> X("memDepAnalysis", "Memory Dependence Analysis");
}
