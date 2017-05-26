#pragma once

#include "Application/WindowedApplication.h"

//pablito
#include "LicenseManager.h"
#include "VideoCardManager.h"


namespace bv {

namespace model
{
    class BasicNode;
    class ModelScene;
}

class Renderer;
class SceneNode;
class Camera;
class MockFrameReader;
class ProcessManager;

class BVAppLogic;

class BlackVisionApp : public WindowedApplication
{
protected:

    BVAppLogic *                m_app;

public:

    static bool			m_sWindowedApplicationInitialized;

public:

    static void			MainInitializer		    ( int argc, char * argv[] );
    static void			LoggerInitializer		( int argc, char * argv[] );
    static bool			RegisterInitializer		();
    static void         RegisterLogicInitializers   ();

public:

                    BlackVisionApp				();
                    ~BlackVisionApp				();

    virtual void    OnKey						( unsigned char c ) override;
    virtual void    OnMouse                     ( MouseAction action, int posX, int posY ) override;
    virtual void    OnPreidle					() override;
    virtual bool    OnIdle						() override;
    virtual void    OnPreMainLoop				() override;
    virtual bool    OnInitialize				() override;
    virtual void    OnTerminate					() override;
    virtual void	OnResize		            ( int w, int h ) override;

protected:

    void            InitializeConsole			();
    void            InitializeProfiling         ();
    void            InitializeAppLogic			();
    void            DeinitializeAppLogic        ();
    void            InitializeSelfState			();
    // pablito
    bool			InitializeLicenses          ();

    void            PostFrame                   ();

private:
    virtual BVAppLogic *    CreateAppLogic      ( bv::Renderer * renderer, audio::AudioRenderer * audioRenderer ) const;
};

} //bv
