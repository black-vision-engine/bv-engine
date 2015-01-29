#pragma once

#include <string>
#include <iostream>

#include "System/BasicTypes.h"


namespace bv
{

class FileImpl;

class File
{
private:

    FileImpl *          m_impl;

    explicit            File( FileImpl * impl );

public:

    enum OpenMode
    {
        FOMReadOnly,
        FOMReadWrite
    };

    SizeType            Read        ( std::ostream & out) const;
    SizeType            Read        ( std::ostream & out, SizeType numBytes ) const;

    SizeType            Write       ( std::istream & in );
    SizeType            Write       ( std::istream & in , SizeType numBytes );
    void                Write       ( const char * in , SizeType numBytes );

    void                Close       ();

    void                operator << ( std::istream & );
    void                operator >> ( std::ostream & );


    virtual             ~File       ();

    static bool         Exists      ( const std::string & fileName );
    static File         Open        ( const std::string & fileName, OpenMode openMode = FOMReadOnly );
    static SizeType     Read        ( std::ostream & out, const std::string & fileName );
    static SizeType     Read        ( char * out, const std::string & fileName );
    static SizeType     Write       ( std::istream & in, const std::string & fileName );
	static SizeType     Size        ( const std::string & fileName );
    static std::string  GetAbsolutPath( const std::string & fileName );
    static std::string  GetDirName  ( const std::string & path );
    static bool         CreateDir   ( const std::string & path );

    friend class FileImpl;
};


} //bv