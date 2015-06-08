import ProjectManager
import Project

from FSTextureDataAccessor import LoadableTextureDataDesc
from FSFontDataAccessor import LoadableFontDataDesc
from FSSurfaceDataAccessor import LoadableSurfaceDataDesc
from FSSequenceDataAccessor import LoadableSequenceDataDesc

import os

def test():
    pm = ProjectManager.ProjectManager("bv_media")

    proj1 = Project.Project(pm.getRootDir(), "proj1")

    pm.addProject(proj1)

    pm.getProject("proj1").appendData("textures", "flagi/pol.jpg", LoadableTextureDataDesc("test_data.file")) # polak
    pm.getProject("proj1").appendData("textures", "flagi/ger.jpg", LoadableTextureDataDesc("test_data.file")) # niemiec
    pm.getProject("proj1").appendData("textures", "flagi/rus.jpg", LoadableTextureDataDesc("test_data.file")) # i rusek

    pm.getProject("proj1").appendData("fonts", "pol/arial.tff", LoadableFontDataDesc("test_data.file"))
    pm.getProject("proj1").appendData("surfaces", "animals/cat.bvsur", LoadableSurfaceDataDesc("test_data.file"))
    pm.getProject("proj1").appendData("sequences", "jedzie", LoadableSequenceDataDesc("test_seq", [f for f in os.listdir("test_seq") if os.path.isfile(os.path.join("test_seq",f))]))




    pm.listProjectsNames()

if __name__ == "__main__":
    test()