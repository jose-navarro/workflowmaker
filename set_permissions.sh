#!/bin/bash

# Look for all the bash scripts (.sh) and add the execution flag to all of them.

find . -type f -name "*.sh" -exec chmod +x {} \;

