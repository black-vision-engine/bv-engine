#include "ProjectEditorUtils.h"



namespace bv
{



bv::ProjectEditorContext::ProjectEditorContext()
{
    m_project = BVProject::Create( nullptr, nullptr );
    m_editor = m_project->GetProjectEditor();

    assert( m_project );
    assert( m_editor );
}


}	// bv


