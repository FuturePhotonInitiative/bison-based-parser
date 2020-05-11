# Microblase parser
This is the full parser and command caller for the PETB MicroBlase based on
 Bison and Flex for the EVT project.

The user guide is EVT12, and the developer guide is EVT13. The github repo is 
located [here](https://github.com/FuturePhotonInitiative/bison-based-parser).

## Build
These are all the files that are necessary to build the software. The steps
 are in EVT 12, but reiterated here:

#### Autogenerate the Parser
Run `bison_flex/make.ps1` in a powershell. This will output the .c files
 based on the parser.y and lexer.l.
 
#### Compile
In Vivado's SDK, click the hammer button to build the ELF file. Now that the
 ELF file exists, it can be integrated with a hardware definition file to
 flash the MicroBlase with.