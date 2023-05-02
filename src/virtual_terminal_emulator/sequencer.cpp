#include "sequencer.h"

#include "parser/parser.h"

#include <QDebug>

namespace vte
{

    Sequencer::Sequencer(ISequenceHandler& seqHandler)
        : m_seqHandler{ seqHandler }
        , m_parameterBuilder { m_sequence.parameters() }

    { }

    void Sequencer::error(std::string_view const& errorString)
    {
        qDebug() << "vte sequencer error: " << errorString;
    }

    void Sequencer::print(char32_t codepoint)
    {
        m_seqHandler.writeText(codepoint);
    }

    void Sequencer::print(std::string_view chars, size_t cellCount)
    {
        assert(!chars.empty());
        m_seqHandler.writeText(chars, cellCount);
    }

    void Sequencer::execute(char controlCode)
    {
        m_seqHandler.executeControlCode(controlCode);
    }

    void Sequencer::collect(char ch)
    {
        m_sequence.intermediateCharacters().push_back(ch);
    }

    void Sequencer::collectLeader(char leader)
    {
        m_sequence.setLeader(leader);
    }

    void Sequencer::param(char ch)
    {
        switch (ch)
        {
            case ';': paramSeparator(); break;
            case ':': paramSubSeparator(); break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': paramDigit(ch); break;
        }
    }

    void Sequencer::dispatchESC(char finalChar)
    {
        m_sequence.setCategory(FunctionCategory::ESC);
        m_sequence.setFinalChar(finalChar);
        handleSequence();
    }

    void Sequencer::dispatchCSI(char finalChar)
    {
        m_sequence.setCategory(FunctionCategory::CSI);
        m_sequence.setFinalChar(finalChar);
        handleSequence();
    }

    void Sequencer::startOSC()
    {
        m_sequence.setCategory(FunctionCategory::OSC);
    }

    void Sequencer::putOSC(char ch)
    {
        if (m_sequence.intermediateCharacters().size() + 1 < Sequence::MAX_OSC_LEN)
        {
            m_sequence.intermediateCharacters().push_back(ch);
        }
    }

    void Sequencer::dispatchOSC()
    {
        auto const [code, skipCount] = parser::extractCodePrefix(m_sequence.intermediateCharacters());
        m_parameterBuilder.set(static_cast<Sequence::Parameter>(code));
        m_sequence.intermediateCharacters().erase(0, skipCount);
        handleSequence();
        clear();
    }

    void Sequencer::hook(char finalChar)
    {
        m_sequence.setCategory(FunctionCategory::DCS);
        m_sequence.setFinalChar(finalChar);
        handleSequence();
    }

    void Sequencer::put(char ch)
    {
        if (m_hookedParser)
            m_hookedParser->pass(ch);
    }

    void Sequencer::unhook()
    {
        if (m_hookedParser)
        {
            m_hookedParser->finalize();
            m_hookedParser.reset();
        }
    }

    size_t Sequencer::maxBulkTextSequenceWidth() const noexcept
    {
        return std::numeric_limits<size_t>::max();
    }

    void Sequencer::handleSequence()
    {
        m_parameterBuilder.fixiate();
        m_seqHandler.processSequence(m_sequence);
    }

    inline void Sequencer::clear()
    {
        m_sequence.clearExceptParameters();
        m_parameterBuilder.reset();
    }

    inline void Sequencer::paramDigit(char ch)
    {
        m_parameterBuilder.multiplyBy10AndAdd(static_cast<uint8_t>(ch - '0'));
    }

    inline void Sequencer::paramSeparator()
    {
        m_parameterBuilder.nextParameter();
    }

    inline void Sequencer::paramSubSeparator()
    {
        m_parameterBuilder.nextSubParameter();
    }

} // namespace vte
