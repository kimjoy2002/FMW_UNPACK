@echo off
setlocal enabledelayedexpansion

rem ù ��° ����: ���� ����
set "SOURCE_DIR=���ø�Ʈ������"
rem �� ��° ����: ��� ����
set "TARGET_DIR=���ø�Ʈ����"

if not exist "%SOURCE_DIR%" (
    echo ����: ���� ������ �������� �ʽ��ϴ�.
    exit /b 1
)

if not exist "%TARGET_DIR%" (
    mkdir "%TARGET_DIR%"
)

rem ���� �������� .dat ���� ��� ��������
for %%F in ("%SOURCE_DIR%\*.dat") do (
    set "FILENAME=%%~nF"
    set "FILEEXT=%%~xF"
    
    rem ���� �����
    set "TARGET_PATH=%TARGET_DIR%\!FILENAME!"
    if not exist "!TARGET_PATH!" (
        mkdir "!TARGET_PATH!"
    )
    
    rem ��ɾ� ����
    echo  ====== %%~nF ======
    pushd "!TARGET_PATH!"
	echo "..\..\FMWICypher" "..\..\%SOURCE_DIR%\%%~nxF" d
    "..\..\FMWICypher" "..\..\%SOURCE_DIR%\%%~nxF" d
	echo ..\..\FMWIUnpack" decyphered.dat
    "..\..\FMWIUnpack" decyphered.dat
    echo  ====== %%~nF END ======
    echo .
    popd
)

echo �۾� �Ϸ�!
