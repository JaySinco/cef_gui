#!/bin/bash

current_dir=`readlink -f .`

pushd ${current_dir}/src/js/ \
&& \
rm -f ./resources/*.js \
&& \
rm -f ./resources/*.js.map \
&& \
tsc \
&& \
rm -rf ${current_dir}/bin/resources \
&& \
cp -r resources ${current_dir}/bin/resources \
&& \
popd