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
    TSA_WeightCenter,
    TSA_BoundingBoxCenter
    //TSA_
};

class TabStop
{
private:

    TabStopType         m_type;
    std::string         m_name;
    float               m_position;

public:
    explicit TabStop( TabStopType type, const std::string& name );
    ~TabStop();

    void                SetPosition     ( float pos )   { m_position = pos; }
    float               GetPosition     ()              { return m_position; }

public:

    void                AlignNode       ( model::BasicNodePtr node, BVProjectEditor* editor );
};

DEFINE_PTR_TYPE( TabStop )

}	// bv
