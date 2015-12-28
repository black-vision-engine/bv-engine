#pragma once

#include "TestAI/TestKeyboardHandler.h"

namespace bv {

class TestVideoStreamDecoderKeyboardHandler : public TestKeyboardHandler
{
public:

    virtual void            HandleKey           ( unsigned char c, BVAppLogic * logic ) override;
};

} //bv
