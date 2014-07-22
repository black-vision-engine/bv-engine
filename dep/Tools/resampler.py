from scipy import signal

def parse_line( l ):
    toks = l.split( " " )
    assert len( toks ) == 2

    return ( int( toks[ 0 ] ), float( toks[ 1 ] ) )
   
def read_height_map( fn ):
    data = []
    with open( fn ) as f:
        pl = parse_line( f.readline() )
        data.append( pl )
        for l in f:
            pl = parse_line( l )
            data.append( pl )
            
    return data


hm_raw = read_height_map( "../Media/heightmaps/BukovinaWysokosci.txt" )

print hm_raw[:300]
