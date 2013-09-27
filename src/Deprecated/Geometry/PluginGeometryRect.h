#pragma once

#include <vector>
#include "glm/glm.hpp"

#include "Engine/Models/Plugins/PluginParameters.h"
#include "PluginGeometry.h"

namespace bv 
{

//// ***************************** DESCRIPTOR **********************************
//class PluginGeometryRectPD : public BaseParametersDescriptor
//{
//public:
//
//    static const std::string pluginName;
//
//    explicit PluginGeometryRectPD();
//
//};
//
//
//// ***************************** PLUGIN ********************************** 
//class PluginGeometryRect : public PluginGeometry< PluginGeometryRectPD >
//{
//private:
//
//    typedef PluginGeometry::Adjacency Adjacency;
//
//private:
//
//    float       m_width;
//    float       m_height;
//
//    bool        m_ccw;
//    Adjacency   m_adjacency;
//
//public:
//
//                        PluginGeometryRect  ( float w = 1.0f, float h = 1.0f );
//    virtual             ~PluginGeometryRect ();
//
//    virtual bool        IsTimeInvariant         () const;
//    virtual Adjacency   AdjacencyType           () const;
//
//    virtual void        ConvertToTraingles      ();
//    virtual void        ConvertToMesh           ();
//
//    virtual bool        IsCW                    () const;
//    virtual bool        IsCCW                   () const;
//
//    virtual void        SetCW                   ();
//    virtual void        SetCCW                  ();
//    virtual void        Update                  (float t) override;
//
//    virtual bool        IsVertexNumConst        () const { return true; };
//    virtual bool        IsAdjacencyConst        () const { return true; };
//    virtual bool        IsVertexTypeConst       () const { return true; };
//
//
//private:
//
//    void                TriStripRep             ();
//    void                TriangleRep             ();
//    void                MeshRep                 ();
//
//};
}
