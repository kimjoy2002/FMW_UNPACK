@echo off
setlocal enabledelayedexpansion

rem ���� ���� ����
set "SOURCE_DIR=.\���ø�Ʈ����"
rem ���� �� ������ ���� ����
set "TARGET_DIR=.\���ø�Ʈ�����ĵ�����"

if not exist "%SOURCE_DIR%" (
    echo ����: ���� ���� ������ �������� �ʽ��ϴ�.
    exit /b 1
)

if not exist "%TARGET_DIR%" (
    mkdir "%TARGET_DIR%"
)

rem ���� ���� �� �� data ���� Ž��
for /d %%D in ("%SOURCE_DIR%\*") do (
    set "FOLDERNAME=%%~nxD"
    echo  ====== %%~nxD ======
    pushd "%%D"
	del repacked.dat
	del cyphered.dat
    
	echo  "..\..\FMWIRepacking" decyphered.dat.gsty
    rem FMWIRepack �� FMWICypher ����
    "..\..\FMWIRepacking" decyphered.dat.gsty
	echo  "..\..\FMWICypher" repacked.dat c
    "..\..\FMWICypher" repacked.dat c
    
    rem ������ encyphered.dat�� ���ο� ��ġ�� �̵� �� �̸� ����
    if exist "cyphered.dat" (
	    echo move "cyphered.dat" "..\..\%TARGET_DIR%\!FOLDERNAME!.dat"
        move "cyphered.dat" "..\..\%TARGET_DIR%\!FOLDERNAME!.dat"
    )
    echo  ====== %%~nxD END ======
    echo .
    
    popd
)

echo �۾� �Ϸ�!
