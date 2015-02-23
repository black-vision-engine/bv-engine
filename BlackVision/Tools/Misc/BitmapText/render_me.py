import os

## ##########################
## size as integer
## fill_color as hex encoded RRGGBB (e.g. ffcc00)
## poin_size as integer
## digit as integer
def prepare_command( size, fill_color, point_size, digit ):
    fmt = """convert -size {}x -background black -fill #{} -font ../../../Assets/fonts/arial.ttf -pointsize {} -fill white -gravity NorthWest caption:"{}" -flatten test_frame_{:05}.jpg"""
    
    return fmt.format( size, fill_color, point_size, digit, digit )

## ##########################
##
def call_command( cmd ):
    print cmd
    os.system( cmd )

## ##########################
##
def prepare_frames( num_entries ):
    
    for i in range( num_entries ):
        cmd = prepare_command( 500, "ffaaff", 450, i )
        call_command( cmd )
        
if __name__ == "__main__":
    prepare_frames( 50 )
