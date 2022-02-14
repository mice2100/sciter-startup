#!/bin/bash

BNFILE=BN.txt
if [ -r $BNFILE ];
then
    let bn=`cat $BNFILE`
else
    let bn=0
fi

let bn=$bn+1
echo $bn > $BNFILE
