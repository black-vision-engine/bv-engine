#pragma once

#include "IDeserializer.h"

#include "Engine/Models/Timeline/Static/OffsetTimeEvaluator.h"

#include "CoreDEF.h"

namespace bv {

// FORWARD DECLARATIONS
namespace model
{
    class RendererContext;
    DEFINE_PTR_TYPE(RendererContext);

    typedef std::vector< RendererContextPtr > RenderContextVec;
}; // model

// ************************
//
class BVDeserializeContext : public DeserializeContext
{
private:
    model::RenderContextVec             m_rendererContextes;

public:
    model::OffsetTimeEvaluatorPtr       m_sceneTimeline;        // FIXME: Move it to private section.

public:
    virtual                             ~BVDeserializeContext   ();

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
