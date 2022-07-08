for /r %%f in (*.vert) do (
	%VK_SDK_PATH%\Bin\glslc %%f -o %%~df%%~pf%%~nf.spv
)

for /r %%f in (*.frag) do (
	%VK_SDK_PATH%\Bin\glslc %%f -o %%~df%%~pf%%~nf.spv
)

pause