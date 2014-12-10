#include "PrismComponent.h"

#include <cassert>

#include "Mathematics/defines.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"


namespace bv { namespace model
{

// *********************************
//
PrismComponent::PrismComponent( /*float startAngle, float endAngle, float innerRadius, float outerRadius, int fragmentsNum*/ )
{
}

std::pair< ConnectedComponentPtr, ConnectedComponentPtr >             PrismComponent::Create                      ( /*float startAngle, float endAngle, float innerRadius, float outerRadius,*/ int fragmentsNum )
{
	ConnectedComponentPtr comp = PrismComponentPtr( new PrismComponent() );
	ConnectedComponentPtr comp_up = PrismComponentPtr( new PrismComponent() );
	//ConnectedComponent* comp = new ConnectedComponent();
	//ConnectedComponent* comp_up = new ConnectedComponent();

	const AttributeChannelDescriptor * descUV = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_GENERATOR );

    Float2AttributeChannelPtr vertArrtUV = std::make_shared< Float2AttributeChannel >( descUV, descUV->SuggestedDefaultName( 0 ), false );
    Float2AttributeChannelPtr vertArrtUV_up = std::make_shared< Float2AttributeChannel >( descUV, descUV->SuggestedDefaultName( 0 ), false );

    //FIXME: not null desc should be created via factory
    const AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

    Float3AttributeChannelPtr vertArrtF3 = std::make_shared< Float3AttributeChannel >( desc, desc->SuggestedDefaultName( 0 ), false );
    Float3AttributeChannelPtr vertArrtF3_up = std::make_shared< Float3AttributeChannel >( desc, desc->SuggestedDefaultName( 0 ), false );

	for( int i = 0; i <= fragmentsNum; i++ )
	{
		double angle = 2*PI * i / fragmentsNum;

		vertArrtF3->AddAttribute( glm::vec3( cos( angle ), -0.5, sin( angle ) ) ); 
		vertArrtF3->AddAttribute( glm::vec3( cos( angle ),  0.5, sin( angle ) ) ); 

		vertArrtUV->AddAttribute( glm::vec2( float( i ) / fragmentsNum, 0 ) );
		vertArrtUV->AddAttribute( glm::vec2( float( i ) / fragmentsNum, 1 ) );

		//vertArrtF3_up->AddAttribute( glm::vec3( cos( angle ), 0.5, sin( angle ) ) );
		//vertArrtUV_up->AddAttribute( glm::vec2( float( i ) / fragmentsNum, 1 ) );

		//if( i%2 == 1 )
		//{
		//	vertArrtF3_up->AddAttribute( glm::vec3( 0, 0.5, 0 ) );
		//	vertArrtUV_up->AddAttribute( glm::vec2( 0, 0 ) );
		//}
	}


	auto vertices = vertArrtF3->GetVertices();
	int n = vertices.size();
	auto uvs = vertArrtUV->GetVertices();
	vertArrtF3_up->AddAttribute( vertices[ 0 ] );
	vertArrtF3_up->AddAttribute( vertices[ 1 ] );
	vertArrtF3_up->AddAttribute( vertices[ 2 ] );
	vertArrtUV_up->AddAttribute( uvs[ 0 ] );
	vertArrtUV_up->AddAttribute( uvs[ 1 ] );
	vertArrtUV_up->AddAttribute( uvs[ 2 ] );
	for( int i = 0; i <= fragmentsNum/2; i++ )
	{
		vertArrtF3_up->AddAttribute( vertices[ n-1 - i ] );
		vertArrtF3_up->AddAttribute( vertices[ 3 + i ] );

		vertArrtUV_up->AddAttribute( uvs[ n-1 - i ] );
		vertArrtUV_up->AddAttribute( uvs[ 3 + i ] );
	}

    comp->AddAttributeChannel( AttributeChannelPtr( vertArrtF3 ) );
	comp->AddAttributeChannel( AttributeChannelPtr( vertArrtUV ) );

	comp_up->AddAttributeChannel( AttributeChannelPtr( vertArrtF3_up ) );
	comp_up->AddAttributeChannel( AttributeChannelPtr( vertArrtUV_up ) );
	
	return std::make_pair( comp, comp_up );
}

} // model
} // bv
