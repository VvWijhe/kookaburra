#!/usr/bin/env python
# Oresis SW files
# aangepast voor pinkeltje - ewout
# 20 Augustus 2001 - aangepast voor Oldenburgh BV, sperwer windregelaar
# 22-01-2012 - aparte versie gemaakt voor indexering LM3S projekt
# 28-01-2012 - nieuwe versie in Python geschreven
# 14-02-2012 - omgezet naar svn commit script

import os,subprocess;

print("SVN Commit de directory ",os.getcwd());

dirs = (".","../gemeenschappelijk");

subprocess.check_call(["emacs -nw veranderingen.txt"],shell=True);

for naam in dirs :
    print("SVN Directory is:",naam);
    arg1 = "/usr/local/bin/svn commit "+naam+" -F veranderingen.txt";
    subprocess.check_call(arg1,shell=True);

subprocess.check_call(["rm veranderingen.txt"],shell=True);










