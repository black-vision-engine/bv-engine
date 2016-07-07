#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"


namespace bv { namespace model {


class IndexedGeometry;

// *********************************
//
class IGeometryGenerator
{
public:

    enum Type 
    { 
        GEOMETRY_ONLY, 
        GEOMETRY_NORMALS, 
        GEOMETRY_NORMALS_UVS
    };

    virtual Type    GetType                 () const = 0;
    
    
    virtual ~IGeometryGenerator             () {}
};


DEFINE_PTR_TYPE( IGeometryGenerator )

// *********************************
//
class IGeometryOnlyGenerator : public IGeometryGenerator
{
public:

    virtual Type    GetType                 () const { return GEOMETRY_ONLY; };
    
    virtual void    GenerateGeometry        ( Float3AttributeChannelPtr posChannel ) = 0;

};

// *********************************
//
class IGeometryNormalsGenerator : public IGeometryGenerator
{
public:

    virtual Type    GetType                 () const { return GEOMETRY_NORMALS; };
    
    virtual void    GenerateGeometryNormals ( Float3AttributeChannelPtr posChannel, Float3AttributeChannelPtr normChannel ) = 0;

};

// *********************************
//
class IGeometryNormalsUVsGenerator : public IGeometryGenerator
{
public:

    virtual Type    GetType                     () const { return GEOMETRY_NORMALS_UVS; }

    virtual void    GenerateGeometryNormalsUVs  ( Float3AttributeChannelPtr posChannel, Float3AttributeChannelPtr normChannel, Float2AttributeChannelPtr uvChannel ) = 0;

};


// *********************************
//
class GeometryGeneratorHelper
{
public:
    
    static void     GenerateNonWeightedNormalsFromTriangleStrips    ( Float3AttributeChannelConstPtr posChannel, Float3AttributeChannelPtr normChannel );
    static void     GenerateNonWeightedNormalsFromTriangles         ( IndexedGeometry & mesh, Float3AttributeChannelPtr normChannel );
};

} //model
} //bv