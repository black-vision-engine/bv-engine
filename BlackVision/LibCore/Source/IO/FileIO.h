#pragma once

#include <string>
#include <iostream>

#include "CoreDEF.h"


namespace bv
{

class File
{
private:

    class Impl;

    std::shared_ptr< Impl >  m_impl;

    explicit            File        ( Impl * impl );

public:

    enum OpenMode
    {
        FOMReadOnly,
        FOMReadWrite,
		FOMWriteAppend,
    };

    File                            ( const File & copy );
    const File &        operator=   ( const File & copy );

    SizeType            Read        ( std::ostream & out) const;
    SizeType            Read        ( std::ostream & out, SizeType numBytes ) const;

    SizeType            Write       ( std::istream & in ) const;
    SizeType            Write       ( std::istream & in , SizeType numBytes ) const;
    
    void                Write       ( const char * in , SizeType numBytes ) const;
	void                Write       ( const std::string & str ) const;

    void                Close       () const;

    void                operator << ( std::istream & ) const;
    void                operator >> ( std::ostream & ) const;

    bool                Good        () const;

	std::fstream *		StreamBuf	() const;

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
	
    static std::string  GetFileName ( const std::string & path, bool withExt = false );
    
    static std::string  GetExtension( const std::string & path );
    
    static UInt64       GetSize     ( const std::string & path );
};

} //bv
