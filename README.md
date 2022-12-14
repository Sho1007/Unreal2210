# RealHorror Game

## 프로젝트 일정 관리
https://trello.com/w/unrealportfoilorealhorror/home

## 프로젝트 문서
* [포트폴리오 계획서](https://github.com/Sho1007/Unreal2210/blob/main/Real_Horror_%ED%8F%AC%ED%8A%B8%ED%8F%B4%EB%A6%AC%EC%98%A4_%EA%B3%84%ED%9A%8D%EC%84%9C.pdf)

## 개발 진행 상황
* Collectable Object Inspect 기능 구현 중
    * Rotate / Zoom 기능 구현 완료

## 깨달은 점
1. Custom Depth
    1. Custom Depth Stencil 을 활용하고 싶다면
        1. 프로젝트 세팅에서
            1. 커스텀 뎁스-스텐실 패스 (Custom Depth-Stencil) 을 Enabled with Stencil 로 변경
            2. 커스텀 깊이 TTA 지터 (Custom Depth TTA Jitter)는 체크 해제
        2. 포스트 프로세스 볼륨에서 
            1. 포스트 프로세스 머터리얼 (Weighted Blendables)에 배열 엘리먼트 추가 -> 에셋 레퍼런스 -> 적용할 머테리얼 선택
            2. 무한 규모 (unbound) 체크 (해당 바운드에만 적용하고 싶다면 체크해제)
        3. 적용하고하 하는 스태틱 메시에 custom depth 검색
            1. 커스텀 뎁스 패스 렌더 (Render Custom Depth) 체크
            2. 커스텀 깊이 스텐실 값 (Custom Depth Stencil Value) 를 적절하게 조절
    1. c++ 에서 CustomDepth 설정을 건들고 싶다면 Set Function을 활용하자
    (직접 변수 자체를 바꾸는건 작동 안함)
    ```c++
    StaticMeshComponent->bRenderCustomDepth = true;                 // 작동 X
    StaticMeshComponent->SetRenderCustomDepth(true);                // 작동 O
    StaticMeshComponent->CustomDepthStencilValue = 1;               // 작동 X
    StaticMeshComponent->SetCustomDepthStencilValue(1);             // 작동 O
    ```
2. Rotater + Rotator 
    ```c++
        // 단순 A + B 로는 원하는 수치만 더할 뿐 원하는 결과가 나오지 않음
        UKismetMathLibrary::ComposeRotators(A, B);
    ```