#include "Parameters.h"

#include <cassert>

namespace bv {

//FIXME: REMOVE FROM PROJECT
void ParametersDummyCaller()
{
    assert( !"Suppress linker warnings" );    
}

//ShaderParameter::ShaderParameter(const std::string& name, char* mem, ParameterSemantic sema)
//    : m_name(name), m_data(mem), m_semantic(sema)
//{}
//
//char* ShaderParameters::getBuffer()
//{
//    if(!m_paramsData)
//    {
//        finalize();
//    }
//
//    return m_paramsData;
//}
//
//void ShaderParameters::finalize()
//{
//    if(m_paramsData)
//        return;
//
//    m_paramsData = nullptr;
//    if(m_capacity > 0)
//    {
//        m_paramsData = new char[m_capacity];
//    }
//}
//
//ShaderParameters::ShaderParameters()
//    : m_capacity(0), m_paramsData(nullptr)
//{}
//
//ShaderParameters::~ShaderParameters()
//{
//    delete m_paramsData;
//}

} // bv