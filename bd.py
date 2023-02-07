import argparse
import os
import shutil
from sys import platform

if platform=='darwin':
    defos="osx"
else:
    defos="x32"

parse = argparse.ArgumentParser()
parse.add_argument("-os", default=defos, dest="os", choices=["x32", "x64", "osx"])
parse.add_argument("-build", default="Release", dest="build", choices=["Debug", "Release"])
parse.add_argument("-step", default="all", dest="step", choices=["cmake", "build", "all"])
parse.add_argument("-company", default="nemo", dest="company", choices=["nemo", "mpp"])

# Increase build number
BNFILE = "./src/BN.txt"
with open(BNFILE) as f:
    bn = int(f.read().strip()) + 1
with open(BNFILE, "w") as f:
    f.write(str(bn))

args = parse.parse_args()
srcfolder = "src"
buildpath = f'build-{args.os}-{args.build}'

if not os.path.isdir(buildpath):
    args.step = 'all'
    os.mkdir(buildpath)

if args.step=='all' or args.step=='cmake':
    if args.os=='x32' or args.os=='x64':
        envpaths = os.environ.get('path').lower().split(';')
        vcpkgpath = 'c:/vcpkg'
        
    if args.os=='x32':
        triplet = 'x86-clang-static'
        envpathsN = [i for i in envpaths if not i.startswith('c:\\msys64\\')]
        envpathsN.append('c:\\msys64\\clang32\\bin')
        os.environ['path'] = ';'.join(envpathsN)
    elif args.os=='x64':
        triplet = 'x64-clang-static'
        envpathsN = [i for i in envpaths if not i.startswith('c:\\msys64\\')]
        envpathsN.append('c:\\msys64\\clang64\\bin')
        os.environ['path'] = ';'.join(envpathsN)
    else:
        vcpkgpath = '/Users/zhijunlou/Documents/code/vcpkg'
        triplet = 'x64-osx-release'

    buildopts = f'cmake --toolchain {vcpkgpath}/scripts/buildsystems/vcpkg.cmake -DVCPKG_OVERLAY_TRIPLETS=./triplets -DVCPKG_TARGET_TRIPLET={triplet} -G Ninja'
    buildargs = f' -S {srcfolder} -B {buildpath}'

    buildopts += f' -DARCH={args.os}'

    if args.build=="Debug":
        buildopts += ' -DLOCALMODE=ON -DDEBUGWINDOW=ON'
    else:
        buildopts += ' -DLOCALMODE=OFF -DDEBUGWINDOW=OFF'

    cmds = buildopts+buildargs 

    os.system(cmds)

if args.step=='all' or args.step=='build':
    cmds = f'cmake --build {buildpath} -t install'
    os.system(cmds)
