@echo off

echo Loading xmake macros

xmake macro --import=Scripts/Macros/BuildAllConfigs.lua BuildAllConfigs
xmake macro --import=Scripts/Macros/Clean.lua Clean
xmake macro --import=Scripts/Macros/DeepClean.lua DeepClean
xmake macro --import=Scripts/Macros/WatchDir.lua WatchDir
xmake macro --import=Scripts/Macros/GenerateSolutionVS2022.lua GenVS
xmake macro --import=Scripts/Macros/GenerateSolutionXCode.lua GenXC

echo Finished loading macros
