#!groovy​

pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
				branch 'jenkins'
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
