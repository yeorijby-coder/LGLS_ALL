# Microsoft Developer Studio Project File - Name="Ecs" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Ecs - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Ecs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Ecs.mak" CFG="Ecs - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Ecs - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Ecs - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Ecs - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../Bin/Release"
# PROP Intermediate_Dir "../../Bin/Release/EcsSv"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /I "../EcsLib" /I "../EcsLib/ButtonST" /I "../../Lib/XmlLib" /I "../../Lib/DciLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /map /debug /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=@echo off	set DirName=%date:~,4%%date:~5,2%%date:~8,2%%time:~,2%%time:~3,2%%time:~6,2%	set PathName="D:\project\카길\SOURCE\Bin\Release"	mkdir "%PathName%\EcsSv\mapfile\%DirName%"	copy "%PathName%\EcsSv\*.map" "%PathName%\EcsSv\mapfile\%DirName%"	copy "%PathName%\EcsSv\*.cod" "%PathName%\EcsSv\mapfile\%DirName%"	copy "%PathName%\*.exe" "%PathName%\EcsSv\mapfile\%DirName%"	copy "%PathName%\*.pdb" "%PathName%\EcsSv\mapfile\%DirName%"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Ecs - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../Bin/Debug"
# PROP Intermediate_Dir "../../Bin/Debug/EcsSv"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../EcsLib" /I "../EcsLib/ButtonST" /I "../../Lib/XmlLib" /I "../../Lib/DciLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Ecs - Win32 Release"
# Name "Ecs - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AsyncSocketEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ByteArrayEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangePasswordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CmdMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Config.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigPptSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigStdPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigSub1Page.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigSub2Page.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigSub3Page.cpp
# End Source File
# Begin Source File

SOURCE=.\Cv.cpp
# End Source File
# Begin Source File

SOURCE=.\CvDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CvFx.cpp
# End Source File
# Begin Source File

SOURCE=.\CvInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DataSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Debuger.cpp
# End Source File
# Begin Source File

SOURCE=.\Ecs.cpp
# End Source File
# Begin Source File

SOURCE=.\Ecs.rc
# End Source File
# Begin Source File

SOURCE=.\EcsDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\EcsDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\EcsLayout.cpp
# End Source File
# Begin Source File

SOURCE=.\EcsView.cpp
# End Source File
# Begin Source File

SOURCE=.\Equipment.cpp
# End Source File
# Begin Source File

SOURCE=.\Host.cpp
# End Source File
# Begin Source File

SOURCE=.\Info.cpp
# End Source File
# Begin Source File

SOURCE=.\InterfaceSk.cpp
# End Source File
# Begin Source File

SOURCE=.\InterfaceThreadSk.cpp
# End Source File
# Begin Source File

SOURCE=.\Job.cpp
# End Source File
# Begin Source File

SOURCE=.\JobDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\JobItem.cpp
# End Source File
# Begin Source File

SOURCE=.\JobItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\JobOfflineDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\JobTestDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Lgv.cpp
# End Source File
# Begin Source File

SOURCE=.\LgvcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LgvDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LgvInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\LgvManualDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LgvVehicle.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib.cpp
# End Source File
# Begin Source File

SOURCE=.\ListenerSk.cpp
# End Source File
# Begin Source File

SOURCE=.\Log.cpp
# End Source File
# Begin Source File

SOURCE=.\LogDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Login.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MelsecEthernetSk.cpp
# End Source File
# Begin Source File

SOURCE=.\MelsecEthernetThreadSk.cpp
# End Source File
# Begin Source File

SOURCE=.\MelsecFXEthernetSk.cpp
# End Source File
# Begin Source File

SOURCE=.\MelsecSerialSk.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageDisplayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MonitorListener.cpp
# End Source File
# Begin Source File

SOURCE=.\MonitorServer.cpp
# End Source File
# Begin Source File

SOURCE=.\msjexhnd.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\PasswordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PortEthernetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PortObject.cpp
# End Source File
# Begin Source File

SOURCE=.\PortSerialDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScDongJin.cpp
# End Source File
# Begin Source File

SOURCE=.\ScDongJinDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScDongJinManualDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ScRelocationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\spreadsheet.cpp
# End Source File
# Begin Source File

SOURCE=.\StartupTip.cpp
# End Source File
# Begin Source File

SOURCE=.\StationInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TrackHS.cpp
# End Source File
# Begin Source File

SOURCE=.\TrackInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\UserManagerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WarningDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AsyncSocketEx.h
# End Source File
# Begin Source File

SOURCE=.\ByteArrayEx.h
# End Source File
# Begin Source File

SOURCE=.\ChangePasswordDlg.h
# End Source File
# Begin Source File

SOURCE=.\CmdMsg.h
# End Source File
# Begin Source File

SOURCE=.\Color.h
# End Source File
# Begin Source File

SOURCE=.\Config.h
# End Source File
# Begin Source File

SOURCE=.\ConfigPptSheet.h
# End Source File
# Begin Source File

SOURCE=.\ConfigStdPage.h
# End Source File
# Begin Source File

SOURCE=.\ConfigSub1Page.h
# End Source File
# Begin Source File

SOURCE=.\ConfigSub2Page.h
# End Source File
# Begin Source File

SOURCE=.\ConfigSub3Page.h
# End Source File
# Begin Source File

SOURCE=.\Cv.h
# End Source File
# Begin Source File

SOURCE=.\CvDlg.h
# End Source File
# Begin Source File

SOURCE=.\CvFx.h
# End Source File
# Begin Source File

SOURCE=.\CvInfo.h
# End Source File
# Begin Source File

SOURCE=.\DataSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\Debuger.h
# End Source File
# Begin Source File

SOURCE=.\Ecs.h
# End Source File
# Begin Source File

SOURCE=.\EcsDef.h
# End Source File
# Begin Source File

SOURCE=.\EcsDefine.h
# End Source File
# Begin Source File

SOURCE=.\EcsDoc.h
# End Source File
# Begin Source File

SOURCE=.\EcsEnv.h
# End Source File
# Begin Source File

SOURCE=.\EcsLayout.h
# End Source File
# Begin Source File

SOURCE=.\EcsView.h
# End Source File
# Begin Source File

SOURCE=.\Equipment.h
# End Source File
# Begin Source File

SOURCE=.\Host.h
# End Source File
# Begin Source File

SOURCE=.\Info.h
# End Source File
# Begin Source File

SOURCE=.\InterfaceSk.h
# End Source File
# Begin Source File

SOURCE=.\InterfaceThreadSk.h
# End Source File
# Begin Source File

SOURCE=.\Job.h
# End Source File
# Begin Source File

SOURCE=.\JobDlg.h
# End Source File
# Begin Source File

SOURCE=.\JobItem.h
# End Source File
# Begin Source File

SOURCE=.\JobItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\JobOfflineDlg.h
# End Source File
# Begin Source File

SOURCE=.\JobTestDlg.h
# End Source File
# Begin Source File

SOURCE=.\Lgv.h
# End Source File
# Begin Source File

SOURCE=.\LgvcDlg.h
# End Source File
# Begin Source File

SOURCE=.\LgvDlg.h
# End Source File
# Begin Source File

SOURCE=.\LgvInfo.h
# End Source File
# Begin Source File

SOURCE=.\LgvManualDlg.h
# End Source File
# Begin Source File

SOURCE=.\LgvVehicle.h
# End Source File
# Begin Source File

SOURCE=.\Lib.h
# End Source File
# Begin Source File

SOURCE=.\ListenerSk.h
# End Source File
# Begin Source File

SOURCE=.\Log.h
# End Source File
# Begin Source File

SOURCE=.\LogDlg.h
# End Source File
# Begin Source File

SOURCE=.\Login.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MelsecEthernetSk.h
# End Source File
# Begin Source File

SOURCE=.\MelsecEthernetThreadSk.h
# End Source File
# Begin Source File

SOURCE=.\MelsecFXEthernetSk.h
# End Source File
# Begin Source File

SOURCE=.\MelsecSerialSk.h
# End Source File
# Begin Source File

SOURCE=.\MessageDisplayDlg.h
# End Source File
# Begin Source File

SOURCE=.\MonitorListener.h
# End Source File
# Begin Source File

SOURCE=.\MonitorServer.h
# End Source File
# Begin Source File

SOURCE=.\msjexhnd.h
# End Source File
# Begin Source File

SOURCE=.\PasswordDlg.h
# End Source File
# Begin Source File

SOURCE=.\PortEthernetDlg.h
# End Source File
# Begin Source File

SOURCE=.\PortObject.h
# End Source File
# Begin Source File

SOURCE=.\PortSerialDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScDongJin.h
# End Source File
# Begin Source File

SOURCE=.\ScDongJinDlg.h
# End Source File
# Begin Source File

SOURCE=.\ScDongJinManualDlg.h
# End Source File
# Begin Source File

SOURCE=.\ScInfo.h
# End Source File
# Begin Source File

SOURCE=.\ScRelocationDlg.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\spreadsheet.h
# End Source File
# Begin Source File

SOURCE=.\StartupTip.h
# End Source File
# Begin Source File

SOURCE=.\StationInfo.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TrackHS.h
# End Source File
# Begin Source File

SOURCE=.\TrackInfo.h
# End Source File
# Begin Source File

SOURCE=.\UserManagerDlg.h
# End Source File
# Begin Source File

SOURCE=.\WarningDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\About.ico
# End Source File
# Begin Source File

SOURCE=.\res\add01.ico
# End Source File
# Begin Source File

SOURCE=.\res\Baloon.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=".\res\아이콘\blob.ico"
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=".\res\아이콘\Box_2.ico"
# End Source File
# Begin Source File

SOURCE=.\res\Butterfly.ico
# End Source File
# Begin Source File

SOURCE=.\res\Cancel.ico
# End Source File
# Begin Source File

SOURCE=.\res\Cancel1_32x32x16.ico
# End Source File
# Begin Source File

SOURCE=.\res\Cancel3_32x32x256.ico
# End Source File
# Begin Source File

SOURCE=.\res\Cancel4_32x32x2.ico
# End Source File
# Begin Source File

SOURCE=.\res\CargillLogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CargillLogo.GIF
# End Source File
# Begin Source File

SOURCE=.\res\CargillLogo02.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CDRom.ico
# End Source File
# Begin Source File

SOURCE=.\res\Classes1_32x32x16.ico
# End Source File
# Begin Source File

SOURCE=.\res\classes2.ico
# End Source File
# Begin Source File

SOURCE=.\res\classes3.ico
# End Source File
# Begin Source File

SOURCE=.\res\classes4.ico
# End Source File
# Begin Source File

SOURCE=.\res\classes5.ico
# End Source File
# Begin Source File

SOURCE=.\res\classes6.ico
# End Source File
# Begin Source File

SOURCE=.\res\CLIP06.ICO
# End Source File
# Begin Source File

SOURCE=.\res\CLIP07.ICO
# End Source File
# Begin Source File

SOURCE=.\res\CLIP08.ICO
# End Source File
# Begin Source File

SOURCE=".\res\close(32x32).ico"
# End Source File
# Begin Source File

SOURCE=".\res\아이콘\close.ico"
# End Source File
# Begin Source File

SOURCE=.\res\config01.ico
# End Source File
# Begin Source File

SOURCE=.\res\config2.ico
# End Source File
# Begin Source File

SOURCE=.\res\CONTACTL.ICO
# End Source File
# Begin Source File

SOURCE=".\res\버튼리소스\COPY.BMP"
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor3.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor4.cur
# End Source File
# Begin Source File

SOURCE=".\res\버튼리소스\CUT.BMP"
# End Source File
# Begin Source File

SOURCE=.\res\cwp_e.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cwp_k.bmp
# End Source File
# Begin Source File

SOURCE=.\res\del01.ico
# End Source File
# Begin Source File

SOURCE=.\res\DISTLSTL.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Ecs.ico
# End Source File
# Begin Source File

SOURCE=.\res\Ecs.rc2
# End Source File
# Begin Source File

SOURCE=.\res\EcsCv.bmp
# End Source File
# Begin Source File

SOURCE=.\res\EcsDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\EcsEquip.bmp
# End Source File
# Begin Source File

SOURCE=.\res\EcsLogo2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\EcsRgv.bmp
# End Source File
# Begin Source File

SOURCE=.\res\eject.ico
# End Source File
# Begin Source File

SOURCE=.\res\EOapp.ico
# End Source File
# Begin Source File

SOURCE=.\res\Exit.ico
# End Source File
# Begin Source File

SOURCE=.\res\Explorer.ico
# End Source File
# Begin Source File

SOURCE=.\res\Frog1.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Frog2.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Frog3.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Frog4.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Halloween1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Halloween2.ico
# End Source File
# Begin Source File

SOURCE=.\res\Help.ico
# End Source File
# Begin Source File

SOURCE=.\res\Help2_32x32x256.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00009.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00010.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00011.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00012.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00013.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00014.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00015.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00016.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00017.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00018.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00019.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00020.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00021.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00022.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00023.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00024.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00025.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00026.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00027.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00028.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00029.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00030.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00031.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00032.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00033.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00034.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00035.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00036.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00037.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00038.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00039.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico211.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon5.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon6.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_.ico
# End Source File
# Begin Source File

SOURCE=.\res\Icon_Bulb.ico
# End Source File
# Begin Source File

SOURCE=.\res\Icon_Complete.ico
# End Source File
# Begin Source File

SOURCE=.\res\Icon_home.ico
# End Source File
# Begin Source File

SOURCE=.\res\Icon_Manual.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_ret.ico
# End Source File
# Begin Source File

SOURCE=.\res\IEDocument_48x48x256.ico
# End Source File
# Begin Source File

SOURCE=.\res\job_all1.ico
# End Source File
# Begin Source File

SOURCE=.\res\job_comp.ico
# End Source File
# Begin Source File

SOURCE=".\res\JPEG Image.ico"
# End Source File
# Begin Source File

SOURCE=".\res\Key manager.ico"
# End Source File
# Begin Source File

SOURCE=.\res\key.ico
# End Source File
# Begin Source File

SOURCE=.\res\Lamp1.ico
# End Source File
# Begin Source File

SOURCE=.\res\lb_fullpack.ico
# End Source File
# Begin Source File

SOURCE=.\res\LedOff.ico
# End Source File
# Begin Source File

SOURCE=".\res\버튼리소스\LedOff.ico"
# End Source File
# Begin Source File

SOURCE=".\res\아이콘\LedOff.ico"
# End Source File
# Begin Source File

SOURCE=.\res\LedOn.ico
# End Source File
# Begin Source File

SOURCE=".\res\버튼리소스\LedOn.ico"
# End Source File
# Begin Source File

SOURCE=".\res\아이콘\LedOn.ico"
# End Source File
# Begin Source File

SOURCE=.\res\left6.ico
# End Source File
# Begin Source File

SOURCE=.\res\Left6_32x32x256.ico
# End Source File
# Begin Source File

SOURCE=.\res\line.ico
# End Source File
# Begin Source File

SOURCE=".\res\아이콘\Log.ico"
# End Source File
# Begin Source File

SOURCE=.\res\log_alar.ico
# End Source File
# Begin Source File

SOURCE=.\res\log_all1.ico
# End Source File
# Begin Source File

SOURCE=.\res\log_erro.ico
# End Source File
# Begin Source File

SOURCE=.\res\login.bmp
# End Source File
# Begin Source File

SOURCE=".\res\login01(32x32).ico"
# End Source File
# Begin Source File

SOURCE=.\res\LogOff.ico
# End Source File
# Begin Source File

SOURCE=.\res\LogoMaker.bmp
# End Source File
# Begin Source File

SOURCE=.\LogoUser.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LogoUser.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NETHOOD.ICO
# End Source File
# Begin Source File

SOURCE=.\res\network.ico
# End Source File
# Begin Source File

SOURCE=.\res\network3.ico
# End Source File
# Begin Source File

SOURCE=.\res\No3_32x32x256.ico
# End Source File
# Begin Source File

SOURCE=.\res\Ok.ico
# End Source File
# Begin Source File

SOURCE=.\res\Ok3_32x32x256.ico
# End Source File
# Begin Source File

SOURCE=.\res\Open.ico
# End Source File
# Begin Source File

SOURCE=.\res\OPEN01.ico
# End Source File
# Begin Source File

SOURCE=".\res\버튼리소스\PASTE.BMP"
# End Source File
# Begin Source File

SOURCE=.\res\PLC01.ico
# End Source File
# Begin Source File

SOURCE=".\res\버튼리소스\PREVIEW.BMP"
# End Source File
# Begin Source File

SOURCE=".\res\버튼리소스\PRINT.BMP"
# End Source File
# Begin Source File

SOURCE=.\res\print01.ico
# End Source File
# Begin Source File

SOURCE=.\res\printer01.ico
# End Source File
# Begin Source File

SOURCE=.\res\product.ico
# End Source File
# Begin Source File

SOURCE=".\res\product01(24x24).ico"
# End Source File
# Begin Source File

SOURCE=.\res\product01.ico
# End Source File
# Begin Source File

SOURCE=.\res\product02.ico
# End Source File
# Begin Source File

SOURCE=.\res\Razor_32x32x256.ico
# End Source File
# Begin Source File

SOURCE=.\res\rect_both.ico
# End Source File
# Begin Source File

SOURCE=.\res\report.ico
# End Source File
# Begin Source File

SOURCE=.\res\report01.ico
# End Source File
# Begin Source File

SOURCE=.\res\Return.ico
# End Source File
# Begin Source File

SOURCE=.\res\right6.ico
# End Source File
# Begin Source File

SOURCE=.\res\Right6_32x32x256.ico
# End Source File
# Begin Source File

SOURCE=.\res\Run.ico
# End Source File
# Begin Source File

SOURCE=.\res\Search.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Search1.ico
# End Source File
# Begin Source File

SOURCE=.\res\sendfile.ico
# End Source File
# Begin Source File

SOURCE=".\res\버튼리소스\sendfile.ico"
# End Source File
# Begin Source File

SOURCE=.\res\Sound.ico
# End Source File
# Begin Source File

SOURCE=.\res\symbol_02.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar_Log.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Tools4.ico
# End Source File
# Begin Source File

SOURCE=.\res\user.ico
# End Source File
# Begin Source File

SOURCE=".\res\아이콘\User.ico"
# End Source File
# Begin Source File

SOURCE=.\res\user1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\user1.ico
# End Source File
# Begin Source File

SOURCE=.\res\user2.ico
# End Source File
# Begin Source File

SOURCE=.\res\usercard.ico
# End Source File
# Begin Source File

SOURCE=.\res\userH.ico
# End Source File
# Begin Source File

SOURCE=.\res\Log\W95MBX01.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Log\W95MBX02.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Log\W95MBX03.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Log\W95MBX04.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Web2.ico
# End Source File
# Begin Source File

SOURCE=.\res\Winzip1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Workgroup.ico
# End Source File
# Begin Source File

SOURCE=".\res\나가기.ico"
# End Source File
# Begin Source File

SOURCE=".\res\저장.ico"
# End Source File
# End Group
# Begin Group "Utility"

# PROP Default_Filter "cpp;h"
# Begin Source File

SOURCE=.\ReportCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ReportHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportHeader.h
# End Source File
# End Group
# Begin Group "MSXML"

# PROP Default_Filter "cpp;h"
# End Group
# Begin Source File

SOURCE=.\Config.xml
# End Source File
# Begin Source File

SOURCE=.\Ecs.xml
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Ecs : {1A42C65C-6463-11D3-9EF9-8582B1AE7F27}
# 	2:21:DefaultSinkHeaderFile:cv.h
# 	2:16:DefaultSinkClass:CCv
# End Section
# Section Ecs : {09721101-ACA9-11D0-B09D-00805FA8E730}
# 	2:5:Class:COpBox
# 	2:10:HeaderFile:opbox.h
# 	2:8:ImplFile:opbox.cpp
# End Section
# Section Ecs : {65DAA0EA-B628-4283-9B6D-8ACE3BAE75B5}
# 	2:5:Class:CGmsBtn
# 	2:10:HeaderFile:gmsbtn.h
# 	2:8:ImplFile:gmsbtn.cpp
# End Section
# Section Ecs : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section Ecs : {4ACC38F3-ABDD-11D0-B09C-0800093C9607}
# 	2:21:DefaultSinkHeaderFile:rack.h
# 	2:16:DefaultSinkClass:CRack
# End Section
# Section Ecs : {293AB9A4-6B70-11D3-9EF9-9466F5748609}
# 	2:5:Class:CSc
# 	2:10:HeaderFile:sc.h
# 	2:8:ImplFile:sc.cpp
# End Section
# Section Ecs : {1A42C65A-6463-11D3-9EF9-8582B1AE7F27}
# 	2:5:Class:CCv
# 	2:10:HeaderFile:cv.h
# 	2:8:ImplFile:cv.cpp
# End Section
# Section Ecs : {AADD219E-665F-11D1-970A-006097933291}
# 	2:21:DefaultSinkHeaderFile:ecsgraph.h
# 	2:16:DefaultSinkClass:CEcsGraph
# End Section
# Section Ecs : {09721103-ACA9-11D0-B09D-00805FA8E730}
# 	2:21:DefaultSinkHeaderFile:opbox.h
# 	2:16:DefaultSinkClass:COpBox
# End Section
# Section Ecs : {4ACC38F1-ABDD-11D0-B09C-0800093C9607}
# 	2:5:Class:CRack
# 	2:10:HeaderFile:rack.h
# 	2:8:ImplFile:rack.cpp
# End Section
# Section Ecs : {B2E7BDA0-A24A-4547-84D3-EE5C3BBE95DC}
# 	2:21:DefaultSinkHeaderFile:gmsbtn.h
# 	2:16:DefaultSinkClass:CGmsBtn
# End Section
# Section Ecs : {AADD219C-665F-11D1-970A-006097933291}
# 	2:5:Class:CEcsGraph
# 	2:10:HeaderFile:ecsgraph.h
# 	2:8:ImplFile:ecsgraph.cpp
# End Section
# Section Ecs : {293AB9A6-6B70-11D3-9EF9-9466F5748609}
# 	2:21:DefaultSinkHeaderFile:sc.h
# 	2:16:DefaultSinkClass:CSc
# End Section
