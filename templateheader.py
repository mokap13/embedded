# Program README
#     Standard libraries  <stdio.h>
#                     <math.h>
#     Header files    “globals.h”
#                     “types.h”
#         Program files program_file.c
#                     File prolog
#                     Usage and operating instructions
#                     Header file includes
#                     External definitions and declarations
#                     Functions
#                     Function prolog
#                     Function parameters
#                     Internal definitions and declarations
#                     Statements
#                     Operators
#                     Expressions
#                     More external data
#                     More functions
#                     Module files module_file.c
#                     Compilation utilities Makefile

moduleName = "usart1";

headerHead = ("#ifndef "+moduleName.upper() + "_H_INCLUDE\n"
+"#define "+ moduleName.upper() +"H_INCLUDE");

headerFooter = ("#endif //"+moduleName.upper());


srcMiddle =(
"/********************       Module describe                ********************/\n"+
"\n/********************       Standard libraries             ********************/\n"+
"\n/********************       RTOS Headers                   ********************/\n"+
"\n/********************       Header file includes           ********************/\n"+
"\n#include \"" + moduleName +".h\"\n"+
"\n/********************       Private types defenitions      ********************/\n"+
"\n/********************       Private defines                ********************/\n"+
"\n/********************       Private variable               ********************/\n"+
"\n/********************       Private funcions               ********************/\n"+
"\n/********************       Funcions implemetations        ********************/\n"
);

headerText =(headerHead +"\n\n"+ headerFooter);

fileHeader = open(moduleName+".h",'w');
fileHeader.write(headerText);

fileSource = open(moduleName+".c",'w');
fileSource.write(srcMiddle);

fileHeader.close();
fileSource.close();