#pragma once

#include <cstdint>
#include <functional>
#include <memory>

#include "parser_sequence.h"
#include "parser_enums.h"

namespace VTE
{
namespace Parser
{


class Parser : public std::enable_shared_from_this<Parser>
{
    Sequence m_seq;
    StateEnum m_state = StateEnum::GROUND;
    bool m_dispatchUnripe = false;

public:
    Parser();
    virtual ~Parser();

    void reset();

    SeqEnum feed(uint32_t raw);
    void setDispatchUnripe(bool enable);
    void ignoreUntilStringTerminated();

private:
    SeqEnum doNop(uint32_t raw);
    SeqEnum doAction(uint32_t raw, ActionEnum action);
    SeqEnum doTransition(uint32_t raw, StateEnum state, ActionEnum action);
    SeqEnum doTransitionNoAction(uint32_t raw, StateEnum state);

    SeqEnum feedToState(uint32_t raw);

    bool checkMatchingControls(uint32_t introducer, uint32_t terminator);

    // Actions:
    SeqEnum doActionClear(uint32_t raw);
    SeqEnum doActionClearIntermediates(uint32_t raw);
    SeqEnum doActionClearIntermediatesAndParams(uint32_t raw);
    SeqEnum doActionClearParamsOnly(uint32_t raw);

    SeqEnum doActionIgnore(uint32_t raw);
    SeqEnum doActionPrint(uint32_t raw);
    SeqEnum doActionExecute(uint32_t raw);

    SeqEnum doActionCollectESC(uint32_t raw);
    SeqEnum doActionCollectCSI(uint32_t raw);
    SeqEnum doActionCollectDCS(uint32_t raw);
    SeqEnum doActionCollectParam(uint32_t raw);

    SeqEnum doActionParam(uint32_t raw);
    SeqEnum doActionFinishParam(uint32_t raw);
    SeqEnum doActionFinishSubparam(uint32_t raw);

    SeqEnum doActionESCDispatch(uint32_t raw);
    SeqEnum doActionCSIDispatch(uint32_t raw);
    SeqEnum doActionDCSStart(uint32_t raw);
    SeqEnum doActionDCSConsume(uint32_t raw);
    SeqEnum doActionDCSCollect(uint32_t raw);
    SeqEnum doActionDCSDispatch(uint32_t raw);
    SeqEnum doActionOSCStart(uint32_t raw);
    SeqEnum doActionOSCCollect(uint32_t raw);
    SeqEnum doActionOSCDispatch(uint32_t raw);
    SeqEnum doActionSCIDispatch(uint32_t raw);
};

} // namespace Parser
} // namespace VTE
