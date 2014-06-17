#pragma once

#include "Engine/Interfaces/IUpdater.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannel.h"

namespace bv {

class RenderableEntity;

namespace model
{
    class IVertexAttributesChannel;
    class IConnectedComponent;
}

class GeometryUpdater : public IUpdater
{
private:

    RenderableEntity *                          m_out;
    model::IVertexAttributesChannelConstPtr     m_in;

public:

                    GeometryUpdater     ( RenderableEntity * out, model::IVertexAttributesChannelConstPtr in ); 
                    ~GeometryUpdater    ();

    virtual void    DoUpdate            ();

private:

    void    UpdatePositions     ();
    void    UpdateTopology      ();

    void    WriteVertexDataToVBO( char * data, model::IConnectedComponent * cc );

};

}
