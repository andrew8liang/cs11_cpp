#ifndef _COMMANDS_HH_
#define _COMMANDS_HH_

#include <iostream>
#include "environment.hh"
#include "expressions.hh"

/*
 * Command class that represents the commands that can be performed. This 
 * is the base class and provides an abstraction of what all commands should
 * be able to do
 */
class Command {
public:
    Command() {}
    virtual ~Command() {}
    virtual void run (Environment &env) const = 0;
};

/*
 * Evaluates an expression and then prints out the result
 */
class PrintCommand : public Command {
    Expression *exp;
public:
    PrintCommand(Expression *exp) : exp(exp) {
        assert(exp != NULL);    
    }

    ~PrintCommand() {
        delete exp;
    }

    void run(Environment &env) const {
        std::cout << "= " << exp->evaluate(env) << std::endl;;
    }
};

/*
 * Evaluates an expression then sets a symbol to that value 
 */
class AssignCommand : public Command {
    Symbol *sym;
    Expression *exp;
public:
    AssignCommand(Symbol *sym, Expression *exp) : sym(sym), exp(exp) {}
    ~AssignCommand() {
        delete sym;
        delete exp;
    }

    void run(Environment &env) const {
        env.setSymbolValue(sym->getName(), exp->evaluate(env));
        std::cout << sym->getName() << " = " << exp->evaluate(env) << 
            std::endl;
    }
};
#endif // COMMANDS_HH_
