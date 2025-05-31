#include "VigemController.hpp"
#include <thread>
#include <chrono>

// Constructor: set pointers to nullptr
VigemController::VigemController()
    : client_(nullptr), target_(nullptr)
{
    // nothing else here
}

// Destructor: if a controller is still registered or client still connected,
// make sure to unregister and cleanup.
VigemController::~VigemController() {
    // If a controller is registered, remove it
    if (target_ != nullptr) {
        vigem_target_remove(client_, target_);
        vigem_target_free(target_);
        target_ = nullptr;
        std::cout << "[Info] Virtual controller removed (from destructor)\n";
    }

    // If the client is still connected, disconnect and free it
    if (client_ != nullptr) {
        vigem_disconnect(client_);
        vigem_free(client_);
        client_ = nullptr;
        std::cout << "[Info] ViGEmClient disconnected and freed (from destructor)\n";
    }
}

// Initialize the ViGEm client.
// Returns true if successful, false otherwise.
bool VigemController::Initialize() {
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

// Allocate and register a virtual Xbox 360 controller.
// Returns true if successful, false otherwise.
bool VigemController::RegisterX360() {
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


// Unregister the virtual controller and free associated resources.
void VigemController::Unregister() {
    if (target_ != nullptr && client_ != nullptr) {
        vigem_target_remove(client_, target_);
        vigem_target_free(target_);
        target_ = nullptr;
        std::cout << "[Info] Virtual controller removed (from Unregister)\n";
    }
}


// Disconnect and free the ViGEm client.
void VigemController::Cleanup() {
    if (client_ != nullptr) {
        // If a controller has not yet been unregistered, free it now
        if (target_ != nullptr) {
            vigem_target_remove(client_, target_);
            vigem_target_free(target_);
            target_ = nullptr;
            std::cout << "[Info] Virtual controller removed (from Cleanup)\n";
        }

        vigem_disconnect(client_);
        vigem_free(client_);
        client_ = nullptr;
        std::cout << "[Info] ViGEmClient disconnected and freed (from Cleanup)\n";
    }
}
