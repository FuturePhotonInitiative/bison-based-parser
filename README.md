This is the frontend for a command parser based on Bison and Flex. This is an implementation of the version 2 command specification, which adds and deletes some commands. Check out the Documentation folder for more details on this project.

Shreya and Sydney proposed the new command specification [here](\\Hawk\ridl\internal\projects\EVT\internal documents\PETB Documentation\Summer_2019\Parser_proposal.docx)

They also handwrote a parser for version 2 that does not use Bison/Flex. It can be found [here](\\blackdog\c\EVT\PETB\Summer_2019\Vivado\PETB_R_v2.0\PETB_R_v2.0.sdk\PETB_SDK_v0_19\src)
Most of these backend files can be used for the B/F implementation too, all that is necessary was to modify command_caller.c
command_caller.c was created based on the api specification found [here](\\Hawk\ridl\internal\projects\EVT\internal documents\PETB Documentation\Summer_2019\Commands-to-functions.docx)

The documentation for the current implementation of version 1 is here: 
[high level description](\\Hawk\ridl\internal\projects\EVT\internal documents\Spring Semester 2019\PETB\Documentation\High Level Description.docx)
[development guide](\\Hawk\ridl\internal\projects\EVT\internal documents\Spring Semester 2019\PETB\Documentation\Developer Guide.docx)

The reason thet there is two implementations of the version 2 command spec is that we dont know which way Precision OT wants to go. If they want to add many more commands it may be more sustainable to keep the Bison implementation. If not, they can just use the handwritten one.

Ideally, the backend should be the same for both implementations, but command_caller.c and the front end parser files will be different.