// 15-745 S13 Assignment 1: FunctionInfo.cpp
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

	class FunctionInfo : public ModulePass {

		private :
		std::map<std::string, int> call_sites ;

		// Output the function information to standard out.
		void printFunctionInfo(Module& M) {
			//std::cout << "Module " << M.getModuleIdentifier().c_str() << std::endl;
			std::cout << "Name,\tArgs,\tCalls,\tBlocks,\tInsns\n";
			// TODO: Print out information about each function in this format.
			bool is_var_arg = false;
			size_t arg_count = 0;
			size_t callsite_count = 0;
			size_t block_count = 0;
			size_t instruction_count = 0;
			//std::cout << "function_name" << ",\t";
			//if (is_var_arg) {
			//	std::cout << "*,";
			//} else {
			//	std::cout << arg_count << ",\t";
			//}
			//std::cout << callsite_count << ",\t" << block_count << ",\t"
			//	<< instruction_count << std::endl;
		}

		//void collectFunctions(Function &F) {
		//	for(Function::iterator i = F.begin() ; i != F.end() ; i++)
		//	{
		//		for(BasicBlock::iterator bi = (*i).begin() ; bi != (*i).end() ; bi++)
		//		{
		//			Instruction* curInst = &*bi ;
		//			CallInst* callInst = NULL ;
		//			InvokeInst* invkInst = NULL ;
		//			if (callInst = dyn_cast<CallInst>(curInst)) {
		//				//std::cout<<"Call Instr = "<<callInst->getName().str()<<std::endl ;
		//				//if(call_sites.find(callInst->getCalledFunction()->getName().str()) 
		//				//	!= call_sites.end())
		//				//	call_sites[callInst->getCalledFunction()->getName().str()]++ ;
		//				//else {
		//				//	call_sites[callInst->getCalledFunction()->getName().str()] = 1 ;
		//				//}
		//			} else if (invkInst = dyn_cast<InvokeInst>(curInst)) {
		//				//std::cout<<"Call Instr = "<<invkInst->getName().str()<<std::endl ;
		//			} else {
		//				//std::cout<<"Neither Call nor Invoke instr"<<curInst->getOpcodeName()<<std::endl ;
		//			}
		//		}
		//	}
		//}

		public:
		static char ID;

		FunctionInfo() : ModulePass(ID), call_sites() { }

		~FunctionInfo() { }

		// We don't modify the program, so we preserve all analyses
		virtual void getAnalysisUsage(AnalysisUsage &AU) const {
			AU.setPreservesAll();
		}

		virtual bool runOnFunction(Function &F) {
			// TODO: implement this.
			std::cout<<F.getName().str()<<",\t" ;
			if(F.isVarArg()) {
				std::cout<<"*,\t" ;
			} else {
				std::cout<<F.arg_size()<<",\t" ;
			}



			size_t instruction_count = 0;
			for(Function::iterator i = F.begin() ; i != F.end() ; i++)
			{
				instruction_count += (*i).size() ;
			}

			size_t call_sites_count = F.getNumUses() ;

			//if(call_sites.find(F.getName().str()) != call_sites.end())
			//	call_sites_count = call_sites.at(F.getName().str()) ;

			std::cout<<call_sites_count<<",\t" ;
			std::cout<<F.size()<<",\t" ;
			std::cout<<instruction_count<<"\n" ;

			return false;
		}

		virtual bool runOnModule(Module& M) {
			printFunctionInfo(M) ;

			//for (Module::iterator MI = M.begin(), ME = M.end(); MI != ME; ++MI) {
			//	collectFunctions(*MI);
			//}

			for (Module::iterator MI = M.begin(), ME = M.end(); MI != ME; ++MI) {
				runOnFunction(*MI);
			}

			return false;
		}

	};

	// LLVM uses the address of this static member to identify the pass, so the
	// initialization value is unimportant.
	char FunctionInfo::ID = 0;
	RegisterPass<FunctionInfo> X("function-info", "15745: Function Information");

}
