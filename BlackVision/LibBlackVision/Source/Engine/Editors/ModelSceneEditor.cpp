#include "stdafx.h"

#include "ModelSceneEditor.h"
#include "Engine/Models/ModelState.h"



#include "Memory/MemoryLeaks.h"



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
	if( m_rootNode && m_rootNode.get() != rootNode.get() )
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
        ModelState::GetInstance().UnselectRecursive( m_rootNode );
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
bool                    ModelSceneEditor::DeleteChildNode    ( BasicNodePtr parentNode, BasicNodePtr childNode )
{
    if( parentNode && childNode )
    {
        ModelState::GetInstance().UnselectRecursive( childNode );
        parentNode->DetachChildNodeOnly( childNode );

        return true;
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
	return AttachChildNode( parent, ( UInt32 )parent->GetNumChildren() );
}

// ********************************
//
bool                    ModelSceneEditor::AttachChildNode    ( BasicNodePtr parent, UInt32 destIdx )
{
    if( parent && m_detachedNode )
    {
        parent->AddChildToModelOnly( m_detachedNode, destIdx );

        m_detachedNode = nullptr;

        return true;
    }

    return false;
}

// ********************************
//
bool                    ModelSceneEditor::DetachChildNode    ( BasicNodePtr parent, BasicNodePtr nodeToDetach )
{
    if( parent && nodeToDetach )
    {
        parent->DetachChildNodeOnly( nodeToDetach );

        m_detachedNode = nodeToDetach;

        return true;
    }

    return false;
}

// ********************************
//
BasicNodePtr    ModelSceneEditor::GetDetachedNode   () const
{
    return m_detachedNode;
}

// ********************************
//
void            ModelSceneEditor::DeleteDetachedNode()
{
    if( m_detachedNode )
        ModelState::GetInstance().UnselectRecursive( m_detachedNode );
    
    m_detachedNode = nullptr;
}

// ********************************
//
BasicNodePtr    ModelSceneEditor::GetRootNode       () const
{
    return m_rootNode;
}

// ********************************
//
IModelNodePtr   ModelSceneEditor::GetNode           ( const std::string & path, const std::string & separator ) const
{
    if( m_rootNode )
    {
        std::string childPath = path;
        auto rootName = BasicNode::SplitPrefix( childPath, separator );
        auto rootIdx = BasicNode::TryParseIndex( rootName );

        if( ( rootIdx == 0 ) || ( rootIdx < 0 && ( m_rootNode->GetName() == rootName ) ) )
        {
            return m_rootNode->GetNode( childPath, separator );
        }
    }

    return nullptr;
}

} // model
} // bv
