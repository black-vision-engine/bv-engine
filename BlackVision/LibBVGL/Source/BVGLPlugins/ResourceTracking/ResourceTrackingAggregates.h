#pragma once

namespace bv {

class AggregateSizeTracker
{
private:

    size_t  m_totalSize;

public:

    AggregateSizeTracker    ();

    void    Reset           ();

    size_t  GetTotalSize    () const;
    void    AddToTotalSize  ( size_t chunkSize );

};

} //bv
