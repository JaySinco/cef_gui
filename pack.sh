#!/bin/bash

current_dir=`readlink -f .`
source_dir=${current_dir}/render
output_dir=${current_dir}/bin/resources

pushd ${source_dir}

if [ ! -d "node_modules" ]; then
    npm install
fi

rm -rf ${output_dir} \
&& \
mkdir -p ${output_dir} \
&& \
npx webpack \
&& \
cp -r ${source_dir}/src/pages/*.html ${output_dir}/

popd