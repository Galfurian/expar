/// @file   enums.hpp
/// @author Enrico Fraccaroli

#pragma once

#include <string>

namespace expar
{
/// @brief All possible operators.
enum Operator {
    op_none,
    op_plus,
    op_minus,
    op_mult,
    op_div,
    op_or,
    op_and,
    op_xor,
    op_not,
    op_bor,
    op_band,
    op_bsl,
    op_bsr,
    op_eq,
    op_neq,
    op_lt,
    op_gt,
    op_le,
    op_ge,
    op_mod,
    op_pow
};

/// @brief Return string representation of given operator.
/// @param op the operator.
/// @return The string representation of given operator.
std::string operator_to_string(Operator op);

/// @brief Return the operator represented by the string.
/// @param s the string.
/// @return The operator.
Operator string_to_operator(const std::string &s);

/// @brief Return the string representation of the given operator enum name (e.g. op_and returns "op_and").
/// @param op the operator.
/// @return The string representation of given operator.
std::string operator_to_plain_string(Operator op);

/// @brief Return the operator for given string (e.g. "op_and" returns op_and).
/// @param s the plain string.
/// @return The operator.
Operator plain_string_to_operator(const std::string &s);

/// @brief Which kind of symbols are used for declaring a scope.
enum ScopeType {
    scp_none,   ///< No scope type.
    scp_round,  ///< Round brackets ( )
    scp_square, ///< Square brackets [ ]
    scp_curly,  ///< Curly brackets { }
    scp_apex,   ///< Apex ' '
    scp_quotes  ///< Quotes " "
};

/// @brief Return the string representation of the given type of scope enum name (e.g. scp_round returns "scp_round").
/// @param scp The type of scope.
/// @return The string representation of given type of scope.
std::string scopetype_to_plain_string(ScopeType scp);

/// @brief Return the type of scope for given string (e.g. "scp_round" returns scp_round).
/// @param s the plain string.
/// @return The type of scope.
ScopeType plain_string_to_scopetype(const std::string &s);

/// @brief International System of Units (SI)
enum SiPrefix {
    si_yotta, ///< Y  10e24
    si_zetta, ///< Z  10e21
    si_exa,   ///< E  10e18
    si_peta,  ///< P  10e15
    si_tera,  ///< T  10e12
    si_giga,  ///< G  10e9
    si_mega,  ///< M  10e6
    si_chilo, ///< k  10e3
    si_none,  ///< -  1
    si_milli, ///< m  10e-3
    si_micro, ///< u  10e-6
    si_nano,  ///< n  10e-9
    si_pico,  ///< p  10e-12
    si_femto, ///< f  10e-15
    si_atto,  ///< a  10e-18
    si_zepto, ///< z  10e-21
    si_yocto, ///< y  10e-24
};

/// @brief Return the string representation of the given type of SI unit (e.g. si_chilo returns "si_chilo").
/// @param si the type of SI unit.
/// @return The string representation of given type of SI unit.
std::string siprefix_to_plain_string(SiPrefix si);

/// @brief Return the type of SI unit for given string (e.g. "si_chilo" returns si_chilo).
/// @param s the plain string.
/// @return The type of SI unit.
SiPrefix plain_string_to_siprefix(const std::string &s);

/// @brief Return the letter representation of the given type of SI unit (e.g. si_chilo returns 'k').
/// @param si the type of SI unit.
/// @return The letter representation of given type of SI unit.
char siprefix_to_letter(SiPrefix si);

/// @brief Return the scaling factor of the given type of SI unit (e.g. si_chilo returns 1e03).
/// @param si the type of SI unit.
/// @return The scaling factor of given type of SI unit.
double siprefix_to_scaling_factor(SiPrefix si);

} // namespace expar