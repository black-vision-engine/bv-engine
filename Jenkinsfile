#!groovy​

pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
				branch 'master'
				checkout scm
                echo 'Building..'
            }
        }
        stage('Test') {
            steps {
				branch 'jenkins'
                echo 'Testing..'
            }
        }
        stage('Deploy') {
            steps {
				branch 'jenkins'
                echo 'Deploying....'
            }
        }
    }
}
