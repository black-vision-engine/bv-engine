#!groovy​

pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
				checkout scm
				script {
					def configurations = ['Debug', 'Release']
					
					for (int i = 0; i < configurations.size(); ++i) {
						echo 'Building ${configurations[i]}'
						bat "\"${tool 'MSBuild'}\" BlackVision\\Projects\\Win\\VS11\\BlackVision.sln /p:Configuration=${configurations[i]} /maxcpucount:4 /p:Platform=\"x64\" /p:ProductVersion=1.0.0.${env.BUILD_NUMBER}"
						echo 'Building ${configurations[i]} FINISHED'
					}
				}				
            }
        }
		stage('Archive'){
			steps {
				script {
					def configurations = ['Debug', 'Release']
				
					for (int i = 0; i < configurations.size(); ++i) {
						stash includes: 'BlackVision/_Builds/x64-v110-${configurations[i]}/Applications/**', name: "app_${configurations[i]}"
						stash includes: 'BlackVision/_Builds/x64-v110-${configurations[i]}/Tests/**', name: "tests_${configurations[i]}"
					}
				}
			}
		}
        stage('Test') {
		
            steps {
                echo 'Testing..'
				
				// step([$class: 'XUnitBuilder',
                // thresholds: [[$class: 'FailedThreshold', unstableThreshold: '1']],
                // tools: [[$class: 'JUnitType', pattern: 'reports/**']]])
            }
        }
    }
}
