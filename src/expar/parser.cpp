
#include "expar/parser.hpp"
#include "antlr4-runtime.h"
#include "ExparParserBaseVisitor.h"
#include "ExparLexer.h"

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
    perror("Cannot type expression number!");
    exit(1);
    return 0;
}

inline std::string to_string(ExparParser::Value_atomContext *ctx)
{
    if (ctx->ID()) {
        return ctx->ID()->toString();
    }
    if (ctx->HEX()) {
        return ctx->HEX()->toString();
    }
    if (ctx->PERCENTAGE()) {
        return ctx->PERCENTAGE()->toString();
    }
    perror("Cannot type expression variable!");
    exit(1);
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
    perror("Cannot type scope!");
    exit(1);
    return scp_none;
}

inline Operator to_operator(ExparParser::Value_unaryContext *ctx)
{
    if (ctx->PLUS())
        return op_plus;
    if (ctx->MINUS())
        return op_minus;
    perror("Cannot type operator!");
    exit(1);
    return op_none;
}

inline Operator to_operator(ExparParser::Value_operatorContext *ctx)
{
    if (ctx->PLUS())
        return op_plus;
    if (ctx->MINUS())
        return op_minus;
    if (ctx->SLASH())
        return op_div;
    if (ctx->EXCLAMATION_MARK() && ctx->EQUAL().empty())
        return op_not;
    if (ctx->PERCENT())
        return op_mod;
    if (!ctx->STAR().empty()) {
        if (ctx->STAR().size() == 1)
            return op_mult;
        return op_pow;
    }
    if (!ctx->PIPE().empty()) {
        if (ctx->PIPE().size() == 1)
            return op_bor;
        return op_or;
    }
    if (!ctx->AMPERSAND().empty()) {
        if (ctx->AMPERSAND().size() == 1)
            return op_band;
        return op_and;
    }
    if (!ctx->CARET().empty()) {
        if (ctx->CARET().size() == 1)
            return op_pow;
        return op_xor;
    }
    if (!ctx->LESS_THAN().empty()) {
        if (ctx->EQUAL().empty()) {
            if (ctx->LESS_THAN().size() == 1)
                return op_lt;
            return op_bsl;
        }
        return op_le;
    }
    if (!ctx->GREATER_THAN().empty()) {
        if (ctx->EQUAL().empty()) {
            if (ctx->GREATER_THAN().size() == 1)
                return op_gt;
            return op_bsr;
        }
        return op_ge;
    }
    if (!ctx->EQUAL().empty()) {
        if (ctx->EQUAL().size() == 2)
            return op_eq;
        return op_neq;
    }
    perror("Cannot type operator!");
    exit(1);
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
            std::cerr << "Executing push and receiving a NULL node!\n";
        stack.emplace_back(node);
    }

    inline AstNode *pop()
    {
        auto node = this->get_back();
        if (node == nullptr)
            std::cerr << "Executing pop and receiving a NULL node!\n";
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
    antlr4::ANTLRInputStream input(str);
    ExparLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);

    tokens.fill();

    ExparParser parser(&tokens);

    ExparVisitor visitor;

    // Parse the circuit.
    parser.value()->accept(&visitor);

    return visitor.root;
}

} // namespace expar::parser