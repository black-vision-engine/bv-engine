#pragma once

#include "Interfaces/IBasicLogic.h"
#include "Application/WindowedApplication.h"

namespace bv {

class BasicWindowApp : public WindowedApplication
{
public:
	typedef IBasicLogicUnqPtr ( * CreateLogicFunc )( bv::Renderer * );

private:
	static	ApplicationBase *	AppInstance;

    IBasicLogicUnqPtr			m_appLogic;
	CreateLogicFunc				CreateLogic;

public:

    static bool			RegisterInitializer			( CreateLogicFunc logicFunc, const char * title, int width, int height, bool fullScreen = false, int xOffset = 0, int yOffset = 0 );
	static bool			RegisterConsoleInitializer	();

protected:

    explicit        BasicWindowApp		( CreateLogicFunc logicFunc, const char * title, int xOffset, int yOffset, int width, int height, bool fullScreen );
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