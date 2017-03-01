#pragma once

#include "Engine/Models/BVProject.h"
#include "Engine/Models/BVProjectEditor.h"



namespace bv
{



/// Minimal context for ProjectEditor.
class ProjectEditorContext
{
private:

    BVProjectPtr        m_project;
    BVProjectEditor*    m_editor;

public:

    explicit ProjectEditorContext();



    BVProjectEditor*    Editor  ()      { return m_editor; }

};



}	// bv