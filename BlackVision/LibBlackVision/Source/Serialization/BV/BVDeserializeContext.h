#pragma once

#include "Serialization/DeserializeContext.h"
#include "Serialization/IDeserializer.h"

#include "CoreDEF.h"

namespace bv {

// FORWARD DECLARATIONS

class AssetDescsWithUIDs;
DEFINE_PTR_TYPE( AssetDescsWithUIDs );

class BVProjectEditor;
class BVDeserializeContext;

namespace model
{
    class RendererContext;
    DEFINE_PTR_TYPE(RendererContext);

    class OffsetTimeEvaluator;
    DEFINE_PTR_TYPE(OffsetTimeEvaluator);

    class ITimeEvaluator;
    DEFINE_PTR_TYPE( ITimeEvaluator );

    class PluginsManager;

    class TimelineManager;
}; // model


class Exception;
DEFINE_PTR_TYPE( Exception );
typedef std::vector< ExceptionPtr > Exceptions;


BVDeserializeContext *      Context             ( const IDeserializer & deser );
void                        WarnWithoutContext  ( const std::string & message );


// ************************
//
class BVDeserializeContext : public DeserializeContext
{
private:
    model::OffsetTimeEvaluatorPtr       m_sceneTimeline;
    AssetDescsWithUIDsPtr               m_assets;

    std::string                         m_nodePath;

    model::PluginsManager *             m_pluginsManager;
    model::TimelineManager *            m_timelineManager;

public:

    BVDeserializeContext    ( model::OffsetTimeEvaluatorPtr timeline, AssetDescsWithUIDsPtr assets, model::PluginsManager * pluginsManager, model::TimelineManager * timelineManager );
    virtual                             ~BVDeserializeContext   ();

    model::OffsetTimeEvaluatorPtr       GetSceneTimeline        ();
    void                                SetSceneTimeline        ( const model::OffsetTimeEvaluatorPtr & timeline );
    model::ITimeEvaluatorPtr            GetTimeline             ( const std::string & name, const std::string & paramName );
    model::TimelineManager *            GetTimelineManager      ();

    AssetDescsWithUIDsPtr               GetAssets               ();
    void                                SetAssets               ( const AssetDescsWithUIDsPtr & assets );

    std::string &                       GetSceneName            ();
    void                                SetSceneName            ( const std::string& sceneName );

    std::string &                       GetNodePath             ();
    void                                SetNodePath             ( const std::string& sceneName );

public:

    static BVDeserializeContext *       CreateContextFromEmptiness  (); // in future this should have some parameters instead of static singletons
    static BVDeserializeContext *       CreateContextFromEmptiness  ( const model::OffsetTimeEvaluatorPtr & timeline ); // in future this should have some parameters instead of static singletons

};


} // bv
