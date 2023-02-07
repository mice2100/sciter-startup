import os
import shutil
from sys import platform

dst="vendors"
# Rebuild because ffmpeg include files-used by windows version will effect macos version
shutil.rmtree(dst, ignore_errors=True)
os.makedirs(dst)

if platform=='win32':
    sct="c:/Users/george/Downloads/sciter-js-sdk-5.0.2.2"
    shutil.copytree(f'{sct}/include', dst+'/include', dirs_exist_ok=True)
    tmp=dst+'/x32'
    os.makedirs(tmp, exist_ok=True)
    shutil.copy(sct+'/bin/windows/x32/sciter.dll', tmp)
    shutil.copy(sct+'/bin/windows/packfolder.exe', tmp)
    shutil.copy(sct+'/bin/windows/x32/scapp.exe', tmp)

    tmp=dst+'/x64'
    os.makedirs(tmp, exist_ok=True)
    shutil.copy(sct+'/bin/windows/x64/sciter.dll', tmp)
elif platform=='darwin':
    sct="/Users/zhijunlou/Downloads/sciter-js-sdk-5.0.1.8"
    shutil.copytree(f'{sct}/include', dst+'/include', dirs_exist_ok=True)
    tmp=dst+'/osx'
    os.makedirs(tmp, exist_ok=True)
    shutil.copy(sct+'/bin/macosx/libsciter.dylib', tmp)
    shutil.copy(sct+'/bin/macosx/packfolder', tmp)
    shutil.copy(sct+'/bin/macosx/scapp', tmp)
    os.system(f'xattr -c {tmp}/*')
