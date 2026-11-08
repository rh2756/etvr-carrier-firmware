#!/usr/bin/env bash

arguments=(
	"--project=build/compile_commands.json"
	"--platform=arm-cortex-m0-plus.xml"
	"--template=gcc"
	"--enable=all"
	"--error-exitcode=1"
	"--inline-suppr"
	"--suppressions-list=cppcheck-suppressions.txt"
	"--file-filter=src/**"
	"-D__GNUC__"
)

if [[ "$1" == "--export-arguments" ]]; then
    echo "@(${arguments[*]})"
    exit 0
fi

cppcheck ${arguments[*]}
