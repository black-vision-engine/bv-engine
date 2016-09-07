#pragma once

#include <vector>

#include "Mathematics/glm_inc.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"

#include "Engine/Types/Enums.h"


namespace bv { namespace model
{

//FIXME: typed instances should be created via factories
class Float4AttributeChannel : public AttributeChannel
{
private:

    std::vector< glm::vec4 >        m_attributes;

public:

    explicit                        Float4AttributeChannel          ( AttributeChannelDescriptorConstPtr desc, const std::string & name, bool readOnly );
    virtual                         ~Float4AttributeChannel         ();

    virtual void                    Update                          ( TimeType t );
    virtual bool                    IsReadOnly                      () const;

    virtual unsigned int            GetNumEntries                   ()                      const;
    virtual const char *            GetData                         ()                      const;

    std::vector< glm::vec4 > &      GetVertices                     ();

    void                            AddAttribute                    ( const glm::vec4 & v );
    void                            AddAttributes                   ( const std::vector< glm::vec4 > & v );

};

DEFINE_PTR_TYPE(Float4AttributeChannel)
DEFINE_CONST_PTR_TYPE(Float4AttributeChannel)


//FIXME: typed instances should be created via factories
class Float3AttributeChannel : public AttributeChannel
{
private:

    std::vector< glm::vec3 >        m_attributes;

public:

    explicit                        Float3AttributeChannel          ( AttributeChannelDescriptorConstPtr desc, const std::string & name, bool readOnly );
    virtual                         ~Float3AttributeChannel         ();

    virtual void                    Update                          ( TimeType t ){ { t; } } // FIXME: suppress unused warning // FIXME: implement
    virtual bool                    IsReadOnly                      () const;

    virtual unsigned int            GetNumEntries                   ()                      const;
    virtual const char *            GetData                         ()                      const;


    std::vector< glm::vec3 > &      GetVertices                     ();

    void                            AddAttribute                    ( const glm::vec3 & v );
    void                            AddAttributes                   ( const std::vector< glm::vec3 > & v );
    void                            ReplaceAttributes               ( std::vector< glm::vec3 > && v );

};

DEFINE_PTR_TYPE(Float3AttributeChannel)
DEFINE_CONST_PTR_TYPE(Float3AttributeChannel)


//FIXME: typed instances should be created via factories
class Float2AttributeChannel : public AttributeChannel
{
private:

    std::vector< glm::vec2 >        m_attributes;

public:

    explicit                        Float2AttributeChannel          ( AttributeChannelDescriptorConstPtr desc, const std::string & name, bool readOnly );

    virtual void                    Update                          ( TimeType t ){ { t; } } // FIXME: suppress unused warning // FIXME: implement
    virtual bool                    IsReadOnly                      () const;

    virtual unsigned int            GetNumEntries                   ()                      const;
    virtual const char *            GetData                         ()                      const;
    
    std::vector< glm::vec2 > &      GetVertices                     ();

    void                            AddAttribute                    ( const glm::vec2 & v );
    void                            AddAttributes                   ( const std::vector< glm::vec2 > & v );

};

DEFINE_PTR_TYPE(Float2AttributeChannel)
DEFINE_CONST_PTR_TYPE(Float2AttributeChannel)

// TODO: Czy kazdy typ moze miec kazda semantyke? 
} // model
} // bv
