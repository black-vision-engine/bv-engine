#include "CacheMissBenchmark.h"

#include "Tools/HRTimer.h"
#include <iostream>
#include <iomanip>


namespace bv {
    
// *********************************
//
CacheMissBenchmark::CacheMissBenchmark  ( unsigned int dataSize, unsigned int numAccesses, unsigned int entrySize, unsigned int entryTries )
    : m_dataSize( dataSize )
    , m_numAccesses( numAccesses )
    , m_entrySize( entrySize )
    , m_entryTries( entryTries )
{
}

// *********************************
//
void    CacheMissBenchmark::RunBenchmark( unsigned int numIterations ) const
{
    std::cout << "Calling test DS: " << m_dataSize / 1024 / 1024 << "MB, numAcs: " << m_numAccesses << ", entrSz: " << m_entrySize << ", entrTr: " << m_entryTries << ", iteras: " << numIterations << std::endl;

    std::cout << "Allocating " << m_dataSize / 1024 / 1024 << "MB of memory" << std::endl;

    char * data = new char[ m_dataSize ];
    char * res = new char[ m_numAccesses * m_entrySize * m_entryTries ];

    std::cout << "Initializing memory" << std::endl;

    for( unsigned int i = 0; i < m_dataSize; ++i )
    {
        data[i] = i % 128;
    }

    unsigned int maxElts = m_dataSize / ( m_entrySize * m_entryTries );

    std::cout << "Initializing rnd generator for interval [0, " << maxElts - 1 << "]" << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, maxElts - 1);

    HighResolutionTimer timer;
    auto start = timer.CurElapsed();
    auto intermediate = start;

    for( unsigned int i = 0; i < numIterations; ++i )
    {
        // std::cout << "Single mem pass transfer: " << i << std::endl;

        SinglePass( data, res, dis, gen );
    
        double timestamp = timer.CurElapsed();
        double intElapsed = timestamp - intermediate;
        intermediate = timestamp;

        double transferRate = double( m_entrySize * m_entryTries * m_numAccesses ) / ( intElapsed * 1024.f );
        { transferRate; }

        // std::cout << "\nCURRENT TRANSFER: Transferring " << float(entrySize * entryTries * numAccesses) / 1024.f << "KB took " << intElapsed << " secs" << std::endl;
        // std::cout << "CURRENT TRANSFER: " << transferRate << " KB/s" << std::endl;
    }

    auto stop = timer.CurElapsed();

    double transferRate = float( numIterations * m_entrySize * m_entryTries * m_numAccesses ) / ( (stop - start) * 1024.f * 1024.f );

    std::cout << "\nTOTAL TRANSFER: Transferring " << float(numIterations * m_entrySize * m_entryTries * m_numAccesses) / 1024.f / 1024.f << " MB took " << (stop - start) << " secs" << std::endl;
    std::cout << "TOTAL TRANSFER: " << transferRate << " MB/s" << std::endl;

    delete[] data;
}

// *********************************
//
void    CacheMissBenchmark::SinglePass  ( char * data, char * res, std::uniform_int_distribution<> & dis, std::mt19937 & gen ) const
{
    for( unsigned int i = 0; i < m_numAccesses; ++i )
    {
        unsigned int offset = dis(gen);
        // std::cout << "    Pass: " << i << ", offset: " << std::setw(2) << offset << std::endl;

        for( unsigned int j = 0; j < m_entryTries; ++j )
        {
            memcpy(&res[ ( i * m_entryTries + j ) * m_entrySize ], &data[ ( offset * m_entryTries + j ) * m_entrySize ], m_entrySize );
        }
    }
}

} //bv
