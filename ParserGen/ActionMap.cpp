#include "ActionMap.h"
#include "XException.h" // RUNTIME_ERROR()
#include <optional>     // std::optional<>

#define GLR_ACTIONS_

namespace {

//
//      Local Functions
//
#ifndef GLR_ACTIONS_
std::string toStr(const C_ReductionStep &srcRStep)
{
    return srcRStep.first->inputName(srcRStep.second, "(.)");
}

std::string toMessage       (
    const std::string       &prefix,
    const C_ReductionStep   &item1,
    const C_ReductionStep   &item2  )
{
    return  prefix + " Conflict :\n"
            "\t" + toStr(item1) + "\n"
            "\t" + toStr(item2);
}
#endif

std::optional<int> makeCompare(
    const C_PriorityMap     &priorMap,
    T_LexID                 left,
    T_LexID                 right   )
{
    const auto i1 = priorMap.find(left);
    if (i1 == priorMap.end())
        return {};

    const auto i2 = priorMap.find(right);
    if (i2 == priorMap.end())
        return {};

    int compare = int(i1->second.m_Weight) - int(i2->second.m_Weight);
    if (!compare && i1->second.m_Assoc == i2->second.m_Assoc)
        switch (i1->second.m_Assoc)
        {
        case ASSOC_NONE:
            return {};
        case LEFT2RIGHT:
            compare = 1;
            break;
        case RIGHT2LEFT:
            compare = -1;
            break;
        default:
            RUNTIME_ERROR("Unknown associativity {}", i1->second.m_Assoc);
        }
    return compare;
}

} // namespace

//
//      External Functions
//
C_ActionMap makeActionMap(const C_ParserInfo &parsed, const C_States &states, C_ActionShifts &loserShits)
{
    // (1/2) Collect all
    C_ActionMap actionMap;
    for (auto &i: states)
    {
        auto &dstLex2Act = actionMap.emplace(i.m_id, parsed).first->second; // C_Lex2Action
        for (auto &j: i.m_rstep2la)
        {
            const auto srcRStep = j.first;
            if (srcRStep.first->m_Rval.size() == srcRStep.second)
                // At end of production - Reduction or Acceptance
            {
#ifdef GLR_ACTIONS_
                for (auto k: j.second)
                {
                    auto &dst = dstLex2Act[k];
                    if (!k && srcRStep.first->m_Lval == "@")
                        dst.m_hasAccept = true;
                    else
                        dst.m_reduces.insert(srcRStep.first);
                }
#else
                const auto &srcLA = j.second;
                if (srcRStep.first->m_Lval == "@" && srcLA.size() == 1 && !*srcLA.begin())
                    // <@> reduced on EOF - End of parsing
                {
                    const auto iDst = dstLex2Act.find(nullptr); // nullptr <-> EOF
                    if (iDst != dstLex2Act.end() &&
                        iDst->second.first != ACCEPT)
                        // Just in case: Simply overwrite the action
                        conflicts.insert(toMessage("EOF", iDst->second.second, srcRStep));

                    auto &t = dstLex2Act[nullptr];
                    t.first = ACCEPT;
                    t.second = srcRStep;
                }
                else for (auto k: srcLA)
                {
                    C_ActionValue   val{REDUCE, srcRStep}; // defaulted to reduce on each lookahead
                    const auto      iDst = dstLex2Act.find(k);
                    if (iDst != dstLex2Act.end())
                        // Conflict iDst
                    {
                        bool conflict = true;
                        const char *prefix = nullptr;
                        if (iDst->second.first == SHIFT)
                            // Shift-Reduce Conflict -- Sovled wherever operator precedency applies
                        {
                            const auto kid = parsed.prodTerm2id(k);
                            const auto &srcRval = srcRStep.first->m_Rval;
                            if (srcRval.empty())
                                // Reduce empty production first
                                conflict = false;
                            else
                                // Check operator precedency
                                for (auto m = srcRval.crbegin(); m != srcRval.crend(); ++m)
                                {
                                    const auto mid = parsed.prodTerm2id(*m);
                                    int compare;
                                    if (makeCompare(parsed.priorityMap(), mid, kid, compare))
                                    {
                                        if (compare < 0)
                                            // Don't change it
                                            val = iDst->second;

                                        conflict = false;
                                        break;
                                    }
                                }

                            if (conflict)
                            {
                                prefix = "Shift-Reduce";
                                val = iDst->second; // Don't change it
                            }
                        }
                        else if (iDst->second.first == REDUCE)
                            // Reduce-Reduce Conflict
                        {
                            /*  Either they are the same, or the longer the better
                             */
                            if (iDst->second.second.first == srcRStep.first ||
                                iDst->second.second.first->m_Rval.size() < srcRStep.first->m_Rval.size())
                                conflict = false;
                            else if (iDst->second.second.first->m_Rval.size() > srcRStep.first->m_Rval.size())
                            {
                                conflict = false;
                                val.second = iDst->second.second;
                            }
                            else
                            {
                                prefix = "Reduce-Reduce";
                                //%%%%% Case Study
                            }
                        }
                        else
                            prefix = "Other(1/2)";

                        if (conflict)
                            conflicts.insert(toMessage(prefix, iDst->second.second, srcRStep));
                    }
                    dstLex2Act[k] = val;
                } // for (auto &k: srcLA)
#endif // GLR_ACTIONS_
            }
            else if (auto const t = dynamic_cast<I_Terminal*>(srcRStep.first->m_Rval.at(srcRStep.second)))
                // Shift next terminal - The sufficient condition to grow the dstLex2Act
            {
#ifdef GLR_ACTIONS_
                dstLex2Act[t].m_hasShift = true;
#else
                C_ActionValue val(SHIFT, srcRStep);   // defaulted to shift
                auto iDst = dstLex2Act.find(t);
                if (iDst != dstLex2Act.end())
                    // Conflict iDst
                {
                    bool conflict = true;
                    const char *prefix = nullptr;
                    const auto tid = parsed.prodTerm2id(t);
                    if (iDst->second.first == REDUCE)
                        // Shift-Reduce Conflict (Again) - Prefer SHIFT unless operator precedency speaks.
                    {
                        auto const prod = iDst->second.second.first;
                        const auto &seq = prod->m_Rval;
                        for (auto m = seq.end(); m != seq.begin();)
                        {
                            const auto mid = parsed.prodTerm2id(*--m);
                            int compare;
                            if (makeCompare(parsed.priorityMap(), mid, tid, compare))
                                // Conflict sovled wherever operator precedency applies
                            {
                                if (compare > 0)
                                    val = iDst->second;

                                break;
                            }
                        }
                        conflict = false;
                    }
                    else if (iDst->second.first == SHIFT)
                        // Shift-Shift Conflict -- %%%%% Why ? Defaulted ?
                    {
                        if (iDst->second.second == srcRStep)
                            conflict = false;
                        else
                        {
                            auto &fstep = iDst->second.second;
                            auto &rval = srcRStep.first->m_Rval;
                            auto &fval = fstep.first->m_Rval;
                            const size_t n = rval.size() - srcRStep.second;
                            if (n == fval.size() - fstep.second)
                            {
                                for (size_t i = 0; i < n; ++i)
                                {
                                    const auto rid = parsed.prodTerm2id(rval[srcRStep.second+i]);
                                    const auto fid = parsed.prodTerm2id(fval[fstep.second+i]);
                                    if (rid != fid)
                                        goto conflictStill;
                                }
                                conflict = false;
                                if (fval.size() > rval.size())
                                    // Prefer longer production
                                    val.second = fstep;

                            conflictStill:;
                            }
                        }
                        if (conflict)
                            prefix = "Shift-Shift";
                    }
                    else
                        prefix = "Other(2/2)";

                    if (conflict)
                        conflicts.insert(toMessage(prefix, iDst->second.second, srcRStep));
                }
                dstLex2Act[t] = val;
#endif // GLR_ACTIONS_
            }
        } // for (auto &j: i.m_rstep2la)
    } // for (auto &i: states)

    // (2/2) Minimize conflicts
    for (auto &i: actionMap)
        for (auto &j: i.second)
        {
            if (j.second.m_hasShift && !j.second.m_reduces.empty())
                // Minimize shift-reduce conflicts by operator precedency
            {
                const auto jid = parsed.prodTerm2id(j.first);
                for (auto k = j.second.m_reduces.begin(); k != j.second.m_reduces.end();)
                {
                    const auto &srcRval = (**k).m_Rval;
                    for (auto m = srcRval.crbegin(); m != srcRval.crend(); ++m)
                        if (auto compare = makeCompare(parsed.priorityMap(), parsed.prodTerm2id(*m), jid))
                        {
                            if (*compare < 0)
                                // Shift wins
                            {
                                k = j.second.m_reduces.erase(k);
                                goto NextK;
                            }
                            else
                                // Reduce wins
                            {
                                j.second.m_hasShift = false;
                                loserShits.emplace_back(i.first, j.first);
                                goto DoneShiftReduce;
                            }
                        }
                    ++k;
                NextK:;
                }
            DoneShiftReduce:;
            }
        }
    return actionMap;
}

//
//      Implement Classes
//
bool C_Actions::operator<(const C_Actions &other) const
{
    if (m_hasShift != other.m_hasShift)
        return m_hasShift < other.m_hasShift;
    if (m_hasAccept != other.m_hasAccept)
        return m_hasAccept < other.m_hasAccept;

    return m_reduces < other.m_reduces;
}

bool C_Actions::canUseLR1() const
{
    switch (m_reduces.size())
    {
    case 0:
        return m_hasShift? !m_hasAccept: m_hasAccept;
    case 1:
        return !m_hasShift && !m_hasAccept;
    default:
        return false;
    }
}
