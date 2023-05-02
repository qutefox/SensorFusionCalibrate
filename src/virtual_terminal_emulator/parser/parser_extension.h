#pragma once

#include <functional>
#include <string>

namespace vte
{

/** VT parser protocol extension.
* Used to implemented sub-parsers.
*/
class ParserExtension
{
  public:
    virtual ~ParserExtension() = default;

    virtual void pass(char ch) = 0;
    virtual void finalize() = 0;
};

class SimpleStringCollector: public ParserExtension
{
  public:
    explicit SimpleStringCollector(std::function<void(std::string_view)> doneFunc)
        : m_doneFunc { std::move(doneFunc) }
    { }

    void pass(char ch) override { m_data.push_back(ch); }

    void finalize() override
    {
        if (m_doneFunc)
        {
            m_doneFunc(m_data);
        }
        m_doneFunc = {};
    }

  private:
    std::string m_data;
    std::function<void(std::string_view)> m_doneFunc;
};

} // namespace vte