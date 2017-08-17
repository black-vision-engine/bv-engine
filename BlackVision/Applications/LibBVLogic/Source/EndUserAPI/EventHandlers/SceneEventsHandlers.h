#pragma once

#include "Engine/Events/Events.h"
#include "Engine/Models/Cameras/CameraModel.h"
#include "Mathematics/Box.h"


namespace bv {

class BVAppLogic;

namespace model
{
    class SceneModel;
    DEFINE_PTR_TYPE( SceneModel )
}


typedef std::map< bv::model::SceneModelPtr, bool >   SceneVisibilityStateMap;


class SceneEventsHandlers
{
private:
    
    enum class ThumbnailType : int
    {
        Scene,
        Preset,
        MeshAsset
    };

private:

    BVAppLogic*                 m_appLogic;

    // Save scene or preset state
    SceneVisibilityStateMap     m_scenesVisibilityState;        // Used to save visibility state while making scene or preset thumbnail.
    bool                        m_closeSavedPreset;
    
    model::SceneModelPtr        m_savedScene;

    std::map< std::string, ThumbnailType >  m_thumbnailTypeMap;

public:

    SceneEventsHandlers( BVAppLogic* appLogic );
    ~SceneEventsHandlers();

    void        NodeStructure       ( bv::IEventPtr evt );
    void        PluginStructure     ( bv::IEventPtr evt );
    void        ProjectStructure    ( bv::IEventPtr evt );
    void        SceneStructure	    ( bv::IEventPtr evt );

    void        LightsManagement	( bv::IEventPtr evt );
    void        CameraManagement    ( bv::IEventPtr evt );

    void        SceneVariable       ( bv::IEventPtr evt );
    void        GridLines           ( bv::IEventPtr evt );

private:

    void        RequestThumbnail        ( model::SceneModelPtr scene, const std::string & saveTo, ThumbnailType thumbnailType );
    void        ThumbnailRendered       ( bv::IEventPtr evt );

    void        SaveVisibilityState     ( const std::string & sceneName );
    void        RestoreVisibilityState  ();

    ThumbnailConstPtr   CreateThumbnail                 ( ThumbnailType thumbnailType, MemoryChunkConstPtr data );
    std::string         GetPrefixDir                    ( ThumbnailType thumbnailType );

    void                GenerateMeshThumbnail           ( const std::string & meshPath, const std::string & destPath );
    glm::vec3           GetMeshTranslationToFitCamera  ( model::CameraModelPtr camera, const mathematics::Box & boundingBox );

};


} //bv
