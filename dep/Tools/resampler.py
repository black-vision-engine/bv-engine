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
def fuckin_simple_mean( samples, res_samples_num ):
    assert len( samples ) > ( 2 * res_samples_num )
    res = []
    window_size = int( math.ceil( float( len( samples ) / float( res_samples_num ) ) ) )
    for i in range( 0, len( samples ), window_size ):
        #print "{}, {}, {}, {}".format( i, i + window_size, sum(samples[i:i+window_size]), len( samples[i:i+window_size] ) )
        res.append(sum(samples[i:i+window_size]) / float(len(samples[i:i+window_size])))
    return res

## ##########################
##
def compose_hm_with_points( hm, points, increment, total_meters ):
    for i, e in enumerate( hm ):
        if i > 0:
            pass


if __name__ == "__main__":

    import sys

    print "Reading height map file {}".format( "../Media/heightmaps/BukovinaWysokosci.txt" )
    hm_raw = read_height_map( "../Media/heightmaps/BukovinaWysokosci.txt" )
    
    total_meters = len( hm_raw )    
    num_samples = 1920 * 4
    ratio = float( len( hm_raw ) ) / num_samples

    print "Meters {} samples {} ratio {}".format( total_meters, num_samples, ratio )

    print "Reading points file {}".format( "../Media/heightmaps/BukovinaMiejsca.txt" )
    hm_points = read_hm_points( "../Media/heightmaps/BukovinaMiejsca.txt" )
    
    print "Total points {}".format( len( hm_points ) )
    
    print "Resampling height map data using fuckin_simple_mean"
    hm_resampled = fuckin_simple_mean( hm_raw, num_samples )

    print "Writing raw hm data to {}".format( "../Media/heightmaps/BukovinaWysokosciProcessed.txt" )
    write_hm( "../Media/heightmaps/BukovinaWysokosciProcessed.txt", hm_raw )

    print "Writing resampled hm data to {}".format( "../Media/heightmaps/BukovinaWysokosciProcessed.txt" )
    write_hm( "../Media/heightmaps/BukovinaWysokosciResampled.txt", hm_resampled, ratio )

    #compose_hm_with_points( hm_resampled, hm_points, len( hm_raw ) / len( hm_resampled ), total_meters )
