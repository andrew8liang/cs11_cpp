#ifndef _COMMANDS_HH_
#define _COMMANDS_HH_

#include <iostream>
#include "environment.hh"
#include "expressions.hh"

class Command {
public:
    Command() {}
    virtual ~Command() {}
    virtual void run (Environment &env) const = 0;
};

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
