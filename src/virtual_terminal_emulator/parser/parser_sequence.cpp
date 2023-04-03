#include "parser_sequence.h"


using namespace VTE::Parser;

Sequence::Sequence()
{
    args.reserve(32);
    argStr.reserve(128);
}

bool Sequence::argStrPush(uint32_t c)
{
    if (argStr.size() > 4096) return false;
    argStr.push_back(c);
    return true;
}

CommandEnum Sequence::hostControl2Command()
{
    switch (terminator)
    {
    case 0x00: return CommandEnum::NUL; /* nul */
    case 0x01: return CommandEnum::SOH; /* start of heading */
    case 0x02: return CommandEnum::STX; /* start of text */
    case 0x03: return CommandEnum::ETX; /* end of text */
    case 0x04: return CommandEnum::EOT; /* end of transmission */
    case 0x05: return CommandEnum::ENQ; /* enquire */
    case 0x06: return CommandEnum::ACK; /* acknowledge */
    case 0x07: return CommandEnum::BEL; /* bell */
    case 0x08: return CommandEnum::BS; /* backspace */
    case 0x09: return CommandEnum::HT; /* horizontal tab */
    case 0x0a: return CommandEnum::LF; /* line feed */
    case 0x0b: return CommandEnum::VT; /* vertical tag */
    case 0x0c: return CommandEnum::FF; /* form feed */
    case 0x0d: return CommandEnum::CR; /* carriage return */
    case 0x0e: return CommandEnum::LS1; /* locking shift 1 */
    case 0x0f: return CommandEnum::LS0; /* locking shift 0 */
    case 0x10: return CommandEnum::DLE; /* data link escape */
    case 0x11: return CommandEnum::DC1; /* device control 1 / XON */
    case 0x12: return CommandEnum::DC2; /* device control 2 */
    case 0x13: return CommandEnum::DC3; /* device control 3 / XOFF */
    case 0x14: return CommandEnum::DC4; /* device control 4 */
    case 0x15: return CommandEnum::NAK; /* negative acknowledge */
    case 0x16: return CommandEnum::SYN; /* synchronise */
    case 0x17: return CommandEnum::ETB; /* end of transmissionblock */
    case 0x19: return CommandEnum::EM; /* end of medium */
    case 0x1a: return CommandEnum::SUB; /* substitute */
    case 0x1c: return CommandEnum::IS4; /* information separator 4 / file separator (FS) */
    case 0x1d: return CommandEnum::IS3; /* information separator 3 / group separator (GS) */
    case 0x1e: return CommandEnum::IS2; /* information separator 2 / record separator (RS) */
    case 0x1f: return CommandEnum::IS1; /* information separator 1 / unit separator (US) */
    case 0x82: return CommandEnum::BPH; /* break permitted here */
    case 0x83: return CommandEnum::NBH; /* no break permitted here */
    case 0x84: return CommandEnum::IND; /* index */
    case 0x85: return CommandEnum::NEL; /* next line */
    case 0x86: return CommandEnum::SSA; /* start of selected area */
    case 0x87: return CommandEnum::ESA; /* end of selected area */
    case 0x88: return CommandEnum::HTS; /* horizontal tab set */
    case 0x89: return CommandEnum::HTJ; /* character tabulation with justification */
    case 0x8a: return CommandEnum::VTS; /* line tabulation set */
    case 0x8b: return CommandEnum::PLD; /* partial line forward */
    case 0x8c: return CommandEnum::PLU; /* partial line backward */
    case 0x8d: return CommandEnum::RI; /* reverse index */
    case 0x8e: return CommandEnum::SS2; /* single shift 2 */
    case 0x8f: return CommandEnum::SS3; /* single shift 3 */
    case 0x91: return CommandEnum::PU1; /* private use 1 */
    case 0x92: return CommandEnum::PU2; /* private use 2 */
    case 0x93: return CommandEnum::STS; /* set transmit state */
    case 0x94: return CommandEnum::CCH; /* cancel character */
    case 0x95: return CommandEnum::MW; /* message waiting */
    case 0x96: return CommandEnum::SPA; /* start of protected area */
    case 0x97: return CommandEnum::EPA; /* end of protected area */
    case 0x9c: return CommandEnum::ST; /* string terminator */
    default:
        return CommandEnum::NONE;
    }
}
