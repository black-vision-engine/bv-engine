#pragma once

#include <string>
#include <iostream>

#include "CoreDEF.h"


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
        FOMReadWrite,
		FOMWriteAppend,
    };

    SizeType            Read        ( std::ostream & out) const;
    SizeType            Read        ( std::ostream & out, SizeType numBytes ) const;

    SizeType            Write       ( std::istream & in );
    SizeType            Write       ( std::istream & in , SizeType numBytes );
    void                Write       ( const char * in , SizeType numBytes );
	void                Write       ( const std::string & str );

    void                Close       ();

    void                operator << ( std::istream & );
    void                operator >> ( std::ostream & );

	std::fstream *		StreamBuf	();

    virtual             ~File       ();

    static void         Touch       ( const std::string & fileName );
    static File         Open        ( const std::string & fileName, OpenMode openMode = FOMReadOnly );
	static File         OpenTmp     ( std::string * name = nullptr );
    static SizeType     Read        ( std::ostream & out, const std::string & fileName );
    static SizeType     Read        ( char * out, const std::string & fileName );
	static SizeType		Write       ( const char * in, SizeType size, const std::string & fileName, bool append );
    static SizeType     Write       ( std::istream & in, const std::string & fileName );
	static SizeType     Size        ( const std::string & fileName );
    static std::string  GetAbsolutPath( const std::string & fileName );
    static std::string  GetDirName  ( const std::string & path );
	static std::string  GetFileName ( const std::string & path );

    friend class FileImpl;
};


} //bv
