#!groovy

def make_build( conf, platform ) {
    def info = conf + '|' + platform
    echo 'Building ' + info
	bat "\"${tool 'MSBuild'}\" BlackVision\\Projects\\Win\\VS11\\BlackVision.sln /p:Configuration=" + conf + " /maxcpucount:4 /p:Platform=\"" + platform + "\" /p:ProductVersion=1.0.0.${env.BUILD_NUMBER}"
	echo 'Building ' + info + ' FINISHED'
}

def make_archive( buildDir, conf, platform ) {
    def d = buildDir + conf + '-v110-' + platform
    def includes_app = d + '/Applications/**'
    def includes_tests = d + '/Tests/**'
    archiveArtifacts artifacts: includes_app, fingerprint: true
    archiveArtifacts artifacts: includes_tests, fingerprint: true
}

node {
    checkout scm
    
    def buildDir = 'BlackVision/_Builds/'
    
    def configurations = ['Debug', 'Release']
    def platforms = ['Win32', 'x64']
    
    def currentConfiguration = configurations[0]
    def currentPlatform = platforms[1]
    
    stage('Build') {
	    make_build( currentConfiguration, currentPlatform )
    }
	stage('Archive') {
	
	    make_archive( buildDir, currentConfiguration, currentPlatform )
	}
    // stage('Test') {
	
        // steps {
            // echo 'Testing..'
			
			// // step([$class: 'XUnitBuilder',
            // // thresholds: [[$class: 'FailedThreshold', unstableThreshold: '1']],
            // // tools: [[$class: 'JUnitType', pattern: 'reports/**']]])
        // }
    // }
}