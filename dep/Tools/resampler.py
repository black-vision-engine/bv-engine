from scipy import signal
import scipy
import numpy
import math

## ##########################
##
def parse_line_hm( l ):
    toks = l.split( " " )
    assert len( toks ) == 2

    return ( int( toks[ 0 ].strip() ), float( toks[ 1 ].strip() ) )

## ##########################
##
def parse_line_points( l ):
    toks = l.split( ":" )
    assert len( toks ) == 2

    return ( int( toks[ 0 ].strip() ), toks[ 1 ].strip() )

## ##########################
##
def read_height_map( fn ):
    data = []
    with open( fn ) as f:
        prev = parse_line_hm( f.readline() )
        data.append( prev[ 1 ] )
        for k, l in enumerate( f ):
            print "\rLine: {}       ".format( k ), 
            cur = parse_line_hm( l )
            if cur[ 0 ] - prev[ 0 ] >= 1:
                w = 1.0 / float( cur[ 0 ] - prev[ 0 ] )
                sw = prev[ 1 ]
                ew = cur[ 1 ]
                #print " -> Appending {} entries ".format( cur[ 0 ] - prev[ 0 ] ),
                for i in range( prev[ 0 ], cur[ 0 ] ):
                    val = sw * w * ( cur[ 0 ] - ( i + 1 ) ) + ew * w * ( ( i + 1 ) - prev[ 0 ] )
                    data.append( val )
            prev = cur
        print ""
        print len( data )
    return data

## ##########################
##
def read_hm_points( fn ):
    data = []
    with open( fn ) as f:
        for l in f:
            pl = parse_line_points( l )
            data.append( pl )

    return data

## ##########################
##
def write_hm( fn, data, increment = 1.0 ):
    with open( fn, "w" ) as f:
        s = 0.0
        for e in data:
            f.write( "{:.2f} {:.4f}\n".format( s, e ) )
            s += increment

## ##########################
##
def resample_hm( points, num_samples ):
    s = numpy.array( points )
    return signal.resample( s, num_samples )
        
## ##########################
## @see: http://stackoverflow.com/questions/20322079/downsample-a-1d-numpy-array        
def resample_hm_nanmean( points, num_samples ):
    rem = len( points ) % num_samples
    
    if rem > 0 :
        v = points[ -1 ]
        for i in range( num_samples - rem ):
            points.append( numpy.nan )

    R = float( len( points ) / num_samples )
    a = numpy.array( points )
    
    #print a.shape
    b = a.reshape( -1, R )
    #print b.shape
    c = numpy.nanmean( b, axis = 1 )
    #c = b.mean( axis = 1 )
    #print c.shape
    return c

## ##########################
##
def prepare_kernel_weights( kernel_len ):
    x = kernel_len

    inner_pixels = int( math.floor( ( x + 1.0 ) * 0.5 ) ) * 2 - 1
    outer_pixels = int( math.ceil( ( x + 1.0 ) * 0.5 ) ) * 2 - 1

    sub_pix = ( x - float( inner_pixels ) ) * 0.5
    main_pix = 1.0 / x
    rest_pix = main_pix * sub_pix

    filter = [main_pix] * outer_pixels
    filter[0] = rest_pix
    filter[-1]= rest_pix

    return filter
    
## ##########################
##
def apply_kernel( samples, kernel ):
    if( len( samples ) >= len( kernel ) ):
        s = 0.0
        for i in range( len( kernel ) ):
            s += samples[ i ] * kernel[ i ]
        
        return s
    else:
        return sum( samples ) / float( len( samples ) )

## ##########################
##
def fuckin_simple_resample( samples, res_samples_num ):
    assert len( samples ) > 3 * res_samples_num

    kernel_len = float( len( samples ) ) / float( res_samples_num )
    kernel = prepare_kernel_weights( kernel_len )

    s = 0.0

    res = []

    while s < len( samples ):
        sample = int( s )
        sample = apply_kernel( samples[sample:], kernel )
        
        print "\rProgress {:.3f}%".format( 100.0 * s / len( samples ) ), 
        res.append( sample )
        s += kernel_len

    print ""

    return res

## ##########################
##
def compose_hm_with_points( hm, points, increment, total_meters ):
    cur_pt_idx = 0
    cur_point = points[ cur_pt_idx ]

    res = []
    
    for i, e in enumerate( hm ):
        if i > 0:
            sx = float( i - 1 ) * increment
            ex = sx + increment
            if cur_point and cur_point[ 0 ] >= sx and cur_point[ 0 ] <= ex:
                cur_pt_idx += 1
                res.append( ( e, cur_point[ 0 ], cur_point[ 1 ] ) )
                if cur_pt_idx < len( points ):
                    cur_point = points[ cur_pt_idx ]
                else:
                    cur_point = None
            else:
                res.append( ( e, None, None ) )
        else:
            res.append( ( e, None, None ) )

    return res

## ##########################
##
def write_result( fn, data, dist ):
    min_v = data[ 0 ][ 0 ]
    max_v = data[ 0 ][ 0 ]
    
    for e in data:
        min_v = min( e[ 0 ], min_v )
        max_v = max( e[ 0 ], max_v )
        
    with open( fn, "w" ) as f:
        f.write( "MinHeight:{:.4f}\n".format( min_v ) )
        f.write( "MaxHeight:{:.4f}\n".format( max_v ) )
        f.write( "Distance:{:.4f}\n".format( dist ) )
        
        for e in data:
            if e[ 1 ] and e[ 2 ]:
                f.write( "{:.4f}:{}:{}\n".format( e[ 0 ], e[ 1 ], e[ 2 ] ) )
            else:
                f.write( "{:.4f}::\n".format( e[ 0 ] ) )

## ##########################
##
def process_hm( input_file_hm, input_file_pt, output_file, num_samples ):
    
    print "Reading height map file {}".format( input_file_hm )
    hm_raw = read_height_map( input_file_hm )
    
    total_meters = len( hm_raw )    
    ratio = float( len( hm_raw ) ) / ( num_samples - 1 )

    print "Distance {}, requested samples {}, resample ratio {}".format( total_meters, num_samples, ratio )

    print "Reading points file {}".format( input_file_pt )
    hm_points = read_hm_points( input_file_pt )
    
    print "Total points {}".format( len( hm_points ) )
    
    print "Resampling height map data using fuckin_simple_resample"
    hm_resampled = fuckin_simple_resample( hm_raw, num_samples )

    print "Generating output list with landmarks"
    result_list = compose_hm_with_points( hm_resampled, hm_points, float( len( hm_raw ) ) / float( len( hm_resampled ) - 1 ), total_meters )
    
    print "Writing formatted data to {}".format( output_file )
    write_result( output_file, result_list, ratio * ( num_samples - 1 ) )
    
if __name__ == "__main__":

    import sys

    if len( sys.argv ) != 5:
        print "Invalid number of arguments specified\n"
        print "Usage:\npython resampler.py <input_file_hm> <input_file_landmarks> <output_file> <num_samples>"
        sys.exit( 0 )

    print "Calling script with args: {} {} {} {}".format( sys.argv[ 1 ], sys.argv[ 2 ], sys.argv[ 3 ], sys.argv[ 4 ] )


    process_hm( sys.argv[ 1 ], sys.argv[ 2 ], sys.argv[ 3 ], int( sys.argv[ 4 ] ) )
