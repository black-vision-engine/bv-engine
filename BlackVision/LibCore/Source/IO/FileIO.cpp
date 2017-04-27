#include "stdafxCore.h"

#include "FileIO.h"
#include "DirIO.h"
#include "System/Path.h"
#include <sys/stat.h>

#include "boost/filesystem/path.hpp"
#include "boost/filesystem.hpp"

#include <fstream>
#include <istream>
#include <iostream>
#include <string>

namespace bv
{

class File::Impl
{
private:

    std::fstream *      m_fileHandle;
    std::string         m_fileName;

    Impl                ( const std::string & fileName );

public:

    SizeType            Read        ( std::ostream & out) const;
    SizeType            Read        ( std::ostream & out, SizeType numBytes ) const;
    SizeType            Read        ( char * out, SizeType numBytes ) const;

    bool                Write       ( std::istream & in );
    bool                Write       ( std::istream & in , SizeType numBytes );
    void                Write       ( const char * in , SizeType numBytes );
	void                Write       ( const std::string & str );

    bool                Good        () const;

	std::fstream *		StreamBuf	();

    void                Close       ();

    virtual             ~Impl       ();

    static bool         Exists      ( const std::string & fileName );
    static Impl *       Open        ( const std::string & fileName, File::OpenMode openMode );
	static Impl *	    OpenTmp     ( std::string * name );
    static SizeType     Read        ( std::ostream & out, const std::string & fileName );
    static SizeType     Read        ( char* out, const std::string & fileName );
    static SizeType     Write       ( std::istream & in, const std::string & fileName );
	static SizeType		Write       ( const char * in, SizeType size, const std::string & fileName, bool append );
    static SizeType     Size        ( const std::string & fileName );
};

// *******************************
//
File::Impl::Impl   ( const std::string & fileName )
    : m_fileHandle( nullptr )
    , m_fileName( fileName )
{}

// *******************************
//
File::Impl::~Impl   ()
{
    delete m_fileHandle;
}

// *******************************
//
SizeType    File::Impl::Read        ( std::ostream & out ) const
{
    return Read( out, Size( m_fileName ) );
}

// *******************************
//
SizeType    File::Impl::Read        ( std::ostream & out, SizeType numBytes ) const
{
    SizeType bytesRead = 0;

    while( m_fileHandle->good() && numBytes > 0 )
    {
        out << (char)m_fileHandle->get();

        bytesRead++;
        numBytes--;
    }

    return bytesRead;
}

// *******************************
//
SizeType    File::Impl::Read        ( char * out, SizeType numBytes ) const
{
    if( m_fileHandle->good() && numBytes > 0 )
    {
        m_fileHandle->read( out, numBytes );
    }

    return numBytes;
}

// *******************************
//
bool        File::Impl::Write       ( std::istream & in )
{
    *m_fileHandle << in.rdbuf();
    return in.cur == in.end;
}

// *******************************
//
bool        File::Impl::Write       ( std::istream & in , SizeType numBytes)
{
    int bytesWritten = 0;
    char c;
    while( m_fileHandle->good() && numBytes > 0 )
    {
        in.get(c);

        *m_fileHandle << c;

        bytesWritten++;
        numBytes--;
    }

    return numBytes == 0;
}

// *******************************
//
void        File::Impl::Write       ( const char * in , SizeType numBytes)
{
    m_fileHandle->write( in, numBytes );
}

// *******************************
//
std::fstream *	File::Impl::StreamBuf()
{
	return m_fileHandle;
}

// *******************************
//
void        File::Impl::Write       ( const std::string & str )
{
	m_fileHandle->write( str.c_str(), str.length() );
}

// *******************************
//
void        File::Impl::Close       ()
{
    m_fileHandle->close();
}

// *******************************
//
SizeType         File::Impl::Size      ( const std::string & fileName )
{
    struct stat info;
    int ret = -1;

    ret = stat( fileName.c_str(), &info );

    return info.st_size;
}

// *******************************
//
bool        File::Impl::Exists      ( const std::string & fileName )
{
    struct stat info;
    int ret = -1;

    ret = stat( fileName.c_str(), &info );
    return 0 == ret;
}

// *******************************
//
bool        File::Impl::Good        () const
{
    return m_fileHandle->good();
}

// *******************************
//
File::Impl *  File::Impl::Open        ( const std::string & fileName, File::OpenMode openMode )
{
    File::Impl * impl = new File::Impl( fileName );

    auto parent = boost::filesystem::path( fileName ).parent_path();

    if( !parent.empty() && !boost::filesystem::exists( parent ) )
    {
        Dir::CreateDir( parent.string(), true );
    }

    if( openMode == File::FOMReadOnly )
        impl->m_fileHandle = new std::fstream( fileName, std::ios::in | std::ios::binary );
    else if ( openMode == File::FOMReadWrite )
        impl->m_fileHandle = new std::fstream( fileName, std::ios::out | std::ios::binary );
	else if( openMode == File::FOMWriteAppend )
		impl->m_fileHandle = new std::fstream( fileName, std::ios::app | std::ios::binary );

    if( impl->m_fileHandle->good() )
        return impl;
    else
    {
        std::cerr << "Cannot open file: " << fileName << std::endl;
        return impl;
    }
}

// *******************************
//
File::Impl *	File::Impl::OpenTmp     ( std::string * name )
{
	auto p = boost::filesystem::unique_path();

	if( name )
	{
		*name = p.string();
	}

	return Open( p.string(), File::OpenMode::FOMReadWrite );
}

// *******************************
//
SizeType    File::Impl::Read        ( std::ostream & out, const std::string & fileName )
{
    auto f = Open( fileName, File::FOMReadOnly );
    auto ret = f->Read( out );
    f->Close();
    delete f;

    return ret;
}

// *******************************
//
SizeType    File::Impl::Read        ( char * out, const std::string & fileName )
{
    auto f = Open( fileName, File::FOMReadOnly );
    auto ret = f->Read( out, File::Impl::Size( fileName ) );
    f->Close();
    delete f;

    return ret;
}

// *******************************
//
SizeType    File::Impl::Write       ( std::istream & in, const std::string & fileName )
{
    auto f = Open( fileName, File::FOMReadWrite );
    auto ret = f->Write( in );
    f->Close();
    delete f;

    return ret;
}

// *******************************
//
SizeType    File::Impl::Write       ( const char * in, SizeType size, const std::string & fileName, bool append )
{
    File::Impl * f = nullptr;
	
	if( append )
	{
		f = File::Impl::Open( fileName, File::OpenMode::FOMWriteAppend );
	}
	else
	{
		f = File::Impl::Open( fileName, File::OpenMode::FOMReadWrite );
	}

	f->Write( in, size );

	f->Close();

    delete f;

	return size;
}

// *******************************
//
File::File( Impl * impl )
    : m_impl( impl )
{}

// *******************************
//
File::~File()
{}

// *******************************
//
File::File( const File & copy )
    : m_impl( copy.m_impl )
{}

// *******************************
//
const File &        File::operator=   ( const File & copy )
{
    m_impl = copy.m_impl;
    return *this;
}

// *******************************
//
SizeType    File::Read        ( std::ostream & out ) const
{
    return m_impl->Read( out );
}

// *******************************
//
SizeType    File::Read        ( std::ostream & out, SizeType numBytes ) const
{
    return m_impl->Read( out, numBytes );
}

// *******************************
//
SizeType    File::Write       ( std::istream & in ) const
{
    return m_impl->Write( in );
}

// *******************************
//
SizeType    File::Write       ( std::istream & in , SizeType numBytes ) const
{
    return m_impl->Write( in, numBytes );
}

// *******************************
//
void         File::Write       ( const char * in , SizeType numBytes ) const
{
    m_impl->Write( in, numBytes );
}

// *******************************
//
void		 File::Write       ( const std::string & str ) const
{
	m_impl->Write( str );
}

// *******************************
//
void        File::Close       () const
{
    return m_impl->Close();
}

// *******************************
//
void        File::operator << ( std::istream & in ) const
{
    m_impl->Write( in );
}

// *******************************
//
void        File::operator >> ( std::ostream & out ) const
{
    m_impl->Read( out );
}

// *******************************
//
std::fstream *	File::StreamBuf	() const
{
	return m_impl->StreamBuf();
}

// *******************************
//
bool             File::Good      () const
{
    return m_impl->Good();
}

// *******************************
//
SizeType    File::Size        ( const std::string & fileName )
{
    return File::Impl::Size( fileName );
}

// *******************************
//
File        File::Open        ( const std::string & fileName, OpenMode openMode )
{
    return File( File::Impl::Open( fileName, openMode ) );
}

// *******************************
//
File         File::OpenTmp     ( std::string * name )
{
	return File( File::Impl::OpenTmp( name ) );
}

// *******************************
//
SizeType    File::Read        ( std::ostream & out, const std::string & fileName )
{
    return File::Impl::Read( out, fileName );
}

// *******************************
//
SizeType    File::Read        ( char* out, const std::string & fileName )
{
    return File::Impl::Read( out, fileName );
}

// *******************************
//
SizeType    File::Write       ( std::istream & in, const std::string & fileName )
{
    return File::Impl::Write( in, fileName );
}


// *******************************
//
SizeType    File::Write       ( const char * in, SizeType size, const std::string & fileName, bool append )
{
	return File::Impl::Write( in, size, fileName, append );
}

// *******************************
//
std::string  File::GetAbsolutPath( const std::string & fileName )
{
    return boost::filesystem::absolute( fileName ).string();
}

// *******************************
//
std::string  File::GetDirName  ( const std::string & path )
{
    boost::filesystem::path p( path );
    return p.parent_path().string();
}

// *******************************
//
std::string  File::GetFileName ( const std::string & path, bool withExt )
{
	boost::filesystem::path p( path );

    if( !withExt )
    {
        return p.stem().string();
    }
    else
    {
        return p.filename().string();
    }
}

// *******************************
//
std::string  File::GetExtension( const std::string & path )
{
    auto ext = boost::filesystem::extension( path );
    return ext;
}

// *******************************
//
void         File::Touch       ( const std::string & fileName )
{
    if( !Path::Exists( fileName ) )
    {
        auto f = File::Open( fileName, OpenMode::FOMReadWrite );
        f.Close();
    }
}

// *******************************
//
UInt64       File::GetSize     ( const std::string & path )
{
    return boost::filesystem::file_size( path );
}

} //bv
