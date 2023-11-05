
for /r %%f in (*.vert) do (
	%VK_SDK_PATH%\Bin\glslc %%f -o %%~f.spv
)

for /r %%f in (*.frag) do (
	%VK_SDK_PATH%\Bin\glslc %%f -o %%~f.spv
)

pause