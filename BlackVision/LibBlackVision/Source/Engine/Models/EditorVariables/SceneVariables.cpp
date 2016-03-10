#include "stdafx.h"
#include "SceneVariables.h"

#include "Serialization/IDeserializer.h"
#include "Serialization/ISerializer.h"



namespace bv
{

// ***********************
//
SceneVariables::SceneVariables()
{}

// ***********************
//
SceneVariables::~SceneVariables()
{}

// ***********************
//
void                SceneVariables::Serialize       ( ISerializer & ser ) const
{
    ser.EnterChild( "sceneVariables" );
        ser.EnterArray( "variablesDict" );
            
            for( auto var : m_varCollection )
            {
                ser.EnterChild( "variable" );

                    ser.SetAttribute( "variableName", var.first );
                    ser.SetAttribute( "variableContent", var.second );

                ser.ExitChild();   //  variable
            }

        ser.ExitChild();   //  variablesDict
    ser.ExitChild();    // sceneVariables
}

// ***********************
//
void                SceneVariables::Deserialize     ( const IDeserializer & deser )
{
    if( deser.EnterChild( "variablesDict" ) )
    {
        if( deser.EnterChild( "variable" ) )
        {
            do
            {
                std::string variableName = deser.GetAttribute( "variableName" );
                std::string variableContent = deser.GetAttribute( "variableContent" );

                // Warn (by assert :P) in debug mode, overwrite variable in release.
                assert( m_varCollection.find( variableName ) == m_varCollection.end() );

                m_varCollection[ variableName ] = variableContent;

            } while( deser.NextChild() );

            deser.ExitChild();  //  variable
        }

        deser.ExitChild();  //  variablesDict
    }
}


// ***********************
//
bool                SceneVariables::AddVariable     ( const std::string & varName, const std::string & varContent )
{
    auto iter = m_varCollection.find( varName );
    if( iter != m_varCollection.end() )
        return false;

    m_varCollection[ varName ] = varContent;
    return true;
}

// ***********************
//
bool                SceneVariables::DeleteVariable  ( const std::string & varName )
{
    auto iter = m_varCollection.find( varName );
    if( iter == m_varCollection.end() )
        return false;

    m_varCollection.erase( iter );

    return true;
}

// ***********************
//
Expected< std::string >     SceneVariables::GetVariable     ( const std::string & varName ) const
{
    auto iter = m_varCollection.find( varName );
    if( iter == m_varCollection.end() )
        return Expected< std::string >();

    return Expected< std::string >( iter->second );
}

}	// bv