#!/bin/sh

cd `dirname $0`
cd ..

cppcheck src/ -q --enable=warning,style --error-exitcode=1

