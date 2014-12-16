#include "FileIO.h"

#include <sys/stat.h>

#include <fstream>

#include "boost/filesystem/path.hpp"
#include "boost/filesystem.hpp"


namespace bv
{

class FileImpl
{
private:

    std::fstream *      m_fileHandle;
    std::string         m_fileName;

    FileImpl            ( const std::string & fileName );

public:

    SizeType            Read        ( std::ostream & out) const;
    SizeType            Read        ( std::ostream & out, SizeType numBytes ) const;
    SizeType            Read        ( char * out, SizeType numBytes ) const;

    bool                Write       ( std::istream & in );
    bool                Write       ( std::istream & in , SizeType numBytes );
    void                Write       ( const char * in , SizeType numBytes );

    void                Close       ();

    virtual             ~FileImpl   ();

    static bool         Exists      ( const std::string & fileName );
    static FileImpl *   Open        ( const std::string & fileName, File::OpenMode openMode );
    static SizeType     Read        ( std::ostream & out, const std::string & fileName );
    static SizeType     Read        ( char* out, const std::string & fileName );
    static SizeType     Write       ( std::istream & in, const std::string & fileName );
    static SizeType     Size        ( const std::string & fileName );
};

// *******************************
//
FileImpl::FileImpl   ( const std::string & fileName )
    : m_fileHandle( nullptr )
    , m_fileName( fileName )
{}

// *******************************
//
FileImpl::~FileImpl   ()
{
    delete m_fileHandle;
}

// *******************************
//
SizeType    FileImpl::Read        ( std::ostream & out ) const
{
    return Read( out, Size( m_fileName ) );
}

// *******************************
//
SizeType    FileImpl::Read        ( std::ostream & out, SizeType numBytes ) const
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
SizeType    FileImpl::Read        ( char * out, SizeType numBytes ) const
{
    if( m_fileHandle->good() && numBytes > 0 )
    {
        m_fileHandle->read( out, numBytes );
    }

    return numBytes;
}

// *******************************
//
bool        FileImpl::Write       ( std::istream & in )
{
    *m_fileHandle << in;
    return in.cur == in.end;
}

// *******************************
//
bool        FileImpl::Write       ( std::istream & in , SizeType numBytes)
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
void        FileImpl::Write       ( const char * in , SizeType numBytes)
{
    m_fileHandle->write( in, numBytes );
}

// *******************************
//
void        FileImpl::Close       ()
{
    m_fileHandle->close();
}

// *******************************
//
SizeType         FileImpl::Size      ( const std::string & fileName )
{
    struct stat info;
    int ret = -1;

    ret = stat( fileName.c_str(), &info );

    return info.st_size;
}

// *******************************
//
bool        FileImpl::Exists      ( const std::string & fileName )
{
    struct stat info;
    int ret = -1;

    ret = stat( fileName.c_str(), &info );
    return 0 == ret;
}

// *******************************
//
FileImpl *  FileImpl::Open        ( const std::string & fileName, File::OpenMode openMode )
{
    FileImpl * impl = new FileImpl( fileName );

    if( openMode == File::FOMReadOnly )
        impl->m_fileHandle = new std::fstream( fileName, std::ios::in | std::ios::binary );
    else if ( openMode == File::FOMReadWrite )
        impl->m_fileHandle = new std::fstream( fileName, std::ios::out | std::ios::binary );

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
SizeType    FileImpl::Read        ( std::ostream & out, const std::string & fileName )
{
    auto f = Open( fileName, File::FOMReadOnly );
    auto ret = f->Read( out );
    f->Close();
    delete f;

    return ret;
}

// *******************************
//
SizeType    FileImpl::Read        ( char * out, const std::string & fileName )
{
    auto f = Open( fileName, File::FOMReadOnly );
    auto ret = f->Read( out, FileImpl::Size( fileName ) );
    f->Close();
    delete f;

    return ret;
}

// *******************************
//
SizeType    FileImpl::Write       ( std::istream & in, const std::string & fileName )
{
    auto f = Open( fileName, File::FOMReadWrite );
    auto ret = f->Write( in );
    f->Close();
    delete f;

    return ret;
}

// *******************************
//
File::File( FileImpl * impl )
    : m_impl( impl )
{}

// *******************************
//
File::~File()
{
    delete m_impl;
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
SizeType    File::Write       ( std::istream & in )
{
    return m_impl->Write( in );
}

// *******************************
//
SizeType    File::Write       ( std::istream & in , SizeType numBytes )
{
    return m_impl->Write( in, numBytes );
}

// *******************************
//
void         File::Write       ( const char* in , SizeType numBytes )
{
    m_impl->Write( in, numBytes );
}

// *******************************
//
void        File::Close       ()
{
    return m_impl->Close();
}

// *******************************
//
void        File::operator << ( std::istream & in )
{
    m_impl->Write( in );
}

// *******************************
//
void        File::operator >> ( std::ostream & out )
{
    m_impl->Read( out );
}

// *******************************
//
SizeType    File::Size        ( const std::string & fileName )
{
    return FileImpl::Size( fileName );
}

// *******************************
//
bool        File::Exists      ( const std::string & fileName )
{
    return FileImpl::Exists( fileName );
}

// *******************************
//
File        File::Open        ( const std::string & fileName, OpenMode openMode )
{
    return File( FileImpl::Open( fileName, openMode ) );
}

// *******************************
//
SizeType    File::Read        ( std::ostream & out, const std::string & fileName )
{
    return FileImpl::Read( out, fileName );
}

// *******************************
//
SizeType    File::Read        ( char* out, const std::string & fileName )
{
    return FileImpl::Read( out, fileName );
}

// *******************************
//
SizeType    File::Write       ( std::istream & in, const std::string & fileName )
{
    return FileImpl::Write( in, fileName );
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
bool         File::CreateDir   ( const std::string & path )
{
    return boost::filesystem::create_directory( path );
}


} //bv
