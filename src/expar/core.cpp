
#include "expar/parser.hpp"

namespace expar
{
void ExpBaseVisitor::visit(AstBinary &e)
{
    e.left->accept(*this);
    e.right->accept(*this);
}

void ExpBaseVisitor::visit(AstUnary &e)
{
    e.right->accept(*this);
}

void ExpBaseVisitor::visit(AstScope &e)
{
    e.content->accept(*this);
}

void ExpBaseVisitor::visit(AstFunction &e)
{
    for (auto it = e.content.begin(); it != e.content.begin(); ++it)
        (*it)->accept(*this);
}

void ExpBaseVisitor::visit(AstVariable &e)
{
    // Nothing to do.
}

void ExpBaseVisitor::visit(AstNumber &e)
{
    // Nothing to do.
}

} // namespace expar