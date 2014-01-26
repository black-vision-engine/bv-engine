#pragma once

#include "Engine/Interfaces/IUpdater.h"


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
    const model::IVertexAttributesChannel *     m_in;

public:

                    GeometryUpdater     ( RenderableEntity * out, const model::IVertexAttributesChannel * in ); 
                    ~GeometryUpdater    ();

    virtual void    DoUpdate            ( TimeType t );

private:

    void    UpdatePositions     ( TimeType t );
    void    UpdateTopology      ( TimeType t );

    void    WriteVertexDataToVBO( char * data, model::IConnectedComponent * cc );

};

}
