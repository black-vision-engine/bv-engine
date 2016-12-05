#include "stdafx.h"
#include "DefaultWallPlugin.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {
namespace DefaultWallPlugin {

const std::string PN::HEIGHT        = "height";
const std::string PN::WIDTH         = "width";
const std::string PN::DEPTH         = "depth";
const std::string PN::TESSELATION   = "tesselation";
const std::string PN::ANGLE         = "angle";
const std::string PN::FRONT         = "front";



PluginDesc::PluginDesc()
            : DefaultGeometryPluginDescBase( UID(), "wall" ) { }

DefaultPluginParamValModelPtr   PluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.SetOrCreateVacModel();
	h.AddSimpleParam( PN::TESSELATION, 20, true, true );
    h.AddSimpleParam( PN::HEIGHT, 1.f, true, true );
    h.AddSimpleParam( PN::WIDTH, 1.f, true, true );
    h.AddSimpleParam( PN::DEPTH, 0.2f, true, true );
    h.AddSimpleParam( PN::ANGLE, 80.f, true, true );
    h.AddSimpleParam( PN::FRONT, true, true, true );

    h.SetOrCreatePSModel();

    return h.GetModel();
}

IPluginPtr                      PluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultPlugin >( name, prev, timeEvaluator );
}

std::string                     PluginDesc::UID                 ()
{
    return "DEFAULT_WALL";
}



#include "Mathematics/Defines.h"
namespace WallGenerator
{
    int     tesselation;
    float   height,
            width,
            depth,
            angle;
    bool    front;

    class Generator : public IGeometryNormalsUVsGenerator
    {
        glm::vec3 * vert;
        glm::vec3 * norm;
        glm::vec2 * uv;

        // helper variables
        float m_scaleFactor;
        float m_radius;

        // ***********************
        //
        void Init() 
        {
            if( tesselation < 2 )
                tesselation = 2;
            if( angle > 180 )
                angle = 180;

            int attribCount = 2 * tesselation;
            vert = new glm::vec3[ attribCount ];
            norm = new glm::vec3[ attribCount ];
            uv = new glm::vec2[ attribCount ];
        }

        // ***********************
        //
        void Deinit()
        {
            delete[] vert;
            delete[] norm;
            delete[] uv;
        }

        // ***********************
        //
        glm::vec3   ComputeVertex   ( float curAngle, float height, float zTranslate )
        {
            glm::vec3 position;
            position.x = m_radius * sin( curAngle );
            position.y = height;
            position.z = -( m_radius * cos( curAngle ) + zTranslate ) * m_scaleFactor;
            return position;
        }

        // ***********************
        //
        glm::vec2   ComputeUV       ( float curAngle, float maxAngle, bool top )
        {
            if( top )
                return glm::vec2( curAngle / maxAngle, 1.0 );
            else
                return glm::vec2( curAngle / maxAngle, 0.0 );
        }

        // ***********************
        //
        glm::vec3   ComputeNormal   ( int prevIndex, int nextIndex )
        {
            glm::vec3 prevPos = vert[ prevIndex ];
            glm::vec3 nextPos = vert[ nextIndex ];

            glm::vec3 deltaPos = nextPos - prevPos;
            
            auto normal = glm::vec3( -deltaPos.z, 0.0, deltaPos.x );
            if( front )
                return normal;
            else
                return glm::vec3( -normal.x, 0.0, -normal.z );
        }

        // ***********************
        //
        void Generate()
        {
            float radiansAngle = glm::radians( angle ) ;
            float deltaAngle = radiansAngle / (tesselation - 1);
            float angleOffset = -radiansAngle / 2;
            
            float topY = height / 2;
            float bottomY = - topY;

            // Compute member variables.
            m_radius = width / ( 2 * sin( -angleOffset ) );

            float zTranslate = -m_radius * cos( angleOffset );
            m_scaleFactor = depth / ( m_radius + zTranslate );  // zTranslate is negative, thats why I add instead of subtract.


            // Verticies and UVs
            for( int i = 0; i < tesselation; ++i )
            {
                int index = i << 1;
                float delta = i * deltaAngle;
                float curAngle = angleOffset + delta;

                vert[ index ] = ComputeVertex( curAngle, topY, zTranslate );
                vert[ index + 1 ] = ComputeVertex( curAngle, bottomY, zTranslate );

                uv[ index ] = ComputeUV( delta, radiansAngle, true );
                uv[ index + 1 ] = ComputeUV( delta, radiansAngle, false );
            }

            // Normals
            norm[ 0 ] = ComputeNormal( 0, 2 );
            norm[ 1 ] = norm[ 0 ];

            int i = 1;
            for( ; i < tesselation - 1; ++i )
            {
                int index = i << 1;

                norm[ index ] = ComputeNormal( index - 2, index + 2 );
                norm[ index + 1 ] = norm[ index ];
            }

            int index = i << 1;
            norm[ index ] = ComputeNormal( index - 2, index );
            norm[ index + 1 ] = norm[ index ];
        }

        // ***********************
        //
        void FillChannels( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs )
        {
            for( int i = 0; i < 2 * tesselation; ++i )
            {
                verts->AddAttribute( vert[ i ] );
                normals->AddAttribute( norm[ i ] );
                uvs->AddAttribute( uv[ i ] );
            }
        }

        // ***********************
        //
        virtual void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs )
        {
            Init();

            Generate();
            FillChannels( verts, normals, uvs );

            Deinit();
        }
    };

}   //  WallGenerator



DefaultPlugin::DefaultPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : DefaultGeometryPluginBase( name, uid, prev, model ) 
{ 
	m_tesselation = QueryTypedValue< ValueIntPtr >( GetValue( PN::TESSELATION ) );
    m_height = QueryTypedValue< ValueFloatPtr >( GetValue( PN::HEIGHT ) );
    m_width = QueryTypedValue< ValueFloatPtr >( GetValue( PN::WIDTH ) );
    m_depth = QueryTypedValue< ValueFloatPtr >( GetValue( PN::DEPTH ) );
    m_angle = QueryTypedValue< ValueFloatPtr >( GetValue( PN::ANGLE ) );
    m_front = QueryTypedValue< ValueBoolPtr >( GetValue( PN::FRONT ) );

    m_pluginParamValModel->Update();
    InitGeometry();
}

std::vector<IGeometryGeneratorPtr>    DefaultPlugin::GetGenerators()
{
    WallGenerator::tesselation  = m_tesselation->GetValue();
    WallGenerator::height       = m_height->GetValue();
    WallGenerator::width        = m_width->GetValue();
    WallGenerator::depth        = m_depth->GetValue();
    WallGenerator::angle        = m_angle->GetValue();
    WallGenerator::front        = m_front->GetValue();

    std::vector<IGeometryGeneratorPtr> gens;
    gens.push_back( std::make_shared< WallGenerator::Generator >() );


    return gens;
}

bool                                DefaultPlugin::NeedsTopologyUpdate()
{
    return ParameterChanged( PN::HEIGHT ) ||
		ParameterChanged( PN::TESSELATION ) ||
        ParameterChanged( PN::HEIGHT ) ||
        ParameterChanged( PN::DEPTH ) ||
        ParameterChanged( PN::ANGLE ) ||
        ParameterChanged( PN::FRONT );
}


}   // DefaultWallPlugin
}   // model
}	// bv
