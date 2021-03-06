#include "stdafx.h"

#include "PrismComponent.h"

#include <cassert>
#include <tuple>

#include "Mathematics/defines.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model
{

    // *********************************
    //
    PrismComponent::PrismComponent( /*float startAngle, float endAngle, float innerRadius, float outerRadius, int fragmentsNum*/ )
    {
    }

    std::tuple< ConnectedComponentPtr, ConnectedComponentPtr, ConnectedComponentPtr >             PrismComponent::Create                      ( int fragmentsNum, PrismComponent::PrismUVType type )
    {
        ConnectedComponentPtr comp = PrismComponentPtr( new PrismComponent() );
        ConnectedComponentPtr comp_up = PrismComponentPtr( new PrismComponent() );
        ConnectedComponentPtr comp_down = PrismComponentPtr( new PrismComponent() );

        auto descUV = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_GENERATOR );

        Float2AttributeChannelPtr vertArrtUV = std::make_shared< Float2AttributeChannel >( descUV, descUV->SuggestedDefaultName( 0 ), false );
        Float2AttributeChannelPtr up_vertArrtUV = std::make_shared< Float2AttributeChannel >( descUV, descUV->SuggestedDefaultName( 0 ), false );
        Float2AttributeChannelPtr down_vertArrtUV = std::make_shared< Float2AttributeChannel >( descUV, descUV->SuggestedDefaultName( 0 ), false );

        //FIXME: not null desc should be created via factory
        auto desc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

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
        }

        glm::vec2 borderUV = glm::vec2( -1, -1 );
        glm::vec2 upUV = glm::vec2( 1, 0 ); // there is absolutely no rule for x coordinate here. seriously
        glm::vec2 downUV = glm::vec2( 0, 1 ); // the same here
                                                // BTW, why are you here, you unfortunate man...? Anyway I hope you are well!

        // up

        auto vertices = vertArrtF3->GetVertices();
        auto n = vertices.size()/2;
        auto uvs = vertArrtUV->GetVertices();
        up_vertArrtF3->AddAttribute( vertices[ 2*0 ] );
        up_vertArrtF3->AddAttribute( vertices[ 2*1 ] );
        up_vertArrtF3->AddAttribute( vertices[ 2*2 ] );
        SizeType i;
        for( i = 0; i < (n-3)/2; i++ )
        {
            up_vertArrtF3->AddAttribute( vertices[ 2*( n-1 - i ) ] );
            up_vertArrtF3->AddAttribute( vertices[ 2*( 3 + i ) ] );
        }
        if( n%2 == 0 ) // we need additional point
            up_vertArrtF3->AddAttribute( vertices[ 2*( n-1 - i ) ] );
        
        switch( type )
        {
        case PrismComponent::PrismUVType::TEXTURED:
            up_vertArrtUV->AddAttribute( borderUV );
            up_vertArrtUV->AddAttribute( borderUV );
            up_vertArrtUV->AddAttribute( borderUV );

            for( i = 0; i < (n-3)/2; i++ )
            {
                up_vertArrtUV->AddAttribute( borderUV );
                up_vertArrtUV->AddAttribute( borderUV );
            }
            if( n%2 == 0 ) // we need additional point
                up_vertArrtUV->AddAttribute( borderUV );
            break;

        case PrismComponent::PrismUVType::LINGRADED:
            up_vertArrtUV->AddAttribute( upUV );
            up_vertArrtUV->AddAttribute( upUV );
            up_vertArrtUV->AddAttribute( upUV );

            for( i = 0; i < (n-3)/2; i++ )
            {
                up_vertArrtUV->AddAttribute( upUV );
                up_vertArrtUV->AddAttribute( upUV );
            }
            if( n%2 == 0 ) // we need additional point
                up_vertArrtUV->AddAttribute( upUV );
            break;

        default:
            assert( false );
        }

        // and down

        //auto vertices = vertArrtF3->GetVertices();
        //int n = vertices.size()/2;
        //auto uvs = vertArrtUV->GetVertices();
        down_vertArrtF3->AddAttribute( vertices[ 2*0+1 ] );
        down_vertArrtF3->AddAttribute( vertices[ 2*1+1 ] );
        down_vertArrtF3->AddAttribute( vertices[ 2*2+1 ] );
        for( i = 0; i < (n-3)/2; i++ )
        {
            down_vertArrtF3->AddAttribute( vertices[ 2*( n-1 - i )+1 ] );
            down_vertArrtF3->AddAttribute( vertices[ 2*( 3 + i )+1 ] );
        }
        if( n%2 == 0 ) // we need additional point
            down_vertArrtF3->AddAttribute( vertices[ 2*( n-1 - i )+1 ] );

        switch( type )
        {
        case PrismComponent::PrismUVType::TEXTURED:
            down_vertArrtUV->AddAttribute( borderUV );
            down_vertArrtUV->AddAttribute( borderUV );
            down_vertArrtUV->AddAttribute( borderUV );

            for( i = 0; i < (n-3)/2; i++ )
            {
                down_vertArrtUV->AddAttribute( borderUV );
                down_vertArrtUV->AddAttribute( borderUV );
            }
            if( n%2 == 0 ) // we need additional point
                down_vertArrtUV->AddAttribute( borderUV );
            break;

        case PrismComponent::PrismUVType::LINGRADED:
            down_vertArrtUV->AddAttribute( downUV );
            down_vertArrtUV->AddAttribute( downUV );
            down_vertArrtUV->AddAttribute( downUV );

            for( i = 0; i < (n-3)/2; i++ )
            {
                down_vertArrtUV->AddAttribute( downUV );
                down_vertArrtUV->AddAttribute( downUV );
            }
            if( n%2 == 0 ) // we need additional point
                down_vertArrtUV->AddAttribute( downUV );
            break;

        default:
            assert( false );
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
