#include "stdafx.h"

#include "TextArranger.h"


#include "Assets/Font/TextHelper.h"
#include "Engine/Models/Plugins/Channels/Geometry/VacAABB.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IAttributeChannelDescriptor.h"


namespace bv { namespace model
{
// *********************************
//
TextArranger::TextArranger( FuncFloat2Vec3Type position, FuncFloat2Vec3Type tangent, FuncFloat2Vec3Type norm )
    : m_position( position )
    , m_tangent( tangent )
    , m_norm( norm )
{}

// *********************************
//
TextArranger::TextArranger( FuncFloat2Vec3Type position, FuncFloat2Vec3Type tangent )
    : m_position( position )
    , m_tangent( tangent )
    , m_norm( nullptr )
{}

// *********************************
//
TextArranger::TextArranger( FuncFloat2Vec3Type position )
    : m_position( position )
    , m_tangent( nullptr )
    , m_norm( nullptr )
{}

// *********************************
//
glm::vec3                           TextArranger::EvalutePosition( float x, float xmin, float xmax )
{
    float t = (x - xmin) / std::abs( xmax - xmin );
    return m_position( t );
}

// *********************************
//
glm::mat4                           TextArranger::EvaluteRotation( float x, float xmin, float xmax )
{
    float t = (x - xmin) / std::abs( xmax - xmin );
    return glm::rotate( glm::mat4( 1.f ), t * 360.f + 90.f, glm::vec3( 0.f, 0.f, 1.f ) );
}

// *********************************
//
glm::vec3                           TextArranger::EvaluteNormal( float x, float xmin, float xmax )
{
    float t = (x - xmin) / std::abs( xmax - xmin );
    return m_norm( t );
}

// *********************************
//
model::VertexAttributesChannel *    TextArranger::Arange( model::VertexAttributesChannel * vac )
{
    mathematics::Rect rect;

    if( AABB( vac, glm::mat4( 1.f ), &rect ) )
    {
        auto x0 = rect.xmin;
        auto x1 = rect.xmax;

        auto desc = vac->GetDescriptor();

        if( desc->GetNumVertexChannels() > 0 )
        {
		    if( desc->GetAttrChannelDescriptor( 0 )->GetSemantic() == AttributeSemantic::AS_POSITION )
            {
                if( desc->GetAttrChannelDescriptor( 0 )->GetType() == AttributeType::AT_FLOAT3 )
			    {
                    if( desc->GetAttrChannelDescriptor( 2 )->GetSemantic() == AttributeSemantic::AS_TEXCOORD )
			        {
                        if( desc->GetAttrChannelDescriptor( 2 )->GetType() == AttributeType::AT_FLOAT2 )
                        {
				            auto ccs = vac->GetComponents();

                            auto firstCCCenter = glm::vec3( 0.f );

				            for( SizeType c = 0 ; c < ccs.size(); ++c )
				            {
					            auto numVerts = ccs[ c ]->GetNumVertices();

					            glm::vec3 * pos   = const_cast< glm::vec3 *>( reinterpret_cast< const glm::vec3 * >( ccs[ c ]->GetAttributeChannels()[ 0 ]->GetData() ) );
                                glm::vec2 * posc  = const_cast< glm::vec2 *>( reinterpret_cast< const glm::vec2 * >( ccs[ c ]->GetAttributeChannels()[ 2 ]->GetData() ) );

                                auto trans = glm::mat4( 1.f );
                                auto rot = glm::mat4( 1.f );
                                auto norm = glm::vec3( 1.f );

					            for( unsigned int i = 0; i < numVerts; ++i )
					            {
                                    if( i == 0 )
                                    {
                                        if( c == 0 )
                                        {
                                            firstCCCenter = glm::vec3( posc[ i ].x, posc[ i ].y, 0.f );
                                        }

                                        auto translate = EvalutePosition( pos[ i ].x, x0, x1 );
                                        trans = glm::translate( glm::mat4( 1.f ), translate );

                                        rot = EvaluteRotation( pos[ i ].x, x0, x1 );
                                        norm = EvaluteNormal( pos[ i ].x, x0, x1 );
                                    }

                                    auto translateToBegin = glm::translate( glm::mat4( 1.f ), firstCCCenter - glm::vec3( posc[ i ].x, posc[ i ].y, 0.f ) );

                                    auto transC = glm::translate( glm::mat4( 1.f ), glm::vec3( posc[ i ].x, posc[ i ].y, 0.f ) );
                                    auto transCInv = glm::translate( glm::mat4( 1.f ), -glm::vec3( posc[ i ].x, posc[ i ].y, 0.f ) );

                                    auto shift = glm::vec3( posc[ i ].x, posc[ i ].y, 0.f ) - firstCCCenter;

                                    auto nShift = EvaluteNormal( pos[ i ].x, x0, x1 ) * shift.y;

                                    auto nTranslate = glm::translate( glm::mat4( 1.f ), nShift );

                                    pos[ i ] = glm::vec3( nTranslate * trans * translateToBegin * transC * rot * transCInv * glm::vec4( pos[ i ].x, pos[ i ].y, pos[ i ].z, 1.f ) );
                                    
						            posc[ i ] = glm::vec2( nTranslate * trans * translateToBegin * transC * rot * transCInv * glm::vec4( posc[ i ].x, posc[ i ].y, 0.f, 1.f ) );
					            }
                            }
                        }
                    }
                }
            }
        }
    }

    return vac;
}

// *********************************
//
namespace
{
  
// *********************************
//
glm::vec3 CircleParametrization( float t )
{
    if( t >= 0.f && t <= 1.f )
    {
        return glm::vec3( 1.f * std::cosf( 2 * 3.1415f * t ), 1.f * std::sinf( 2 * 3.1415f * t ), 0.f );
    }
    else
    {
        return CircleParametrization( 0.f );
    }
}

// *********************************
//
glm::vec3 CircleParametrizationTangent( float t )
{
    if( t >= 0.f && t <= 1.f )
    {
        return glm::vec3( 1.f * -std::sinf( 2 * 3.1415f * t ), 1.f * std::cosf( 2 * 3.1415f * t ), 0.f );
    }
    else
    {
        return CircleParametrization( 0.f );
    }
}

// *********************************
//
glm::vec3 CircleParametrizationNorm( float t )
{
    if( t >= 0.f && t <= 1.f )
    {
        return glm::normalize( CircleParametrization( t ) );
    }
    else
    {
        return CircleParametrization( 0.f );
    }
}

} // anonymous

// *********************************
//
TextArranger CircleArranger( CircleParametrization, CircleParametrizationTangent, CircleParametrizationNorm );

}  // model
}  // bv