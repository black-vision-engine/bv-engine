#pragma once


namespace bv
{

class InitSubsystem
{
public:

    typedef void (*Initializer)( int argc, char * argv[] );

    static void AddInitializer		( Initializer fun );
    static void CallInitializers	( int argc, char * argv[] );

private:

    enum
    {
        MAX_ELEMENTS = 512
    };

    static int          m_sNumInitializers;
    static Initializer  m_sInitializers[ MAX_ELEMENTS ];

};

} //bv
