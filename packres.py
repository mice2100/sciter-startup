from sys import platform
import os

if platform=='darwin':
    scapp = 'vendors/osx/scapp'
    packutil = 'vendors/osx/packfolder'
else:
    scapp = 'vendors\\x32\\scapp.exe'
    packutil = 'vendors\\x32\\packfolder.exe'

os.system(f'{packutil} src/ui src/resources.cpp')

