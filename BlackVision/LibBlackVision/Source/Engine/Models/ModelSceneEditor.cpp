#include "ModelSceneEditor.h"


namespace bv { namespace model {

// ********************************
//
ModelSceneEditor::ModelSceneEditor                          ( BasicNodePtr & rootNode )
    : m_rootNode( rootNode )
    , m_detachedNode( nullptr )
{
}

// ********************************
//
void                    ModelSceneEditor::SetRootNode       ( BasicNodePtr mainRootNode, BasicNodePtr rootNode )
{
	if( m_rootNode && m_rootNode != rootNode )
	{
		mainRootNode->DetachChildNodeOnly( m_rootNode );
	}

	m_rootNode = rootNode;

	if( rootNode != nullptr )
	{
		mainRootNode->AddChildToModelOnly( rootNode );
	}
}

// ********************************
//
bool                    ModelSceneEditor::DeleteRootNode     ( BasicNodePtr mainRootNode )
{
    if( m_rootNode )
    {
		mainRootNode->DetachChildNodeOnly( m_rootNode );
        m_rootNode = nullptr;

        return true;
    }

    return false;
}

// ********************************
//
void                    ModelSceneEditor::AddChildNode       ( BasicNodePtr parentNode, BasicNodePtr childNode )
{
    if ( parentNode && childNode )
    {
        parentNode->AddChildToModelOnly( childNode );
    }
}

// ********************************
//
bool                    ModelSceneEditor::DeleteChildNode    ( BasicNodePtr parentNode, const std::string & childNodeName )
{
    if( parentNode )
    {
        auto childNode = std::static_pointer_cast< BasicNode >( parentNode->GetChild( childNodeName ) );

        if( childNode )
        {
            parentNode->DetachChildNodeOnly( childNode );

            return true;
        }
    }

    return false;
}

// ********************************
//
void                    ModelSceneEditor::AttachRootNode     ()
{
    m_rootNode = m_detachedNode;
    m_detachedNode = nullptr;
}

// ********************************
//
bool                    ModelSceneEditor::DetachRootNode     ()
{
    m_detachedNode = m_rootNode;
    
    if( m_rootNode )
    {
        m_rootNode = nullptr;

        return true;
    }

    return false;
}

// ********************************
//
bool                    ModelSceneEditor::AttachChildNode    ( BasicNodePtr parent )
{
    if( parent && m_detachedNode )
    {
        parent->AddChildToModelOnly( m_detachedNode );

        m_detachedNode = nullptr;

        return true;
    }

    return false;
}

// ********************************
//
bool                    ModelSceneEditor::DetachChildNode    ( BasicNodePtr parent, const std::string & nodeToDetach )
{
    if( parent )
    {
        auto childNode = std::static_pointer_cast< BasicNode >( parent->GetChild( nodeToDetach ) );

        if( childNode )
        {
            parent->DetachChildNodeOnly( childNode );

            m_detachedNode = childNode;

            return true;
        }
    }

    return false;
}

// ********************************
//
BasicNodePtr     ModelSceneEditor::GetDetachedNode    ()
{
    return m_detachedNode;
}

// ********************************
//
void                    ModelSceneEditor::DeleteDetachedNode ()
{
    m_detachedNode = nullptr;
}

// ********************************
//
BasicNodePtr     ModelSceneEditor::GetRootNode        ()
{
    return m_rootNode;
}

} // model
} // bv
