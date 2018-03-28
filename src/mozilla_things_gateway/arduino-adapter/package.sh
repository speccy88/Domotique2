#!/bin/bash

set -e

version=$(grep version package.json | cut -d: -f2 | cut -d\" -f2)

# Clean up from previous releases
rm -rf *.tgz package
rm -f SHA256SUMS
rm -rf lib

# Prep new package
mkdir lib
mkdir package

# Pull down Python dependencies
pip3 install -r requirements.txt -t lib --prefix ""
