#include "DefaultTorusPlugin.h"

namespace bv { namespace model {
	
typedef ParamEnum< DefaultTorus::Plugin::OpenAngleMode > ParamEnumOAM;

VoidPtr    ParamEnumOAM::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultTorus::Plugin::OpenAngleMode >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultTorus::Plugin::OpenAngleMode >( name, timeline );
}

#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.inl"
	
namespace DefaultTorus {

const std::string PN::OPENANGLE = "open angle";
const std::string PN::OPENANGLEMODE = "open angle mode";
const std::string PN::RADIUS = "radius";
const std::string PN::RADIUSCROSSSECTION = "radius2";
const std::string PN::TESSELATION = "tesselation";


PluginDesc::PluginDesc()
    : DefaultGeometryPluginDescBase( UID(), "torus" )
{
}

DefaultPluginParamValModelPtr   PluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.CreateVacModel();
    h.AddSimpleParam( PN::TESSELATION, 10, true, true );
    h.AddSimpleParam( PN::RADIUS, 1.f, true, true );
    h.AddSimpleParam( PN::RADIUSCROSSSECTION, 0.1f, true, true );
    h.AddSimpleParam( PN::OPENANGLE, 360.f, true, true );
    h.AddParam< IntInterpolator, DefaultTorus::Plugin::OpenAngleMode, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumOAM >
        ( DefaultTorus::PN::OPENANGLEMODE, DefaultTorus::Plugin::OpenAngleMode::CW, true, true );

    return h.GetModel();
}

IPluginPtr                      PluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< Plugin >( name, prev, timeEvaluator );
}

std::string                     PluginDesc::UID                 ()
{
    return "DEFAULT_TORUS";
}


bool                                Plugin::NeedsTopologyUpdate()
{
    return ParameterChanged( PN::TESSELATION ) ||
        ParameterChanged( PN::RADIUS ) ||
        ParameterChanged( PN::RADIUSCROSSSECTION ) ||
        ParameterChanged( PN::OPENANGLE ) ||
		ParameterChanged( PN::OPENANGLEMODE );
}

Plugin::Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : DefaultGeometryPluginBase( name, uid, prev, model )
{
    m_tesselation = QueryTypedValue< ValueIntPtr >( GetValue( PN::TESSELATION ) );
    m_radius = QueryTypedValue< ValueFloatPtr >( GetValue( PN::RADIUS ) );
    m_radiusCrossSection = QueryTypedValue< ValueFloatPtr >( GetValue( PN::RADIUSCROSSSECTION ) );
    m_openAngle = QueryTypedValue< ValueFloatPtr >( GetValue( PN::OPENANGLE ) );
	m_openAngleMode = QueryTypedParam< std::shared_ptr< ParamEnum< OpenAngleMode > > >( GetParameter( PN::OPENANGLEMODE ) );
    
	
	m_pluginParamValModel->Update();
    InitGeometry();
}



#include "Mathematics/Defines.h"
class Generator : public IGeometryAndUVsGenerator
{
    int tesselation;
    float radius, radius2, openangle;
	Plugin::OpenAngleMode open_angle_mode;

    glm::vec3 **v;
    int n, m;
public:
    Generator( int t, float r, float r2, float oa, Plugin::OpenAngleMode oam )
		: tesselation( t ), radius( r ), radius2( r2), openangle( oa ), open_angle_mode( oam ) { }

    Type GetType() { return Type::GEOMETRY_AND_UVS; }

	float computeAngle2Clamped( float angle, float stripe_num )
	{
		float ret_value = angle * ( stripe_num + 1 );
		if( openangle > 0.0 )
		{
			float max_angle = float( TWOPI - TO_RADIANS( openangle ) );
			if( ret_value > max_angle )
				return max_angle;
		}
		//else if( openangle == 360 )
		//{
		//	return float( TWOPI );
		//}

		return ret_value;
	}

	float computeAngleOffset( Plugin::OpenAngleMode mode, float open_angle )
	{
		float angle_offset = 0.0f;

		if( mode == Plugin::OpenAngleMode::CW )
			angle_offset = -float( PI /2 );
		else if( mode == Plugin::OpenAngleMode::CCW )
			angle_offset = float( TO_RADIANS( open_angle ) - PI /2 );
		else if( mode == Plugin::OpenAngleMode::SYMMETRIC )
			angle_offset = float( TO_RADIANS( open_angle / 2 ) - PI /2 );
		else
			assert( false );

		return angle_offset;
	}

	void generateClosure( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, int j, float angle_offset )
	{
		double theta = computeAngle2Clamped( float( TWOPI / tesselation), float( j - 1 ) ) + angle_offset;
		//double theta = j * TWOPI / tesselation + angle_offset;
		double cos_theta = cos( theta );
		double sin_theta = sin( theta );

		for( int i = 0; i <= tesselation; ++i )
		{
			double phi = i * TWOPI / tesselation + angle_offset;

			verts->AddAttribute( glm::vec3( cos_theta*( radius + radius2*cos( phi ) ), sin_theta * ( radius + radius2 * cos(phi) ), radius2 * sin(phi) ) );
			uvs->AddAttribute( glm::vec2( float(i) / tesselation, float(j) / tesselation ) );

			verts->AddAttribute( glm::vec3( cos_theta* radius , sin_theta * radius, radius2 * sin(phi) ) );
			uvs->AddAttribute( glm::vec2( float(i) / tesselation, float(j) / tesselation ) );
		}
	}

	glm::vec2 getUV( int i, int j, int maxLoop )
	{
		glm::vec2 uv( float(i) / tesselation, float(j) / tesselation );
		if( j == maxLoop - 1 )
			uv.y = float( j - 1.0 / 2.0 ) / tesselation;
		return uv;
	}

    void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs ) override
    {
    //    Init();
    //    GenerateV();
    //    CopyV( verts, uvs );
    //    Deinit();
		float angle_offset = computeAngleOffset( open_angle_mode, openangle );

		int max_loop;
		if( openangle != 0.0/* && openangle != 360 */)		// Uncomment if you want to see organ.
			max_loop = static_cast<int>( ceil( float( ( TWOPI - TO_RADIANS( openangle ) ) / ( TWOPI / tesselation ) ) ) );
		else
			max_loop = tesselation;

		// We close beginning of the torus (only if there's an openangle set)
		if( openangle != 0.0f )
			generateClosure( verts, uvs, 0, angle_offset );

        for( int j = 0; j < max_loop; j++ )
            for( int i = 0; i <= tesselation; i++ )
            {
                double phi = i * TWOPI / tesselation + angle_offset;
                double theta = j * TWOPI / tesselation + angle_offset;

                verts->AddAttribute( glm::vec3( cos( theta )*( radius + radius2*cos( phi ) ), sin(theta) * ( radius + radius2 * cos(phi) ), radius2 * sin(phi) ) );
                uvs->AddAttribute( glm::vec2( float(i) / tesselation, float(j) / tesselation ) );

                phi = i * TWOPI / tesselation + angle_offset;
				theta = computeAngle2Clamped( float( TWOPI / tesselation), float( j ) ) + angle_offset;		//zamiast theta = (j+1) * 2*PI / tesselation;

                verts->AddAttribute( glm::vec3( cos( theta )*( radius + radius2*cos( phi ) ), sin(theta) * ( radius + radius2 * cos(phi) ), radius2 * sin(phi) ) );
				if( j < max_loop - 1 )
					uvs->AddAttribute( glm::vec2( float(i) / tesselation, float(j+1) / tesselation ) );
				else
					uvs->AddAttribute( glm::vec2( float(i) / tesselation, ( theta - angle_offset ) / TWOPI ) );
            }

		// We close ending of the torus (only if there's an openangle set)
		if( openangle != 0.0f )
			generateClosure( verts, uvs, max_loop, angle_offset );
    }

    //void Init() 
    //{ 
    //    n = tesselation;
    //    m = tesselation;
    //    assert( n >= 0 );
    //    v = new glm::vec3*[ n ];
    //    for( int i = 0; i < n; i++ )
    //        v[ i ] = new glm::vec3[ m ];
    //}

    //void Deinit()
    //{
    //    for( int i = 0; i < n; i++ )
    //        delete[] v[i];
    //    delete[] v;
    //}

    //void CopyV( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
    //{
    //    for( int i = 0; i < n-1; i++ )
    //        for( int j = 0; j < m; j++ )
    //        {
    //            verts->AddAttribute( v[ i   ][ j ] );
    //            verts->AddAttribute( v[ i+1 ][ j ] );
    //        }
    //    for( int j = 0; j < m; j++ )
    //    {
    //        verts->AddAttribute( v[ n-1 ][ j ] );
    //        verts->AddAttribute( v[ 0   ][ j ] );
    //    }

    //    for( SizeType v = 0; v < verts->GetNumEntries(); v++ )
    //    {
    //        glm::vec3 vert = verts->GetVertices()[ v ];
    //        uvs->AddAttribute( glm::vec2( 0.5*( vert.x + vert.y + 1.f ),
    //                                        vert.z + 0.5 ) ); // FIXME: scaling
    //    }
    //}

    //void GenerateV()
    //{
    //    for( int i = 0; i < n; i++ )
    //        for( int j = 0; j < m; j++ )
    //        {

    //        }
    //}
};

std::vector<IGeometryGeneratorPtr>  Plugin::GetGenerators()
{
    std::vector< IGeometryGeneratorPtr > gens;
    
    gens.push_back( IGeometryGeneratorPtr( new Generator( 
        m_tesselation->GetValue(),
        m_radius->GetValue(),
        m_radiusCrossSection->GetValue(),
        m_openAngle->GetValue(),
		m_openAngleMode->Evaluate()
        ) ) );

    return gens;
}


} } }
