# FMW_UNPACK
환소대 컴플리트 언팩 합본 및 스위치 기계 번역 데이터

환소대 컴플리트를 언팩할 수 있는 툴을 조금 수정해서 간편하게 취합해두었습니다.

그냥 예전에 시간이 남아서 심심할때 건드렸는데 번역을 주도할 성격은 안되고 방법을 썩히는것도 아까워서 공유하려고 올립니다.


![image](https://github.com/user-attachments/assets/5ca18b50-3786-48c2-bfee-2b963c0f186b)



# 파일 목록

* font폴더 : 컴플리트에서 쓰는 폰트가 한글을 지원하지않기때문에 생성한 폰트입니다. 일본어도 대응되도록 일본어+한글로 만들어두긴했는데 일부 빠진 일본어가 있는등 임시 폰트에 가깝습니다. 
  * https://www.angelcode.com/products/bmfont/ : Bitmap Font Generator을 이용하면 원하는 폰트를 만들 수 있습니다.
* FMWICypher.cpp : 두 번째 인수가 d이면 FMWI에서 가져온 파일을 해독합니다. 두 번째 인수가 c이면 리패키징 프로그램에서 만든 파일을 다시 암호화합니다.
* FMWIUnpack.cpp : 해독된 데이터 파일에서 모든 파일을 추출합니다. 
* FMWIRepacking.cpp : 압축 해제 프로그램이 생성한 gsty 파일을 읽고, 그 정보를 사용하여 압축 해제된 파일을 찾아 다시 합칩니다.
* 스위치기계번역: "무설탕"님의 스위치 손번역에 남은 부분을 기계 번역으로 돌린 번역 파일입니다. 


# 컴플리트 언팩 사용법


1. cpp파일을 빌드합니다.

    * g++ FMWIUnpack.cpp -o FMWIUnpack -static
    * g++ FMWICypher.cpp -o FMWICypher -static
    * g++ FMWIRepacking.cpp -o FMWIRepacking -static

2. "컴플리트데이터" 폴더를 만들어 컴플리트에서 추출하고싶은 dat를 그 안에 둡니다.
(일부 언팩이 안되는 dat도 있기때문에 번역을 위해서는 data1.dat, data4.dat, data5.dat, data9.dat정도만 가져오면 됩니다.)


3. 언팩.bat를 실행. 이걸로 "컴플리트원본"폴더가 생성됩니다.

```
C:\Users\admin\Documents\git\FMW_UNPACK>언팩.bat
 ====== data1 ======
"..\..\FMWICypher" "..\..\컴플리트데이터\data1.dat" d
..\..\FMWIUnpack" decyphered.dat
Successfully extracted 22 files!
 ====== data1 END ======
.
 ====== data4 ======
"..\..\FMWICypher" "..\..\컴플리트데이터\data4.dat" d
..\..\FMWIUnpack" decyphered.dat
Successfully extracted 325 files!
 ====== data4 END ======
.
 ====== data5 ======
"..\..\FMWICypher" "..\..\컴플리트데이터\data5.dat" d
..\..\FMWIUnpack" decyphered.dat
Successfully extracted 271 files!
 ====== data5 END ======
.
 ====== data9 ======
"..\..\FMWICypher" "..\..\컴플리트데이터\data9.dat" d
..\..\FMWIUnpack" decyphered.dat
Successfully extracted 183 files!
 ====== data9 END ======
.
작업 완료
```


4. 생성된 "컴플리트원본" 폴더의 이름을 "컴플리트번역"으로 변경. font폴더에 있는 폰트를 data1폴더에 덮어씌워서 한글을 지원할 수 있게 하고 번역을 진행할수있으면 진행


5. 리팩.bat를 실행. 이걸로 "컴플리트번역후데이터"폴더가 생성됨. 여기에 있는 data파일들을 환소대 data로 교체하면 패치 완료
(혹시모를 문제를 위해 원본data를 백업합시다.)





# 스위치 번역 사용법

스위치 손번역을 선배포 해주신 분의 자료에서 번역 안된 부분만 기계 번역을 돌려만 두었습니다.

스위치 에뮬을 사용해야한다고하며 실행시 data 덮어 씌우면 됩니다.

캐릭터 이름 정도만 수정해둔 상태나 기계 번역이라 번역의 질이 나쁨에 유의 (하늘이 모두 우츠호로 변역된다던지 하는 찐빠 존재)

모든 스토리 검수가 끝난게 아니라 일부 부분에서 크래시가 날수도있음. 고칠수있다면 에러난 해당 txt에서 수정이 난 부분을 수정후 다시 덮어 씌우는 식으로 해결 가능

다만 스위치와 컴플리트판간의 파일이 동일하지않아서, 컴플리트판에 스위치data를 그대로 쓸순 없음. (내부 구분자 형식이라던가, 스크립트용 함수가 확연히 다름)



# 참고 링크

* 가져온 언팩툴
  * https://pastebin.com/dYK9w7w9
  * 해외분이 예전에 만들어뒀던 언팩툴입니다. 언팩/리팩하는데 cpp파일은 이 파일을 사용하였습니다.

* 국내 스위치판 손번역
  * https://gall.dcinside.com/mgallery/board/view/?id=genshou&no=85&page=1
  * 스위치 손번역의 대부분은 이 분이 작업하셨고 전 남아있는 부분만 기계번역을 돌렸습니다.
  * 아직 작업하고 계신걸로 알고있어서 혹시나 문제되면 내리겠습니다.


