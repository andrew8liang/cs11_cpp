#ifndef _EXPRESSIONS_HH_
#define _EXPRESSIONS_HH_

#include <iostream>
#include <cassert>
#include "environment.hh"

class Expression {
public:
    Expression() {}
    virtual ~Expression() {}
    virtual double evaluate(const Environment &env) const = 0;
};

class Value : public Expression {
    double val;
//    Value(double val) : val(val) {}
public: 
    Value(double vval) {
        val = vval;
    }
    double evaluate(const Environment &env) const {
        return (double)val;
    }
};

class Symbol : public Expression {
    string name;
public:
    Symbol(string name) : name(name) {}
    string getName() { return name; }
    double evaluate(const Environment &env) const {
        return env.getSymbolValue(name);
    }
};

class UnaryOperator : public Expression {
    Expression *rhs;
public:
    UnaryOperator(Expression *pRHS) : rhs(pRHS) {
        assert(pRHS != NULL);
    }
    ~UnaryOperator() {
        delete rhs;
    }
    
    Expression *getRHS() const { return rhs; }
};

class BinaryOperator : public Expression {
    Expression *lhs;
    Expression *rhs;
public:
    BinaryOperator(Expression *pLHS, Expression* pRHS) : lhs(pLHS), 
        rhs(pRHS) {
        assert(pLHS != NULL);
        assert(pRHS != NULL);
    }

    ~BinaryOperator() {
        delete lhs;
        delete rhs;
    }

    Expression *getLHS() const { return lhs; }

    Expression *getRHS() const { return rhs; }
};

class NegOper : public UnaryOperator {
public:
    NegOper(Expression *pRHS) : UnaryOperator(pRHS) {}
    double evaluate(const Environment &env) const {
        return -1 * getRHS()->evaluate(env);
    }
};

class AddOper : public BinaryOperator {
public:
    AddOper(Expression *pLHS, Expression *pRHS) : BinaryOperator(pLHS, pRHS){}
    double evaluate(const Environment &env) const {
        double rhs_val = getRHS()->evaluate(env);
        if (rhs_val == 0) {
            throw runtime_error("divide by zero");
        }
        else {
            double lhs_val = getLHS()->evaluate(env);
            return lhs_val + rhs_val;
        }
    }
};

class SubOper : public BinaryOperator {
public:
    SubOper(Expression *pLHS, Expression *pRHS) : BinaryOperator(pLHS, pRHS){}
    double evaluate(const Environment &env) const {
        double rhs_val = getRHS()->evaluate(env);
        if (rhs_val == 0) {
            throw runtime_error("divide by zero");
        }
        else {
            double lhs_val = getLHS()->evaluate(env);
            return lhs_val - rhs_val;
        }
    }

};

class MulOper : public BinaryOperator {
public:
    MulOper(Expression *pLHS, Expression *pRHS) : BinaryOperator(pLHS, 
        pRHS) {}
    double evaluate(const Environment &env) const {
        double rhs_val = getRHS()->evaluate(env);
        if (rhs_val == 0) {
            throw runtime_error("divide by zero");
        }
        else {
            double lhs_val = getLHS()->evaluate(env);
            return lhs_val * rhs_val;
        }
    }

};

class DivOper : public BinaryOperator {
public:
    DivOper(Expression *pLHS, Expression *pRHS) : BinaryOperator(pLHS, pRHS){}
    double evaluate(const Environment &env) const {
        double rhs_val = getRHS()->evaluate(env);
        if (rhs_val == 0) {
            throw runtime_error("divide by zero");
        }
        else {
            double lhs_val = getLHS()->evaluate(env);
            return lhs_val / rhs_val;
        }
    }
};
#endif // _EXPRESSIONS_HH_
