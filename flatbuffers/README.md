# Flatbuffers

This folder contains the Flatbuffers definition file and the required configuration for Typescript to Javascript compilation.

Here are the initial steps:

1. Install node.js version 16 at the minimum
1. Unzip `flatc.exe` into the current folder
1. `npm install typescript --save-dev`
1. `npm install flatbuffers`

Then, when the schema has been updated, regenerated the C++ and JS files

1. `flatc --cpp --scoped-enums --gen-all --no-emit-min-max-enum-values -o ../include ohab_weather.fbs`
1. `flatc --ts ohab_weather.fbs`
1. `npx tsc`
1. Take the various classes generated in the build folder and paste them in place of the existing ones in the openHAB rule