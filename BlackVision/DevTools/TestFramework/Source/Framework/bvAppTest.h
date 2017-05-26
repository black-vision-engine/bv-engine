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

    virtual bool    OnInitialize				() override;


private:
    virtual BVAppLogic *    CreateAppLogic      ( bv::Renderer * renderer, audio::AudioRenderer * audioRenderer ) const;

};


}	// bv
