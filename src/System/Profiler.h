#pragma once

#include <sys/timeb.h>

#include <iostream>


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