
#include "expar/parser.hpp"
#include "antlr4-runtime.h"
#include "ExparParserBaseVisitor.h"
#include "ExparLexer.h"
#include "logging.hpp"

namespace expar::parser
{
/// @brief Helper function to cast a pointer of type **Base** to a pointer
///        of type **Derived**. **Base** must be father of **Derived**.
/// @tparam Derived the derived class type.
/// @tparam Base    the base class type.
/// @param ptr the pointer to cast.
/// @return the pointer to the same object of type **Derived**.
template <typename Derived, typename Base>
inline Derived *to(Base *ptr)
{
    static_assert(std::is_convertible<Derived *, Base *>::value, "Class must inherit Base as public");
    return dynamic_cast<Derived *>(ptr);
}

/// @brief Helper function to check if a pointer of type **Base** is actually
///        a pointer to **Derived**. **Base** must be father of **Derived**.
/// @tparam Derived the derived class type.
/// @tparam Base    the base class type.
/// @param ptr the pointer to check.
/// @return **true**  if the object is actually of type **Derived**.
/// @return **false** if the object is not of type **Derived**.
template <typename Derived, typename Base>
inline bool is_a(Base *ptr)
{
    static_assert(std::is_convertible<Derived *, Base *>::value, "Class must inherit Base as public");
    return to<Derived>(ptr) != nullptr;
}

inline double to_number(ExparParser::Value_atomContext *ctx)
{
    if (ctx->NUMBER()) {
        std::stringstream ss;
        ss << ctx->NUMBER()->toString();
        double value;
        ss >> value;
        return value;
    }
    _error("Cannot type expression number!");
    return 0;
}

inline std::string to_string(ExparParser::Value_atomContext *ctx)
{
    if (ctx->ID())
        return ctx->ID()->toString();
    if (ctx->PERCENTAGE())
        return ctx->PERCENTAGE()->toString();
    _error("Cannot type expression variable!");
    return "";
}

inline ScopeType to_scope(ExparParser::Value_scopeContext *ctx)
{
    if (ctx->OPEN_ROUND())
        return scp_round;
    if (ctx->OPEN_SQUARE())
        return scp_square;
    if (ctx->OPEN_CURLY())
        return scp_curly;
    _error("Cannot type scope!");
    return scp_none;
}

inline Operator to_operator(ExparParser::Value_unaryContext *ctx)
{
    if (ctx->PLUS())
        return op_plus;
    if (ctx->MINUS())
        return op_minus;
    _error("Cannot type operator of unary operation!");
    return op_none;
}

inline Operator to_operator(ExparParser::Value_operatorContext *ctx)
{
    // : EQUAL
    // | PLUS | MINUS | STAR | SLASH
    // | LOGIC_AND | LOGIC_BITWISE_AND
    // | LOGIC_OR  | LOGIC_BITWISE_OR
    // | LOGIC_EQUAL | LOGIC_NOT_EQUAL
    // | LOGIC_XOR
    // | LESS_THAN    | LESS_THEN_EQUAL
    // | GREATER_THAN | GREATER_THEN_EQUAL
    // | EXCLAMATION_MARK
    // | BITWISE_SHIFT_LEFT | BITWISE_SHIFT_RIGHT
    // | POWER_OPERATOR | CARET
    // | PERCENT
    if (ctx->EQUAL())
        return op_assign;
    if (ctx->PLUS())
        return op_plus;
    if (ctx->MINUS())
        return op_minus;
    if (ctx->STAR())
        return op_mult;
    if (ctx->SLASH())
        return op_div;
    if (ctx->LOGIC_AND())
        return op_and;
    if (ctx->LOGIC_BITWISE_AND())
        return op_band;
    if (ctx->LOGIC_OR())
        return op_or;
    if (ctx->LOGIC_BITWISE_OR())
        return op_bor;
    if (ctx->LOGIC_EQUAL())
        return op_eq;
    if (ctx->LOGIC_NOT_EQUAL())
        return op_neq;
    if (ctx->LOGIC_XOR())
        return op_xor;
    if (ctx->LESS_THAN())
        return op_lt;
    if (ctx->LESS_THAN_EQUAL())
        return op_le;
    if (ctx->GREATER_THAN())
        return op_lt;
    if (ctx->GREATER_THAN_EQUAL())
        return op_ge;
    if (ctx->EXCLAMATION_MARK())
        return op_not;
    if (ctx->BITWISE_SHIFT_LEFT())
        return op_bsl;
    if (ctx->BITWISE_SHIFT_RIGHT())
        return op_bsr;
    if (ctx->POWER_OPERATOR() || ctx->CARET())
        return op_pow;
    if (ctx->PERCENT())
        return op_mod;
    _error("Cannot type operator of binary operation!");
    return op_none;
}

class ExparVisitor : public ExparParserVisitor {
public:
    antlrcpp::Any visitValue(ExparParser::ValueContext *ctx) override
    {
        // :    value_unary
        // |    value_function_call
        // |    value_scope
        // |    value_atom
        // | -> value value_operator value
        if (ctx->value().size() == 2 && ctx->value_operator()) {
            auto node = new AstBinary(op_none, nullptr, nullptr);
            this->add_to_parent(node);
            this->push(node);
            auto result = visitChildren(ctx);
            this->pop();
            return result;
        }
        return visitChildren(ctx);
    }

    antlrcpp::Any visitValue_unary(ExparParser::Value_unaryContext *ctx) override
    {
        auto node = new AstUnary(to_operator(ctx), nullptr);
        this->add_to_parent(node);
        this->push(node);
        auto result = visitChildren(ctx);
        this->pop();
        return result;
    }

    antlrcpp::Any visitValue_function_call(ExparParser::Value_function_callContext *ctx) override
    {
        assert(ctx->ID() && "There is a function without ID!");
        auto node = new AstFunction(ctx->ID()->toString(), {});
        this->add_to_parent(node);
        this->push(node);
        auto result = visitChildren(ctx);
        this->pop();
        return result;
    }

    antlrcpp::Any visitValue_scope(ExparParser::Value_scopeContext *ctx) override
    {
        auto node = new AstScope(to_scope(ctx), nullptr);
        this->add_to_parent(node);
        this->push(node);
        auto result = visitChildren(ctx);
        this->pop();
        return result;
    }

    antlrcpp::Any visitValue_operator(ExparParser::Value_operatorContext *ctx) override
    {
        auto parent = this->get_back();
        assert(parent && "There is no parent!");
        auto binary = to<AstBinary>(parent);
        if (binary) {
            binary->type = to_operator(ctx);
        } else {
            auto unary = to<AstUnary>(parent);
            if (unary) {
                unary->type = to_operator(ctx);
            } else {
                perror("Parent node is not a binary/unary operator!");
                exit(1);
            }
        }
        return visitChildren(ctx);
    }

    antlrcpp::Any visitValue_atom(ExparParser::Value_atomContext *ctx) override
    {
        if (ctx->NUMBER()) {
            auto leaf = new AstNumber(to_number(ctx));
            this->add_to_parent(leaf);
        } else {
            auto leaf = new AstVariable(to_string(ctx));
            this->add_to_parent(leaf);
        }
        return visitChildren(ctx);
    }

    AstNode *root;

private:
    std::vector<AstNode *> stack;

    inline AstNode *get_back() const
    {
        if (stack.empty())
            return nullptr;
        return stack.back();
    }

    inline void push(AstNode *node)
    {
        if (node == nullptr)
            _error("Executing push and receiving a NULL node!");
        stack.emplace_back(node);
    }

    inline AstNode *pop()
    {
        auto node = this->get_back();
        if (node == nullptr)
            _error("Executing pop and receiving a NULL node!");
        stack.pop_back();
        return node;
    }

    inline void add_to_parent(AstNode *node)
    {
        AstNode *parent = this->get_back();
        if (parent) {
            auto unary = to<AstUnary>(parent);
            if (unary) {
                unary->right = node;
                return;
            }
            auto binary = to<AstBinary>(parent);
            if (binary) {
                if (!binary->left)
                    binary->left = node;
                else if (!binary->right)
                    binary->right = node;
                return;
            }
            auto function = to<AstFunction>(parent);
            if (function) {
                function->content.emplace_back(node);
                return;
            }
            auto scope = to<AstScope>(parent);
            if (scope) {
                scope->content = node;
                return;
            }
        } else {
            root = node;
        }
    }
};

AstNode *parse(const std::string &str)
{
    _debug("Reading stream...");
    antlr4::ANTLRInputStream input(str);
    _debug("Building the lexer...");
    ExparLexer lexer(&input);
    _debug("Generating the tokens...");
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();
    _debug("Initializing the parser...");
    ExparParser parser(&tokens);
    _debug("Parsing the equation...");
    ExparVisitor visitor;
    parser.value()->accept(&visitor);
    _debug("Returning the result...");
    return visitor.root;
}

} // namespace expar::parser