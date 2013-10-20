#pragma once

#include "IUpdater.h"

namespace bv {

class RenderableEntity;

namespace model
{
    class IGeometryChannel;
}

class GeometryUpdater : public IUpdater
{
private:

    RenderableEntity *                  m_out;
    const model::IGeometryChannel *     m_in;

public:

            GeometryUpdater     ( RenderableEntity * out, const model::IGeometryChannel * in ); 
            ~GeometryUpdater    ();

    void    Update              ( float t );

private:

    void    UpdatePositions     ( float t );
    void    UpdateTopology      ( float t );

};

}
