#include "PrismComponent.h"

#include <cassert>
#include <tuple>

#include "Mathematics/defines.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"


namespace bv { namespace model
{

// *********************************
//
PrismComponent::PrismComponent( /*float startAngle, float endAngle, float innerRadius, float outerRadius, int fragmentsNum*/ )
{
}

std::tuple< ConnectedComponentPtr, ConnectedComponentPtr, ConnectedComponentPtr >             PrismComponent::Create                      ( /*float startAngle, float endAngle, float innerRadius, float outerRadius,*/ int fragmentsNum )
{
	ConnectedComponentPtr comp = PrismComponentPtr( new PrismComponent() );
	ConnectedComponentPtr comp_up = PrismComponentPtr( new PrismComponent() );
	ConnectedComponentPtr comp_down = PrismComponentPtr( new PrismComponent() );

	const AttributeChannelDescriptor * descUV = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_GENERATOR );

    Float2AttributeChannelPtr vertArrtUV = std::make_shared< Float2AttributeChannel >( descUV, descUV->SuggestedDefaultName( 0 ), false );
    Float2AttributeChannelPtr up_vertArrtUV = std::make_shared< Float2AttributeChannel >( descUV, descUV->SuggestedDefaultName( 0 ), false );
    Float2AttributeChannelPtr down_vertArrtUV = std::make_shared< Float2AttributeChannel >( descUV, descUV->SuggestedDefaultName( 0 ), false );

    //FIXME: not null desc should be created via factory
    const AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

    Float3AttributeChannelPtr vertArrtF3 = std::make_shared< Float3AttributeChannel >( desc, desc->SuggestedDefaultName( 0 ), false );
    Float3AttributeChannelPtr up_vertArrtF3 = std::make_shared< Float3AttributeChannel >( desc, desc->SuggestedDefaultName( 0 ), false );
    Float3AttributeChannelPtr down_vertArrtF3 = std::make_shared< Float3AttributeChannel >( desc, desc->SuggestedDefaultName( 0 ), false );

	for( int i = 0; i <= fragmentsNum; i++ )
	{
		double angle = 2*PI * i / fragmentsNum;

		vertArrtF3->AddAttribute( glm::vec3( cos( angle ), 0, sin( angle ) ) ); 
		vertArrtF3->AddAttribute( glm::vec3( cos( angle ), 1, sin( angle ) ) ); 

		vertArrtUV->AddAttribute( glm::vec2( float( i ) / fragmentsNum, 0 ) );
		vertArrtUV->AddAttribute( glm::vec2( float( i ) / fragmentsNum, 1 ) );

		//up_vertArrtF3->AddAttribute( glm::vec3( cos( angle ), 0.5, sin( angle ) ) );
		//up_vertArrtUV->AddAttribute( glm::vec2( float( i ) / fragmentsNum, 1 ) );

		//if( i%2 == 1 )
		//{
		//	up_vertArrtF3->AddAttribute( glm::vec3( 0, 0.5, 0 ) );
		//	up_vertArrtUV->AddAttribute( glm::vec2( 0, 0 ) );
		//}
	}

// up

	auto vertices = vertArrtF3->GetVertices();
	auto n = vertices.size()/2;
	auto uvs = vertArrtUV->GetVertices();
	up_vertArrtF3->AddAttribute( vertices[ 2*0 ] );
	up_vertArrtF3->AddAttribute( vertices[ 2*1 ] );
	up_vertArrtF3->AddAttribute( vertices[ 2*2 ] );
	up_vertArrtUV->AddAttribute( uvs[ 2*0 ] );
	up_vertArrtUV->AddAttribute( uvs[ 2*1 ] );
	up_vertArrtUV->AddAttribute( uvs[ 2*2 ] );
	int i;
	for( i = 0; i < (n-3)/2; i++ )
	{
		up_vertArrtF3->AddAttribute( vertices[ 2*( n-1 - i ) ] );
		up_vertArrtF3->AddAttribute( vertices[ 2*( 3 + i ) ] );

		up_vertArrtUV->AddAttribute( uvs[ 2*( n-1 - i ) ] );
		up_vertArrtUV->AddAttribute( uvs[ 2*( 3 + i ) ] );
	}
	if( n%2 == 0 ) // we need additional point
	{
		up_vertArrtF3->AddAttribute( vertices[ 2*( n-1 - i ) ] );

		up_vertArrtUV->AddAttribute( uvs[ 2*( n-1 - i ) ] );
	}
	
// and down

	//auto vertices = vertArrtF3->GetVertices();
	//int n = vertices.size()/2;
	//auto uvs = vertArrtUV->GetVertices();
	down_vertArrtF3->AddAttribute( vertices[ 2*0+1 ] );
	down_vertArrtF3->AddAttribute( vertices[ 2*1+1 ] );
	down_vertArrtF3->AddAttribute( vertices[ 2*2+1 ] );
	down_vertArrtUV->AddAttribute( uvs[ 2*0+1 ] );
	down_vertArrtUV->AddAttribute( uvs[ 2*1+1 ] );
	down_vertArrtUV->AddAttribute( uvs[ 2*2+1 ] );
	for( i = 0; i < (n-3)/2; i++ )
	{
		down_vertArrtF3->AddAttribute( vertices[ 2*( n-1 - i )+1 ] );
		down_vertArrtF3->AddAttribute( vertices[ 2*( 3 + i )+1 ] );

		down_vertArrtUV->AddAttribute( uvs[ 2*( n-1 - i )+1 ] );
		down_vertArrtUV->AddAttribute( uvs[ 2*( 3 + i )+1 ] );
	}
	if( n%2 == 0 ) // we need additional point
	{
		down_vertArrtF3->AddAttribute( vertices[ 2*( n-1 - i )+1 ] );

		down_vertArrtUV->AddAttribute( uvs[ 2*( n-1 - i )+1 ] );
	}

// and end

    comp->AddAttributeChannel( AttributeChannelPtr( vertArrtF3 ) );
	comp->AddAttributeChannel( AttributeChannelPtr( vertArrtUV ) );

	comp_up->AddAttributeChannel( AttributeChannelPtr( up_vertArrtF3 ) );
	comp_up->AddAttributeChannel( AttributeChannelPtr( up_vertArrtUV ) );
	
	comp_down->AddAttributeChannel( AttributeChannelPtr( down_vertArrtF3 ) );
	comp_down->AddAttributeChannel( AttributeChannelPtr( down_vertArrtUV ) );
	
	return std::make_tuple( comp, comp_up, comp_down );
}

} // model
} // bv
