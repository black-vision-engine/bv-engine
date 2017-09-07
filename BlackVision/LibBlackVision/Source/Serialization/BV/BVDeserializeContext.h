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

// ************************
//
class BVDeserializeContext : public DeserializeContext
{
private:
    model::OffsetTimeEvaluatorPtr       m_sceneTimeline;
    AssetDescsWithUIDsPtr               m_assets;

    std::string                         m_sceneName;
    std::string                         m_nodePath;

public:
    BVDeserializeContext( model::OffsetTimeEvaluatorPtr timeline, AssetDescsWithUIDsPtr assets );
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
};



//template< class Type >
//struct SaveCall
//{
//    static_assert( false );
//};
//
//
//template< class Type >
//struct SaveCall< Type * >
//{
//private:
//    Type *      m_ptr;
//
//public:
//    explicit    SaveCall< Type * >( Type * ptr )
//        : m_ptr( ptr )
//    {}
//
//    Type *      operator -> ()
//    {
//        assert( m_ptr );
//
//        if( m_ptr
//
//        return m_ptr;
//    }
//};
//
//
//

} // bv
