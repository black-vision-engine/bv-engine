#pragma once

#include "bvApp.h"




namespace bv
{

class Renderer;


// ***********************
//
class BlackVisionAppFramework : public BlackVisionApp
{
private:
public:

    static void			MainFrameworkInitializer        ( int argc, char * argv[] );
    static bool			RegisterFrameworkInitializer    ();

public:
private:
    virtual BVAppLogic *    CreateAppLogic      ( bv::Renderer * renderer, audio::AudioRenderer * audioRenderer ) const;

};


}	// bv
