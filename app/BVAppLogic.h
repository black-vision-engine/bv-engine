#pragma once

#include "Engine/Events/Events.h"
#include "MockFrameReader.h"


namespace bv
{

namespace model
{
    class BasicNode;
    class ModelScene;
}

class SceneNode;
class Camera;
class MockFrameReader;
class ProcessManager;

//FIXME: possibly add an interface such as IAppLogic (if necessary)
class BVAppLogic
{
private:

    model::ModelScene *         m_modelScene;
    SceneNode *                 m_mockSceneEng;

    MockFrameReader *           m_mockFrameReader;

    char *                      m_frameData;

    unsigned long               m_startTime;

public:

                    BVAppLogic      ();
                    ~BVAppLogic     ();

    void            OnInitialize    ();

    void            SetStartTime    ( unsigned long millis );

    virtual void    OnUpdate        ( unsigned long millis );

    //FIXME: add if necessary
    //virtual void OnChangeState(<type> state);

    //Delegates
    void            FrameRendered   ( IEventPtr evt );

};

} //bv
