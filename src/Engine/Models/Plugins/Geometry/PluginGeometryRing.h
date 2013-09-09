#pragma once

#include <vector>
#include "glm/glm.hpp"

#include "PluginGeometry.h"

namespace bv
{

class PluginGeometryRing  : public PluginGeometry
{
	FloatInterpolator m_InnerRadius;
	FloatInterpolator m_OuterRadius;

	FloatInterpolator m_StartAngle;
	FloatInterpolator m_EndAngle;


	bool        m_ccw;
    Adjacency   m_adjacency;
	int			m_numSegments;

public:

	PluginGeometryRing(	const FloatInterpolator& innerRadius, const FloatInterpolator& outerRadius,
						const FloatInterpolator& startAngle, const FloatInterpolator& endAngle, int numSegments);

	virtual bool                IsTimeInvariant     () const ;
    virtual Adjacency           AdjacencyType       () const ;

    virtual void                ConvertToTraingles  () ;
    virtual void                ConvertToMesh       () ;

    virtual bool                IsCW                () const ;
    virtual bool                IsCCW               () const ;

    virtual void                SetCW               () ;
    virtual void                SetCCW              () ;

    virtual bool                IsVertexNumConst    () const ;
    virtual bool                IsAdjacencyConst    () const ;
    virtual bool                IsVertexTypeConst   () const ;

	virtual void                Update				(float t);


	virtual ~PluginGeometryRing(){}

public:
	void						TriStripRep             ();

};

}