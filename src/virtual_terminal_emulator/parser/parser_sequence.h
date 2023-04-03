#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "parser_enums.h"
#include "sequence_argument.h"

namespace VTE
{
namespace Parser
{

class Sequence
{
public:
    SeqEnum type = SeqEnum::NONE;
    CommandEnum command = CommandEnum::NONE;
    uint32_t terminator = 0;
    unsigned int intermediates = 0;
    unsigned int nIntermediates = 0;
    unsigned int charset = 0;
    unsigned int nArgs = 0;
    unsigned int nFinalArgs = 0;
    std::vector<SequenceArgument> args;
    std::string argStr;
    uint32_t introducer = 0;
    uint32_t st = 0;

    Sequence();

    bool argStrPush(uint32_t c);

    CommandEnum hostEscape2Command(unsigned int& charset);
    CommandEnum hostCSI2Command();
    CommandEnum hostSCI2Command();
    CommandEnum hostControl2Command();
    CommandEnum hostDCS2Command(unsigned int& flags);
};

} // namespace Parser
} // namespace VTE
