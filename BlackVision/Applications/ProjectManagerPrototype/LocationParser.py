from ProjectManager import ProjectManager
from Project import Project
from Location import Location

# ./            == current project
# /             == global                                   TODO
# file://       == system path to a file                    TODO
# seq://        == system path to directory with a sequence TODO


class LocationParser:
    def __init__(self, location, currentProjectName):
        assert  isinstance(location, Location)
        self.currentProjectName = currentProjectName
        self.location = location

    def parseLocationString(self, string):
        assert isinstance(string, str)
        try:
            projName = ""
            categoryName = ""
            internalPath = ""
            isGlobal = False
            if string.startswith("./"):
                if self.currentProjectName:
                    self.location.projectName = self.currentProjectName
                    self.location.categoryName = string.split("/", 2)[1]
                    self.location.internalPath = string.split("/", 2)[2]
            elif string.startswith("/"):
                if string[1:7] == "global":
                    self.location.isGlobalLocation = True
                else:
                    self.location.projectName = string.split("/", 2)[0]
                self.location.categoryName = string.split("/", 2)[1]
                self.location.internalPath = string.split("/", 2)[2]
            elif string.startswith("file://"):
                self.location.prefix = "file"
                self.location.internalPath = string[7:]
            elif string.startswith("seq://"):
                self.location.prefix = "seq"
                self.location.internalPath = string[6:]

        except Exception as exc:
            print("Parsing location error: {}".format(exc))

