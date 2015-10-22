#pragma once

#include "Engine/Models/BasicNode.h"
#include "TestAI/TestKeyboardHandler.h"


namespace bv {

class TestEditorsKeyboardHandler : public TestKeyboardHandler
{
public:

    virtual void            HandleKey       ( unsigned char c, BVAppLogic * logic ) override;

};

} //bv


