#pragma once


namespace bv
{

class InitSubsystem
{
public:

	typedef void (*Initializer)(void);

    static void AddInitializer		( Initializer fun );
    static void CallInitializers	();

private:

	enum
    {
        MAX_ELEMENTS = 512
    };

    static int			m_sNumInitializers;
    static Initializer	m_sInitializers[ MAX_ELEMENTS ];

};

}
