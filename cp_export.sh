#!/bin/bash

cp content/export/*.mesh data/models/
if [[ -e content/export/*.jpg ]]; then
    cp content/export/*.jpg data/textures/
fi
if [[ -e content/export/*.png ]]; then
    cp content/export/*.png data/textures/
fi
cp content/export/*.material data/materials/
