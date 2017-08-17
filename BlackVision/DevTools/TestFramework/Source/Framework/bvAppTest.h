#pragma once

#include "bvApp.h"


#include <string>

namespace bv
{

class Renderer;


// ***********************
//
class BlackVisionAppFramework : public BlackVisionApp
{
public:

    static bool         m_configOverrideInitialized;

private:

    std::string         m_testName;
    bool                m_reportToConsole;

public:

    static void			MainFrameworkInitializer        ( int argc, char * argv[] );
    static void         RegisterTestParams              ( int argc, char * argv[] );
    static bool			RegisterFrameworkInitializer    ();

    void                ParseTestParameters             ( int argc, char * argv[] );

protected:

    virtual void        PostFrame                       () override;

public:

    static bool         OverrideConfig                  ( const std::string & configPath );

private:
    virtual BVAppLogic *    CreateAppLogic      ( bv::Renderer * renderer, audio::AudioRenderer * audioRenderer ) const;

};


}	// bv
