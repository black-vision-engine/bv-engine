#pragma once

#include "TestKeyboardHandler.h"

namespace bv
{
class BVAppLogic;

class TestRemoteEventsKeyboardHandler : public TestKeyboardHandler
{
public:
    TestRemoteEventsKeyboardHandler();
    virtual ~TestRemoteEventsKeyboardHandler();

    virtual void            HandleKey           ( unsigned char c, BVAppLogic* logic );
};

} //bv
