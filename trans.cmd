@echo off

utils\TransProject.exe libs\irbis\irbis.2017.vcxproj libs\irbis\irbis.2015.vcxproj
utils\TransProject.exe libs\irbis\irbis.2017.vcxproj libs\irbis\irbis.2019.vcxproj

utils\TransProject.exe tests\safeTests\safeTests.2017.vcxproj tests\safeTests\safeTests.2015.vcxproj
utils\TransProject.exe tests\safeTests\safeTests.2017.vcxproj tests\safeTests\safeTests.2019.vcxproj

