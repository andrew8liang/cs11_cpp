#ifndef _EXPRESSIONS_HH_
#define _EXPRESSIONS_HH_

#include <iostream>
#include <cassert>
#include "environment.hh"

/*
 * An expression is anything that evaluates to a specific value. This is the
 * base class, which declares what every single expression will provide
 */
class Expression {
public:
    Expression() {}
    virtual ~Expression() {}
    virtual double evaluate(const Environment &env) const = 0;
};

/* 
 * Value represents any literal numeric value specified in an expression
 */
class Value : public Expression {
    double val;
public: 
    Value(double val) : val(val) {}
    double evaluate(const Environment &env) const {
        return (double)val;
    }
};

/*
 * Represents a symbol in an expression
 */
class Symbol : public Expression {
    string name;
public:
    Symbol(string name) : name(name) {}
    string getName() { return name; }
    double evaluate(const Environment &env) const {
        return env.getSymbolValue(name);
    }
};

/*
 * unary operator class has on expression that it operates on
 */
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

/*
 * binary operator has a lhs and rhs expression
 */
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

/*
 * Negates an expression
 */
class NegOper : public UnaryOperator {
public:
    NegOper(Expression *pRHS) : UnaryOperator(pRHS) {}
    double evaluate(const Environment &env) const {
        return -1 * getRHS()->evaluate(env);
    }
};

/*
 * Adds two expressions
 */
class AddOper : public BinaryOperator {
public:
    AddOper(Expression *pLHS, Expression *pRHS) : BinaryOperator(pLHS, pRHS){}
    double evaluate(const Environment &env) const {
        double rhs_val = getRHS()->evaluate(env);
        double lhs_val = getLHS()->evaluate(env);
        return lhs_val + rhs_val;
    }
};

/*
 * Subtracts two expressions
 */
class SubOper : public BinaryOperator {
public:
    SubOper(Expression *pLHS, Expression *pRHS) : BinaryOperator(pLHS, pRHS){}
    double evaluate(const Environment &env) const {
        double rhs_val = getRHS()->evaluate(env);
        double lhs_val = getLHS()->evaluate(env);
        return lhs_val - rhs_val;
    }

};

/*
 * Multiplies two expressions
 */
class MulOper : public BinaryOperator {
public:
    MulOper(Expression *pLHS, Expression *pRHS) : BinaryOperator(pLHS, 
        pRHS) {}
    double evaluate(const Environment &env) const {
        double rhs_val = getRHS()->evaluate(env);
        double lhs_val = getLHS()->evaluate(env);
        return lhs_val * rhs_val;
    }

};

/*
 * Divides two expressions
 */
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
