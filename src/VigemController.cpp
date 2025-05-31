#include "VigemController.hpp"

// 생성자: member 포인터를 nullptr 로 초기화
VigemController::VigemController()
    : client_(nullptr), target_(nullptr)
{
}

// 소멸자: 아직 남아 있는 controller나 client를 제거
VigemController::~VigemController()
{
    // 컨트롤러가 남아 있으면 먼저 제거
    if (target_ != nullptr && client_ != nullptr) {
        vigem_target_remove(client_, target_);
        vigem_target_free(target_);
        target_ = nullptr;
        std::cout << "[Info] Virtual controller removed (from destructor)\n";
    }
    // client가 남아 있으면 연결 해제
    if (client_ != nullptr) {
        vigem_disconnect(client_);
        vigem_free(client_);
        client_ = nullptr;
        std::cout << "[Info] ViGEmClient disconnected and freed (from destructor)\n";
    }
}

// ViGEm client 초기화
bool VigemController::Initialize()
{
    client_ = vigem_alloc();
    if (client_ == nullptr) {
        std::cerr << "[Error] vigem_alloc() failed\n";
        return false;
    }
    if (!VIGEM_SUCCESS(vigem_connect(client_))) {
        std::cerr << "[Error] vigem_connect() failed\n";
        vigem_free(client_);
        client_ = nullptr;
        return false;
    }
    std::cout << "[Info] ViGEmBus connected\n";
    return true;
}

// 가상 Xbox 360 컨트롤러 등록
bool VigemController::RegisterX360()
{
    if (client_ == nullptr) {
        std::cerr << "[Error] Cannot register controller: client is not initialized\n";
        return false;
    }
    target_ = vigem_target_x360_alloc();
    if (target_ == nullptr) {
        std::cerr << "[Error] vigem_target_x360_alloc() failed\n";
        return false;
    }
    if (!VIGEM_SUCCESS(vigem_target_add(client_, target_))) {
        std::cerr << "[Error] vigem_target_add() failed\n";
        vigem_target_free(target_);
        target_ = nullptr;
        return false;
    }
    std::cout << "[Info] Virtual Xbox 360 controller registered\n";
    return true;
}

// 가상 컨트롤러 제거
void VigemController::Unregister()
{
    if (target_ != nullptr && client_ != nullptr) {
        vigem_target_remove(client_, target_);
        vigem_target_free(target_);
        target_ = nullptr;
        std::cout << "[Info] Virtual controller removed (from Unregister)\n";
    }
}

// client 연결 해제 및 메모리 해제
void VigemController::Cleanup()
{
    // 먼저 남아 있는 컨트롤러 해제
    if (target_ != nullptr && client_ != nullptr) {
        vigem_target_remove(client_, target_);
        vigem_target_free(target_);
        target_ = nullptr;
        std::cout << "[Info] Virtual controller removed (from Cleanup)\n";
    }
    // client 연결 해제
    if (client_ != nullptr) {
        vigem_disconnect(client_);
        vigem_free(client_);
        client_ = nullptr;
        std::cout << "[Info] ViGEmClient disconnected and freed (from Cleanup)\n";
    }
}

// XUSB_REPORT를 가상 컨트롤러에 전송
bool VigemController::SendX360Report(const XUSB_REPORT& report)
{
    if (client_ == nullptr || target_ == nullptr) {
        std::cerr << "[Error] Cannot send report: client or target is not valid\n";
        return false;
    }
    VIGEM_ERROR status = vigem_target_x360_update(client_, target_, report);
    if (!VIGEM_SUCCESS(status)) {
        std::cerr << "[Error] vigem_target_x360_update() failed with code " << status << "\n";
        return false;
    }
    return true;
}
