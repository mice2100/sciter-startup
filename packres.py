from sys import platform
import os

if platform=='darwin':
    scapp = 'vendors/osx/scapp'
    packutil = 'vendors/osx/packfolder'
else:
    scapp = 'C:\\Users\\george\\Downloads\\sciter-js-sdk-5.0.3.15\\bin\\windows\\x32\\scapp.exe'
    packutil = 'C:\\Users\\george\\Downloads\\sciter-js-sdk-5.0.3.15\\bin\\windows\\packfolder.exe'

os.system(f'{packutil} src/ui src/resources.cpp')

