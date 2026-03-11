cmake --preset "Release-ms" -DDO_TESTS=ON
cmake --build --preset "VS-Build-Release-ms"
cd __BUILD__/Release/Tests/
Tests.exe
pause