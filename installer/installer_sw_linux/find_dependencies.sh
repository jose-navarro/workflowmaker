#!/bin/bash

dpkg-shlibdeps -O ./usr/local/bin/toolkiteditor     > dependencies.txt
dpkg-shlibdeps -O ./usr/local/bin/workfloweditor   >> dependencies.txt
dpkg-shlibdeps -O ./usr/local/bin/workflowlauncher >> dependencies.txt

