#pragma once


class GLUtils
{
public:

                GLUtils             ();

    static int  CheckForOpenGLError ( const char *, int );
    static void DumpGLInfo          ( bool dumpExtensions = false );
    static void DumpGLInfoCaps      ();

};
