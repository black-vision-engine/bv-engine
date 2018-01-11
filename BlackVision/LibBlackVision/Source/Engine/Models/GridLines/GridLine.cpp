#include "stdafx.h"
#include "GridLine.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "Engine/Models/ModelState.h"

#include "Engine/Models/BoundingVolume.h"

#include "Serialization/SerializationHelper.inl"




namespace bv {
namespace Convert
{


// ***********************
//
std::pair< bv::model::GridLineType, const char* > GridLineTypeMapping[] =
{
    std::make_pair( bv::model::GridLineType::TST_Horizontal, "Horizontal" )
    , std::make_pair( bv::model::GridLineType::TST_Vertical, "Vertical" )
    , std::make_pair( bv::model::GridLineType::TST_TOTAL, "" )      // default
};


// ***********************
//
std::pair< bv::model::GridLineAlignement, const char* > GridLineAlignementMapping[] =
{
    std::make_pair( bv::model::GridLineAlignement::TSA_BoundingBoxCenter, "BoundingBoxCenter" )
    , std::make_pair( bv::model::GridLineAlignement::TSA_BoundingBoxMajor, "BoundingBoxMajor" )
    , std::make_pair( bv::model::GridLineAlignement::TSA_BoundingBoxMinor, "BoundingBoxMinor" )
    , std::make_pair( bv::model::GridLineAlignement::TSA_GeometryCenter, "GeometryCenter" )
    , std::make_pair( bv::model::GridLineAlignement::TSA_WeightCenter, "WeightCenter" )
    , std::make_pair( bv::model::GridLineAlignement::TSA_TOTAL, "" )      // default
};

IMPLEMENT_ENUM_SERIALIZATION( bv::model::GridLineType, GridLineTypeMapping );
IMPLEMENT_ENUM_SERIALIZATION( bv::model::GridLineAlignement, GridLineAlignementMapping );


}   // Convert




namespace model
{







// ========================================================================= //
// Helper functions
// ========================================================================= //

namespace
{
    
glm::mat4           GetSingleNodeTransform  ( const model::IModelNode* node )
{
    if( node == nullptr )
        return glm::mat4( 1 ); //  Identity matrix;

    glm::mat4 transform = glm::mat4( 1 ); //  Identity matrix;
    auto transPlugin = node->GetPlugin( "transform" );

    if( transPlugin != nullptr )
    {
        auto transformParam = node->GetFinalizePlugin()->GetParamTransform();
        if( transformParam != nullptr )
            transform = transformParam->Evaluate();
    }
    return transform;
}

// ***********************
//
glm::mat4           GetTransform            ( const model::IModelNode* node )
{
    glm::mat4 transform = GetSingleNodeTransform( node );

    auto parentNode = model::ModelState::GetInstance().QueryNodeParent( node );

    if( parentNode )
    {
        glm::mat4 parentTransform = GetTransform( parentNode );
        return parentTransform * transform;
    }
    else
    {
        return transform;
    }
}


// ***********************
//
const bv::mathematics::Box *        GetBoundignBox( model::BasicNodePtr& node )
{
    auto boundingVolume = node->GetBoundingVolume();
    assert( boundingVolume );

    auto boundingBox = boundingVolume->GetBoundingBox();
    assert( boundingBox );

    return boundingBox;
}

}   // anonymous

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
    auto parentNode = model::ModelState::GetInstance().QueryNodeParent( node.get() );

    glm::mat4 childTransform = GetSingleNodeTransform( node.get() );
    glm::mat4 parentTransform = GetTransform( parentNode );

    glm::mat4 transform = parentTransform;
    if( alignement != GridLineAlignement::TSA_WeightCenter )
    {
        transform = transform * childTransform;
    }
    else
    {
        childTransform = glm::mat4( 1 );
    }

    glm::vec4 referencePos = glm::vec4( ReferencePos( node, alignement ), 1.0 );

    glm::vec3 transformedReferencePos = glm::vec3( transform * referencePos );

    glm::vec3 translate = ComputeTranslation( transformedReferencePos );
    glm::vec3 newPosition = transformedReferencePos + translate;

    // Compute translation in local coordinates system of object
    glm::vec4 localPosition = glm::inverse( transform ) * glm::vec4( newPosition, 1.0 );
    glm::vec3 localTranslate = glm::vec3( childTransform * ( localPosition - referencePos ) );
    
    return UpdateTransform( node, localTranslate );
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

        glm::vec3 center = paramTransform->Transform().GetCenter( timeline->GetLocalTime() );
        glm::vec3 position = paramTransform->Transform().GetTranslation( timeline->GetLocalTime() );

        return position + center;
    }
    else if( alignement == GridLineAlignement::TSA_BoundingBoxCenter )
    {
        auto boundingBox = GetBoundignBox( node );

        return glm::vec3( ( boundingBox->xmin + boundingBox->xmax ) / 2, ( boundingBox->ymax + boundingBox->ymin ) / 2, ( boundingBox->zmax + boundingBox->zmin ) / 2 );
    }
    else if( alignement == GridLineAlignement::TSA_BoundingBoxMinor )
    {
        auto boundingBox = GetBoundignBox( node );

        if( m_type == GridLineType::TST_Horizontal )
        {
            return glm::vec3( ( boundingBox->xmin + boundingBox->xmax ) / 2, boundingBox->ymin, ( boundingBox->zmax + boundingBox->zmin ) / 2 );
        }
        else 
        {
            return glm::vec3( boundingBox->xmin, ( boundingBox->ymax + boundingBox->ymin ) / 2, ( boundingBox->zmax + boundingBox->zmin ) / 2 );
        }

    }
    else if( alignement == GridLineAlignement::TSA_BoundingBoxMajor )
    {
        auto boundingBox = GetBoundignBox( node );

        if( m_type == GridLineType::TST_Horizontal )
        {
            return glm::vec3( ( boundingBox->xmin + boundingBox->xmax ) / 2, boundingBox->ymax, ( boundingBox->zmax + boundingBox->zmin ) / 2 );
        }
        else 
        {
            return glm::vec3( boundingBox->xmax, ( boundingBox->ymax + boundingBox->ymin ) / 2, ( boundingBox->zmax + boundingBox->zmin ) / 2 );
        }
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
        return glm::vec3( 0.0, m_position - referencePosition.y, 0.0 );
    }
    else if( m_type == GridLineType::TST_Vertical )
    {
        return glm::vec3( m_position - referencePosition.x, 0.0, 0.0 );
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