@echo off 
echo GITHub mozhet tormozit'. Ozhidayte...
echo I ne trogay'te faily...! :-E
echo GITHub might be unresponsible - just wait... Zzz...
echo And do not touch the files! :-E
git gc --auto
git pull https://github.com/HeIIoween/opensirius.git
.\Misc\PortableGit\bin\git gc --auto
.\Misc\PortableGit\bin\git pull https://github.com/HeIIoween/opensirius.git

taskkill /IM freelancer.bin /F
taskkill /IM freelancer.exe /F
start /min exe/freelancer.bin -dx -noframelimit -w
