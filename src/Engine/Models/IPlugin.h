#pragma once

#include "IParameter.h"
#include <string>
#include <vector>
#include <iostream>

namespace bv {

//class ParamDesc
//{
//    std::string         m_name;
//    ParamType           m_type;
//    ParameterSemantic   m_semantic;
//
//public:
//    ParamType           getParamType() const { return m_type; }
//    const std::string&  getName() const { return m_name; }
//    ParameterSemantic   getSemantic() const { return m_semantic; }
//
//    explicit ParamDesc(const std::string& name, TypeKind type, ParameterSemantic semantic)
//        : m_name(name), m_type(type), m_semantic(semantic)
//    {}
//};
//
//class ParametersDescriptors
//{
//private:
//    std::vector<ParamDesc > m_params;
//
//public:
//    void    addParam(const std::string& name, TypeKind type, ParameterSemantic semantic = ParameterSemantic::NONE) { m_params.push_back(ParamDesc(name, type, semantic)); }
//    void    merge(const ParametersDescriptors& paramDesc)
//    {
//        for(auto p : paramDesc.m_params)
//        {
//            m_params.push_back(p);
//        }
//    }
//
//    int     size() const { return m_params.size(); }
//    
//    const ParamDesc& getParam(int i) const { return m_params[i]; }
//};

class IPlugin
{
public:

    virtual const std::string &                 GetName             ()                                      const = 0;

    //virtual bool                              IsUpToDate          ()                                      const = 0;
    virtual void                                Update              ( float t )                                   = 0;
    virtual void                                Print               ( std::ostream & out, int tabs = 0 )    const = 0;

    virtual                                     ~IPlugin            (){}

    virtual const std::vector<IValue*> &        GetValuesList       ()                                      const = 0;
    virtual const std::vector<IParameter*> &    GetParametersList   ()                                      const = 0;

};

class IShaderPlugin : public IPlugin
{
public:

    virtual std::string         GetShaderFile(/* TODO; Przekazaæ tu coœ */) const  = 0;
};

class IGeometryGenPlugin : public IPlugin
{
public:
    typedef std::vector< glm::vec3 >    TVertexVec;
    typedef std::vector< int >          TIndexVec;

    enum class Role : int
    {
        R_SOURCE = 0,
        R_PROCESSOR,

        R_TOTAL
    };


    enum class Adjacency : int
    {
        A_TRIANGLES = 0,
        A_TRIANGLE_STRIP,
        A_TRIANGLE_MESH,

        A_TOTAL
    };


    virtual bool                IsTimeInvariant     () const = 0;
    virtual Adjacency           AdjacencyType       () const = 0;

    virtual void                ConvertToTraingles  () = 0;
    virtual void                ConvertToMesh       () = 0;

    virtual bool                IsCW                () const = 0;
    virtual bool                IsCCW               () const = 0;

    virtual void                SetCW               () = 0;
    virtual void                SetCCW              () = 0;

    virtual bool                IsVertexNumConst    () const = 0;
    virtual bool                IsAdjacencyConst    () const = 0;
    virtual bool                IsVertexTypeConst   () const = 0;

    virtual const TVertexVec &  Vertices            () const = 0;
    virtual const TIndexVec &   Indices             () const = 0;

    virtual int                 VertexSize          () const = 0;

    virtual Role                GetRole             () const = 0;
    virtual void                SetRole             ( Role role ) = 0;


    virtual ~IGeometryGenPlugin(){}
};

class ITransformPlugin : public IPlugin
{
public:

    virtual const glm::mat4 &   TransformMatrix     () const = 0;
};

typedef std::vector< ITransformPlugin * > TTransformPluginVec;
typedef std::vector< IGeometryGenPlugin * > TGeometryPluginVec;

}