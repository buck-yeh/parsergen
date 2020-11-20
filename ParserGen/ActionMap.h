#ifndef ActionMapH
#define ActionMapH

#include "ParserGenBase.h"

//
//      Types
//
struct C_Actions
{
    std::set<const C_IndexedProd*>  m_reduces;
    bool                            m_hasShift{}, m_hasAccept{};

    bool operator<(const C_Actions &other) const;
    bool canUseLR1() const;
};

struct C_Lex2Action: std::map<const I_Terminal*,C_Actions,FC_LessLex>
{
    C_Lex2Action(const C_ParserInfo &parsed): std::map<const I_Terminal*,C_Actions,FC_LessLex>(parsed)
        {}
};
typedef std::map<size_t,C_Lex2Action> C_ActionMap;

typedef std::vector<std::pair<size_t,const I_Terminal*>> C_ActionShifts;

//
//      Functions
//
C_ActionMap makeActionMap(const C_ParserInfo &parsed, const C_States &states, C_ActionShifts &loserShits);

#endif // ActionMapH
