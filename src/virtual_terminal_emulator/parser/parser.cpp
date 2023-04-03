#include "parser.h"

#include <cassert>

#define VTE_DISPATCH_UNRIPE            (1)
#define VTE_SEQ_PARAMETER_BITS         (3)
#define VTE_SEQ_INTERMEDIATE_BITS      (5)

using namespace VTE::Parser;

Parser::Parser()
{

}

Parser::~Parser()
{

}

SeqEnum Parser::doNop(uint32_t raw)
{
    return SeqEnum::NONE;
}

SeqEnum Parser::doAction(uint32_t raw, ActionEnum action)
{
    switch(action)
    {
    case ActionEnum::CLEAR: return doActionClear(raw);
    case ActionEnum::CLEAR_INT: return doActionClearIntermediates(raw);
    case ActionEnum::CLEAR_INT_AND_PARAMS: return doActionClearIntermediatesAndParams(raw);
    case ActionEnum::CLEAR_PARAMS_ONLY: return doActionClearParamsOnly(raw);
    case ActionEnum::IGNORE: return doActionIgnore(raw);
    case ActionEnum::PRINT: return doActionPrint(raw);
    case ActionEnum::EXECUTE: return doActionExecute(raw);
    case ActionEnum::COLLECT_ESC: return doActionCollectESC(raw);
    case ActionEnum::COLLECT_CSI: return doActionCollectCSI(raw);
    case ActionEnum::COLLECT_DCS: return doActionCollectDCS(raw);
    case ActionEnum::COLLECT_PARAM: return doActionCollectParam(raw);
    case ActionEnum::PARAM: return doActionParam(raw);
    case ActionEnum::FINISH_PARAM: return doActionFinishParam(raw);
    case ActionEnum::FINISH_SUBPARAM: return doActionFinishSubparam(raw);
    case ActionEnum::ESC_DISPATCH: return doActionESCDispatch(raw);
    case ActionEnum::CSI_DISPATCH: return doActionCSIDispatch(raw);
    case ActionEnum::DCS_START: return doActionDCSStart(raw);
    case ActionEnum::DCS_CONSUME: return doActionDCSConsume(raw);
    case ActionEnum::DCS_COLLECT: return doActionDCSCollect(raw);
    case ActionEnum::DCS_DISPATCH: return doActionDCSDispatch(raw);
    case ActionEnum::OSC_START: return doActionOSCStart(raw);
    case ActionEnum::OSC_COLLECT: return doActionOSCCollect(raw);
    case ActionEnum::OSC_DISPATCH: return doActionOSCDispatch(raw);
    case ActionEnum::SCI_DISPATCH: return doActionSCIDispatch(raw);
    };
    return SeqEnum::NONE;
}

SeqEnum Parser::doTransition(uint32_t raw, StateEnum state, ActionEnum action)
{
    m_state = state;
    return doAction(raw, action);
}

SeqEnum Parser::doTransitionNoAction(uint32_t raw, StateEnum state)
{
    m_state = state;
    return SeqEnum::NONE;
}

void Parser::reset()
{

}

bool Parser::checkMatchingControls(uint32_t introducer, uint32_t terminator)
{
    return ((introducer ^ terminator) & 0x80) == 0;
}

SeqEnum Parser::doActionClear(uint32_t raw)
{
    /* seq.command is set when the sequence is executed,
    * seq.terminator is set when the final character is received,
    * and seq.introducer is set when the introducer is received,
    * and all this happens before the sequence is dispatched.
    * Therefore these fiedls need not be cleared in any case.
    */
    return SeqEnum::NONE;
}

SeqEnum Parser::doActionClearIntermediates(uint32_t raw)
{
    m_seq.intermediates = 0;
    m_seq.nIntermediates = 0;
    return doActionClear(raw);
}

SeqEnum Parser::doActionClearIntermediatesAndParams(uint32_t raw)
{
    doActionClearIntermediates(raw);
    return doActionClearParamsOnly(raw);
}

SeqEnum Parser::doActionClearParamsOnly(uint32_t raw)
{
    m_seq.args.clear();
    m_seq.nArgs = 0;
    m_seq.nFinalArgs = 0;
    return doActionClear(raw);
}

SeqEnum Parser::doActionIgnore(uint32_t raw)
{
    m_seq.type = SeqEnum::IGNORE;
    m_seq.command = CommandEnum::NONE;
    m_seq.terminator = raw;
    return m_seq.type;
}

SeqEnum Parser::doActionPrint(uint32_t raw)
{
    m_seq.type = SeqEnum::GRAPHIC;
    m_seq.command = CommandEnum::GRAPHIC;
    m_seq.terminator = raw;
    return m_seq.type;
}

SeqEnum Parser::doActionExecute(uint32_t raw)
{
    m_seq.type = SeqEnum::CONTROL;
    m_seq.terminator = raw;
    m_seq.command = m_seq.hostControl2Command();
    return m_seq.type;
}

SeqEnum Parser::doActionCollectESC(uint32_t raw)
{
    assert(raw >= 0x20 && raw <= 0x2f);

    /* ESCAPE sequences only have intermediates or 2/0..2/15, so there's no
    * need for the extra shift as below for CSI/DCS sequences
    */
    m_seq.intermediates |= (VTE_SEQ_MAKE_INTERMEDIATE(raw) << (VTE_SEQ_INTERMEDIATE_BITS * m_seq.nIntermediates++));

    return SeqEnum::NONE;
}

SeqEnum Parser::doActionCollectCSI(uint32_t raw)
{
    assert(raw >= 0x20 && raw <= 0x2f);

    /* In addition to 2/0..2/15 intermediates, CSI/DCS sequence
    * can also have one parameter byte 3/12..3/15 at the
    * start of the parameters (see doActionCollectParam below);
    * that's what the extra shift is for.
    */
    m_seq.intermediates |= (VTE_SEQ_MAKE_INTERMEDIATE(raw) << (VTE_SEQ_PARAMETER_BITS +
        VTE_SEQ_INTERMEDIATE_BITS * m_seq.nIntermediates++));

    return SeqEnum::NONE;
}

SeqEnum Parser::doActionCollectDCS(uint32_t raw)
{

}

SeqEnum Parser::doActionCollectParam(uint32_t raw)
{
    assert(raw >= 0x3c && raw <= 0x3f);

    /* CSI/DCS may optionally have one parameter byte from 3/12..3/15
    * at the start of the parameters; we put that into the lowest
    * part of @seq.intermediates.
    * Note that there can only be *one* such byte; the state machine
    * already enforces that, so we do not need any additional checks
    * here.
    */
    m_seq.intermediates |= VTE_SEQ_MAKE_PARAMETER(raw);

    return SeqEnum::NONE;
}

SeqEnum Parser::doActionParam(uint32_t raw)
{
    // assert(raw >= '0' && raw <= '9');

    m_seq.args.back().push(raw);

    return SeqEnum::NONE;
}

SeqEnum Parser::doActionFinishParam(uint32_t raw)
{
    m_seq.args.back().finish(false);
    ++m_seq.nArgs;
    ++m_seq.nFinalArgs;
    return SeqEnum::NONE;
}

SeqEnum Parser::doActionFinishSubparam(uint32_t raw)
{
    m_seq.args.back().finish(true);
    ++m_seq.nArgs;
    return SeqEnum::NONE;
}

SeqEnum Parser::doActionESCDispatch(uint32_t raw)
{
    m_seq.type = SeqEnum::ESCAPE;
    m_seq.terminator = raw;
    m_seq.charset = VTE_CHARSET_NONE;
    m_seq.command = m_seq.hostEscape2Command(m_seq.charset);
    return m_seq.type;
}

SeqEnum Parser::doActionCSIDispatch(uint32_t raw)
{
    /* m_seq is cleared during CSI-ENTER state, thus there's no need
    * to clear invalid fields here. */

    if (m_seq.nArgs > 0 || m_seq.args.back().hasValue())
    {
        m_seq.args.back().finish(false);
        ++m_seq.nArgs;
        ++m_seq.nFinalArgs;
    }

    m_seq.type = SeqEnum::CSI;
    m_seq.terminator = raw;
    m_seq.command = m_seq.hostCSI2Command();

    return m_seq.type;
}

SeqEnum Parser::doActionDCSStart(uint32_t raw)
{
    doActionClearIntermediatesAndParams(raw);
    m_seq.argStr.clear();
    m_seq.introducer = raw;
    return SeqEnum::NONE;
}

SeqEnum Parser::doActionDCSConsume(uint32_t raw)
{
    /* m_seq is cleared during DCS-START state, thus there's no need
    * to clear invalid fields here. */

    if (m_seq.nArgs > 0 || m_seq.args.back().hasValue())
    {
        m_seq.args.back().finish(false);
        ++m_seq.nArgs;
        ++m_seq.nFinalArgs;
    }

    m_seq.type = SeqEnum::DCS;
    m_seq.terminator = raw;
    m_seq.st = 0;

    unsigned int flags;
    m_seq.command = m_seq.hostDCS2Command(flags);

    if ((flags & VTE_DISPATCH_UNRIPE) && m_dispatchUnripe) return SeqEnum::DCS;
    return SeqEnum::NONE;
}

SeqEnum Parser::doActionDCSCollect(uint32_t raw)
{
    if(!m_seq.argStrPush(raw))
    {
        m_state = StateEnum::DCS_IGNORE;
    }
    return SeqEnum::NONE;
}

SeqEnum Parser::doActionDCSDispatch(uint32_t raw)
{
    /* Most of parser->seq was already filled in parser_dcs_consume() */
    m_seq.st = raw;

    /* We only dispatch a DCS if the introducer and string
    * terminator are from the same control set, i.e. both
    * C0 or both C1; we discard sequences with mixed controls.
    */
    if (!checkMatchingControls(m_seq.introducer, raw))
    {
        return SeqEnum::IGNORE;
    }

    return m_seq.type;
}

SeqEnum Parser::doActionOSCStart(uint32_t raw)
{
    doActionClear(raw);
    m_seq.argStr.clear();
    m_seq.introducer = raw;
    return SeqEnum::NONE;
}

SeqEnum Parser::doActionOSCCollect(uint32_t raw)
{
    /*
    * Only characters from 0x20..0x7e and >= 0xa0 are allowed here.
    * Our state-machine already verifies those restrictions.
    */

    if (!m_seq.argStrPush(raw))
    {
        m_state = StateEnum::ST_IGNORE;
    }

    return SeqEnum::NONE;
}

SeqEnum Parser::doActionOSCDispatch(uint32_t raw)
{
    /* m_seq is cleared during OSC_START state, thus there's no need
    * to clear invalid fields here.
    *
    * We only dispatch a DCS if the introducer and string
    * terminator are from the same control set, i.e. both
    * C0 or both C1; we discard sequences with mixed controls.
    */
    if (!checkMatchingControls(m_seq.introducer, raw))
    {
        return SeqEnum::IGNORE;
    }

    m_seq.type = SeqEnum::OSC;
    m_seq.command = CommandEnum::OSC;
    m_seq.st = raw;

    return m_seq.type;
}

SeqEnum Parser::doActionSCIDispatch(uint32_t raw)
{
    m_seq.type = SeqEnum::SCI;
    m_seq.terminator = raw;
    m_seq.command = m_seq.hostSCI2Command();

    return m_seq.type;
}

SeqEnum Parser::feed(uint32_t raw)
{
    switch (raw)
    {
    case 0x18: // CAN
        return doTransition(raw, StateEnum::GROUND, ActionEnum::IGNORE);
    case 0x1a: // SUB
        return doTransition(raw, StateEnum::GROUND, ActionEnum::EXECUTE);
    case 0x7f: // DEL
        return doNop(raw);
    case 0x80 ... 0x8f: // C1 \ {DCS, SOS, SCI, CSI, ST, OSC, PM, APC}
    case 0x91 ... 0x97:
    case 0x99:
            return doTransition(raw, StateEnum::GROUND, ActionEnum::EXECUTE);
    case 0x98: // SOS
    case 0x9e: // PM
    case 0x9f: // APC
        return doTransitionNoAction(raw, StateEnum::ST_IGNORE);
        // Shouldn't this use ACTION_CLEAR?
    case 0x90: // DCS
        return doTransition(raw, StateEnum::DCS_ENTRY, ActionEnum::DCS_START);
    case 0x9a: // SCI
        return doTransition(raw, StateEnum::SCI, ActionEnum::CLEAR);
    case 0x9d: // OSC
        return doTransition(raw, StateEnum::OSC_STRING, ActionEnum::OSC_START);
    case 0x9b: // CSI
        return doTransition(raw, StateEnum::CSI_ENTRY, ActionEnum::CLEAR_INT_AND_PARAMS);
    default:
        return feedToState(raw);
    }
}

SeqEnum Parser::feedToState(uint32_t raw)
{
    switch (m_state)
    {
    case StateEnum::GROUND:
        switch (raw)
        {
            case 0x00 ... 0x1a: // C0 \ { ESC }
            case 0x1c ... 0x1f:
            case 0x80 ... 0x9f: // C1
                return doAction(raw, ActionEnum::EXECUTE);
            case 0x1b: // ESC
                return doTransition(raw, StateEnum::ESC, ActionEnum::CLEAR_INT);
        }
        return doAction(raw, ActionEnum::PRINT);

    case StateEnum::DCS_PASS_ESC:
    case StateEnum::OSC_STRING_ESC:
        if (raw == 0x5c) // '\'
        {
            switch (m_state)
            {
            case StateEnum::DCS_PASS_ESC:
                return doTransition(raw, StateEnum::GROUND, ActionEnum::DCS_DISPATCH);
            case StateEnum::OSC_STRING_ESC:
                return doTransition(raw, StateEnum::GROUND, ActionEnum::OSC_DISPATCH);
            default:
                break;
            }
        }

        // Do the deferred clear and fallthrough to STATE_ESC
        doTransition(0x1b, // ESC
            StateEnum::ESC, ActionEnum::CLEAR_INT);

        [[fallthrough]];
        case StateEnum::ESC:
            switch (raw)
            {
            case 0x00 ... 0x1a: // C0 \ { ESC }
            case 0x1c ... 0x1f:
                return doAction(raw, ActionEnum::EXECUTE);
            case 0x1b: // ESC
                return doTransition(raw, StateEnum::ESC, ActionEnum::CLEAR_INT);
            case 0x20 ... 0x2f: // [' ' - '\']
                return doTransition(raw, StateEnum::ESC_INT, ActionEnum::COLLECT_ESC);
            case 0x30 ... 0x4f: // ['0' - '~']
            case 0x51 ... 0x57: // { 'P', 'X', 'Z' '[', ']', '^', '_' }
            case 0x59:
            case 0x5c:
            case 0x60 ... 0x7e:
                return doTransition(raw, StateEnum::GROUND, ActionEnum::ESC_DISPATCH);
            case 0x50: // 'P'
                return doTransition(raw, StateEnum::DCS_ENTRY, ActionEnum::DCS_START);
            case 0x5a: // 'Z'
                return doTransition(raw, StateEnum::SCI, ActionEnum::CLEAR);
            case 0x5b: // '['
                return doTransition(raw, StateEnum::CSI_ENTRY, ActionEnum::CLEAR_PARAMS_ONLY);
                // rest already cleaned on ESC state entry
            case 0x5d: // ']'
                return doTransition(raw, StateEnum::OSC_STRING, ActionEnum::OSC_START);
            case 0x58: // 'X'
            case 0x5e: // '^'
            case 0x5f: // '_'
                return doTransitionNoAction(raw, StateEnum::ST_IGNORE);
            case 0x9c: // ST
                return doTransition(raw, StateEnum::GROUND, ActionEnum::IGNORE);
            }
            return doTransition(raw, StateEnum::GROUND, ActionEnum::IGNORE);

        case StateEnum::ESC_INT:
            switch (raw)
            {
            case 0x00 ... 0x1a: // C0 \ { ESC }
            case 0x1c ... 0x1f:
                return doAction(raw, ActionEnum::EXECUTE);
            case 0x1b: // ESC
                    return doTransition(raw, StateEnum::ESC, ActionEnum::CLEAR_INT);
            case 0x20 ... 0x2f: // [' ' - '\']
                    return doAction(raw, ActionEnum::COLLECT_ESC);
            case 0x30 ... 0x7e: // ['0' - '~']
                return doTransition(raw, StateEnum::GROUND, ActionEnum::ESC_DISPATCH);
            case 0x9c: // ST
                return doTransition(raw, StateEnum::GROUND, ActionEnum::IGNORE);
            }
            return doTransition(raw, StateEnum::GROUND, ActionEnum::IGNORE);

        case StateEnum::CSI_ENTRY:
            switch (raw)
            {
            case 0x00 ... 0x1a: // C0 \ { ESC }
            case 0x1c ... 0x1f:
                return doAction(raw, ActionEnum::EXECUTE);
            case 0x1b: // ESC
                return doTransition(raw, StateEnum::ESC, ActionEnum::CLEAR_INT);
            case 0x20 ... 0x2f: // [' ' - '\']
                return doTransition(raw, StateEnum::CSI_INT, ActionEnum::COLLECT_CSI);
            case 0x30 ... 0x39: // ['0' - '9']
                return doTransition(raw, StateEnum::CSI_PARAM, ActionEnum::PARAM);
            case 0x3a: // ':'
                return doTransition(raw, StateEnum::CSI_PARAM, ActionEnum::FINISH_SUBPARAM);
            case 0x3b: // ';'
                return doTransition(raw, StateEnum::CSI_PARAM, ActionEnum::FINISH_PARAM);
            case 0x3c ... 0x3f: // ['<' - '?']
                return doTransition(raw, StateEnum::CSI_PARAM, ActionEnum::COLLECT_PARAM);
            case 0x40 ... 0x7e: // ['@' - '~']
                return doTransition(raw, StateEnum::GROUND, ActionEnum::CSI_DISPATCH);
            case 0x9c: // ST
                return doTransition(raw, StateEnum::GROUND, ActionEnum::IGNORE);
            }
            return doTransitionNoAction(raw, StateEnum::CSI_IGNORE);

        case StateEnum::CSI_PARAM:
            switch (raw)
            {
            case 0x00 ... 0x1a: // C0 \ { ESC }
            case 0x1c ... 0x1f:
                    return doAction(raw, ActionEnum::EXECUTE);
            case 0x1b: // ESC
                    return doTransition(raw, StateEnum::ESC, ActionEnum::CLEAR_INT);
            case 0x20 ... 0x2f: // [' ' - '\']
                    return doTransition(raw, StateEnum::CSI_INT, ActionEnum::COLLECT_CSI);
            case 0x30 ... 0x39: // ['0' - '9']
                    return doAction(raw, ActionEnum::PARAM);
            case 0x3a: // ':'
                    return doAction(raw, ActionEnum::FINISH_SUBPARAM);
            case 0x3b: // ';'
                    return doAction(raw, ActionEnum::FINISH_PARAM);
            case 0x3c ... 0x3f: // ['<' - '?']
                    return doTransitionNoAction(raw, StateEnum::CSI_IGNORE);
            case 0x40 ... 0x7e: // ['@' - '~']
                    return doTransition(raw, StateEnum::GROUND, ActionEnum::CSI_DISPATCH);
            case 0x9c: // ST
                    return doTransition(raw, StateEnum::GROUND, ActionEnum::IGNORE);
            }
            return doTransitionNoAction(raw, StateEnum::CSI_IGNORE);

        case StateEnum::CSI_INT:
            switch (raw)
            {
            case 0x00 ... 0x1a: // C0 \ { ESC }
            case 0x1c ... 0x1f:
                return doAction(raw, ActionEnum::EXECUTE);
            case 0x1b: // ESC
                return doTransition(raw, StateEnum::ESC, ActionEnum::CLEAR_INT);
            case 0x20 ... 0x2f: // [' ' - '\']
                return doAction(raw, ActionEnum::COLLECT_CSI);
            case 0x30 ... 0x3f: // ['0' - '?']
                return doTransitionNoAction(raw, StateEnum::CSI_IGNORE);
            case 0x40 ... 0x7e: // ['@' - '~']
                return doTransition(raw, StateEnum::GROUND, ActionEnum::CSI_DISPATCH);
            case 0x9c: // ST
                return doTransition(raw, StateEnum::GROUND, ActionEnum::IGNORE);
            }
            return doTransitionNoAction(raw, StateEnum::CSI_IGNORE);

        case StateEnum::CSI_IGNORE:
            switch (raw)
            {
            case 0x00 ... 0x1a: // C0 \ { ESC }
            case 0x1c ... 0x1f:
                return doAction(raw, ActionEnum::EXECUTE);
            case 0x1b: // ESC
                return doTransition(raw, StateEnum::ESC, ActionEnum::CLEAR_INT);
            case 0x20 ... 0x3f: // [' ' - '?']
                return doNop(raw);
            case 0x40 ... 0x7e: // ['@' - '~']
                return doTransitionNoAction(raw, StateEnum::GROUND);
            case 0x9c: // ST
                return doTransition(raw, StateEnum::GROUND, ActionEnum::IGNORE);
            }
            return doNop(raw);

        case StateEnum::DCS_ENTRY:
            switch (raw)
            {
            case 0x00 ... 0x1a: // C0 \ ESC
            case 0x1c ... 0x1f:
                return doAction(raw, ActionEnum::IGNORE);
            case 0x1b: // ESC
                return doTransition(raw, StateEnum::ESC, ActionEnum::CLEAR_INT);
            case 0x20 ... 0x2f: // [' ' - '\']
                return doTransition(raw, StateEnum::DCS_INT, ActionEnum::DCS_COLLECT);
            case 0x30 ... 0x39: // ['0' - '9']
                return doTransition(raw, StateEnum::DCS_PARAM, ActionEnum::PARAM);
            case 0x3a: // ':'
                return doTransition(raw, StateEnum::DCS_PARAM, ActionEnum::FINISH_SUBPARAM);
            case 0x3b: // ';'
                return doTransition(raw, StateEnum::DCS_PARAM, ActionEnum::FINISH_PARAM);
            case 0x3c ... 0x3f: // ['<' - '?']
                return doTransition(raw, StateEnum::DCS_PARAM, ActionEnum::COLLECT_PARAM);
            case 0x40 ... 0x7e: // ['@' - '~']
                return doTransition(raw, StateEnum::DCS_PASS, ActionEnum::DCS_CONSUME);
            case 0x9c: // ST
                return doTransition(raw, StateEnum::GROUND, ActionEnum::IGNORE);
            }
            return doTransition(raw, StateEnum::DCS_PASS, ActionEnum::DCS_CONSUME);

        case StateEnum::DCS_PARAM:
            switch (raw)
            {
            case 0x00 ... 0x1a: // C0 \ { ESC }
            case 0x1c ... 0x1f:
                return doAction(raw, ActionEnum::IGNORE);
            case 0x1b: // ESC
                return doTransition(raw, StateEnum::ESC, ActionEnum::CLEAR_INT);
            case 0x20 ... 0x2f: // [' ' - '\']
                return doTransition(raw, StateEnum::DCS_INT, ActionEnum::DCS_COLLECT);
            case 0x30 ... 0x39: // ['0' - '9']
                return doAction(raw, ActionEnum::PARAM);
            case 0x3a: // ':'
                return doAction(raw, ActionEnum::FINISH_SUBPARAM);
            case 0x3b: // ';'
                return doAction(raw, ActionEnum::FINISH_PARAM);
            case 0x3c ... 0x3f: // ['<' - '?']
                return doTransitionNoAction(raw, StateEnum::DCS_IGNORE);
            case 0x40 ... 0x7e: // ['@' - '~']
                return doTransition(raw, StateEnum::DCS_PASS, ActionEnum::DCS_CONSUME);
            case 0x9c: // ST
                return doTransition(raw, StateEnum::GROUND, ActionEnum::IGNORE);
            }
            return doTransition(raw, StateEnum::DCS_PASS, ActionEnum::DCS_CONSUME);

        case StateEnum::DCS_INT:
            switch (raw)
            {
            case 0x00 ... 0x1a: // C0 \ { ESC }
            case 0x1c ... 0x1f:
                    return doAction(raw, ActionEnum::IGNORE);
            case 0x1b: // ESC
                    return doTransition(raw, StateEnum::ESC, ActionEnum::CLEAR_INT);
            case 0x20 ... 0x2f: // [' ' - '\']
                    return doAction(raw, ActionEnum::DCS_COLLECT);
            case 0x30 ... 0x3f: // ['0' - '?']
                    return doTransitionNoAction(raw, StateEnum::DCS_IGNORE);
            case 0x40 ... 0x7e: // ['@' - '~']
                    return doTransition(raw, StateEnum::DCS_PASS, ActionEnum::DCS_CONSUME);
            case 0x9c: // ST
                    return doTransition(raw, StateEnum::GROUND, ActionEnum::IGNORE);
            }
            return doTransition(raw, StateEnum::DCS_PASS, ActionEnum::DCS_CONSUME);

        case StateEnum::DCS_PASS:
            switch (raw)
            {
            case 0x00 ... 0x1a: // ASCII \ { ESC }
            case 0x1c ... 0x7f:
                return doAction(raw, ActionEnum::DCS_COLLECT);
            case 0x1b: // ESC
                return doTransitionNoAction(raw, StateEnum::DCS_PASS_ESC);
            case 0x9c: // ST
                return doTransition(raw, StateEnum::GROUND, ActionEnum::DCS_DISPATCH);
            }
            return doAction(raw, ActionEnum::DCS_COLLECT);

        case StateEnum::DCS_IGNORE:
            switch (raw)
            {
            case 0x00 ... 0x1a: // ASCII \ { ESC }
            case 0x1c ... 0x7f:
                    return doNop(raw);
            case 0x1b: // ESC
                    return doTransition(raw, StateEnum::ESC, ActionEnum::CLEAR_INT);
            case 0x9c: // ST
                    return doTransitionNoAction(raw, StateEnum::GROUND);
            }
            return doNop(raw);

        case StateEnum::OSC_STRING:
            switch (raw)
            {
            case 0x00 ... 0x06: // C0 \ { BEL, ESC }
            case 0x08 ... 0x1a:
            case 0x1c ... 0x1f:
                return doNop(raw);
            case 0x1b: // ESC
                return doTransitionNoAction(raw, StateEnum::OSC_STRING_ESC);
            case 0x20 ... 0x7f: // [' ' - DEL]
                return doAction(raw, ActionEnum::OSC_COLLECT);
            case 0x07: // BEL
            case 0x9c: // ST
                return doTransition(raw, StateEnum::GROUND, ActionEnum::OSC_DISPATCH);
            }
            return doAction(raw, ActionEnum::OSC_COLLECT);

        case StateEnum::ST_IGNORE:
            switch (raw)
            {
            case 0x00 ... 0x1a: // ASCII \ { ESC }
            case 0x1c ... 0x7f:
                    return doNop(raw);
            case 0x1b: // ESC
                    return doTransition(raw, StateEnum::ESC, ActionEnum::CLEAR_INT);
            case 0x9c: // ST
                    return doTransition(raw, StateEnum::GROUND, ActionEnum::IGNORE);
            }
            return doNop(raw);

        case StateEnum::SCI:
            switch (raw)
            {
            case 0x1b: // ESC
                return doTransition(raw, StateEnum::ESC, ActionEnum::CLEAR_INT);
            case 0x08 ... 0x0d: // BS, HT, LF, VT, FF, CR
            case 0x20 ... 0x7e: // [' ' - '~']
                return doTransition(raw, StateEnum::GROUND, ActionEnum::SCI_DISPATCH);
            }
            return doTransition(raw, StateEnum::GROUND, ActionEnum::IGNORE);
        }

        // Should never reach this!
        return SeqEnum::NONE;

}

void Parser::setDispatchUnripe(bool enable)
{
    m_dispatchUnripe = enable;
}

void Parser::ignoreUntilStringTerminated()
{
    switch (m_state)
    {
    case StateEnum::DCS_PASS:
        doTransitionNoAction(0, StateEnum::DCS_IGNORE);
        break;
    default:
        // Should never ever reach here.
        // Maybe add assert!
        break;
    }
}

