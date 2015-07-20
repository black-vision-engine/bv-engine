

from FSTextureAssetAccessor import TextureDesc
from FSSequenceAssetAccessor import SequenceDesc
from Scene import Scene, Node, Plugin, SceneReader

import os
import shutil

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

    if os.path.exists("bv_media"):
        shutil.rmtree("bv_media")

    if os.path.exists("bv_media1"):
        shutil.rmtree("bv_media1")

    from ProjectManager import PM as pm

    pm.addNewProject("proj1")

    pm.getProject("proj1").appendAsset("textures", "flagi/pol.jpg", TextureDesc("test_data.file")) # polak
    pm.getProject("proj1").appendAsset("textures", "flagi/ger.jpg", TextureDesc("test_data.file")) # niemiec
    pm.getProject("proj1").appendAsset("textures", "flagi/rus.jpg", TextureDesc("test_data.file")) # i rusek
    pm.getProject("proj1").appendAsset("sequences", "jedzie", SequenceDesc("test_seq", [f for f in os.listdir("test_seq") if os.path.isfile(os.path.join("test_seq",f))]))



    pm.addNewProject("proj2")

    pm.getProject("proj2").appendAsset("textures", "flags/pol1.jpg", TextureDesc("test_data.file")) # polak
    pm.getProject("proj2").appendAsset("textures", "flags/ger1.jpg", TextureDesc("test_data.file")) # niemiec
    pm.getProject("proj2").appendAsset("textures", "flags/rus1.jpg", TextureDesc("test_data.file")) # i rusek
    pm.getProject("proj2").appendAsset("sequences", "jedzie1", SequenceDesc("test_seq", [f for f in os.listdir("test_seq") if os.path.isfile(os.path.join("test_seq", f))]))

    pm.getProject("proj1").saveScene(generateScene1("proj1", "proj2"), "test_scenes/p1s1.scn")
    pm.getProject("proj2").saveScene(generateScene1("proj1", "proj2"), "test_scenes1/p1s2.scn")

    pm.copyScene("proj1", "test_scenes/p1s1.scn", "proj1", "test_scenes/p1s1copy.scn")
    pm.copyScene("proj1", "test_scenes/p1s1.scn", "proj2", "test_scenes/p1s1copy.scn")
    pm.removeScene("proj2", "test_scenes/p1s1copy.scn")
    pm.moveScene("proj1", "test_scenes/p1s1copy.scn", "proj2", "test_scenes/p1s1moved.scn")

    pm.listProjectsNames()

    print(pm.listScenes())
    print(pm.listScenes("proj1"))
    print(pm.listScenes("proj2"))
    print(pm.listCategories())
    print(pm.listCategories("proj1"))
    print(pm.listCategories("proj2"))
    print(pm.listAssets())
    print(pm.listAssets("proj1", "textures"))
    print(pm.listAssets("proj2", "sequences"))
    print(pm.listAssets("proj2"))

    pm.exportProjectToFile("proj1", "proj1.exp")
    pm.exportSceneToFile("proj2", "test_scenes1/p1s2.scn", "proj1.expscene")

    from ProjectManager import ProjectManager
    pm1 = ProjectManager("bv_media1")

    pm1.addNewProject("proj3")

    pm1.importProjectFromFile("proj1.exp", "proj3")

    print(pm1.listScenes())

    sceneFilePath = pm1.getSceneDesc("proj3", "test_scenes/p1s1.scn").absPath

    s = SceneReader(sceneFilePath).loadScene()
    r = s.check("bv_media1")


    pm1.importSceneFromFile("proj3", "test_scenes5/p1s2.scn", "proj1.expscene")


    sceneFilePath = pm1.getSceneDesc("proj3", "test_scenes5/p1s2.scn").absPath

    s = SceneReader(sceneFilePath).loadScene()
    r = s.check("bv_media1")

    return 0


if __name__ == "__main__":
    test()