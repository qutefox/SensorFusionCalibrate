#pragma once

#include <memory>

#include "parser/parser_events_interface.h"
#include "parser/parser_extension.h"
#include "sequence.h"
#include "sequence_handler_interface.h"
#include "sequence_parameters_builder.h"

namespace vte
{

    class Sequencer : public vte::parser::IParserEvents
    {
    public:
        explicit Sequencer(ISequenceHandler& seqHandler);

        void error(std::string_view const&) override;
        void print(char32_t) override;
        void print(std::string_view, size_t) override;

        [[nodiscard]] std::size_t maxBulkTextSequenceWidth() const noexcept override;
        void execute(char) override;
        void clear() override;
        void collect(char) override;
        void collectLeader(char) override;
        void param(char) override;
        void paramDigit(char) override;
        void paramSeparator() override;
        void paramSubSeparator() override;
        void dispatchESC(char) override;
        void dispatchCSI(char) override;
        void startOSC() override;
        void putOSC(char) override;
        void dispatchOSC() override;
        void hook(char) override;
        void put(char) override;
        void unhook() override;

        void startAPC() override {}
        void putAPC(char) override {}
        void dispatchAPC() override {}
        void startPM() override {}
        void putPM(char) override {}
        void dispatchPM() override {}

    private:
        void handleSequence();

        Sequence m_sequence {};
        ISequenceHandler& m_seqHandler;
        SequenceParameterBuilder m_parameterBuilder;
        std::unique_ptr<ParserExtension> m_hookedParser;
    };

} // namespace vte
