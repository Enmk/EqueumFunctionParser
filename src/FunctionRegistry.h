/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/
#ifndef EQUEUM_FUNCTION_PARSER_FUNCTION_REGISTRY_H_INCLUDED
#define EQUEUM_FUNCTION_PARSER_FUNCTION_REGISTRY_H_INCLUDED

#include <string>
#include <unordered_map>

struct FunctionSpec;
struct FunctionCall;

class FunctionRegistry
{
public:
    FunctionRegistry();
    ~FunctionRegistry();

    std::string addFunction(const std::string& functionSpecification);
    FunctionSpec getFunctionSpecByName(const std::string& functionName) const;
    bool deleteFunctionSpecByName(const std::string& functionName);

    FunctionCall updateFunctionCall(const FunctionCall& call) const;

private:
    std::unordered_map<std::string, FunctionSpec> functionSpecs;
};

#endif // EQUEUM_FUNCTION_PARSER_FUNCTION_REGISTRY_H_INCLUDED
