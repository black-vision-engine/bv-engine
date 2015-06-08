import ProjectManager
import Project

def test():
    pm = ProjectManager.ProjectManager("bv_media")

    #pm.setGlobalSceneAccessor(FSSceneAccessor.FSSceneAccessor("global/scenes/", None))

    proj1 = Project.Project(pm.getRootDir(), "proj1")

    pm.addProject(proj1)



    pm.listProjectsNames()

if __name__ == "__main__":
    test()