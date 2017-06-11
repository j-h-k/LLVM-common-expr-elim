#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/CFG.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/Constants.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

#include <iostream>
#include <set>
#include <unordered_map>

namespace {

class CommonExprElimPass : public llvm::FunctionPass {
private:
  std::unordered_map<std::string, llvm::Value *> available_expressions;

  // If wanted to store the string of the instruction.
  // std::unordered_map<std::string, std::pair<std::string, llvm::Value*>>
  // available_expressions_detailed;

public:
  // Identifier
  static char ID;

  // Constructor. The first argument of the parent constructor is
  // a unique pass identifier.
  CommonExprElimPass() : llvm::FunctionPass(ID) {}

  // Replaces all apperances of repeated instruction with already in
  // available_expressions.
  void replaceExpression(llvm::Instruction *&instruction, std::string &key) {
    llvm::BasicBlock::iterator ii(instruction);
    llvm::Value *replacewiththis = available_expressions[key];
    llvm::ReplaceInstWithValue(instruction->getParent()->getInstList(), ii,
                               replacewiththis);
  }

  bool runOnFunction(llvm::Function &function) override {
    bool modified = false;
    std::string key;

    // Uncomment if want to see string presentation of instruction.
    // printFunction(function);

    // Each basic block
    for (llvm::Function::iterator basic_block = function.begin(),
                                  e = function.end();
         basic_block != e; ++basic_block) {
      // Each instruction
      for (llvm::BasicBlock::iterator instr = basic_block->begin(),
                                      instrEND = basic_block->end();
           instr != instrEND;
           /*Read below important notice.*/) {
        /*
         * Important note^^ llvm::Instruction * instruction = instr++;
         * This is necessary in order to manipulate individual instruction.
         * If this is not used and "instr" is incremented in the for loop
         * a segfault would arise. This is because when replaceExpression is
         * called the llvm::ReplaceInstWithValue function erases the instruction
         * from the linked list of instructions.
         */
        llvm::Instruction *instruction = instr++;

        key = "";
        key += instruction->getOpcodeName();

        // No need to replace if alloca, br, store.
        if (key == "alloca" || key == "br" || key == "store" || key == "load")
          continue;

        // Check the operands
        for (int i = 0; i < instruction->getNumOperands(); ++i) {

          llvm::Value *v = instruction->getOperand(i);

          // Check if ConstantInt
          if (llvm::ConstantInt *CI = llvm::dyn_cast<llvm::ConstantInt>(v)) {
            int x = CI->getSExtValue();
            key += std::string(" ");
            key += std::to_string(x);
          }
          // LLVM Variable otherwise
          else {
            key += std::string(" ");
            key += v->getName();
          }
        }

        // Push expression into the map
        if (available_expressions[key]) { // if already exists, replace...
          replaceExpression(instruction, key);

          // Function modified, so return true later.
          modified = true;
        } else { // else push into map.
          available_expressions[key] = instruction;
        }
      }
    }

    // Uncomment if want to see string presentation of instruction.
    // printFunction(function);

    // If function modified return true, else false
    return modified;
  }

  void printFunction(llvm::Function &function) {
    // Print function name
    llvm::errs() << "Pass on function " << function.getName() << '\n';

    // Traverse basic blocks in function
    for (llvm::Function::iterator basic_block = function.begin(),
                                  e = function.end();
         basic_block != e; ++basic_block) {
      printBasicBlock(*basic_block);
      llvm::errs() << '\n';
    }
  }

  void printBasicBlock(llvm::BasicBlock &basic_block) {
    // Print basic block
    llvm::errs() << basic_block.getName() << '\n';

    // Traverse instructions
    for (llvm::BasicBlock::iterator instruction = basic_block.begin(),
                                    e = basic_block.end();
         instruction != e; ++instruction) {
      printInstruction(*instruction);
    }
  }

  void printInstruction(llvm::Instruction &instruction) {
    // Print instruction
    llvm::errs() << " \t" << instruction.getName() << ' '
                 << instruction.getOpcodeName() << ' ';
    // Traverse arguments
    for (unsigned i = 0; i < instruction.getNumOperands(); i++) {
      // llvm::Value *arg = instruction.getOperand(i);
      // llvm::errs() << "\tArgument " << arg->getName() << '\n';
      llvm::Value *v = instruction.getOperand(i);
      // Check if ConstantInt
      if (llvm::ConstantInt *CI = llvm::dyn_cast<llvm::ConstantInt>(v)) {
        int x = CI->getSExtValue();
        llvm::errs() << std::to_string(x) << ' ';
      }
      // LLVM Variable otherwise
      else {
        llvm::errs() << v->getName() << ' ';
      }
    }

    llvm::errs() << '\n';
  }
};

// Pass identifier
char CommonExprElimPass::ID = 0;

// Pass registration. Pass will be available as 'common-expr-elim' from the LLVM
// optimizer tool.
static llvm::RegisterPass<CommonExprElimPass>
    X("common-expr-elim", "My function pass", false, false);

} // namespace
