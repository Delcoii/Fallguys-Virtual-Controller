#pragma once

#include <Windows.h>            // 윈도우용 타입 정의 (VOID, USHORT 등)
#include <ViGEm/ViGEmClient.h>
#include <iostream>

// VigemController
// - 초기화(Initialize), 가상 X360 컨트롤러 등록(RegisterX360), 
//   보고서 전송(SendX360Report), 리소스 해제(Cleanup) 등을 한 클래스에 묶었습니다.
class VigemController
{
public:
    VigemController();              // 생성자: client_, target_ 를 nullptr 로 초기화
    ~VigemController();             // 소멸자: controller나 client가 남아 있으면 자동 해제

    bool Initialize();              // ViGEm client 연결
    bool RegisterX360();            // 가상 Xbox 360 controller 등록
    void Unregister();              // controller 제거
    void Cleanup();                 // client 연결 해제 및 메모리 해제

    // XUSB_REPORT를 가상 컨트롤러로 보내는 메서드
    // client_나 target_이 nullptr이거나 업데이트 실패 시 false 반환
    bool SendX360Report(const XUSB_REPORT& report);

    bool IsClientValid() const    { return client_ != nullptr; }
    bool IsControllerValid() const{ return target_ != nullptr; }

    static const int WAIT_SECONDS = 20;  // 예시: main()에서 잠시 대기할 때 쓰이는 상수

    // 복사 생성자 및 복사 대입 금지
    VigemController(const VigemController&) = delete;
    VigemController& operator=(const VigemController&) = delete;

private:
    PVIGEM_CLIENT client_;   // ViGEm bus와의 연결 핸들
    PVIGEM_TARGET  target_;   // 등록된 가상 X360 controller 핸들
};
