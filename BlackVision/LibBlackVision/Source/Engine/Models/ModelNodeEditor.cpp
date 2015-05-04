#include "ModelNodeEditor.h"

namespace bv { namespace model {

// *******************************
//
				ModelNodeEditor::ModelNodeEditor		( model::BasicNodePtr model )
    : m_model( model )
{
}

// ********************************
//
void			ModelNodeEditor::AddPlugin				( model::IPluginPtr plugin )
{
	if( plugin )
	{
		m_model->GetPlugins()->AttachPlugin( plugin );
	}
}

// ********************************
//
bool			ModelNodeEditor::DeletePlugin			( const std::string & name )
{
	if( m_model->GetPlugins()->DetachPlugin( name ) )
	{
		return true;
	}
	return false;
}

// ********************************
//
void			ModelNodeEditor::AttachPlugin			()
{
	if( m_detachedPlugin )
    {
		m_model->GetPlugins()->AttachPlugin( m_detachedPlugin );
        m_detachedPlugin = nullptr;
    }
}

// ********************************
//
bool				ModelNodeEditor::DetachPlugin		( const std::string & name )
{
	if ( m_detachedPlugin )
	{
		m_detachedPlugin = m_model->GetPlugins()->DetachPlugin( name );
		return true;
	}

    return false;
}

// ********************************
//
IPluginPtr			ModelNodeEditor::GetDetachedPlugin	()
{
    return m_detachedPlugin;
}

// ********************************
//
void				ModelNodeEditor::DeleteDetachedPlugin	()
{
    m_detachedPlugin = nullptr;
}


} //model
} //bv