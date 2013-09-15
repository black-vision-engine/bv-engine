#pragma once

#include "Engine/Models/Plugins/PluginParameters.h"
#include "PluginGeometry.h"

namespace bv
{
// ***************************** DESCRIPTOR **********************************
class PluginGeometryUVSinglePD : public BaseParametersDescriptor
{
public:

    static const std::string pluginName;

    explicit PluginGeometryUVSinglePD();

};


// ***************************** PLUGIN ********************************** 
class PluginGeometryUVSingle : public PluginGeometry< PluginGeometryUVSinglePD >
{
private:

    IGeometryGenPlugin  * m_predcessor;

public:

                        PluginGeometryUVSingle  ( IGeometryGenPlugin * pPrev );
    virtual             ~PluginGeometryUVSingle ();

    virtual const IGeometryGenPlugin::TVertexVec & Vertices  () const { return m_predcessor->Vertices(); }
    virtual const IGeometryGenPlugin::TIndexVec &  Indices   () const { return m_predcessor->Indices(); }

    virtual bool        IsTimeInvariant         () const { return true; }
    virtual Adjacency   AdjacencyType           () const { return m_predcessor->AdjacencyType(); }

    virtual void        ConvertToTraingles      () { m_predcessor->ConvertToTraingles(); }
    virtual void        ConvertToMesh           () { m_predcessor->ConvertToMesh(); }

    virtual bool        IsCW                    () const { return m_predcessor->IsCW(); }
    virtual bool        IsCCW                   () const { return m_predcessor->IsCCW(); };

    virtual void        SetCW                   () { m_predcessor->SetCW(); }
    virtual void        SetCCW                  () { m_predcessor->SetCW(); }
    virtual void        Update                  ( float t ) override { m_predcessor->Update( t ); }

    virtual bool        IsVertexNumConst        () const { return true; };
    virtual bool        IsAdjacencyConst        () const { return true; };
    virtual bool        IsVertexTypeConst       () const { return true; };

};

}
