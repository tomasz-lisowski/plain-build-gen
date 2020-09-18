# Tools
## Setup
Ensure that the path to `vcvars64.bat` has been added to the `PATH` environment variable then 
create a shortcut to the command line and append the following to the target making sure to 
replace `%ProjectPath%` with path to the root of the cloned repo 
`/k %ProjectPath%\misc\shell.bat`. This setup will allow access to the `build` script which 
runs the compiler and linker. The result of compilation can then be found in the `build` 
directory.