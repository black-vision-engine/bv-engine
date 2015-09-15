#pragma once

#include "Interfaces/IBasicLogic.h"
#include "Application/WindowedApplication.h"

namespace bv {

class BasicWindowApp : public WindowedApplication
{
private:
	static	ApplicationBase *	AppInstance;

    IBasicLogic *				m_appLogic;

public:

    static void			StaticInitializer	();
    static bool			RegisterInitializer ( IBasicLogic * appLogic, const char * title, int width, int height, bool fullScreen = false, int xOffset = 0, int yOffset = 0 );

	static bool			InitializeConsole	( const char * title );

protected:

    explicit        BasicWindowApp		( IBasicLogic * appLogic, const char * title, int xOffset, int yOffset, int width, int height, bool fullScreen );
                    ~BasicWindowApp		();

    virtual void    OnKey               ( unsigned char c ) override;
    virtual void    OnPreidle	        ()					override;
    virtual void    OnIdle              ()					override;
    virtual void    OnPreMainLoop       ()					override;
    virtual bool    OnInitialize        ()					override;
    virtual void    OnResize            ( int w, int h )	override;
    virtual void    OnTerminate         ()					override;

};

} // bv