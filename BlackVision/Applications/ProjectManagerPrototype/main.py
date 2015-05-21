import ProjectManager
import Project
import DataCategory
import FSTextureDataAccessor

def test():
    pm = ProjectManager.ProjectManager()

    proj1 = Project.Project("proj1")

    fstda = FSTextureDataAccessor.FSTextureDataAccessor("./proj1/textures/")
    texturesProj1 = DataCategory.DataCategory("textures", fstda)

    proj1.registerCategory(texturesProj1)
    pm.addProject(proj1)

    pm.listProjectsNames()

if __name__ == "__main__":
    test()