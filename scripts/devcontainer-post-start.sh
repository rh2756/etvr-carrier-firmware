#!/usr/bin/env bash

git config --global --add safe.directory $PWD
git submodule update --init --recursive
