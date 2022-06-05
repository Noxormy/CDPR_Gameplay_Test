import os
import re


def get_all_files(basedir):
    for root, subfolders, files in os.walk(basedir):
        for file in os.listdir(root):
            yield root, file


def all_text_from_file(file):
    with open(file, 'r') as fd:
        return fd.read()


def update_vcxproj():
    project_name = 'reactphysics3d'
    directory_path = f'{os.path.dirname(os.path.realpath(__file__))}/packages/{project_name}/'
    tab = '      '

    property_group = '\<PropertyGroup\>(\n.*)*\<\/PropertyGroup\>\n'

    place_for_standard_regex = '(\<ClCompile\>)'
    standard = '<LanguageStandard>stdcpp17</LanguageStandard>'

    windows_target = '(\<WindowsTargetPlatformVersion\>).*(\</WindowsTargetPlatformVersion\>)'
    version = '10.0.17763.0'

    for root, file in get_all_files(directory_path):
        if project_name not in file.lower() or not file.lower().endswith('.vcxproj'):
            continue

        content = all_text_from_file(f'{root}/{file}')
        content = re.sub(property_group, '', content)
        content = re.sub(place_for_standard_regex, fr'\g<1>\n{tab}{standard}', content)
        content = re.sub(windows_target, fr'\g<1>{version}\g<2>', content)

        with open('{0}/{1}'.format(root, file), 'w') as fd:
            fd.write(content)


update_vcxproj()
