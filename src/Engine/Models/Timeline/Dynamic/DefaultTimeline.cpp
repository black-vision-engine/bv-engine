#include "DefaultTimeline.h"


namespace bv { namespace model {

DefaultTimeline::DefaultTimeline     ( const std::string & name, TimeType duration, ITimeEvaluator * parent )
{
}

} //model
} //bv

/*
import time

DELAY = 0.05
DURATION = 30.0

############################
class CommandStart:
    def __init__( self, t, e ):
        self.e = e
        self.t = t

    def trigger( self ):
        print "Triggering {:4} START   ".format( self.t ),
        self.e.start()

############################
class CommandStop:
    def __init__( self, t, e ):
        self.e = e
        self.t = t

    def trigger( self ):
        print "Triggering {:4} STOP    ".format( self.t ),
        self.e.stop()

############################
class CommandReverse:
    def __init__( self, t, e ):
        self.e = e
        self.t = t

    def trigger( self ):
        print "Triggering {:4} REVERSE ".format( self.t ),
        self.e.reverse()

############################
class TimeSegmentEval:

    DIR_FORWARD = 0
    DIR_BACKWARD = 1
   
    BEHAVIOR_CLAMP = 0
    BEHAVIOR_REPEAT = 1
    BEHAVIOR_MIRROR = 2

    PREC = 1000.0
   
    def __init__( self, duration, preb = BEHAVIOR_CLAMP, postb = BEHAVIOR_CLAMP, direction = DIR_FORWARD ):
        self.duration = duration
        self.direction = direction
        self.t = 0.0
        self.startTime   = self.t
        self.startTimeBack  = self.t
        self.started = False
        self.paused = False
        self.pauseDuration = 0.0
        self.pauseDurationBack = 0.0

        self.preEvaluator = self._evalPreClamp
        self.postEvaluator = self._evalPostClamp

    def update( self, t ):
        prevT = self.t
        self.t = t
        if not self.started:
            self.startTime = self.t

        if self.paused:
            self.pauseDuration += self.t - prevT
                          
    def start( self ):
        if not self.started:
            self.startTime = self.t
            self.started = True
        
        if self.paused:
            self.paused = False

        self._printT()

    def stop( self ):
        if self.started and not self.paused:
            self.paused = True

        self._printT()

    def reverse( self ):
        t = self.getLocalTime()

        if self.direction == self.DIR_FORWARD:
            self.direction = self.DIR_BACKWARD
        else:
            self.direction = self.DIR_FORWARD

        self._resetTo( t )
        
        self._printT()

    def getLocalTime( self ):
        t = self._getLocalTimeNoClamp()

        return self._clamp( t )

    def _getLocalTimeNoClamp( self ):
        if self.direction == self.DIR_FORWARD:
            return self.t - self.startTime - self.pauseDuration
        else:
            return 2.0 * self.startTimeBack - self.startTime - self.t + self.pauseDuration

    def _clamp( self, t ):
        if t > self.duration:
            return self.postEvaluator( t )
        elif t < 0.0:
            return self.preEvaluator( t )
        else:
            return t

    def _evalPostClamp( self, t ):
        return self.duration

    def _evalPreClamp( self, t ):
        return 0.0

    def _evalRepeat( self, t ):        
        return divmod( t, self.duration )[ 1 ]

    def _evalMirror( self, t ):
        c, q = divmod( t, self.duration )
        c = int( c )

        if c % 2 == 0:
            return q
        else:
            return self.duration - q

    def _dirMultiplier( self ):
        if self.direction == self.DIR_FORWARD:
            return 1.0
           
        return -1.0

    def _resetTo( self, t ):
        self.pauseDuration = 0.0
        self.startTime = self.t - t
        self.pauseDuration = 0.0
        
        if self.direction == self.DIR_BACKWARD:
            self.startTimeBack = self.t

    def _printT( self, msg = "" ):
        print "{} {}".format( msg, int( self.t * TimeSegmentEval.PREC ) / TimeSegmentEval.PREC )
    
############################
def prepareCommands( e ):
    cmds = []

    cmds.append( [ 1.0, CommandStart( 0.0, e ), False, 0.0 ] )
    cmds.append( [ 5.0, CommandReverse( 4.0, e ), False, 4.0 ] )
    cmds.append( [ 7.0, CommandStop( 2.0, e ), False, 2.0 ] )
    cmds.append( [ 8.0, CommandReverse( 2.0, e ), False, 2.0 ] )
    cmds.append( [ 10.0, CommandStart( 2.0, e ), False, 2.0 ] )
    cmds.append( [ 15.0, CommandStop( 7.0, e ), False, 7.0 ] )
    cmds.append( [ 16.0, CommandReverse( 7.0, e ), False, 7.0 ] )
    cmds.append( [ 18.0, CommandStart( 7.0, e ), False, 7.0 ] )
    cmds.append( [ 22.0, CommandReverse( 3.0, e ), False, 3.0 ] )
    cmds.append( [ 28.0, CommandStop( 9.0, e ), False, 9.0 ] )
#    cmds.append( [ 19.0, CommandStart( e ), False ] )
#    cmds.append( [ 25.0, CommandStop( e ), False ] )

    return cmds

############################
def prepareCommands1( e ):
    cmds = []

    cmds.append( [ 1.0, CommandStart( 0.0, e ), False, 0.0 ] )
    cmds.append( [ 15.0, CommandReverse( 4.0, e ), False, 4.0 ] )

    return cmds

############################
def evalCommand( cmds, prevT, curT ):
    for c in cmds:
        if prevT < c[ 0 ] and curT >= c[ 0 ] and c[ 2 ] == False:
            c[ 1 ].trigger()
            c[ 2 ] = True
            return

if __name__ == "__main__":

    import sys

    print divmod( 12.2, 15.6 )
    print divmod( 15.6, 15.6 )
    print divmod( 2 * 12.2, 15.6 )
    print divmod( 0.0, 15.6 )
    print divmod( -1.0, 15.6 )
    print divmod( -17.0, 15.6 )

    tse = TimeSegmentEval( 5.0 )

    cmds = prepareCommands( tse )

    for c in cmds:
        print c

    print ""

    start = time.time()
    lastTime = 0

    for i in range( int( DURATION / DELAY ) ):         
        time.sleep( DELAY )
        curTime = time.time() - start
        
        tse.update( curTime )

        evalCommand( cmds, lastTime, curTime )

        gt = "Global: {}".format( int( curTime * TimeSegmentEval.PREC ) / TimeSegmentEval.PREC )
        lt = "Local: {}".format( int( tse.getLocalTime() * TimeSegmentEval.PREC ) / TimeSegmentEval.PREC )

        print "\r{:14} {:14} ".format( gt, lt ),

        lastTime = curTime
*/
