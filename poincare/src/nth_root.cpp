#include <poincare/nth_root.h>
#include <poincare/complex.h>
#include <poincare/fraction.h>
#include <poincare/power.h>
#include "layout/nth_root_layout.h"

extern "C" {
#include <assert.h>
#include <math.h>
}

namespace Poincare {

NthRoot::NthRoot() :
  Function("root")
{
}

Expression::Type NthRoot::type() const {
  return Type::NthRoot;
}

Expression * NthRoot::cloneWithDifferentOperands(Expression** newOperands,
        int numberOfOperands, bool cloneOperands) const {
  assert(numberOfOperands == 1);
  assert(newOperands != nullptr);
  NthRoot * r = new NthRoot();
  r->setArgument(newOperands, numberOfOperands, cloneOperands);
  return r;
}

float NthRoot::approximate(Context& context, AngleUnit angleUnit) const {
  return powf(m_args[0]->approximate(context, angleUnit), 1.0f/m_args[1]->approximate(context, angleUnit));
}

ExpressionLayout * NthRoot::createLayout(DisplayMode displayMode) const {
  return new NthRootLayout(m_args[0]->createLayout(displayMode), m_args[1]->createLayout(displayMode));
}

Expression * NthRoot::evaluate(Context& context, AngleUnit angleUnit) const {
  Expression * baseEvaluation = m_args[0]->evaluate(context, angleUnit);
  Expression * indexEvaluation = m_args[1]->evaluate(context, angleUnit);
  assert(baseEvaluation->type() == Type::Matrix || baseEvaluation->type() == Type::Complex);
  assert(indexEvaluation->type() == Type::Matrix || indexEvaluation->type() == Type::Complex);
  if (baseEvaluation->type() == Type::Matrix || indexEvaluation->type() == Type::Matrix) {
    delete baseEvaluation;
    delete indexEvaluation;
    return new Complex(NAN);
  }
  Expression * operands[2];
  operands[0] = baseEvaluation;
  Expression * operandChildren[2];
  operandChildren[0] = new Complex(1.0f);
  operandChildren[1] = indexEvaluation;
  Expression * fraction = new Fraction(operandChildren, true);
  operands[1] = fraction->evaluate(context, angleUnit);
  Expression * power = new Power(operands, true);
  Expression * newResult = power->evaluate(context, angleUnit);
  delete baseEvaluation;
  delete operandChildren[0];
  delete indexEvaluation;
  delete fraction;
  delete operands[1];
  delete power;
  return newResult;
}

}
