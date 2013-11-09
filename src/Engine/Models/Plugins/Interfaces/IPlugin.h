#pragma once

#include "IParameter.h"
#include <string>
#include <vector>
#include <iostream>

namespace bv { namespace model {

class IGeometryChannel;
class ITransformChannel;
class IPixelShaderChannel;
class IVertexShaderChannel;
class IGeometryShaderChannel;
class ResourceHandle;

struct TextureInfo;

class IPlugin
{
public:
    typedef std::vector< TextureInfo* >         Textures;

public:

    virtual const std::string &                 GetName                     () const = 0;

    virtual const IGeometryChannel*             GetGeometryChannel          () const = 0;
    virtual const ITransformChannel*            GetTransformChannel         () const = 0;
    virtual const IPixelShaderChannel*          GetPixelShaderChannel       () const = 0;
    virtual const IVertexShaderChannel*         GetVertexShaderChannel      () const = 0;
    virtual const IGeometryShaderChannel*       GetGeometryShaderChannel    () const = 0;

    virtual const Textures&                     GetTextures                 () const = 0; // FIXME: use resources

    virtual void                                Update                      ( float t )                                   = 0;
    virtual void                                Print                       ( std::ostream & out, int tabs = 0 )    const = 0;

    virtual const std::vector<IValue*> &        GetValuesList               () const = 0;
    virtual const std::vector<IParameter*> &    GetParametersList           () const = 0;

    virtual                                     ~IPlugin                    (){}

};

//class IShaderPlugin : public IPlugin
//{
//public:
//
//    virtual std::string         GetShaderFile(/* TODO; Przekazaæ tu cos */) const  = 0;
//};
//
//class IGeometryGenPlugin : public IPlugin
//{
//public:
//    typedef std::vector< glm::vec3 >    TVertexVec;
//    typedef std::vector< int >          TIndexVec;
//
//    enum class Role : int
//    {
//        R_SOURCE = 0,
//        R_PROCESSOR,
//
//        R_TOTAL
//    };
//
//
//    enum class Adjacency : int
//    {
//        A_TRIANGLES = 0,
//        A_TRIANGLE_STRIP,
//        A_TRIANGLE_MESH,
//
//        A_TOTAL
//    };
//
//
//    virtual bool                IsTimeInvariant     () const = 0;
//    virtual Adjacency           AdjacencyType       () const = 0;
//
//    virtual void                ConvertToTraingles  () = 0;
//    virtual void                ConvertToMesh       () = 0;
//
//    virtual bool                IsCW                () const = 0;
//    virtual bool                IsCCW               () const = 0;
//
//    virtual void                SetCW               () = 0;
//    virtual void                SetCCW              () = 0;
//
//    virtual bool                IsVertexNumConst    () const = 0;
//    virtual bool                IsAdjacencyConst    () const = 0;
//    virtual bool                IsVertexTypeConst   () const = 0;
//
//    virtual const TVertexVec &  Vertices            () const = 0;
//    virtual const TIndexVec &   Indices             () const = 0;
//
//    virtual int                 VertexSize          () const = 0;
//
//    virtual Role                GetRole             () const = 0;
//    virtual void                SetRole             ( Role role ) = 0;
//
//
//    virtual ~IGeometryGenPlugin(){}
//};

//typedef std::vector< ITransformPlugin * > TTransformPluginVec;
//typedef std::vector< IGeometryGenPlugin * > TGeometryPluginVec;

} // model
} // bv