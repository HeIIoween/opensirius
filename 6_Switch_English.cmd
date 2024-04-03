@echo off
echo GITHub might be unresponsible - just wait... Zzz...
echo And do not touch the files! :-E
git restore .
git pull https://github.com/HeIIoween/opensirius.git
.\Misc\PortableGit\bin\git restore .
.\Misc\PortableGit\bin\git pull https://github.com/HeIIoween/opensirius.git

xcopy .\Misc\Eng\equipresources.dll .\exe /y
xcopy .\Misc\Eng\infocards.dll .\exe /y
xcopy .\Misc\Eng\misctext.dll .\exe /y
xcopy .\Misc\Eng\misctextinfo2.dll .\exe /y
xcopy .\Misc\Eng\nameresources.dll .\exe /y
xcopy .\Misc\Eng\offerbriberesources.dll .\exe /y
xcopy .\Misc\Eng\resources.dll .\exe /y

xcopy .\Misc\Eng\Audio\*.* .\data\audio\ /y /r /e

taskkill /IM freelancer.bin /F
taskkill /IM freelancer.exe /F
start /min exe/freelancer.bin -s213.141.152.43:2302 -I 213.141.152.43:2302
