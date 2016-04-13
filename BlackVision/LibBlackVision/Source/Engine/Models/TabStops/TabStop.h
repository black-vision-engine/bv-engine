#pragma once

#include <string>

#include "Engine/Models/BasicNode.h"


namespace bv
{

class BVProjectEditor;


enum TabStopType
{
    TST_Vertical,
    TST_Horizontal,

    TST_TOTAL
};


enum TabStopAlignement
{
    TSA_GeometryCenter,     ///< Point ( 0.0, 0.0, 0.0 )
    TSA_WeightCenter,
    TSA_BoundingBoxCenter,
    TSA_BoundingBoxMajor,   ///< Maximal coordinate of bounding box
    TSA_BoundingBoxMinor,   ///< Minimal coordinate of bounding box

    TSA_TOTAL
};

class TabStop
{
private:

    TabStopType         m_type;
    std::string         m_name;
    float               m_position;

public:
    explicit TabStop    ( TabStopType type, const std::string & name );
    ~TabStop();

    void                SetPosition     ( float pos )   { m_position = pos; }
    float               GetPosition     ()              { return m_position; }

public:

    bool                AlignNode       ( model::BasicNodePtr node, TabStopAlignement alignement );

private:

    glm::vec3           ReferencePos        ( model::BasicNodePtr& node, TabStopAlignement alignement );
    glm::vec3           ComputeTranslation  ( glm::vec3 referencePosition );
    bool                UpdateTransform     ( model::BasicNodePtr& node, glm::vec3 translation );
};

DEFINE_PTR_TYPE( TabStop )

}	// bv
