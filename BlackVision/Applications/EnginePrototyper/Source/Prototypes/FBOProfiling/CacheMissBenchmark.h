#pragma once

#include <random>


namespace bv {

class CacheMissBenchmark
{
private:

    unsigned int m_dataSize;
    unsigned int m_numAccesses;
    unsigned int m_entrySize;
    unsigned int m_entryTries;

public:

            CacheMissBenchmark  ( unsigned int dataSize, unsigned int numAccesses, unsigned int entrySize, unsigned int entryTries );

    void    RunBenchmark        ( unsigned int numIterations ) const;

private:

    void    SinglePass          ( char * data, char * res, std::uniform_int_distribution<> & dis, std::mt19937 & gen ) const;

};

} //bv
