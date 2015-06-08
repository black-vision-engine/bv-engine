import ProjectManager
import Project
import DataCategory
import FSTextureDataAccessor
import FSSceneAccessor

def test():
    pm = ProjectManager.ProjectManager()

    pm.setGlobalSceneAccessor(FSSceneAccessor.FSSceneAccessor("global/scenes/", None))

    proj1 = Project.Project("proj1")

    fstda = FSTextureDataAccessor.FSTextureDataAccessor("./proj1/textures/", ['jpg', 'tga'])
    texturesProj1 = DataCategory.DataCategory("textures", fstda)

    proj1.registerCategory(texturesProj1)

    proj1.setSceneAccessor(FSSceneAccessor.FSSceneAccessor("./proj1/scenes/", proj1))

    pm.addProject(proj1)



    pm.listProjectsNames()

if __name__ == "__main__":
    test()