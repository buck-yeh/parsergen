#ifndef AST_h_
#define AST_h_

#include "bux/GLR.h"

struct C_BinaryOp: bux::I_LexAttr
{
    bux::GLR::C_LexPtr m_lexpr, m_rexpr;
    std::string m_op;

    template<class TLeft, class TRight>
    C_BinaryOp(const std::string &op, TLeft &&l, TRight &&r):
        m_lexpr(std::forward<TLeft>(l)),
        m_rexpr(std::forward<TRight>(r)),
        m_op(op) {}
    std::string toStr() const;
};

struct C_TypeDecl: bux::I_LexAttr
{
    std::string m_name;
    bux::GLR::C_LexPtr m_type;

    template<class T>
    C_TypeDecl(const std::string &name, T &&t):
        m_name(name),
        m_type(std::forward<T>(t)) {}
};

typedef std::vector<std::string> C_IdList;

#endif // AST_h_
