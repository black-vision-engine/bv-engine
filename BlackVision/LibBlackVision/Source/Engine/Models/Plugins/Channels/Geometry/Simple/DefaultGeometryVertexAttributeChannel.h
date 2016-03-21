#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"


namespace bv { namespace model {


// *********************************
//
class IGeometryGenerator
{
public:

    enum Type { GEOMETRY_ONLY, GEOMETRY_AND_UVS };

    virtual Type GetType() = 0;

};

DEFINE_PTR_TYPE( IGeometryGenerator )

// *********************************
//
class IGeometryOnlyGenerator : public IGeometryGenerator
{
public:

    virtual Type GetType() { return GEOMETRY_ONLY; }
    
    virtual void GenerateGeometry( Float3AttributeChannelPtr ) = 0;

};

// *********************************
//
class IGeometryAndUVsGenerator : public IGeometryGenerator
{
public:

    virtual Type GetType() { return GEOMETRY_AND_UVS; }

    virtual void GenerateGeometryAndUVs( Float3AttributeChannelPtr, Float2AttributeChannelPtr ) = 0;

};


// *********************************
//
class DefaultGeometryVertexAttributeChannel : public VertexAttributesChannel
{
private:

    AttributeChannelDescriptor * m_compVertDesc;
    AttributeChannelDescriptor * m_compNormDesc;
    AttributeChannelDescriptor * m_compUVDesc;

public:

    DefaultGeometryVertexAttributeChannel( PrimitiveType type, bool isReadOnly = false, bool isTimeInvariant = false );

    void AddAttributeChannelDescriptorsIfNeeded( IGeometryGenerator & generator_ );

    void GenerateNormals( Float3AttributeChannelPtr vertChannel, Float3AttributeChannelPtr normalChannel );

    void GenerateAndAddConnectedComponent( IGeometryGenerator & generator_ );

};

} //model
} //bv