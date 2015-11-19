#pragma once


#include "Engine/Models/BasicNode.h"


namespace bv {

class BVAppLogic;


class TestKeyboardHandler
{
public:

    virtual                 ~TestKeyboardHandler();

    virtual void            HandleKey           ( unsigned char c, BVAppLogic * logic );
    
};

} //bv


