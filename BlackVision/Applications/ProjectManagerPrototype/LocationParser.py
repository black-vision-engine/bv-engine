
# ./            == current project
# /             == global                                   TODO
# file://       == system path to a file                    TODO
# seq://        == system path to directory with a sequence TODO


class LocationParser:
    def __init__(self, location, currentProjectName):
        self.currentProjectName = currentProjectName
        self.location = location

    def parseLocationString(self, projectName, categoryName, internelPath):
        assert isinstance(projectName, str)
        assert isinstance(internelPath, str)
        try:
            projName = projectName
            categoryName = categoryName
            internalPath = internelPath
            isGlobal = False
            if len(projectName) > 0:
                if projectName == ".":
                    if self.currentProjectName:
                        self.location.projectName = self.currentProjectName
            else:
                if projectName == "global":
                    self.location.isGlobalLocation = True
                elif internelPath.startswith("file://"):
                    self.location.prefix = "file"
                    self.location.internalPath = internelPath[7:]
                elif internelPath.startswith("seq://"):
                    self.location.prefix = "seq"
                    self.location.internalPath = internelPath[6:]

        except Exception as exc:
            print("Parsing location error: {}".format(exc))

