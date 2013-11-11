#pragma once

#include <map>
#include <vector>

#include "Engine/Graphics/Renderers/OGLRenderer/glslprogram.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"


//FIXME: REMOVE FROM PROJECT
namespace bv {

void ParametersDummyCaller();

//class ShaderParameters
//{
//    int m_capacity;
//
//    char* m_paramsData;
//
//    void finalize();
//public:
//
//    template<typename InterpolatorType>
//    void registerParameter(const InterpolatorType&)
//    {
//        m_capacity += InterpolatorType::value_size;
//    }
//
//    char* getBuffer();
//
//    ShaderParameters();
//    virtual ~ShaderParameters();
//    //static ShaderParameters& get();
//};
//
//class ShaderParameter
//{
//protected:
//    char* m_data;
//    std::string m_name;
//    ParameterSemantic m_semantic;
//
//public:
//    virtual void update(GLSLProgram * p) = 0;
//
//    ShaderParameter(const std::string& name, char* mem, ParameterSemantic pramSemantic = ParameterSemantic::NONE);
//
//    char*   getLocalBuffer()    { return m_data; }
//
//    ParameterSemantic   getSemantic() const { return m_semantic; }
//    const std::string&  getName() const { return m_name; }
//
//protected:
//
//    template<typename ParamType>
//    void setUniform(GLSLProgram * p)
//    {
//        p->SetUniform( m_name.c_str(), *((ParamType*)m_data) );
//    }
//};
//
//template<typename ValueType>
//class ShaderParameterT : public ShaderParameter
//{
//public:
//    virtual void update(GLSLProgram * p)
//    {
//        ShaderParameter::setUniform<ValueType>(p);
//    }
//
//    ShaderParameterT(const std::string& name, char* mem, ParameterSemantic semantic = ParameterSemantic::NONE)
//        : ShaderParameter(name, mem, semantic) 
//    {}
//};
//
//
//typedef ShaderParameterT<float> ShaderParameterFloat;
//typedef ShaderParameterT<glm::vec3> ShaderParameterVec3;
//typedef ShaderParameterT<glm::vec4> ShaderParameterVec4;
//typedef ShaderParameterT<glm::mat4x4> ShaderParameterMat4;


} // bv