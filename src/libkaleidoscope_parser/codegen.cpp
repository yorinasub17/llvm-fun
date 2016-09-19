#include <llvm/IR/Value.h>

#include "ast.h"


static llvm::Value *log_error_value(const char *str)
{
    fprintf(stderr, "ERROR: %s\n", str);
    return nullptr;
}


llvm::Value *NumberExprAST::codegen()
{
    return nullptr;
}


llvm::Value *VariableExprAST::codegen()
{
    return nullptr;
}


llvm::Value *BinaryExprAST::codegen()
{
    return nullptr;
}


llvm::Value *CallExprAST::codegen()
{
    return nullptr;
}


llvm::Function *FunctionAST::codegen()
{
    return nullptr;
}


llvm::Function *PrototypeAST::codegen()
{
    return nullptr;
}
