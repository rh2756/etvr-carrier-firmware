#!/usr/bin/env bash

git config --global --add safe.directory /workspaces/etvr-carrier-firmware
git submodule update --init --recursive
