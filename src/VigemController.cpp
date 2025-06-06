#include "VigemController.hpp"


VigemController::VigemController()
    : client_(nullptr), target_(nullptr) {}

// Destructor: Remove any remaining controller or client
VigemController::~VigemController() {
    // If the controller is still registered, remove it first
    if (target_ != nullptr && client_ != nullptr) {
        vigem_target_remove(client_, target_);
        vigem_target_free(target_);
        target_ = nullptr;
        std::cout << "[Info] Virtual controller removed (from destructor)\n";
    }

    // if the client is still connected, disconnect it
    if (client_ != nullptr) {
        vigem_disconnect(client_);
        vigem_free(client_);
        client_ = nullptr;
        std::cout << "[Info] ViGEmClient disconnected and freed (from destructor)\n";
    }
}

// Initialize the ViGEm client
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


// Register a virtual Xbox 360 controller
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

// Unregister the virtual controller
void VigemController::Unregister()
{
    if (target_ != nullptr && client_ != nullptr) {
        vigem_target_remove(client_, target_);
        vigem_target_free(target_);
        target_ = nullptr;
        std::cout << "[Info] Virtual controller removed (from Unregister)\n";
    }
}


// Disconnect the client and free memory
void VigemController::Cleanup()
{
    // If the controller is still registered, remove it first
    if (target_ != nullptr && client_ != nullptr) {
        vigem_target_remove(client_, target_);
        vigem_target_free(target_);
        target_ = nullptr;
        std::cout << "[Info] Virtual controller removed (from Cleanup)\n";
    }
    // if the client is still connected, disconnect it
    if (client_ != nullptr) {
        vigem_disconnect(client_);
        vigem_free(client_);
        client_ = nullptr;
        std::cout << "[Info] ViGEmClient disconnected and freed (from Cleanup)\n";
    }
}

// Send an XUSB_REPORT to the virtual controller
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
