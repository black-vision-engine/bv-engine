#pragma once

#include "Serialization/DeserializeContext.h"

//#include "Engine/Models/Timeline/Static/OffsetTimeEvaluator.h"

#include "CoreDEF.h"

namespace bv {

// FORWARD DECLARATIONS

class AssetDescsWithUIDs;
DEFINE_PTR_TYPE( AssetDescsWithUIDs );

class BVProjectEditor;

namespace model
{
    class RendererContext;
    DEFINE_PTR_TYPE(RendererContext);

    class OffsetTimeEvaluator;
    DEFINE_PTR_TYPE(OffsetTimeEvaluator);

    class ITimeEvaluator;
    DEFINE_PTR_TYPE( ITimeEvaluator );
}; // model

class Exception;
DEFINE_PTR_TYPE( Exception );
typedef std::vector< ExceptionPtr > Exceptions;

// ************************
//
class BVDeserializeContext : public DeserializeContext
{
private:
    model::OffsetTimeEvaluatorPtr       m_sceneTimeline;
    AssetDescsWithUIDsPtr               m_assets;

    std::string                         m_sceneName;
    std::string                         m_nodePath;

    Exceptions                          m_warnings;

public:
                                        BVDeserializeContext    ( model::OffsetTimeEvaluatorPtr timeline, AssetDescsWithUIDsPtr assets );
    virtual                             ~BVDeserializeContext   ();

    model::OffsetTimeEvaluatorPtr       GetSceneTimeline        ();
    void                                SetSceneTimeline        ( const model::OffsetTimeEvaluatorPtr & timeline );
    model::ITimeEvaluatorPtr            GetTimeline             ( const std::string & name, const std::string & paramName );

    AssetDescsWithUIDsPtr               GetAssets               ();
    void                                SetAssets               ( const AssetDescsWithUIDsPtr & assets );

    std::string &                       GetSceneName            ();
    void                                SetSceneName            ( const std::string& sceneName );

    std::string &                       GetNodePath             ();
    void                                SetNodePath             ( const std::string& sceneName );

    Exceptions                          GetWarnings             ();
    void                                AddWarning              ( ExceptionPtr warning );

    template< typename Type >
    void                                AddWarning              ( const std::string & message );
};

// ========================================================================= //
// Implmentation
// ========================================================================= //

// ***********************
//
template< typename WarningType >
inline void             BVDeserializeContext::AddWarning        ( const std::string & message )
{
    AddWarning( std::make_shared< WarningType >( message ) );
}

} // bv
