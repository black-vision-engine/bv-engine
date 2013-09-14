#pragma once

#include <vector>
#include "glm/glm.hpp"

#include "Engine/Models/Plugins/PluginParameters.h"
#include "PluginGeometry.h"

namespace bv
{

// ***************************** DESCRIPTOR **********************************
class PluginGeometryRingPD : public BaseParametersDescriptor
{
public:

    static const std::string pluginName;

    explicit PluginGeometryRingPD();

};


// ***************************** PLUGIN ********************************** 
class PluginGeometryRing  : public PluginGeometry< PluginGeometryRingPD >
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