#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

namespace bv { namespace model {

class DefaultGeometryVertexAttributeChannel : public VertexAttributesChannel
	{
		AttributeChannelDescriptor * m_compDesc;

	public:
		DefaultGeometryVertexAttributeChannel( PrimitiveType type, bool isReadOnly = false, bool isTimeInvariant = false ) 
			: VertexAttributesChannel ( type, isReadOnly, isTimeInvariant )  
		{
			AttributeChannelDescriptor * compDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

			VertexAttributesChannelDescriptor vaDesc;

			vaDesc.AddAttrChannelDesc( static_cast< const AttributeChannelDescriptor * >( compDesc ) );

			m_compDesc = compDesc;

			m_desc = vaDesc;
		}

		void GenerateAndAddConnectedComponent( void (*GenerateConnectedComponent) (Float3AttributeChannelPtr) )
		{
			ConnectedComponentPtr comp = ConnectedComponent::Create();

			Float3AttributeChannelPtr vertArrtF3 = std::make_shared< Float3AttributeChannel >( m_compDesc, m_compDesc->SuggestedDefaultName( 0 ), false );

			GenerateConnectedComponent( vertArrtF3 );

			comp->AddAttributeChannel( vertArrtF3 );

			AddConnectedComponent( comp );
		}
	};

	class DefaultGeometryAndUVsVertexAttributeChannel : public VertexAttributesChannel
	{
		AttributeChannelDescriptor * m_compVertDesc;
		AttributeChannelDescriptor * m_compUVDesc;

	public:
		DefaultGeometryAndUVsVertexAttributeChannel( PrimitiveType type, bool isReadOnly = false, bool isTimeInvariant = false ) 
			: VertexAttributesChannel ( type, isReadOnly, isTimeInvariant )  
		{
			VertexAttributesChannelDescriptor vaDesc;

			AttributeChannelDescriptor * compVertDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
			vaDesc.AddAttrChannelDesc( static_cast< const AttributeChannelDescriptor * >( compVertDesc ) );

			AttributeChannelDescriptor * compUVDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_GENERATOR );
			vaDesc.AddAttrChannelDesc( static_cast< const AttributeChannelDescriptor * >( compUVDesc ) );

			m_compVertDesc = compVertDesc;
			m_compUVDesc = compUVDesc;

			m_desc = vaDesc;
		}

		void GenerateAndAddConnectedComponent( void (*GenerateConnectedComponent) (Float3AttributeChannelPtr, Float2AttributeChannelPtr) )
		{
			ConnectedComponentPtr comp = ConnectedComponent::Create();

			Float3AttributeChannelPtr vertArrtF3 = std::make_shared< Float3AttributeChannel >( m_compVertDesc, m_compVertDesc->SuggestedDefaultName( 0 ), false );
			Float2AttributeChannelPtr vertArrtUV = std::make_shared< Float2AttributeChannel >( m_compUVDesc, m_compUVDesc->SuggestedDefaultName( 0 ), false );

			GenerateConnectedComponent( vertArrtF3, vertArrtUV );

			comp->AddAttributeChannel( vertArrtF3 );
			comp->AddAttributeChannel( vertArrtUV );

			AddConnectedComponent( comp );
		}
	};

} }