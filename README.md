# DirectX11_Project

youtube 영상: https://www.youtube.com/watch?v=tILvwj5kjTI 
트렐로: https://trello.com/b/vvWjFuK5/2016-cpp-adv-stack

##코딩 규칙
* 절대 마스터에 푸시하지 않기
* 각자 작업 브랜치 생성 후 풀 리퀘스트 날리기
* 파트너가 확인 후 커밋 & 머지
* 함수명은 무조건 대문자로 시작
* 멤버변수는 m_으로 시작
* 전역 변수는 g_로 시작
* 중괄호는 밑줄에서 시작
* 한줄짜리 조건문 또는 반복문은 중괄호 없어도 됨
* 상수는 대문자 및 _ 만 사용
* 멤버변수 줄 맞추기
* 포인터의 *는 클래스명에 붙이기.
* 그 외에는 구글 코딩 가이드를 따름. 
* 추가 바람.


##엔진 사용법  
  
###Scene만들기
* Scene클래스를 상속받는 객체를 만든다.
* Update와, Start함수를 구현한다. (완전 가상함수이므로 반드시 구현해야함.)
* Update함수는 매 프레임 실행 되며, Start함수는 클래스 생성시에만 실행 된다.
* Update함수는 카메라와 입력을 다룰 수 있다.

###모델 생성
* ModelClass 객체를 만들고 Scene에서 AddModel함수에 객체를 전달한다.
* 현재 fadeout 기능은 NodelClass에 있지 않고 사라지는 VanishingBlock객체가 따로 있다.
