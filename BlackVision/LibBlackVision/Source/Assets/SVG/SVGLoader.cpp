#include "stdafx.h"

#include "SVGLoader.h"

#include "SVGAssetDescriptor.h"

#include "ProjectManager.h"

#include "Serialization/BV/XML/BVXMLDeserializer.h"
#include "Engine/Models/Timeline/Static/OffsetTimeEvaluator.h" // FIXME: this is...
#include "Assets/AssetDescsWithUIDs.h" // FIXME: ...just shit

#pragma warning( disable : 4996 )

namespace bv {

namespace {

enum {
    svgtiny_PATH_MOVE,
    svgtiny_PATH_CLOSE,
    svgtiny_PATH_LINE,
    svgtiny_PATH_BEZIER
};

void svgtiny_transform_path(float *p, unsigned int n, SVGAssetPtr mesh )
{
    float a = 0.01f, c = 0, e = 0;
    float b = 0, d = -0.01f, f = 0;

    unsigned int j;

    for (j = 0; j != n; ) {
        unsigned int points = 0;
        unsigned int k;
        switch ((int) p[j]) {
        case svgtiny_PATH_MOVE:
        case svgtiny_PATH_LINE:
            points = 1;
            break;
        case svgtiny_PATH_CLOSE:
            points = 0;
            break;
        case svgtiny_PATH_BEZIER:
            points = 3;
            break;
        default:
            assert(0);
        }
        j++;
        for (k = 0; k != points; k++) {
            float x0 = p[j], y0 = p[j + 1];
            float x = a * x0 + c * y0 + e;
            float y = b * x0 + d * y0 + f;
            p[j] = x;
            p[j + 1] = y;
            j += 2;
        }
    }
}

typedef bool svgtiny_code;

svgtiny_code svgtiny_add_path(float *p, unsigned int n, SVGAssetPtr mesh )
{
    //struct svgtiny_shape *shape;

    //if (state->fill == svgtiny_LINEAR_GRADIENT)
    //    return svgtiny_add_path_linear_gradient(p, n, state);

    svgtiny_transform_path(p, n, mesh);

    //shape = svgtiny_add_shape(state);
    //if (!shape) {
    //    free(p);
    //    return svgtiny_OUT_OF_MEMORY;
    //}

    auto geometry = std::make_shared< SVGAsset::MeshGeometry >();

    float first_x = 0.0f, first_y = 0.0f;

    float x = 0.0f, y = 0.0f;
    for( unsigned int i = 0; i < n; )
        if( p[ i ] == svgtiny_PATH_MOVE )
        {
            first_x = x = p[ i + 1 ];
            first_y = y = p[ i + 2 ];
            i += 3;
        }
        else if( p[ i ] == svgtiny_PATH_LINE )
        {
            geometry->positions.push_back( glm::vec3( x, y, 0 ) );
            x = p[ i + 1 ];
            y = p[ i + 2 ];
            geometry->positions.push_back( glm::vec3( x, y, 0 ) );
            i += 3;
        }
        else if( p[ i ] == svgtiny_PATH_CLOSE )
        {
            geometry->positions.push_back( glm::vec3( x, y, 0 ) );
            geometry->positions.push_back( glm::vec3( first_x, first_y, 0 ) );
            i++;
        }
        else if( p[ i ] == svgtiny_PATH_BEZIER )
        {
            glm::vec2 A( x, y ),
                B( p[ i + 1 ], p[ i + 2 ] ),
                C( p[ i + 3 ], p[ i + 4 ] ),
                D( p[ i + 5 ], p[ i + 6 ] );

            const int N = 10;

            for( int j = 0; j <= N; j++ )
            {
                float t = float( j ) / N;

                glm::vec2 P = pow( 1 - t, 3 ) * A +
                            3 * t * pow( 1 - t, 2 ) * B +
                            3 * pow( t, 2 ) * ( 1 - t ) * C +
                            pow( t, 3 ) * D;
                geometry->positions.push_back( glm::vec3( P[ 0 ], P[ 1 ], 0 ) );
                if( j > 0 && j < N )
                    geometry->positions.push_back( glm::vec3( P[ 0 ], P[ 1 ], 0 ) );
            }

            x = p[ i + 1 + 4 ];
            y = p[ i + 2 + 4 ];
            i += 7;
        }
        else
        {
            assert( false );
        }
    //shape->path = p;
    //shape->path_length = n;
    //state->diagram->shape_count++;

    auto size = geometry->positions.size();

    geometry->normals.resize( size, glm::vec3( 1, 0, 0 ) );
    geometry->tangents.resize( size, glm::vec4( 1, 0, 0, 0 ) );
    geometry->uvs.resize( size, glm::vec2( 0, 0 ) );

    mesh->SetGeometry( geometry );

    return true;
}

bool svgtiny_parse_path(IDeserializer & path, SVGAssetPtr mesh )
{
    const char *path_d_str;
    char *s, *path_d;
    float *p; /* path elemets */
        unsigned int palloc; /* number of path elements allocated */
    unsigned int i;
    float last_x = 0, last_y = 0;
    float last_cubic_x = 0, last_cubic_y = 0;
    float last_quad_x = 0, last_quad_y = 0;
    float subpath_first_x = 0, subpath_first_y = 0;

    //svgtiny_setup_state_local(&state);

    //svgtiny_parse_paint_attributes(path, &state);
    //svgtiny_parse_transform_attributes(path, &state);

    /* read d attribute */
    auto d = path.GetAttribute( "d" );
    path_d_str = d.c_str();
    //if (exc != DOM_NO_ERR) {
    //    state.diagram->error_line = -1; /* path->line; */
    //    state.diagram->error_message = "path: error retrieving d attribute";
    //    svgtiny_cleanup_state_local(&state);
    //    return svgtiny_SVG_ERROR;
    //}

    //if (path_d_str == NULL) {
    //    state.diagram->error_line = -1; /* path->line; */
    //    state.diagram->error_message = "path: missing d attribute";
    //    svgtiny_cleanup_state_local(&state);
    //    return svgtiny_SVG_ERROR;
    //}

        /* empty path is permitted it just disables the path */
        palloc = (unsigned int)strlen( path_d_str ); //dom_string_byte_length(path_d_str);
        if (palloc == 0) {
        //svgtiny_cleanup_state_local(&state);
        return true;
        }

        /* local copy of the path data allowing in-place modification */
    s = path_d = _strdup(path_d_str/*, palloc*/);
//    dom_string_unref(path_d_str);
    //if (s == NULL) {
    //    svgtiny_cleanup_state_local(&state);
    //    return svgtiny_OUT_OF_MEMORY;
    //}

        /* ensure path element allocation is sensibly bounded */
        if (palloc < 8) {
            palloc = 8;
        } else if (palloc > 64) {
            palloc = palloc / 2;
        }

    /* allocate initial space for path elements */
    p = (float*) malloc(sizeof p[0] * palloc);
    //if (p == NULL) {
    //    free(path_d);
    //    svgtiny_cleanup_state_local(&state);
    //    return svgtiny_OUT_OF_MEMORY;
    //}

    /* parse d and build path */
    for (i = 0; s[i]; i++)
        if (s[i] == ',')
            s[i] = ' ';
    i = 0;
    while (*s) {
        char command[2];
        int plot_command;
        float x, y, x1, y1, x2, y2, rx, ry, rotation, large_arc, sweep;
        int n;

                /* Ensure there is sufficient space for path elements */
#define ALLOC_PATH_ELEMENTS(NUM_ELEMENTS)                               \
                        if ((palloc - i) < NUM_ELEMENTS) {              \
                                float *tp;                              \
                                palloc = (palloc * 2) + (palloc / 2);   \
                                tp = (float*)realloc(p, sizeof p[0] * palloc);  \
                                p = tp;                                 \
                        }                                               


        /* moveto (M, m), lineto (L, l) (2 arguments) */
        if (sscanf(s, " %1[MmLl] %f %f %n", command, &x, &y, &n) == 3) {
            /*LOG(("moveto or lineto"));*/
            if (*command == 'M' || *command == 'm')
                plot_command = svgtiny_PATH_MOVE;
            else
                plot_command = svgtiny_PATH_LINE;
            do {
                                ALLOC_PATH_ELEMENTS(3);
                p[i++] = (float)plot_command;
                if ('a' <= *command) {
                    x += last_x;
                    y += last_y;
                }
                if (plot_command == svgtiny_PATH_MOVE) {
                    subpath_first_x = x;
                    subpath_first_y = y;
                }
                p[i++] = last_cubic_x = last_quad_x = last_x
                        = x;
                p[i++] = last_cubic_y = last_quad_y = last_y
                        = y;
                s += n;
                plot_command = svgtiny_PATH_LINE;
            } while (sscanf(s, "%f %f %n", &x, &y, &n) == 2);

        /* closepath (Z, z) (no arguments) */
        } else if (sscanf(s, " %1[Zz] %n", command, &n) == 1) {
            /*LOG(("closepath"));*/
                        ALLOC_PATH_ELEMENTS(1);

            p[i++] = svgtiny_PATH_CLOSE;
            s += n;
            last_cubic_x = last_quad_x = last_x = subpath_first_x;
            last_cubic_y = last_quad_y = last_y = subpath_first_y;

        /* horizontal lineto (H, h) (1 argument) */
        } else if (sscanf(s, " %1[Hh] %f %n", command, &x, &n) == 2) {
            /*LOG(("horizontal lineto"));*/
            do {
                                ALLOC_PATH_ELEMENTS(3);

                p[i++] = svgtiny_PATH_LINE;
                if (*command == 'h')
                    x += last_x;
                p[i++] = last_cubic_x = last_quad_x = last_x
                        = x;
                p[i++] = last_cubic_y = last_quad_y = last_y;
                s += n;
            } while (sscanf(s, "%f %n", &x, &n) == 1);

        /* vertical lineto (V, v) (1 argument) */
        } else if (sscanf(s, " %1[Vv] %f %n", command, &y, &n) == 2) {
            /*LOG(("vertical lineto"));*/
            do {
                                ALLOC_PATH_ELEMENTS(3);

                p[i++] = svgtiny_PATH_LINE;
                if (*command == 'v')
                    y += last_y;
                p[i++] = last_cubic_x = last_quad_x = last_x;
                p[i++] = last_cubic_y = last_quad_y = last_y
                        = y;
                s += n;
            } while (sscanf(s, "%f %n", &x, &n) == 1);

        /* curveto (C, c) (6 arguments) */
        } else if (sscanf(s, " %1[Cc] %f %f %f %f %f %f %n", command,
                &x1, &y1, &x2, &y2, &x, &y, &n) == 7) {
            /*LOG(("curveto"));*/
            do {
                                ALLOC_PATH_ELEMENTS(7);

                p[i++] = svgtiny_PATH_BEZIER;
                if (*command == 'c') {
                    x1 += last_x;
                    y1 += last_y;
                    x2 += last_x;
                    y2 += last_y;
                    x += last_x;
                    y += last_y;
                }
                p[i++] = x1;
                p[i++] = y1;
                p[i++] = last_cubic_x = x2;
                p[i++] = last_cubic_y = y2;
                p[i++] = last_quad_x = last_x = x;
                p[i++] = last_quad_y = last_y = y;
                s += n;
            } while (sscanf(s, "%f %f %f %f %f %f %n",
                    &x1, &y1, &x2, &y2, &x, &y, &n) == 6);

        /* shorthand/smooth curveto (S, s) (4 arguments) */
        } else if (sscanf(s, " %1[Ss] %f %f %f %f %n", command,
                &x2, &y2, &x, &y, &n) == 5) {
            /*LOG(("shorthand/smooth curveto"));*/
            do {
                                ALLOC_PATH_ELEMENTS(7);

                p[i++] = svgtiny_PATH_BEZIER;
                x1 = last_x + (last_x - last_cubic_x);
                y1 = last_y + (last_y - last_cubic_y);
                if (*command == 's') {
                    x2 += last_x;
                    y2 += last_y;
                    x += last_x;
                    y += last_y;
                }
                p[i++] = x1;
                p[i++] = y1;
                p[i++] = last_cubic_x = x2;
                p[i++] = last_cubic_y = y2;
                p[i++] = last_quad_x = last_x = x;
                p[i++] = last_quad_y = last_y = y;
                s += n;
            } while (sscanf(s, "%f %f %f %f %n",
                    &x2, &y2, &x, &y, &n) == 4);

        /* quadratic Bezier curveto (Q, q) (4 arguments) */
        } else if (sscanf(s, " %1[Qq] %f %f %f %f %n", command,
                &x1, &y1, &x, &y, &n) == 5) {
            /*LOG(("quadratic Bezier curveto"));*/
            do {
                                ALLOC_PATH_ELEMENTS(7);

                p[i++] = svgtiny_PATH_BEZIER;
                last_quad_x = x1;
                last_quad_y = y1;
                if (*command == 'q') {
                    x1 += last_x;
                    y1 += last_y;
                    x += last_x;
                    y += last_y;
                }
                p[i++] = float( 1./3 * last_x + 2./3 * x1 );
                p[i++] = float( 1./3 * last_y + 2./3 * y1 );
                p[i++] = float( 2./3 * x1 + 1./3 * x );
                p[i++] = float( 2./3 * y1 + 1./3 * y );
                p[i++] = last_cubic_x = last_x = x;
                p[i++] = last_cubic_y = last_y = y;
                s += n;
            } while (sscanf(s, "%f %f %f %f %n",
                    &x1, &y1, &x, &y, &n) == 4);

        /* shorthand/smooth quadratic Bezier curveto (T, t)
           (2 arguments) */
        } else if (sscanf(s, " %1[Tt] %f %f %n", command,
                &x, &y, &n) == 3) {
            /*LOG(("shorthand/smooth quadratic Bezier curveto"));*/
            do {
                                ALLOC_PATH_ELEMENTS(7);

                p[i++] = svgtiny_PATH_BEZIER;
                x1 = last_x + (last_x - last_quad_x);
                y1 = last_y + (last_y - last_quad_y);
                last_quad_x = x1;
                last_quad_y = y1;
                if (*command == 't') {
                    x1 += last_x;
                    y1 += last_y;
                    x += last_x;
                    y += last_y;
                }
                p[i++] = float( 1./3 * last_x + 2./3 * x1 );
                p[i++] = float( 1./3 * last_y + 2./3 * y1 );
                p[i++] = float( 2./3 * x1 + 1./3 * x );
                p[i++] = float( 2./3 * y1 + 1./3 * y );
                p[i++] = last_cubic_x = last_x = x;
                p[i++] = last_cubic_y = last_y = y;
                s += n;
            } while (sscanf(s, "%f %f %n",
                    &x, &y, &n) == 2);

        /* elliptical arc (A, a) (7 arguments) */
        } else if (sscanf(s, " %1[Aa] %f %f %f %f %f %f %f %n", command,
                &rx, &ry, &rotation, &large_arc, &sweep,
                &x, &y, &n) == 8) {
            do {
                                ALLOC_PATH_ELEMENTS(3);

                p[i++] = svgtiny_PATH_LINE;
                if (*command == 'a') {
                    x += last_x;
                    y += last_y;
                }
                p[i++] = last_cubic_x = last_quad_x = last_x
                        = x;
                p[i++] = last_cubic_y = last_quad_y = last_y
                        = y;
                s += n;
            } while (sscanf(s, "%f %f %f %f %f %f %f %n",
                &rx, &ry, &rotation, &large_arc, &sweep,
                &x, &y, &n) == 7);

        } else {
            fprintf(stderr, "parse failed at \"%s\"\n", s);
            break;
        }
    }

    free(path_d);

    if (i <= 4) {
        /* no real segments in path */
        free(p);
        //svgtiny_cleanup_state_local(&state);
        //return svgtiny_OK;
        return true;
    }

        /* resize path element array to not be over allocated */
        if (palloc != i) {
                float *tp;

                /* try the resize, if it fails just continue to use the old
                 * allocation
                 */
                tp = (float*)realloc(p, sizeof p[0] * i);
                if (tp != NULL) {
                        p = tp;
                }
        }

    return svgtiny_add_path(p, i, mesh);
}

}

// ***********************
//
AssetConstPtr		SVGLoader::LoadAsset       ( const AssetDescConstPtr & desc )  const
{
    auto svgDesc = QueryTypedDesc< SVGAssetDescriptorConstPtr >( desc );
    auto path = ProjectManager::GetInstance()->ToAbsPath( svgDesc->GetPath() ).Str(); // FIXME GetKey -> GetPath
    
    if( Path::Exists( path ) )
    {
        auto mesh = std::make_shared< SVGAsset >( path );

        BVXMLDeserializer deser( path, nullptr, nullptr );

        auto success = deser.EnterChild( "svg" );

        if( !success )
            return nullptr;

        ParsePath( deser, mesh );
        ParseGroup( deser, mesh );

        return mesh;
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Can't load SVG. Asset: [" << path.c_str() << "] doesn't exists.";
        return nullptr;
    }
}

// ***********************
//
AssetDescConstPtr	SVGLoader::CreateDescriptor( const IDeserializer & deser ) const
{
    if( deser.GetAttribute( "type" ) != SVGAssetDescriptor::UID() )
        return nullptr;
    return SVGAssetDescriptor::Create( deser.GetAttribute( "path" ) );
}

// ***********************
//
ThumbnailConstPtr   SVGLoader::LoadThumbnail   ( const AssetDescConstPtr & desc ) const
{
    return nullptr; desc;
}

// ***********************
//
void                SVGLoader::ParsePath        ( IDeserializer & deser, SVGAssetPtr mesh ) const
{
    if( deser.EnterChild( "path" ) )
    {
        do
        {
            auto child = std::make_shared< SVGAsset >( mesh->GetKey() );
            svgtiny_parse_path( deser, child );
            mesh->AddChild( child );

        } while( deser.NextChild() ); // FIXME when Triangulate is done
        deser.ExitChild(); // path
    }
}

// ***********************
//
void                SVGLoader::ParseGroup       ( IDeserializer & deser, SVGAssetPtr mesh ) const
{
    if( deser.EnterChild( "g" ) )
    {
        do
        {
            ParsePath( deser, mesh );
            ParseGroup( deser, mesh );

        } while( deser.NextChild() );
        deser.ExitChild(); // g
    }
}

}
