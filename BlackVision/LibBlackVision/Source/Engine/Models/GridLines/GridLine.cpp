#include "stdafx.h"
#include "GridLine.h"

#include "Engine/Models/BVProjectEditor.h"
#include "Engine/Models/ModelState.h"

#include "Engine/Models/BoundingVolume.h"


namespace bv { namespace model
{

// ========================================================================= //
// Helper functions
// ========================================================================= //

// ***********************
//
glm::mat4           GetTransform    ( const model::IModelNode* node )
{
    glm::mat4 transform;
    auto transPlugin = node->GetPlugin( "transform" );

    if( transPlugin != nullptr )
    {
        auto transformParam = node->GetFinalizePlugin()->GetParamTransform();
        if( transformParam != nullptr )
            transform = transformParam->Evaluate();
        else
            transform = glm::mat4( 1 ); //  Identity matrix
    }

    auto parentNode = model::ModelState::GetInstance().QueryNodeParent( node );

    if( parentNode )
    {
        glm::mat4 parentTransform = GetTransform( parentNode );
        return transform * parentTransform;
    }
    else
    {
        return transform;
    }
}

// ========================================================================= //
// GridLine
// ========================================================================= //

// ***********************
//
GridLine::GridLine( GridLineType type, const std::string & name )
    :   m_type( type )
    ,   m_name( name )
    ,   m_position( 0.0 )
{}

// ***********************
//
GridLine::~GridLine()
{}


// ***********************
//
bool        GridLine::AlignNode       ( model::BasicNodePtr node, GridLineAlignement alignement )
{
    glm::mat4 transform = GetTransform( node.get() );
    glm::vec4 referencePos = glm::vec4( ReferencePos( node, alignement ), 1.0 );

    glm::vec3 position = glm::vec3( transform * referencePos );

    glm::vec3 translate = ComputeTranslation( position );
    
    return UpdateTransform( node, translate );
}

// ***********************
//
void        GridLine::Rename              ( const std::string & newName )
{
    m_name = newName;
}

// ***********************
//
glm::vec3   GridLine::ReferencePos    ( model::BasicNodePtr& node, GridLineAlignement alignement )
{
    if( alignement == GridLineAlignement::TSA_WeightCenter )
    {
        auto paramTransform = node->GetFinalizePlugin()->GetParamTransform();
        assert( paramTransform );

        auto timeline = paramTransform->GetTimeEvaluator();
        assert( timeline );

        return paramTransform->Transform().GetCenter( timeline->GetLocalTime() );
    }
    else if( alignement == GridLineAlignement::TSA_BoundingBoxCenter )
    {
        auto boundingVolume = node->GetBoundingVolume();
        assert( boundingVolume );

        auto boundingBox = boundingVolume->GetBoundingBox();
        assert( boundingBox );

        return glm::vec3( ( boundingBox->xmin + boundingBox->xmax ) / 2, ( boundingBox->ymax + boundingBox->ymin ) / 2, ( boundingBox->zmax + boundingBox->zmin ) / 2 );
    }
    else if( alignement == GridLineAlignement::TSA_BoundingBoxMinor )
    {
        assert( !"Not implemented" );
    }
    else if( alignement == GridLineAlignement::TSA_BoundingBoxMajor )
    {
        assert( !"Not implemented" );
    }
    else if( alignement == GridLineAlignement::TSA_GeometryCenter )
    {
        return glm::vec3( 0.0, 0.0, 0.0 );
    }
    else
        assert( false );

    return glm::vec3( 0.0, 0.0, 0.0 );
}

// ***********************
//
glm::vec3   GridLine::ComputeTranslation  ( glm::vec3 referencePosition )
{
    if( m_type == GridLineType::TST_Horizontal )
    {
        return glm::vec3( m_position - referencePosition.x, 0.0, 0.0 );
    }
    else if( m_type == GridLineType::TST_Vertical )
    {
        return glm::vec3( 0.0, m_position - referencePosition.y, 0.0 );
    }
    else 
        assert( false );

    return glm::vec3( 0.0, 0.0, 0.0 );
}

// ***********************
//
bool        GridLine::UpdateTransform     ( model::BasicNodePtr& node, glm::vec3 translation )
{
    auto paramTransform = node->GetFinalizePlugin()->GetParamTransform();
    assert( paramTransform );
    if( !paramTransform )       return false;

    auto timeline = paramTransform->GetTimeEvaluator();
    assert( timeline );
    if( !timeline )       return false;

    auto time = timeline->GetLocalTime();
    auto& transform = paramTransform->Transform();

    transform.SetTranslation( transform.GetTranslation( time ) + translation, time );

    return true;
}

}   // model
}	// bv