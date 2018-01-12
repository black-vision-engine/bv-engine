#include "Framework/FrameworkTest.h"


// ***********************
//
class Engine_InputSlots_VideoInputPipeline : public bv::FrameworkTest
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( Engine_InputSlots_VideoInputPipeline )
public:

    virtual void        PreEvents           () override;
    virtual void        PostRender          () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( Engine_InputSlots_VideoInputPipeline, VideoInputPipeline, Engine_InputSlots )



// ***********************
//
void        Engine_InputSlots_VideoInputPipeline::PreEvents           ()
{

}

// ***********************
//
void        Engine_InputSlots_VideoInputPipeline::PostRender            ()
{

}






