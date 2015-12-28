#pragma once

#include "Engine/Models/BasicNode.h"
#include "TestAI/TestKeyboardHandler.h"


namespace bv {

class TestVideoOutputKeyboardHandler : public TestKeyboardHandler
{
private:

    float   m_alpha;
    bool    m_overwriteAlpha;

public:

                            TestVideoOutputKeyboardHandler  ();

    virtual void            HandleKey                       ( unsigned char c, BVAppLogic * logic ) override;
        
private:

    void                    SetChannelMapping               ( BVAppLogic * logic, unsigned char rIdx, unsigned char gIdx, unsigned char bIdx, unsigned char aIdx );

    void                    ToggleOverwriteAlpha            ( BVAppLogic * logic );
    void                    AddToAlpha                      ( BVAppLogic * logic, float val );
    void                    SetHeight                       ( BVAppLogic * logic, int height );

};

} //bv
