#!/usr/bin/env python
# Oresis SW files
# aangepast voor pinkeltje - ewout
# 20 Augustus 2001 - aangepast voor Oldenburgh BV, sperwer windregelaar
# 22-01-2012 - aparte versie gemaakt voor indexering LM3S projekt
# 28-01-2012 - nieuwe versie in Python geschreven

import os
import subprocess

print("TAGS vinden voor de directory ", os.getcwd());

dirs = (".", "../gemeenschappelijk/STM32F4Discovery", "../gemeenschappelijk/Libraries/STM32F4xx_StdPeriph_Driver",
        "../gemeenschappelijk/Libraries/CMSIS/Device/ST/STM32F4xx");

subprocess.check_call(["find ./config -name \"*.[ch]\" | grep -v Base | etags -"], shell=True);

for naam in dirs:
    print("Directory is:", naam);
    arg1 = "find " + naam + " -name \"*.[chS]\" | grep -v Base | etags -a -";
    arg2 = "find " + naam + " -name \"*.cpp\" | grep -v Base | etags -a -";
    subprocess.check_call(arg1, shell=True);
    subprocess.check_call(arg2, shell=True);

# dirs = ("./efm32geckos","./efm32geckos/h",
# "./efm32geckos/gecko","./efm32geckos/gecko/efm32g880",
#	"./efm32geckos/efm32lib/src","./efm32geckos/efm32usb/src");

# for naam in dirs :
#    print("Directory is:",naam);
#    arg1 = "find "+naam+" \( ! -name "+naam+"  -prune \) -name \"*.[chS]\" | grep -v Base | etags -a -";
#    arg2 = "find "+naam+" \( ! -name "+naam+"  -prune \)  -name \"*.cpp\" | grep -v Base | etags -a -";
#    subprocess.check_call(arg1,shell=True);
#    subprocess.check_call(arg2,shell=True);
