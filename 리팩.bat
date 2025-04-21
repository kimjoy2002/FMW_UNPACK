@echo off
setlocal enabledelayedexpansion

rem 원본 번역 폴더
set "SOURCE_DIR=.\컴플리트번역"
rem 번역 후 데이터 저장 폴더
set "TARGET_DIR=.\컴플리트번역후데이터"

if not exist "%SOURCE_DIR%" (
    echo 오류: 원본 번역 폴더가 존재하지 않습니다.
    exit /b 1
)

if not exist "%TARGET_DIR%" (
    mkdir "%TARGET_DIR%"
)

rem 번역 폴더 내 각 data 폴더 탐색
for /d %%D in ("%SOURCE_DIR%\*") do (
    set "FOLDERNAME=%%~nxD"
    echo  ====== %%~nxD ======
    pushd "%%D"
	del repacked.dat
	del cyphered.dat
    
	echo  "..\..\FMWIRepacking" decyphered.dat.gsty
    rem FMWIRepack 및 FMWICypher 실행
    "..\..\FMWIRepacking" decyphered.dat.gsty
	echo  "..\..\FMWICypher" repacked.dat c
    "..\..\FMWICypher" repacked.dat c
    
    rem 생성된 encyphered.dat를 새로운 위치로 이동 및 이름 변경
    if exist "cyphered.dat" (
	    echo move "cyphered.dat" "..\..\%TARGET_DIR%\!FOLDERNAME!.dat"
        move "cyphered.dat" "..\..\%TARGET_DIR%\!FOLDERNAME!.dat"
    )
    echo  ====== %%~nxD END ======
    echo .
    
    popd
)

echo 작업 완료!
