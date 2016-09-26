# This script helps creating/updating vcxproj based on source & destination version.

import glob
import os.path
import re

from lxml import etree
from io import StringIO
from shutil import copyfile


SRC_VERSION = "VS11"
DST_VERSION = "VS14"

INCLUDE_ATTR = "Include"
LABEL_ATTR = "Label"

ITEM_GROUP_TAG = "ItemGroup"
ITEM_DEFINITION_GROUP_TAG = "ItemDefinitionGroup"
INCLUDE_TAGS = ["ClCompile", "ClInclude", "None"]

CUSTOM_XML_ATTR_VALUE = "CustomXML"
CUSTOM_XML = '<ItemDefinitionGroup Label="CustomXML"><ClCompile><PreprocessorDefinitions>_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS=1;%(PreprocessorDefinitions)</PreprocessorDefinitions><DisableSpecificWarnings>4458;4459;4714;4996;</DisableSpecificWarnings></ClCompile></ItemDefinitionGroup>'

   
def update_includes(src_path, dst_path):
    ''' Copies source/include items from source to destination vcxproj. '''
    #with open(src_path, 'r', encoding='utf-8-sig') as f:
     #   xml_str = bytes(bytearray(f.read(), encoding='utf-8'))
    parser = etree.XMLParser(remove_blank_text=True)
    src_tree = etree.parse(src_path, parser)
    src_root = src_tree.getroot()
    #with open(dst_path, 'r', encoding='utf-8') as f:
     #   xml_str = bytes(bytearray(f.read(), encoding='utf-8'))
    parser = etree.XMLParser(remove_blank_text=True)
    dst_tree = etree.parse(dst_path, parser)
    dst_root = dst_tree.getroot()
    
    src_nodes = []
    for include_tag in INCLUDE_TAGS:
        item_groups = set()

        src_include_nodes = src_root.findall(".//{{{0}}}{1}[@{2}]".format(src_root.nsmap[None], include_tag, INCLUDE_ATTR))
        dst_include_tags = dst_root.findall(".//{{{0}}}{1}[@{2}]".format(dst_root.nsmap[None], include_tag, INCLUDE_ATTR))

        for child_node in dst_include_tags:
            item_groups.add(child_node.getparent())
            #child_node.getparent().remove(child_node)
        
        for item_group in item_groups:
            item_group.getparent().remove(item_group)
        
        if len(src_include_nodes) > 0:
            item_group_node = etree.Element(ITEM_GROUP_TAG)
            for child_node in src_include_nodes:
                item_group_node.append(child_node)
                
            dst_root.append(item_group_node)
        
    dst_tree.write(dst_path, xml_declaration=True, pretty_print=True)

def clone_and_modify_vcxproj(src_path, dst_path, src_version, dst_version):
    ''' Make a copy of existing vcxproj and replace source version occurrences with destination version. '''
    with open(src_path, 'r', encoding='utf-8-sig') as f:
        file_data = f.read()

    with open(dst_path, 'w+', encoding='utf-8') as f:
        src_ver = re.findall('\d+', src_version)[0]
        dst_ver = re.findall('\d+', dst_version)[0]

        file_data = file_data.replace(src_version, dst_version)
        file_data = file_data.replace("v{0}0".format(src_ver), "v{0}0".format(dst_ver))
        file_data = file_data.replace("msvc-{0}".format(src_ver), "msvc-{0}".format(dst_ver))

        f.write(file_data)
        f.close()


def append_xml(path, custom_xml, tag):
    ''' Append custom xml to all vcxprojs. '''
    #with open(path, 'r', encoding='utf-8') as f:
        #xml_str = bytes(bytearray(f.read(), encoding='utf-8'))
    parser = etree.XMLParser(remove_blank_text=True)
    tree = etree.parse(path, parser)
    root = tree.getroot()
    
    children = root.findall("{{{0}}}{1}[@{2}]".format(root.nsmap[None], tag, LABEL_ATTR))
    for child in children:
        if(child.attrib[LABEL_ATTR] == CUSTOM_XML_ATTR_VALUE):
            child.getparent().remove(child)
            
    node = etree.fromstring(custom_xml)
    root.append(node)

    tree.write(path, xml_declaration=True, pretty_print=True)

def process_projects_xml(src_version=SRC_VERSION, dst_version=DST_VERSION, custom_xml=CUSTOM_XML, append_custom_xml=True):
    ''' Find all vcxproj in source version and create/update destination version. '''
    for src_path in glob.iglob("./**/"+src_version+"/*.vcxproj", recursive=True):
        print(src_path)
        dst_path = src_path.replace(src_version, dst_version)

        dir = os.path.dirname(dst_path)
        if not os.path.exists(dir):
            os.makedirs(dir)

        if os.path.exists("{0}.filters".format(src_path)):
            copyfile("{0}.filters".format(src_path), "{0}.filters".format(dst_path))

        if not os.path.exists(dst_path):
            clone_and_modify_vcxproj(src_path, dst_path, src_version, dst_version)
        else:
            update_includes(src_path, dst_path)

        if append_custom_xml and custom_xml:
            append_xml(dst_path, custom_xml, ITEM_DEFINITION_GROUP_TAG)
   

#FIXME: add arguments handling
if __name__ == '__main__':
    process_projects_xml()
