from scipy import signal
import scipy
import numpy
import math

def parse_line_hm( l ):
    toks = l.split( " " )
    assert len( toks ) == 2

    return ( int( toks[ 0 ].strip() ), float( toks[ 1 ].strip() ) )

def parse_line_points( l ):
    toks = l.split( ":" )
    assert len( toks ) == 2

    return ( int( toks[ 0 ].strip() ), toks[ 1 ].strip() )

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

    return data

def read_hm_points( fn ):
    data = []
    with open( fn ) as f:
        for l in f:
            pl = parse_line_points( l )
            data.append( pl )

    return data

def write_hm( fn, data, increment = 1.0 ):
    with open( fn, "w" ) as f:
        s = 0.0
        for e in data:
            f.write( "{:.2f} {:.4f}\n".format( s, e ) )
            s += increment

def resample_hm( points, num_samples ):
    s = numpy.array( points )
    return signal.resample( s, num_samples )
        
# @see: http://stackoverflow.com/questions/20322079/downsample-a-1d-numpy-array        
def resample_hm_nanmean( points, num_samples ):
    print len( points )
    print num_samples
    rem = len( points ) % num_samples
    print rem
    
    if rem > 0 :
        v = points[ -1 ]
        for i in range( num_samples - rem ):
            points.append( v )
    
    R = float( len( points ) / num_samples )
    print R
    a = numpy.array( points )
    
    print a.shape

    b = a.reshape( -1, R ).mean( axis = 1 )
    print b.shape
    
    return b

print "Reading height map file {}".format( "../Media/heightmaps/BukovinaWysokosci.txt" )
hm_raw = read_height_map( "../Media/heightmaps/BukovinaWysokosci.txt" )

print "Reading points file {}".format( "../Media/heightmaps/BukovinaMiejsca.txt" )
hm_points = read_hm_points( "../Media/heightmaps/BukovinaMiejsca.txt" )

print "Resampling height map data"
hm_resampled = resample_hm( hm_raw, 1920 * 4 )
hm_resampled_nm = resample_hm_nanmean( hm_raw, 1920 * 4 )

write_hm( "../Media/heightmaps/BukovinaWysokosciProcessed.txt", hm_raw )
write_hm( "../Media/heightmaps/BukovinaWysokosciResampled.txt", hm_resampled, len( hm_raw ) / len( hm_resampled ) )
write_hm( "../Media/heightmaps/BukovinaWysokosciResampled_nm.txt", hm_resampled_nm, len( hm_raw ) / len( hm_resampled ) )
