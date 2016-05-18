#pragma once


#include "Engine/Models/BasicNode.h"


namespace bv {

class BVAppLogic;


class TestKeyboardHandler
{
private:

    bool            m_moveCamera;

public:
    explicit                TestKeyboardHandler();
    virtual                 ~TestKeyboardHandler();

    virtual void            HandleKey           ( unsigned char c, BVAppLogic * logic );
    virtual void            OnMouse             ( MouseAction action, int posX, int posY, BVAppLogic * logic );
};

} //bv


