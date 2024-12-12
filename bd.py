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
    preset = f'{args.os}-{args.build}'
    buildopts = f'cmake --preset {preset} -S {srcfolder} -B {buildpath}'

    cmds = buildopts

    print(cmds)
    os.system(cmds)

if args.step=='all' or args.step=='build':
    cmds = f'cmake --build {buildpath} --config {args.build} -t install'
    os.system(cmds)
