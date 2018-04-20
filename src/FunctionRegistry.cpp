/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/
#include "FunctionParser.h"
#include "FunctionRegistry.h"

#include <unordered_set>

FunctionRegistry::FunctionRegistry()
{
}

FunctionRegistry::~FunctionRegistry()
{}

std::string FunctionRegistry::addFunction(const std::string& functionSpecification)
{
    FunctionSpec spec = parseFunctionSpec(functionSpecification);

    const std::string name = spec.name;
    functionSpecs.emplace(name, std::move(spec));

    return name;
}

FunctionSpec FunctionRegistry::getFunctionSpecByName(const std::string& functionName) const
{
    return functionSpecs.at(functionName);
}

bool FunctionRegistry::deleteFunctionSpecByName(const std::string& functionName)
{
    const auto p = functionSpecs.find(functionName);
    if (p != functionSpecs.end())
    {
        functionSpecs.erase(p);

        return true;
    }

    return false;
}

FunctionCall FunctionRegistry::updateFunctionCall(const FunctionCall& call) const
{
    // Add default values of any missing parameters.
    FunctionCall result(call);

    // Collection of named call parameters.
    std::unordered_set<std::string> callParameters;
    for (const auto& callParam : call.parameters)
    {
        if (callParam.name)
        {
            callParameters.insert(*callParam.name);
        }
    }
    const FunctionSpec& spec = functionSpecs.at(call.name);

    for (size_t i = 0; i < result.parameters.size(); ++i)
    {
        if (!result.parameters[i].name)
        {
            result.parameters[i].name = spec.parameters[i].name;
        }
    }

    for (const auto& specParam : spec.parameters)
    {
        if (!specParam.value)
        {
            // No Default value.
            continue;
        }

        // if given parameter is absent in call, add it with default value.
        if (callParameters.find(specParam.name) == callParameters.end())
        {
            result.parameters.push_back(FunctionCallParameter{specParam.name, *specParam.value});
        }
    }

    return result;
}
