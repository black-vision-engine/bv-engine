#pragma once

#include "Serialization/DeserializeContext.h"

#include "Engine/Models/Timeline/Static/OffsetTimeEvaluator.h"

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

    typedef std::vector< RendererContextPtr > RenderContextVec;

    //class OffsetTimeEvaluator;
    //DEFINE_PTR_TYPE(OffsetTimeEvaluator);
}; // model

// ************************
//
class BVDeserializeContext : public DeserializeContext
{
private:
    model::RenderContextVec             m_rendererContextes;
    model::OffsetTimeEvaluatorPtr       m_sceneTimeline;
    AssetDescsWithUIDsPtr               m_assets;

    std::string                         m_sceneName;
    std::string                         m_nodePath;

public:
    BVDeserializeContext( model::OffsetTimeEvaluatorPtr timeline, AssetDescsWithUIDsPtr assets );
    virtual                             ~BVDeserializeContext   ();

    model::OffsetTimeEvaluatorPtr       GetSceneTimeline        ();
    void                                SetSceneTimeline        ( const model::OffsetTimeEvaluatorPtr & timeline );

    AssetDescsWithUIDsPtr               GetAssets               ();
    void                                SetAssets               ( const AssetDescsWithUIDsPtr & assets );

    std::string &                       GetSceneName            ();
    void                                SetSceneName            ( const std::string& sceneName );

    std::string &                       GetNodePath             ();
    void                                SetNodePath             ( const std::string& sceneName );

    void                                Push                    ( const model::RendererContextPtr & context );
    const model::RenderContextVec &     RendererContextes       () const;
    void                                ClearRendererContextes  ();
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