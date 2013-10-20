#pragma once

#include "Engine\Models\Plugins\Channels\Geometry\VertexAttributeChannel.h"
#include "Engine\Models\Plugins\Channels\Geometry\VertexAttributeChannelDescriptor.h"

#include "Engine\Types\Enums.h"

#include <glm/glm.hpp>

#include <vector>

namespace bv { namespace model
{

//FIXME: typed instances should be created via factories
class Float3VertexAttributeChannel : public VertexAttributeChannel
{
private:

    std::vector< glm::vec3 >        m_attributes;

public:

    explicit                        Float3VertexAttributeChannel    ( const VertexAttributeChannelDescriptor * desc, const std::string & name, bool readOnly );
    virtual                         ~Float3VertexAttributeChannel   ();

    virtual void                    Update                          ( float t ){} // FIXME: implement
    virtual bool                    IsReadOnly                      () const;

    virtual unsigned int            GetNumEntries                   ()                      const;
    virtual const char *            GetData                         ()                      const;

    std::vector< glm::vec3 > &      GetVertices                     ();

    //virtual int                     GetEntrySize                    ()                      const;

    void                            AddVertexAttribute              ( const glm::vec3 & v );

};


//FIXME: typed instances should be created via factories
class Float2VertexAttributeChannel : public VertexAttributeChannel
{
private:

    std::vector< glm::vec2 >        m_attributes;

public:

    explicit                        Float2VertexAttributeChannel    ( const VertexAttributeChannelDescriptor * desc, const std::string & name, bool readOnly );

    virtual void                    Update                          ( float t ){} // FIXME: implement
    virtual bool                    IsReadOnly                      () const;

    virtual unsigned int            GetNumEntries                   ()                      const;
    virtual const char *            GetData                         ()                      const;

    //virtual int                     GetEntrySize                    ()                      const;

    void                            AddVertexAttribute              ( const glm::vec2 & v );

};


// TODO: Czy kazdy typ moze mieæ ka¿d¹ semantykê? 
} // model
} // bv
