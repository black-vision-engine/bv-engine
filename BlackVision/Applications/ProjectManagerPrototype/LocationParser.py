import Location

# ./            == current project
# /             == global                                   TODO
# file://       == system path to a file                    TODO
# seq://        == system path to directory with a sequence TODO


class LocationParser:
    def __init__(self, location, currentProjectName):
        assert  isinstance(location, Location)
        self.currentProjectName = currentProjectName
        self.location = location

    def parseLocationString(self, projectName, pathInProject):
        assert isinstance(projectName, str)
        assert isinstance(pathInProject, str)
        try:
            projName = projectName
            categoryName = ""
            internalPath = ""
            isGlobal = False
            if projectName == ".":
                if self.currentProjectName:
                    self.location.projectName = self.currentProjectName
                    self.location.categoryName = pathInProject.split("/", 2)[1]
                    self.location.internalPath = pathInProject.split("/", 2)[2]
            elif len(projectName) == 0:
                if pathInProject[1:7] == "global":
                    self.location.isGlobalLocation = True
                else:
                    self.location.projectName = pathInProject.split("/", 2)[0]
                self.location.categoryName = pathInProject.split("/", 2)[1]
                self.location.internalPath = pathInProject.split("/", 2)[2]
            elif pathInProject.startswith("file://"):
                self.location.prefix = "file"
                self.location.internalPath = pathInProject[7:]
            elif pathInProject.startswith("seq://"):
                self.location.prefix = "seq"
                self.location.internalPath = pathInProject[6:]

        except Exception as exc:
            print("Parsing location error: {}".format(exc))

