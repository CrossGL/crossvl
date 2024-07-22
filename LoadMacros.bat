@echo off

echo Loading xmake macros

xmake macro --import=Scripts/Macros/BuildAllConfigs.lua BuildAllConfigs
xmake macro --import=Scripts/Macros/Clean.lua Clean
xmake macro --import=Scripts/Macros/DeepClean.lua DeepClean
xmake macro --import=Scripts/Macros/GenerateSolutionVS2022.lua GenVS

echo Finished loading macros
