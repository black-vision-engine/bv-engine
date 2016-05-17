#pragma once


#include "Engine/Models/Cameras/CameraModel.h"

#include <vector>


namespace bv { namespace model {

class CamerasLogic
{
private:

    std::vector< CameraModelPtr >   m_cameras;
    CameraModelPtr                  m_currentCamera;

    ITimeEvaluatorPtr               m_defaultTimeline;

public:
    explicit                CamerasLogic            ( ITimeEvaluatorPtr timeEvaluator );
    ~CamerasLogic();


    void                    Deserialize             ( const IDeserializer & deser );
    void                    Serialize               ( ISerializer & ser ) const;

    bool                    AddCamera               ();
    bool                    RemoveCamera            ( unsigned int index );

    CameraModelPtr &        GetCurrentCamera        ();
    bool                    SetCurrentCamera        ( unsigned int index );

    void                    SetDefaultTimeline      ( ITimeEvaluatorPtr timeEvaluator );

private:

    void                    ClearAll                ();
};


} // model
} //bv

