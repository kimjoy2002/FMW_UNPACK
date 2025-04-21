@echo off
setlocal enabledelayedexpansion

rem 첫 번째 인자: 원본 폴더
set "SOURCE_DIR=컴플리트데이터"
rem 두 번째 인자: 대상 폴더
set "TARGET_DIR=컴플리트원본"

if not exist "%SOURCE_DIR%" (
    echo 오류: 원본 폴더가 존재하지 않습니다.
    exit /b 1
)

if not exist "%TARGET_DIR%" (
    mkdir "%TARGET_DIR%"
)

rem 원본 폴더에서 .dat 파일 목록 가져오기
for %%F in ("%SOURCE_DIR%\*.dat") do (
    set "FILENAME=%%~nF"
    set "FILEEXT=%%~xF"
    
    rem 폴더 만들기
    set "TARGET_PATH=%TARGET_DIR%\!FILENAME!"
    if not exist "!TARGET_PATH!" (
        mkdir "!TARGET_PATH!"
    )
    
    rem 명령어 실행
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

echo 작업 완료!
