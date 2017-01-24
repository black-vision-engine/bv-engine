#pragma once

#include <string>

// FIXME: this is so much of a hack
#include "win_sock.h"

#include "CoreDEF.h"


namespace bv {

class SharedMemoryBuffer;

DEFINE_PTR_TYPE( SharedMemoryBuffer )
DEFINE_CONST_PTR_TYPE( SharedMemoryBuffer )

class SharedMemoryBuffer
{

private:

    UInt32          m_bufferSize;

    HANDLE          m_hMapFile;

    LPCTSTR         m_pBuf;

private:

                                    SharedMemoryBuffer  ( HANDLE hMapFile, LPTSTR pBuf, UInt32 bufferSize );
public:

                                    ~SharedMemoryBuffer ();

    UInt32                          GetBufferSize       () const;

    void                            Write               ( const char * data, UInt32 size );

public:

    static  SharedMemoryBufferPtr   Create              ( const std::wstring & name, UInt32 bufferSize );

};

} //bv