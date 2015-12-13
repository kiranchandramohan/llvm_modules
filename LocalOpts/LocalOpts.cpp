// 15-745 S13 Assignment 1: FunctionInfo.cpp
// Group: bovik, bovik2
////////////////////////////////////////////////////////////////////////////////

#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Module.h"
#include "llvm/Instructions.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Constants.h"

#include <ostream>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace llvm;

namespace {
	class LocalOpts : public ModulePass {
		private :
		bool isIdentity(Instruction& inst, ConstantInt* ci) ;
		ConstantInt* evaluate(Instruction& inst, ConstantInt* ci1, ConstantInt* ci2) ;
		bool foldConstants(Instruction& inst) ;
		bool replace_InstructionWithIdentity(Instruction& inst, Value* opnd1, Value* opnd2) ;
		bool identityGeneratingInstruction(Instruction& inst) ;
		Value* getIdentity(Instruction& inst) ;
		bool algebraicIdentities(Instruction& inst) ;
		bool strengthReduce(Instruction& inst) ;
		unsigned int getReducedOpcode(unsigned int opc) ;
		
		public:
		static char ID;

		LocalOpts() : ModulePass(ID) { }
		~LocalOpts() { }
		// We don't modify the program, so we preserve all analyses
		virtual void getAnalysisUsage(AnalysisUsage &AU) const ;
		virtual bool runOnFunction(Function &F) ;
		virtual bool runOnModule(Module& M) ;
	};

	bool LocalOpts::isIdentity(Instruction& inst, ConstantInt* ci)
	{
		if(inst.getOpcode() == Instruction::Add || inst.getOpcode() == Instruction::Sub) {
			std::cout<<"is add or sub instruction\n" ;
			return ci->isZero() ;
		} else if(inst.getOpcode() == Instruction::Mul 
				|| inst.getOpcode() == Instruction::SDiv || inst.getOpcode() == Instruction::UDiv) {
			std::cout<<"is mul or div instruction\n" ;
			return ci->isOne() ;
		}

		return false ;
	}

	ConstantInt* LocalOpts::evaluate(Instruction& inst, ConstantInt* ci1, ConstantInt* ci2)
	{
		uint64_t val1 = ci1->getValue().getLimitedValue() ;
		uint64_t val2 = ci2->getValue().getLimitedValue() ;
		uint64_t val3 ;
		IntegerType* it = ci1->getType() ;

		switch(inst.getOpcode()) {
			case Instruction::Add :
				val3 = val1 + val2 ;
				break ;
			case Instruction::Sub :
				val3 = val1 - val2 ;
				break ;
			case Instruction::Mul :
				val3 = val1 * val2 ;
				break ;
			case Instruction::SDiv :
				val3 = val1 / val2 ;
				break ;
			case Instruction::UDiv :
				val3 = val1 / val2 ;
				break ;
		}

		if(val3 < 0) {
			return ConstantInt::get(it, val3, true) ;
		} else {
			return ConstantInt::get(it, val3, false) ;
		}

	}

	bool LocalOpts::foldConstants(Instruction& inst)
	{
		bool inst_replaced = false ;


		if(inst.getNumOperands() != 2)
			return inst_replaced ;

		Value* opnd1 = inst.getOperand(0) ;
		Value* opnd2 = inst.getOperand(1) ;
		ConstantInt* ci1 ;
		ConstantInt* ci2 ;
		BasicBlock::iterator bi(inst) ;

		if((ci2=dyn_cast<ConstantInt>(opnd2)) && (ci1=dyn_cast<ConstantInt>(opnd1))) {
			std::cout<<"First operand is identity\n" ;
			std::cout<<"Second operand is identity\n" ;
			ConstantInt* ci3 = evaluate(inst, ci1, ci2) ;
			ReplaceInstWithValue(inst.getParent()->getInstList(), bi, ci3) ;
			inst_replaced = true ;
		}

		return inst_replaced ;
	}

	bool LocalOpts::replace_InstructionWithIdentity(Instruction& inst, Value* opnd1, Value* opnd2)
	{
		bool inst_replaced = false ;
		BasicBlock::iterator bi(inst) ;
		ConstantInt* ci1 ;
		ConstantInt* ci2 ;

		if((ci2=dyn_cast<ConstantInt>(opnd2)) && isIdentity(inst, ci2)) {
			ReplaceInstWithValue(inst.getParent()->getInstList(), bi, opnd1);
			std::cout<<"second operand is identity\n" ;
			inst_replaced = true ;
		} else if(inst.isCommutative() && 
				((ci1=dyn_cast<ConstantInt>(opnd1)) && isIdentity(inst, ci1))) {
			ReplaceInstWithValue(inst.getParent()->getInstList(), bi, opnd2);
			std::cout<<"First operand is identity\n" ;
			inst_replaced = true ;
		}

		return inst_replaced ;
	}

	bool LocalOpts::identityGeneratingInstruction(Instruction& inst)
	{
		switch(inst.getOpcode()) {
			case Instruction::Sub :
			case Instruction::SDiv :
			case Instruction::UDiv :
				return true ;
			default :
				return false ;
		}
	}

	Value* LocalOpts::getIdentity(Instruction& inst)
	{
		Type* it = inst.getOperand(0)->getType() ;
		uint64_t valZero = 0 ;
		uint64_t valOne = 1 ;

		switch(inst.getOpcode()) {
			case Instruction::Add :
			case Instruction::Sub :
				return ConstantInt::get(it, valZero, true) ;
			case Instruction::Mul :
			case Instruction::SDiv :
			case Instruction::UDiv :
				return ConstantInt::get(it, valOne, true) ;
			default :
				return ConstantInt::get(it, valOne, true) ;
		}
	}

	bool LocalOpts::algebraicIdentities(Instruction& inst)
	{
		bool inst_replaced = false ;

		std::cout<<"Instruction name = "<<inst.getOpcodeName()
			<<",operands="<<inst.getNumOperands()<<std::endl ;

		if(inst.getNumOperands() != 2)
			return inst_replaced ;

		Value* opnd1 = inst.getOperand(0) ;
		Value* opnd2 = inst.getOperand(1) ;
		BasicBlock::iterator bi(inst) ;

		inst_replaced = replace_InstructionWithIdentity(inst, opnd1, opnd2) ;
		if(!inst_replaced)
			if(opnd1 == opnd2 && identityGeneratingInstruction(inst)) {
				Value* opnd3 = getIdentity(inst) ;
				ReplaceInstWithValue(inst.getParent()->getInstList(), bi, opnd3);
				inst_replaced = true ;
			}

		return inst_replaced ;
	}

	unsigned int LocalOpts::getReducedOpcode(unsigned int opc)
	{
		if(opc == Instruction::Mul) {
			return Instruction::Shl ;
		} else if(opc == Instruction::UDiv || opc == Instruction::SDiv) {
			return Instruction::LShr ;
		} else {
			return Instruction::LShr ;
		}
	}

	bool LocalOpts::strengthReduce(Instruction& inst)
	{
		bool inst_replaced = false ;

		if(!(inst.getOpcode() == Instruction::Mul || 
			inst.getOpcode() == Instruction::UDiv || 
			inst.getOpcode() == Instruction::SDiv))
			return inst_replaced ;

		Value* opnd1 = inst.getOperand(0) ;
		Value* opnd2 = inst.getOperand(1) ;
		BasicBlock::iterator bi(inst) ;
		ConstantInt* ci1 ;
		ConstantInt* ci2 ;
		Instruction::BinaryOps opcode = Instruction::BinaryOps(inst.getOpcode()) ;

		if((ci2=dyn_cast<ConstantInt>(opnd2)) && ci2->getValue().isPowerOf2()) {
			//ReplaceInstWithValue(inst.getParent()->getInstList(), bi, opnd1);
			std::cout<<"second operand is power of two\n" ;
			uint64_t val = ci2->getValue().getLimitedValue() ;
			uint64_t log_val = Log2_64(val) ;
			Type* it = inst.getOperand(0)->getType() ;
			Value* opnd3 = ConstantInt::get(it, log_val, true) ;
			Instruction* to = BinaryOperator::Create(
						Instruction::BinaryOps(getReducedOpcode(opcode)),opnd1,opnd3) ;
			ReplaceInstWithInst(&inst, to) ;
			inst_replaced = true ;
		} else if(inst.isCommutative() && 
				((ci1=dyn_cast<ConstantInt>(opnd1)) && ci1->getValue().isPowerOf2())) {
			//ReplaceInstWithValue(inst.getParent()->getInstList(), bi, opnd2);
			std::cout<<"First operand is power of two\n" ;
			uint64_t val = ci1->getValue().getLimitedValue() ;
			uint64_t log_val = Log2_64(val) ;
			Type* it = inst.getOperand(0)->getType() ;
			Value* opnd3 = ConstantInt::get(it, log_val, true) ;
			Instruction* to = BinaryOperator::Create(
						Instruction::BinaryOps(getReducedOpcode(opcode)),opnd2,opnd3) ;
			ReplaceInstWithInst(&inst, to) ;
			inst_replaced = true ;
		}

		return inst_replaced ;
	}

	// We don't modify the program, so we preserve all analyses
	void LocalOpts::getAnalysisUsage(AnalysisUsage &AU) const
	{
		AU.setPreservesAll();
	}

	bool LocalOpts::runOnFunction(Function &F)
	{
		std::cout<<F.getName().str()<<"\n" ;
		bool inst_replaced = false ;
		for(Function::iterator i = F.begin() ; i != F.end() ; i++)
		{
			for(BasicBlock::iterator bi = (*i).begin() ; bi != (*i).end() ; )
			{
				Instruction* inst = bi++ ;
				inst_replaced = algebraicIdentities(*inst) || inst_replaced ;
				std::cout<<"---------------"<<std::endl ;
			}

			for(BasicBlock::iterator bi = (*i).begin() ; bi != (*i).end() ; )
			{
				Instruction* inst = bi++ ;
				inst_replaced = foldConstants(*inst) || inst_replaced ;
				std::cout<<"++++++++++++++++"<<std::endl ;
			}

			for(BasicBlock::iterator bi = (*i).begin() ; bi != (*i).end() ; )
			{
				Instruction* inst = bi++ ;
				inst_replaced = strengthReduce(*inst) || inst_replaced ;
				std::cout<<"*****************"<<std::endl ;
			}
		}

		return inst_replaced ;
	}

	bool LocalOpts::runOnModule(Module& M)
	{
		for (Module::iterator MI = M.begin(), ME = M.end(); MI != ME; ++MI) {
			runOnFunction(*MI);
		}

		return false;
	}


	// LLVM uses the address of this static member to identify the pass, so the
	// initialization value is unimportant.
	char LocalOpts::ID = 0;
	RegisterPass<LocalOpts> X("local-opts", "15745: Local Optimizations");
}
