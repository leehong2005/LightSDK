@echo Off

set ReleaseFolder=20110603
set SVNPath=C:\Progra~1\TortoiseSVN\bin\TortoiseProc.exe
set SVNFileName=TortoiseProc.exe
set SVNTargetURL=http://192.168.5.243/svn/LightSDK/Source/Trunk


md %ReleaseFolder%
md %ReleaseFolder%\Document
md %ReleaseFolder%\Include
md %ReleaseFolder%\Lib
md %ReleaseFolder%\Lib\x86
md %ReleaseFolder%\Lib\x64
md %ReleaseFolder%\Bin\
md %ReleaseFolder%\Bin\x86
md %ReleaseFolder%\Bin\x64
md %ReleaseFolder%\Source



echo -------- Copy document files ------------------------------------
copy ..\Document\SDB\LightSDK_SDS.docx		%ReleaseFolder%\Document\LightSDK_SDS.docx
copy ..\Document\ReleaseNote\LightSDK_ReleaseNotes.docx	%ReleaseFolder%\Document\LightSDK_ReleaseNotes.docx




echo -------- Copy header files ---------------------------------------
xcopy ..\Source\Trunk\SdkCommonLib\Src\Include		%ReleaseFolder%\Include /E /C /S /I /Y /Q
xcopy ..\Source\Trunk\Extinc				%ReleaseFolder%\Include /E /C /S /I /Y /Q





echo -------- Copy redist files ---------------------------------------
xcopy ..\Other\Resources\redist\x86			%ReleaseFolder%\Bin\x86 /E /C /S /I /Y /Q
xcopy ..\Other\Resources\redist\x64			%ReleaseFolder%\Bin\x64 /E /C /S /I /Y /Q




echo -------- Copy source files ---------------------------------------
xcopy ..\Source\Trunk			%ReleaseFolder%\Source\ /E /C /S /I /Y /Q




echo -------- Delete these disuse files. ------------------------------
rd %ReleaseFolder%\Source\Test /s /q
del %ReleaseFolder%\Source\LightSDK.Test.sln /s /q




echo -------- Copy binary files ---------------------------------------
copy ..\Source\Trunk\Bin\Debug\SdkCommonLib.dll			%ReleaseFolder%\Bin\x86\SdkCommonLib.dll
copy ..\Source\Trunk\Bin\Debug\SdkFrameworkLib.dll		%ReleaseFolder%\Bin\x86\SdkFrameworkLib.dll
copy ..\Source\Trunk\Bin\Debug\SampleAnimation.exe		%ReleaseFolder%\Bin\x86\SampleAnimation.exe
copy ..\Source\Trunk\Bin\Debug\SampleAssocHandler.exe		%ReleaseFolder%\Bin\x86\SampleAssocHandler.exe
copy ..\Source\Trunk\Bin\Debug\SampleCryptTool.exe		%ReleaseFolder%\Bin\x86\SampleCryptTool.exe
copy ..\Source\Trunk\Bin\Debug\SampleFileSearch.exe		%ReleaseFolder%\Bin\x86\SampleFileSearch.exe
copy ..\Source\Trunk\Bin\Debug\SampleInkTool.exe		%ReleaseFolder%\Bin\x86\SampleInkTool.exe
copy ..\Source\Trunk\Bin\Debug\SampleKnownFolder.exe		%ReleaseFolder%\Bin\x86\SampleKnownFolder.exe
copy ..\Source\Trunk\Bin\Debug\SamplePreviewHandler.exe		%ReleaseFolder%\Bin\x86\SamplePreviewHandler.exe
copy ..\Source\Trunk\Bin\Debug\SampleVolumeController.exe	%ReleaseFolder%\Bin\x86\SampleVolumeController.exe


copy ..\Source\Trunk\Bin\Debug\SdkCommonLib.lib			%ReleaseFolder%\Lib\x86\SdkCommonLib.lib
copy ..\Source\Trunk\Bin\Debug\SdkFrameworkLib.lib		%ReleaseFolder%\Lib\x86\SdkFrameworkLib.lib



cd %ReleaseFolder%\Source
echo -------- Clean up ---------------------------------------
@echo Off
del /s /a *.suo *.ncb *.user *.pdb *.netmodule *.aps *.ilk 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\x64" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\Debug" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\Release" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\Bin" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\Obj" 2>nul

rem If the Properties directory is empty, remove it
FOR /R . %%d in (.) do rd /q "%%d\Properties" 2> nul





cd %ReleaseFolder%\Sample
@echo Off
del /s /a *.suo *.ncb *.user *.pdb *.netmodule *.aps *.ilk 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\x64" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\Debug" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\Release" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\Bin" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\Obj" 2>nul

rem If the Properties directory is empty, remove it
FOR /R . %%d in (.) do rd /q "%%d\Properties" 2> nul



pause