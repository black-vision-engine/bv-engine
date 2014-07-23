from scipy import signal

def parse_line_hm( l ):
    toks = l.split( " " )
    assert len( toks ) == 2

    return ( int( toks[ 0 ].strip() ), float( toks[ 1 ].strip() ) )

def parse_line_points( l ):
    toks = l.split( ":" )
    print l
    assert len( toks ) == 2

    return ( int( toks[ 0 ].strip() ), toks[ 1 ].strip() )

def read_height_map( fn ):
    data = []
    with open( fn ) as f:
        prev = parse_line_hm( f.readline() )
        data.append( prev )
        for l in f:
            cur = parse_line_hm( l )
            if cur[ 0 ] - prev[ 0 ] >= 1:
                for i in range( prev[ 0 ], cur[ 0 ] ):
                    val = float( ( i + 1 ) - prev[ 0 ] ) / float( cur[ 0 ] - prev[ 0 ] ) * cur[ 1 ] + float( cur[ 0 ] - ( i + 1 ) ) / float( cur[ 0 ] - prev[ 0 ] ) * prev[ 1 ]
                    data.append( ( ( i + 1 ), val ) )

    return data

def read_hm_points( fn ):
    data = []
    with open( fn ) as f:
        for l in f:
            pl = parse_line_points( l )
            data.append( pl )
            
    return data

hm_raw = read_height_map( "../Media/heightmaps/BukovinaWysokosci.txt" )
hm_points = read_hm_points( "../Media/heightmaps/BukovinaMiejsca.txt" )

print len( hm_raw )
print hm_points
