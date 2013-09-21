#pragma once

#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannel.h"
#include "Engine\Types\Enums.h"

#include <glm/glm.hpp>

#include <vector>

namespace bv { namespace model
{

class VertexAttributeChannel : public IVertexAttributeChannel
{
    AttrType                        m_type;
    AttrSemantic                    m_semantic;

public:
    virtual AttrType                GetType         ()                      const;
    virtual AttrSemantic            GetSemantic     ()                      const;

    explicit                        VertexAttributeChannel(AttrType type, AttrSemantic semantic);
};

class Float3VertexAttributeChannel : public VertexAttributeChannel
{
    std::vector< glm::vec3 >        m_f3attritutes;
public:
    virtual void                    Update( float t ){} // FIXME: implement

    virtual int                     GetNumEntries   ()                      const;
    virtual const char*             GetData         ()                      const;

    virtual int                     GetEntrySize    ()                      const;
    void                            AddVertexAttribute(const glm::vec3& v);

    explicit                        Float3VertexAttributeChannel(AttrSemantic semantic);
};

class Float2VertexAttributeChannel : public VertexAttributeChannel
{
    std::vector< glm::vec2 >        m_f2attritutes;
public:
    virtual void                    Update( float t ){} // FIXME: implement

    virtual int                     GetNumEntries   ()                      const;
    virtual const char*             GetData         ()                      const;

    virtual int                     GetEntrySize    ()                      const;
    void                            AddVertexAttribute(const glm::vec2& v);

    explicit                        Float2VertexAttributeChannel(AttrSemantic semantic);
};


// TODO: Czy kazdy typ moze mieæ ka¿d¹ semantykê? 
} // model
} // bv