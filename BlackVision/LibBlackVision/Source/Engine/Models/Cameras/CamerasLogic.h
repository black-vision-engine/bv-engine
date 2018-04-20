#pragma once


#include "Engine/Models/Cameras/CameraModel.h"

#include <vector>

/**@page Cameras Cameras

@section Cameras_CurrentCamera Current Camera

Each @ref Scenes "Scene" has own set of cameras. One of them is used to render scene.
You can change current camera using @ref API_CamerasManagment "API".

@section Cameras_AddingRemovingCamera Adding/Removing camera

Adding and removing camera using API is described @ref API_CamerasManagment "here".

@copydetails bv::model::CameraModel


@see bv::model::CamerasLogic, bv::model::CameraModel
*/

namespace bv { namespace model {


/**@brief Camera model
@ingroup Model
*/
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

