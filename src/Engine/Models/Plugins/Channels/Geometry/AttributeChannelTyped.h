#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"

#include "Engine/Types/Enums.h"


namespace bv { namespace model
{

//FIXME: typed instances should be created via factories
class Float3AttributeChannel : public AttributeChannel
{
private:

    std::vector< glm::vec3 >        m_attributes;

public:

    explicit                        Float3AttributeChannel          ( const AttributeChannelDescriptor * desc, const std::string & name, bool readOnly );
    virtual                         ~Float3AttributeChannel         ();

    virtual void                    Update                          ( TimeType t ){} // FIXME: implement
    virtual bool                    IsReadOnly                      () const;

    virtual unsigned int            GetNumEntries                   ()                      const;
    virtual const char *            GetData                         ()                      const;


    std::vector< glm::vec3 > &      GetVertices                     ();

    //virtual int                     GetEntrySize                    ()                      const;

    void                            AddAttribute                    ( const glm::vec3 & v );

};


//FIXME: typed instances should be created via factories
class Float2AttributeChannel : public AttributeChannel
{
private:

    std::vector< glm::vec2 >        m_attributes;

public:

    explicit                        Float2AttributeChannel          ( const AttributeChannelDescriptor * desc, const std::string & name, bool readOnly );

    virtual void                    Update                          ( TimeType t ){} // FIXME: implement
    virtual bool                    IsReadOnly                      () const;

    virtual unsigned int            GetNumEntries                   ()                      const;
    virtual const char *            GetData                         ()                      const;
    std::vector< glm::vec2 > &      GetVertices                     ();

    //virtual int                     GetEntrySize                    ()                      const;

    void                            AddAttribute                    ( const glm::vec2 & v );

};

// TODO: Czy kazdy typ moze miec kazda semantyke? 
} // model
} // bv
