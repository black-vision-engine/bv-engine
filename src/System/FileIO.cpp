#include "FileIO.h"

#include <sys/stat.h>

#include <fstream>


namespace bv
{

class FileImpl
{
private:

    std::fstream *      m_fileHandle;
    std::string         m_fileName;

    FileImpl            ( const std::string & fileName );

public:

    int                 Read        ( std::ostream & out) const;
    int                 Read        ( std::ostream & out, long numBytes ) const;
    int                 Read        ( char* out, long numBytes ) const;

    bool                Write       ( std::istream & in );
    bool                Write       ( std::istream & in , long numBytes );
    void                Write       ( const char* in , long numBytes);

    void                Close       ();

    virtual             ~FileImpl   ();

    static bool         Exists      ( const std::string & fileName );
    static FileImpl *   Open        ( const std::string & fileName, File::OpenMode openMode );
    static int          Read        ( std::ostream & out, const std::string & fileName );
    static int          Read        ( char* out, const std::string & fileName );
    static int          Write       ( std::istream & in, const std::string & fileName );
    static int          Size        ( const std::string & fileName );
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
int         FileImpl::Read        ( std::ostream & out) const
{
    return Read( out, Size( m_fileName ) );
}

// *******************************
//
int         FileImpl::Read        ( std::ostream & out, long numBytes ) const
{
    int bytesRead = 0;
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
int         FileImpl::Read        ( char* out, long numBytes ) const
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
bool        FileImpl::Write       ( std::istream & in , long numBytes)
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
void        FileImpl::Write       ( const char* in , long numBytes)
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
int         FileImpl::Size      ( const std::string & fileName )
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
FileImpl*   FileImpl::Open        ( const std::string & fileName, File::OpenMode openMode )
{
    FileImpl* impl = new FileImpl( fileName );

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
int         FileImpl::Read        ( std::ostream & out, const std::string & fileName )
{
    auto f = Open( fileName, File::FOMReadOnly );
    return f->Read( out );
}

// *******************************
//
int         FileImpl::Read        ( char* out, const std::string & fileName )
{
    auto f = Open( fileName, File::FOMReadOnly );
    return f->Read( out, FileImpl::Size( fileName ) );
}

// *******************************
//
int         FileImpl::Write       ( std::istream & in, const std::string & fileName )
{
    auto f = Open( fileName, File::FOMReadWrite );
    return f->Write( in );
}

// *******************************
//
File::File( FileImpl* impl )
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
int         File::Read        ( std::ostream & out) const
{
    return m_impl->Read( out );
}

// *******************************
//
int         File::Read        ( std::ostream & out, int numBytes ) const
{
    return m_impl->Read( out, numBytes );
}

// *******************************
//
int         File::Write       ( std::istream & in )
{
    return m_impl->Write( in );
}

// *******************************
//
int         File::Write       ( std::istream & in , int numBytes )
{
    return m_impl->Write( in, numBytes );
}

// *******************************
//
void         File::Write       ( const char* in , int numBytes )
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
void        File::operator << ( std::istream& in )
{
    m_impl->Write( in );
}

// *******************************
//
void        File::operator >> ( std::ostream& out )
{
    m_impl->Read( out );
}

// *******************************
//
int         File::Size        ( const std::string & fileName )
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
int         File::Read        ( std::ostream & out, const std::string & fileName )
{
    return FileImpl::Read( out, fileName );
}

// *******************************
//
int         File::Read        ( char* out, const std::string & fileName )
{
    return FileImpl::Read( out, fileName );
}

// *******************************
//
int         File::Write       ( std::istream & in, const std::string & fileName )
{
    return FileImpl::Write( in, fileName );
}

} //bv
