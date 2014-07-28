from PIL import Image
import sys

FIXED_POINT_SPEC = (12,12) #12:12 fixed point arithmetic to be used

## ##########################
##
def parse_float_entry( l, entry_id ):
    toks = l.split(":")
    assert len( toks ) == 2
    assert toks[ 0 ] == entry_id
    
    return float( toks[ 1 ] )

## ##########################
##
def parse_data_entry( e ):
    l = e.split(":")
    
    mark = False

    if l[1] != "":
        mark = True

    return ( float( l[0] ), mark )

## ##########################
##
def generate_desc( fn, base_name, min_height, max_height, dist ):
    with open( fn, "w" ) as f:
        f.write( "Descriptor file for {}\n".format( base_name ) )
        f.write( "FixedPointSpec:({},{})\n".format( FIXED_POINT_SPEC[0], FIXED_POINT_SPEC[1] ) )
        f.write( "MinHeight:{:.4f}\n".format( min_height ) )
        f.write( "MaxHeight:{:.4f}\n".format( max_height ) )
        f.write( "Distance:{:.4f}\n".format( dist ) )

## ##########################
##
def fixed_point_RGB( val, factor ):
    v = int( val * factor + 0.5 )
    assert v <= ( 2 ** 24 - 1 )

    #FORWARD AND BACKWARD CONVERSION
    #r = ( v >> 16 ) & 0xFF
    #g = ( v >> 8 ) & 0xF;
    #b = v & 0xFF
    #print val, v, r, g, 256 * r + g, float( 256 * r + g ) / factor

    return ( v >> 16 ) & 0xFF, ( v >> 8 ) & 0xFF, v & 0xFF

## ##########################
##
def RGBA_from_raw( val, has_alpha, fixed_factor ):
    assert val >= 0.0
    assert val < 4096.0
    
    r, g, b = fixed_point_RGB( val, fixed_factor )
    a = 255 if has_alpha else 0

    return ( r, g, b, a )

## ##########################
##
def convert_raw_data_to_pixels( data, fixed_point_spec ):
    fp_maj = fixed_point_spec[0]
    fp_min = fixed_point_spec[1]
    
    fixed_factor = float( 1 << fp_min )
    
    res = []
    
    for e in data:
        rgba = RGBA_from_raw( e[0], e[1], fixed_factor )
        res.append( rgba )
    
    return res

## ##########################
##
def generate_cooked_img( fn, data, format="PNG" ):
    w = len( data )
    h = 1

    pixels = convert_raw_data_to_pixels( data, FIXED_POINT_SPEC )
    assert len( pixels ) == len( data )
 
    im = Image.new('RGBA', (w,h))
    im.putdata( pixels )

    im.save( fn )

## ##########################
##
def read_input_data( fn ):
    with open( fn ) as f:
        min_height = parse_float_entry( f.readline(), "MinHeight" )
        max_height = parse_float_entry( f.readline(), "MaxHeight" )
        dist = parse_float_entry( f.readline(), "Distance" )

        data = []
        for l in f:
            data.append( parse_data_entry( l.strip() ) )

    return ( min_height, max_height, dist, data )

## ##########################
##
def process_raw( input_file, output_base_name ):
    desc_file = output_base_name + ".dsc"
    img_file = output_base_name + ".png"
    
    print "Reading input data from {}".format( input_file )
    data_tuple = read_input_data( input_file )

    print "Generating desc file {}".format( desc_file )
    generate_desc( desc_file, output_base_name, data_tuple[ 0 ], data_tuple[ 1 ], data_tuple[ 2 ] )

    print "Generating cooked image file {}".format( img_file )
    generate_cooked_img( img_file, data_tuple[ 3 ], format = "PNG" )
        
if __name__ == "__main__":

    if len( sys.argv ) != 3:
        print "Invalid number of arguments specified\n"
        print "Usage:\npython resampler.py <input_file_raw> <output_file_base_name>"
        sys.exit( 0 )

    if "." in sys.argv[ 2 ]:
        print "Invalid base name {}".format( sys.argv[ 2 ] )
        print "Base name cannot contain '.' characters"

        sys.exit( 0 )
        
    process_raw( sys.argv[ 1 ], sys.argv[ 2 ] )
    