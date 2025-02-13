#!/bin/bash
git init
pip install pre-commit
pre-commit install
code myfile.cpp
git add myfile.cpp 
git commit -m myfile.cpp