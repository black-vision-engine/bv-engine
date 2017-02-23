#!groovy

def get_tests_dir( buildDir, conf, platform ) {
     return buildDir + platform + '-v110-' + conf + '\\Tests\\'
}

def get_app_dir( buildDir, conf, platform ) {
     return buildDir + platform + '-v110-' + conf + '\\Applications\\'
}

def make_build( conf, platform ) {
    def info = conf + '|' + platform
    echo 'Building ' + info
	bat "\"${tool 'MSBuild'}\" BlackVision\\Projects\\Win\\VS11\\BlackVision.sln /p:Configuration=" + conf + " /maxcpucount:4 /p:Platform=\"" + platform + "\" /p:ProductVersion=1.0.0.${env.BUILD_NUMBER}"
	echo 'Building ' + info + ' FINISHED'
}

def list_test_execs( buildDir, conf, platform ) {
    //def g =  get_tests_dir( buildDir, conf, platform ) + '**/*.exe'
	//def testExecs = findFiles(glob: g )
	//return "list of exec paths"
	
	def testDir = get_tests_dir( buildDir, conf, platform )
	
	return [    testDir + "TestAssetManager\\TestAssetManager.exe" ,
	            testDir + "TestMipMapBuilder\\TestMipMapBuilder.exe" ,
	            testDir + "TestMultipass\\TestMultipass.exe" ,
	            testDir + "TestProjectManager\\TestProjectManager.exe"
	]
}

def make_archive( buildDir, conf, platform, fEnabled ) {
    def d = buildDir + conf + '-v110-' + platform
    def includes_app = get_app_dir( buildDir, conf, platform ) + '/**'
    def includes_tests = get_tests_dir( buildDir, conf, platform ) + '/**'
    archiveArtifacts artifacts: includes_app, fingerprint: fEnabled
    archiveArtifacts artifacts: includes_tests, fingerprint: fEnabled
}


node {
    checkout scm
    
    def buildDir = 'BlackVision\\_Builds\\'
    
    def configurations = ['Debug', 'Release']
    def platforms = ['Win32', 'x64']
    
    def currentConfiguration = configurations[0]
    def currentPlatform = platforms[1]
    
    stage('Build') {
	    make_build( currentConfiguration, currentPlatform )
    }
 	stage('Archive') {
 	    make_archive( buildDir, currentConfiguration, currentPlatform, true )
 	}
    stage('Test') {
	
        echo 'Testing..'
		echo pwd()
			
		def testExecsList = list_test_execs( buildDir, currentConfiguration, currentPlatform )
		
		echo testExecsList.size() + ' tests found.'
		
		for( int i = 0; i < testExecsList.size(); ++i ) {
		    try {
		        bat testExecsList.get( i ) + " --gtest_output=xml:test_reports\\"
		    }
		    catch(err) {
		        echo "test fail."
		    }
		}
		
			
		step([$class: 'XUnitBuilder',
        thresholds: [[$class: 'FailedThreshold', unstableThreshold: '1']],
        tools: [[$class: 'GoogleTestType', pattern: 'test_reports/**']]])
    }
}