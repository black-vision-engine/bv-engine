#pragma once

#include "Engine/Models/Updaters/IUpdater.h"


namespace bv {

class RenderableEntity;

namespace model
{
    class IGeometryChannel;
    class IConnectedComponent;
    class IGeometryChannelDescriptor;
}

class GeometryUpdater : public IUpdater
{
private:

    RenderableEntity *                  m_out;
    const model::IGeometryChannel *     m_in;

public:

                    GeometryUpdater     ( RenderableEntity * out, const model::IGeometryChannel * in ); 
                    ~GeometryUpdater    ();

    virtual void    Update              ( float t );

private:

    void    UpdatePositions     ( float t );
    void    UpdateTopology      ( float t );

    void    WriteVertexDataToVBO( char * data, model::IConnectedComponent * cc );

};

}
