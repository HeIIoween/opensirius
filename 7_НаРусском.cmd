xcopy .\Misc\Rus\equipresources.dll .\exe /y
xcopy .\Misc\Rus\infocards.dll .\exe /y
xcopy .\Misc\Rus\misctext.dll .\exe /y
xcopy .\Misc\Rus\misctextinfo2.dll .\exe /y
xcopy .\Misc\Rus\nameresources.dll .\exe /y
xcopy .\Misc\Rus\offerbriberesources.dll .\exe /y
xcopy .\Misc\Rus\resources.dll .\exe /y

xcopy .\Misc\Rus\Audio\*.* .\data\audio\ /y /r /e

taskkill /IM freelancer.bin /F
taskkill /IM freelancer.exe /F
start /min exe/freelancer.bin -s213.141.152.43:2302 -I 213.141.152.43:2302