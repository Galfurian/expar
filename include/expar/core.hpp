#pragma once

#include "enums.hpp"
#include <vector>

namespace expar
{
class AstBinary;
class AstUnary;
class AstScope;
class AstFunction;
class AstVariable;
class AstNumber;

class ExpVisitor {
public:
    virtual void visit(AstBinary &e)   = 0;
    virtual void visit(AstUnary &e)    = 0;
    virtual void visit(AstScope &e)    = 0;
    virtual void visit(AstFunction &e) = 0;
    virtual void visit(AstVariable &e) = 0;
    virtual void visit(AstNumber &e)   = 0;
};

class ExpBaseVisitor : public ExpVisitor {
public:
    void visit(AstBinary &e) override;
    void visit(AstUnary &e) override;
    void visit(AstScope &e) override;
    void visit(AstFunction &e) override;
    void visit(AstVariable &e) override;
    void visit(AstNumber &e) override;
};

class AstNode {
public:
    AstNode()                          = default;
    virtual ~AstNode()                 = default;
    virtual void accept(ExpVisitor &v) = 0;
};

class AstBinary : public AstNode {
public:
    Operator type;
    AstNode *left;
    AstNode *right;

    AstBinary(Operator _type,
              AstNode *_left,
              AstNode *_right)
        : type(_type),
          left(_left),
          right(_right)
    {
        // Nothing to do.
    }

    ~AstBinary() override
    {
        delete left;
        delete right;
    }

    inline void accept(ExpVisitor &v) override
    {
        v.visit(*this);
    }
};

class AstUnary : public AstNode {
public:
    Operator type;
    AstNode *right;

    AstUnary(Operator _type,
             AstNode *_right)
        : type(_type),
          right(_right)
    {
        // Nothing to do.
    }
    ~AstUnary() override
    {
        delete right;
    }
    inline void accept(ExpVisitor &v) override
    {
        v.visit(*this);
    }
};

class AstScope : public AstNode {
public:
    ScopeType type;
    AstNode *content;

    AstScope(ScopeType _type,
             AstNode *_content)
        : type(_type),
          content(_content)
    {
        // Nothing to do.
    }

    ~AstScope() override
    {
        delete content;
    }

    inline void accept(ExpVisitor &v) override
    {
        v.visit(*this);
    }
};

class AstFunction : public AstNode {
public:
    std::string name;
    std::vector<AstNode *> content;

    AstFunction(std::string _name,
                std::vector<AstNode *> _content)
        : name(std::move(_name)),
          content(std::move(_content))
    {
        // Nothing to do.
    }

    ~AstFunction() override
    {
        for (auto it = content.begin(); it != content.begin(); ++it) {
            delete *it;
        }
    }

    inline void accept(ExpVisitor &v) override
    {
        v.visit(*this);
    }
};

class AstVariable : public AstNode {
public:
    std::string name;

    AstVariable(std::string _name)
        : name(std::move(_name))
    {
        // Nothing to do.
    }

    ~AstVariable() override = default;

    inline void accept(ExpVisitor &v) override
    {
        v.visit(*this);
    }
};

class AstNumber : public AstNode {
public:
    double value;

    AstNumber(double _value)
        : value(_value)
    {
        // Nothing to do.
    }

    ~AstNumber() override = default;

    inline void accept(ExpVisitor &v) override
    {
        v.visit(*this);
    }
};

class Factory {
public:
    /// @brief Construct a new Factory.
    Factory() = default;

    AstBinary *astBinary(Operator type, AstNode *left, AstNode *right)
    {
        return new AstBinary(type, left, right);
    }

    AstUnary *astUnary(Operator type, AstNode *right)
    {
        return new AstUnary(type, right);
    }

    AstScope *astScope(ScopeType type, AstNode *content)
    {
        return new AstScope(type, content);
    }

    AstVariable *astVariable(std::string name)
    {
        return new AstVariable(name);
    }

    AstNumber *astNumber(double value)
    {
        return new AstNumber(value);
    }
};

} // namespace expar