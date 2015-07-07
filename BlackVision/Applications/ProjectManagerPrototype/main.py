from ProjectManager import PM as pm
import Project

from FSTextureDataAccessor import LoadableTextureDataDesc
from FSFontDataAccessor import LoadableFontDataDesc
from FSSurfaceDataAccessor import LoadableSurfaceDataDesc
from FSSequenceDataAccessor import LoadableSequenceDataDesc

from Scene import Scene, Node, Plugin

import os

def generateScene1(projectName1, projectName2):
    rootNode = Node("p1s1_root")

    n0 = Node("ch0")
    n1 = Node("ch1")

    p0 = Plugin("texture")
    p0.addResource(projectName1, "sequences", "jedzie")
    p0.addResource(projectName2, "sequences", "jedzie1")

    p1 = Plugin("texture")
    p1.addResource(projectName1, "textures", "flagi/pol.jpg")
    p1.addResource(projectName2, "textures", "flags/pol1.jpg")

    n0.addPlugin(p0)
    n1.addPlugin(p1)

    rootNode.addChildNode(n0)
    rootNode.addChildNode(n1)


    return Scene("p1s1", rootNode)

def test():

    proj1 = Project.Project(pm, "proj1")

    pm.addProject(proj1)

    pm.getProject("proj1").appendData("textures", "flagi/pol.jpg", LoadableTextureDataDesc("test_data.file")) # polak
    pm.getProject("proj1").appendData("textures", "flagi/ger.jpg", LoadableTextureDataDesc("test_data.file")) # niemiec
    pm.getProject("proj1").appendData("textures", "flagi/rus.jpg", LoadableTextureDataDesc("test_data.file")) # i rusek

    pm.getProject("proj1").appendData("fonts", "pol/arial.tff", LoadableFontDataDesc("test_data.file"))
    pm.getProject("proj1").appendData("surfaces", "animals/cat.bvsur", LoadableSurfaceDataDesc("test_data.file"))
    pm.getProject("proj1").appendData("sequences", "jedzie", LoadableSequenceDataDesc("test_seq", [f for f in os.listdir("test_seq") if os.path.isfile(os.path.join("test_seq",f))]))


    proj2 = Project.Project(pm, "proj2")

    pm.addProject(proj2)

    pm.getProject("proj2").appendData("textures", "flags/pol1.jpg", LoadableTextureDataDesc("test_data.file")) # polak
    pm.getProject("proj2").appendData("textures", "flags/ger1.jpg", LoadableTextureDataDesc("test_data.file")) # niemiec
    pm.getProject("proj2").appendData("textures", "flags/rus1.jpg", LoadableTextureDataDesc("test_data.file")) # i rusek

    pm.getProject("proj2").appendData("fonts", "pol1/arial1.tff", LoadableFontDataDesc("test_data.file"))
    pm.getProject("proj2").appendData("surfaces", "animals1/cat1.bvsur", LoadableSurfaceDataDesc("test_data.file"))
    pm.getProject("proj2").appendData("sequences", "jedzie1", LoadableSequenceDataDesc("test_seq", [f for f in os.listdir("test_seq") if os.path.isfile(os.path.join("test_seq",f))]))

    pm.getProject("proj1").saveScene(generateScene1("proj1", "proj2"), "test_scenes/p1s1.scn")

    #pm.getProject("proj1").exportScene("exportedScene1", "test_scenes/p1s1.scn")

    pm.listProjectsNames()

    pm.getProject("proj1").exportToFile("proj1.exp")

    from ProjectManager import ProjectManager
    pm1 = ProjectManager("bv_media1")

    pm1.importFromFile("proj1.exp", "proj3")

    #
    # proj11 = Project.Project(pm1, "1proj1")
    # pm1.addProject(proj11)
    # pm1.importSceneFromFile("1proj1", "imported_scene/p1s1.scn", "exportedScene1")



if __name__ == "__main__":
    test()