#pragma once

#include <iostream>

#include <sys/timeb.h>

namespace bv
{

class Profiler
{
private:
    std::string     m_name;
    __int64         m_startTimeMicro;
    std::ostream*   m_pOutput;

public:
    explicit Profiler(const std::string& name = "", std::ostream* output = nullptr);
    ~Profiler();
};

} // bv