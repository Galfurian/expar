#include "expar/parser.hpp"
#include <iostream>

class ExpDebugPrinter : public expar::ExpBaseVisitor {
public:
    void visit(expar::AstBinary &e) override
    {
        std::cout << "AstBinary[";
        if (e.left)
            e.left->accept(*this);
        else
            std::cout << "NULL";
        std::cout << expar::operator_to_string(e.type);
        if (e.right)
            e.right->accept(*this);
        else
            std::cout << "NULL";
        std::cout << "]";
    }

    void visit(expar::AstUnary &e) override
    {
        std::cout << "AstUnary[";
        std::cout << expar::operator_to_string(e.type);
        if (e.right)
            e.right->accept(*this);
        else
            std::cout << "NULL";
        std::cout << "]";
    }

    void visit(expar::AstScope &e) override
    {
        std::cout << "AstScope[";
        if (e.type == expar::scp_round)
            std::cout << "(";
        else if (e.type == expar::scp_square)
            std::cout << "[";
        else if (e.type == expar::scp_curly)
            std::cout << "{";
        if (e.content)
            e.content->accept(*this);
        else
            std::cout << "NULL";
        if (e.type == expar::scp_round)
            std::cout << ")";
        else if (e.type == expar::scp_square)
            std::cout << "]";
        else if (e.type == expar::scp_curly)
            std::cout << "}";
        std::cout << "]";
    }

    void visit(expar::AstFunction &e) override
    {
        std::cout << "AstFunction[";
        std::cout << e.name << "(";
        for (auto it : e.content) {
            if (it)
                it->accept(*this);
            else
                std::cout << "NULL";
        }
        std::cout << ")";
        std::cout << "]";
    }

    void visit(expar::AstVariable &e) override
    {
        std::cout << "AstVariable[";
        std::cout << e.name;
        std::cout << "]";
    }

    void visit(expar::AstNumber &e) override
    {
        std::cout << "AstNumber[";
        std::cout << e.value;
        std::cout << "]";
    }
};

class ExpPrinter : public expar::ExpBaseVisitor {
public:
    void visit(expar::AstBinary &e) override
    {
        if (e.left)
            e.left->accept(*this);
        else
            std::cout << "NULL";
        std::cout << expar::operator_to_string(e.type);
        if (e.right)
            e.right->accept(*this);
        else
            std::cout << "NULL";
    }

    void visit(expar::AstUnary &e) override
    {
        std::cout << expar::operator_to_string(e.type);
        if (e.right)
            e.right->accept(*this);
        else
            std::cout << "NULL";
    }

    void visit(expar::AstScope &e) override
    {
        if (e.type == expar::scp_round)
            std::cout << "(";
        else if (e.type == expar::scp_square)
            std::cout << "[";
        else if (e.type == expar::scp_curly)
            std::cout << "{";
        if (e.content)
            e.content->accept(*this);
        else
            std::cout << "NULL";
        if (e.type == expar::scp_round)
            std::cout << ")";
        else if (e.type == expar::scp_square)
            std::cout << "]";
        else if (e.type == expar::scp_curly)
            std::cout << "}";
    }

    void visit(expar::AstFunction &e) override
    {
        std::cout << e.name << "(";
        for (size_t i = 0; i < e.content.size(); ++i) {
            if (e.content[i])
                e.content[i]->accept(*this);
            else
                std::cout << "NULL";
            if (i < (e.content.size() - 1))
                std::cout << ", ";
        }
        std::cout << ")";
    }

    void visit(expar::AstVariable &e) override
    {
        std::cout << e.name;
    }

    void visit(expar::AstNumber &e) override
    {
        std::cout << e.value;
    }
};

ExpDebugPrinter debug_printer;
ExpPrinter printer;

void Test(const std::string &text)
{
    auto node = expar::parser::parse(text);
    printf("%-30s ", text.c_str());
    if (node) {
        std::cout << " OK ";
        node->accept(printer);
    } else {
        std::cout << " FAILED";
    }
    std::cout << "\n";
}

int main(int argc, char *argv[])
{
    Test("1+2+3+4");
    Test("1*2*3*4");
    Test("1-2-3-4");
    Test("1/2/3/4");
    Test("1*2+3*4");
    Test("1+2*3+4");
    Test("(1+2)*(3+4)");
    Test("1+(2*3)*(4+5)");
    Test("1+(2*3)/4+5");
    Test("5/(4+3)/2");
    Test("1 + 2.5");
    Test("125");
    Test("-1");
    Test("-1+(-2)");
    Test("-1+(-2.0)");
    Test("   1*2,5");
    Test("   1*2.5e2");
    Test("M1 + 2.5");
    Test("1 + 2&5");
    Test("1 * 2.5.6");
    Test("1 ** 2.5");
    Test("1 / 2.5");
    Test("A(1, 2, 3)");
}