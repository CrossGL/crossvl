xmake macro --import=./Scripts/Macros/BuildAllConfigs.lua BuildAllConfigs
xmake macro --import=./Scripts/Macros/Clean.lua Clean
xmake macro --import=./Scripts/Macros/DeepClean.lua DeepClean
if [[ "$OSTYPE" == "darwin"* ]]; then
    xmake macro --import=./Scripts/Macros/GenerateSolutionXCode.lua GenXCode 
    xmake macro --import=./Scripts/Macros/macOS.lua MetalBuild
fi
