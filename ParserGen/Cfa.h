#ifndef CfaH
#define CfaH

#include "ParserGenBase.h"

//
//      Types
//
typedef std::pair<const C_State*, const I_ProductionTerm*> C_GotoKey;

class FC_LessGotoKey
{
public:

    // Nonvirtuals
    FC_LessGotoKey(const C_ParserInfo &parsed);
    bool operator()(const C_GotoKey &a, const C_GotoKey &b) const;

private:

    // Data
    FC_LessLex              m_LessTerm;
};

struct C_GotoMap: std::map<C_GotoKey,const C_State*,FC_LessGotoKey>
{
    C_GotoMap(const C_ParserInfo &parsed);
};

//
//      Functions
//
size_t makeCfa(const C_ParserInfo &parsed, C_States &states, C_GotoMap &stateMap);

#endif // CfaH
