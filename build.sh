#!/bin/bash

if [[ ! -d build ]]
then
	mkdir build
	pushd build 1> /dev/null;

	cmake ../

 	popd;
fi

pushd build 1> /dev/null;

cmake --build .

popd 1> /dev/null;
