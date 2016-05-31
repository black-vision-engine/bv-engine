#pragma once


#include "Engine/Models/Cameras/CameraModel.h"

#include <vector>


namespace bv { namespace model {

class CamerasLogic : public IUpdatable
{
private:

    std::vector< CameraModelPtr >   m_cameras;
    CameraModelPtr                  m_currentCamera;

    ITimeEvaluatorPtr               m_defaultTimeline;

    UInt64                          m_cameraUpdateID;

public:
    explicit                CamerasLogic            ( ITimeEvaluatorPtr timeEvaluator );
    ~CamerasLogic();


    void                    Deserialize             ( const IDeserializer & deser );
    void                    Serialize               ( ISerializer & ser ) const;

    virtual void            Update                  ( TimeType t ) override;

    bool                    AddCamera               ();
    bool                    RemoveCamera            ( unsigned int index );
    CameraModelPtr          GetCamera               ( unsigned int index );
    SizeType                GetNumCameras           ();

    CameraModelPtr &        GetCurrentCamera        ();
    bool                    SetCurrentCamera        ( unsigned int index );

    UInt64                  GetUpdateID             ()                              { return m_cameraUpdateID; }


    void                    SetDefaultTimeline      ( ITimeEvaluatorPtr timeEvaluator );

private:

    void                    ClearAll                ();
    void                    UpdateID                ();
};


} // model
} //bv

