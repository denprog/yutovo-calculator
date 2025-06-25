#ifndef __EXPORT_H__
#define __EXPORT_H__

#include <vector>
#include "ast.h"
#include "custom_unit.h"

namespace yutovo_calculator
{

struct Export
{
    template<typename Number>
    void AddVariable(const VariableNode<Number>& var);

    template<typename Number>
    void AddFunction(const FunctionNode<Number>& func);

    template<typename Number>
    void AddUnit(const CustomUnit<Number>& unit);

    std::vector<VariableNode<Integer>> variables_integer;
    std::vector<VariableNode<Real>> variables_real;
    std::vector<VariableNode<Rational>> variables_rational;
    std::vector<VariableNode<Complex>> variables_complex;

    std::vector<FunctionNode<Integer>> functions_integer;
    std::vector<FunctionNode<Real>> functions_real;
    std::vector<FunctionNode<Rational>> functions_rational;
    std::vector<FunctionNode<Complex>> functions_complex;

    std::vector<CustomUnit<Real>> units_real;
    std::vector<CustomUnit<Rational>> units_rational;
};

}

#endif