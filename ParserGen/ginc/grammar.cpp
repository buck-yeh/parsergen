%HEADERS_FOR_CPP    [[
#include "ParseFile.h"      // parseFile()
//-----------------------------------------------------
#include <bux/StrUtil.h>    // bux::expand_env()
#include <bux/UnicodeCvt.h> // bux::to_utf8()
]]

//
//      Grammar with Semantic Rules
//
<All> ::= <Line>            [[]]
<All> ::= <All> "\n" <Line> [[]]

<Line> ::=  [[]]
<Line> ::=  # <PreProcLine> [[]]
<Line> ::= class <QualifiedName> <OptionalTemplateArgs> [[
    auto &c = $c;
    if (!c.testCond())
        return;

    if (auto targs = bux::tryUnlex<C_TemplateArgs>($3))
    {
        if (auto err = c.setClassName(bux::unlex<C_StringList>($2), *targs))
            $p.onError($1, *err);
    }
    else
    {
        C_TemplateArgs t;
        if (auto sarg = bux::tryUnlex<std::string>($3))
            t.emplace_back().emplace_back(*sarg);

        if (auto err = c.setClassName(bux::unlex<C_StringList>($2), t))
            $p.onError($1, *err);
    }
]]
<Line> ::= <NewLexIds>              [[]]
<Line> ::= <OperatorAssociation>    [[
    auto &c = $c;
    if (!c.testCond())
        return;

    $1.m_attr.disown(); // not a pointer
    c.incWeight();
]]
<Line> ::= % $Id <Semantics>        [[
    auto &c = $c;
    if (!c.testCond())
        return;

    c.addOption(bux::unlex<std::string>($2), bux::tryUnlex<C_Semantic>($3));
]]
<Line> ::= <Production> <Semantics> [[
    auto &c = $c;
    if (!c.testCond())
        return;

    auto &prod = dynamic_cast<C_Production&>(*$1);
    if (!c.addProduction(prod, bux::tryUnlex<C_Semantic>($2)))
        $p.onError($1, "Production re-defined:\n"
                             "\t" + prod.str());
]]

<PreProcLine> ::= ifdef $Id         [[
    auto &c = $c;
    c.ifCond(c.getOption(bux::unlex<std::string>($2)), $1);
]]
<PreProcLine> ::= ifndef $Id        [[
    auto &c = $c;
    c.ifCond(!c.getOption(bux::unlex<std::string>($2)), $1);
]]
<PreProcLine> ::= else              [[
    $c.elseCond($1);
]]
<PreProcLine> ::= endif             [[
    $c.endifCond($1);
]]
<PreProcLine> ::= include $String   [[
    auto &C = $c;
    if (!C.testCond())
        return;

    const auto filename = C.expand_include(bux::unlex<std::string>($2));
    $p.reservePostShift([&,filename]{
            parseFile(filename, $P, '\n');
        }, 1);
]]

<OptionalTemplateArgs> ::=                          [[]]
<OptionalTemplateArgs> ::= < <TemplateArgList> >    [[
    $r = $2;
]]
<OptionalTemplateArgs> ::= $Nonterminal             [[
    $r = $1;
]]

<NewLexIds> ::= lexid           [[]]
<NewLexIds> ::= <NewLexIds> $Id [[
    auto &c = $c;
    if (!c.testCond())
        return;

    auto &lex =$2;
    const auto &key = bux::unlex<std::string>(lex);
    if (!c.addLexId(key))
        $p.onError(lex, "Duplicate lex id: "+key);
]]

<OperatorAssociation> ::= <AssocType>                       [[ $r = $1; ]]
<OperatorAssociation> ::= <OperatorAssociation> <Terminal>  [[
    auto &c = $c;
    if (!c.testCond())
        return;

    c.addPriority(bux::unlex<E_Associativity>($1), $2);
    $r = $1;
]]

<AssocType> ::= left    [[ $r = bux::createLex(LEFT2RIGHT); ]]
<AssocType> ::= right   [[ $r = bux::createLex(RIGHT2LEFT); ]]
<AssocType> ::= prec    [[ $r = bux::createLex(ASSOC_NONE); ]]

<Semantics> ::=                                         [[]]
<Semantics> ::= <Semantics> $Bracketed <InterBracketed> [[
    C_NewLex<C_Semantic> dst($1);
    auto &s = bux::unlex<std::string>($2);
    if (!s.empty())
        dst->m_data.emplace_back(new C_BracketedLex(s));

    if (I_SemanticChunk *t = $3)
    {
        dst->m_data.emplace_back(t);
        $3.m_attr.disown();
    }
    $r = dst;
]]

<InterBracketed> ::=                            [[]]
<InterBracketed> ::= $LexSymbol                 [[
    $r = new C_LexSymbol(bux::unlex<std::string>($1));
]]
<InterBracketed> ::= % $Id                      [[
    $r = new C_OptionLex(bux::unlex<std::string>($2));
]]

<ShiftBase> ::=         [[]]
<ShiftBase> ::= $Num    [[ $r = $1; ]]
<ShiftBase> ::= + $Num  [[ $r = $2; ]]
<ShiftBase> ::= - $Num  [[
    $r =$1;
    dynamic_cast<C_IntegerLex&>(*$r).negate();
]]

/* not allowed on left side of production
<@keyword> ::=
<@operator> ::=
*/
<Production> ::= $Nonterminal ::=           [[
    auto const t = new C_ProductionLex;
    try
    {
        t->m_Lval = bux::unlex<std::string>($1);
        if (t->m_Lval == "@keyword" ||
            t->m_Lval == "@operator" )
            $p.onError($1, std::format("Reserved non-terminal <{}> not allowed on left side of production", t->m_Lval));

        $r = t; // good or bad
    }
    catch (...)
    {
        delete t;
        throw;
    }
]]
<Production> ::= <Production> $Nonterminal  [[
    $r = $1;
    const auto s = bux::unlex<std::string>($2);
    if (s.size() > 1 && s[0] == '@')
        // Builtin nonterminal which triggers extra productions
    {
        if (!$c.activateBuiltinNonterminal(s.substr(1)))
            $p.onError($2, "Unrecognized as a builtin nonterminal");
    }
    auto t = new C_Nonterminal(s);
    try
    {
        dynamic_cast<C_Production&>(*$r).m_Rval.emplace_back(t);
    }
    catch (...)
    {
        delete t;
        throw;
    }
]]
<Production> ::= <Production> <Terminal> [[
    $r = $1;
    auto &dst = dynamic_cast<C_Production&>(*$r).m_Rval;
    C_LexPtr &src = $2;
    if (I_Terminal *t = src)
    {
        dst.emplace_back(t);
        src.disown();
    }
    else if (C_IntegerLex *i = src)
        dst.emplace_back(new C_StrLiteral(i->str()));
    else
        $p.onError($2, "Unknown production term");
]]

<QualifiedName> ::= $Id                     [[
    C_StringList t;
    t.emplace_back(bux::unlex<std::string>($1));
    $r = bux::createLex(t);
]]
<QualifiedName> ::= <QualifiedName> :: $Id  [[
    bux::unlex<C_StringList>($1).emplace_back(bux::unlex<std::string>($3));
    $r = $1;
]]

<TemplateArgList> ::= <TemplateArg>                     [[
    C_NewLex<C_TemplateArgs> t;
    t->m_data.emplace_back(bux::unlex<C_StringList>($1));
    $r = t;
]]
<TemplateArgList> ::= <TemplateArgList> , <TemplateArg> [[
    bux::unlex<C_TemplateArgs>($1).emplace_back(bux::unlex<C_StringList>($3));
    $r = $1;
]]

<TemplateArg> ::= $Id               [[
    C_StringList t;
    t.emplace_back(bux::unlex<std::string>($1));
    $r = bux::createLex(t);
]]
<TemplateArg> ::= <TemplateArg> $Id [[
    bux::unlex<C_StringList>($1).emplace_back(bux::unlex<std::string>($2));
    $r = $1;
]]
<TemplateArg> ::= <TemplateArg> *   [[
    bux::unlex<C_StringList>($1).emplace_back("*");
    $r = $1;
]]

<Terminal> ::= $LexSymbol    [[
    const auto &s = bux::unlex<std::string>($1);
    $c.addLexId(s);
    $r = new C_LexSymbol(s);
]]
<Terminal> ::= $String       [[
    $r = new C_StrLiteral(bux::unlex<std::string>($1), '\"');
]]
<Terminal> ::= $Num          [[ $r = $1; ]]
<Terminal> ::= $Key          [[
    const auto c = bux::unlex<T_Utf32>($1);
    $r = new C_StrLiteral{to_utf8(c), '\''};
]]
<Terminal> ::= $Operator     [[
    $r = new C_StrLiteral(bux::unlex<std::string>($1));
]]
<Terminal> ::= <@operator>   [[
    $r = new C_StrLiteral(bux::unlex<std::string>($1));
]]
<Terminal> ::= $Id           [[
    $r = new C_Id(bux::unlex<std::string>($1));
]]
<Terminal> ::= <@keyword>    [[
    $r = new C_Id(bux::unlex<std::string>($1));
]]
