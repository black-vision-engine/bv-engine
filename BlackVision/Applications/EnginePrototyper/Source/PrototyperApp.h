#pragma once

#include "Interfaces/IAppLogicPrototype.h"
#include "Application/WindowedApplication.h"

namespace bv {

class PrototyperApp : public WindowedApplication
{
private:

    IAppLogicPrototype *    m_appLogicPrototype;

public:

    static bool			m_sWindowedApplicationInitialized;

public:

    static void			StaticInitializer	();
    static bool			RegisterInitializer ();

public:

    explicit        PrototyperApp		();
                    ~PrototyperApp	    ();

    virtual void    OnKey               ( unsigned char c ) override;
    virtual void    OnPreidle	        ()  override;
    virtual void    OnIdle              ()  override;
    virtual void    OnPreMainLoop       ()  override;
    virtual bool    OnInitialize        ()  override;
    virtual void    OnTerminate         ()  override;

};

} // bv