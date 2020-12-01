# Plain Build Gen
A utility which reads a config file describing what should be built and gnerates a windows batch 
script which can then be used to build the program. This avoids having to run a build system which 
might be desirable.
## Usage
There is an example config file at `/misc/test.plain_build_gen.cfg` which can be passed into the 
tool to generate a build script for building itself. This config file also serves as a showcase of 
all supported sections that can be present in a config file. A sensible file extension for config 
files for this tool could be `.plain_build_gen.cfg` as used with the example.
## Setup
Ensure that the path to `vcvars64.bat` has been added to the `PATH` environment variable then 
create a shortcut to the command line and append the following to the target making sure to 
replace `%ProjectPath%` with path to the root of the cloned repo 
`/k %ProjectPath%\misc\shell.bat`. This setup will allow access to the `build` script which 
runs the compiler and linker. The result of compilation can then be found in the `build` 
directory.
