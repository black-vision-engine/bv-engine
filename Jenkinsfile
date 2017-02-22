#!groovy​

pipeline {
    agent any

    stages {
		def configurations = ['Debug', 'Release']
	
        stage('Build') {
            steps {
				checkout scm
				
				for (int i = 0; i < configurations.size(); ++i) {
					echo 'Building ${configurations[i]}'
					bat "\"${tool 'MSBuild'}\" BlackVision\\Projects\\Win\\VS11\\BlackVision.sln /p:Configuration=${configurations[i]} /maxcpucount:4 /p:Platform=\"x64\" /p:ProductVersion=1.0.0.${env.BUILD_NUMBER}"
					echo 'Building ${configurations[i]} FINISHED'
                }
				
				// bat "\"${tool 'MSBuild'}\" BlackVision\\Projects\\Win\\VS11\\BlackVision.sln /p:Configuration=Debug /maxcpucount:4 /p:Platform=\"x64\" /p:ProductVersion=1.0.0.${env.BUILD_NUMBER}"
				
				// bat "\"${tool 'MSBuild'}\" BlackVision\\Projects\\Win\\VS11\\BlackVision.sln /p:Configuration=Release /maxcpucount:4 /p:Platform=\"x64\" /p:ProductVersion=1.0.0.${env.BUILD_NUMBER}"
            }
        }
		stage('Archive'){
			steps {
				for (int i = 0; i < configurations.size(); ++i) {
					archive 'BlackVision/_Builds/x64-v110-${configurations[i]}/Applications/**'
					archive 'BlackVision/_Builds/x64-v110-${configurations[i]}/Tests/**'
				}
				// archive 'BlackVision/_Builds/x64-v110-Debug/Applications/**'
				// archive 'BlackVision/_Builds/x64-v110-Debug/Tests/**'
				
				// archive 'BlackVision/_Builds/x64-v110-Release/Applications/**'
				// archive 'BlackVision/_Builds/x64-v110-Release/Tests/**'

			}
		}
        stage('Test') {
            steps {
				branch 'jenkins'
                echo 'Testing..'
            }
        }
    }
}
