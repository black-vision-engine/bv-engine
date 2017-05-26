#pragma once


#include "Engine/Models/BasicNode.h"


namespace bv {

class BVAppLogic;


class TestKeyboardHandler
{
private:

    bool            m_moveCamera;
    float           m_moveMultiplier;
    float           m_rotationMultiplier;

    float           m_moveMultiplierBase;
    float           m_rotationMultiplierBase;

    int             m_lastX;
    int             m_lastY;

public:
    explicit                TestKeyboardHandler();
    virtual                 ~TestKeyboardHandler();

    virtual void            HandleKey           ( unsigned char c, BVAppLogic * logic );
    virtual void            OnMouse             ( MouseAction action, int posX, int posY, BVAppLogic * logic );

private:
    float                   ComputeMoveMultiplier       ( char c );
    float                   ComputeRotationMultiplier   ( char c );
};

} //bv


