import Project
import Location

class ProjectManager:

    def __init__(self):
        self.projects       = {}
        self.currentProject = None

    def getData(self, path):
        assert isinstance(path, str)

        loc = Location(path, self.currentProject.getName() if self.currentProject else "")

        if loc:
            proj = self.getProject(loc.getProjectName())
            if proj:
                return proj.getData(loc.getCategoryName(), loc.getInternalPath())

        print("Cannot find data '{}'".format(path))
        return None

    def getProject(self, name):
        assert isinstance(name, str)
        if name in self.projects:
            return self.projects[name]
        else:
            print("Cannot find project '{}'".format(name))
            return None

    def listProjectsNames(self):
        return self.projects.keys()

    def setCurrentProject(self, projectName):
        if projectName in self.projects:
            self.currentProject = self.projects[projectName]
        else:
            print("Cannot set current project. Project '{}' doesn't exist".format(projectName))

    def addProject(self, project):
        assert isinstance(project, Project)
        if not project.getName() in self.projects:
            self.projects[project.getName()] = project
        else:
            print("Project '{}' already exists. Cannot add".format(project.getName()))

