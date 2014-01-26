#pragma once

#include <string>
#include <iostream>


namespace bv
{

class FileImpl;

class File
{
private:
    FileImpl*           m_impl;

    explicit            File( FileImpl* impl );

public:

    enum OpenMode
    {
        FOMReadOnly,
        FOMReadWrite
    };

    int                 Read        ( std::ostream & out) const;
    int                 Read        ( std::ostream & out, int numBytes ) const;

    int                 Write       ( std::istream & in );
    int                 Write       ( std::istream & in , int numBytes );

    void                Close       ();

    void                operator << ( std::istream& );
    void                operator >> ( std::ostream& );


    virtual             ~File       ();

    static bool         Exists      ( const std::string & fileName );
    static File         Open        ( const std::string & fileName, OpenMode openMode = FOMReadOnly );
    static int          Read        ( std::ostream & out, const std::string & fileName );
    static int          Read        ( char* out, const std::string & fileName );
    static int          Write       ( std::istream & in, const std::string & fileName );
    static int          Size        ( const std::string & fileName );

    friend class FileImpl;
};


} //bv
