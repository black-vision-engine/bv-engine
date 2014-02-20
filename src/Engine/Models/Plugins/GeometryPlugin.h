#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model { 

class VertexAttributesChannel;

// ***************************** UID **********************************
class GeometryPluginUID
{
public:

    static const char *       GetName   ()  { return "geometry_plugin"; }

};


// ***************************** PLUGIN ********************************** 
class GeometryPlugin : public BasePlugin< IPlugin >
{
private:

    VertexAttributesChannel *   m_vaChannel;

public:

    explicit            GeometryPlugin  ( model::VertexAttributesChannel * vaChannel );
                        ~GeometryPlugin ();

    const IVertexAttributesChannel *    GetVertexAttributesChannel  () const override;

    virtual void                Update              ( TimeType t ) override;
    virtual void                Print               ( std::ostream & out, int tabs = 0 ) const;

};

} //model
} //bv
