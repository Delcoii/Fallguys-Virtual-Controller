#pragma once

#include <Windows.h>    // Required for Windows API functions
#include <ViGEm/ViGEmClient.h>
#include <iostream>

// VigemController
// Wraps ViGEmClient initialization, virtual controller registration,
// and cleanup into a single C++ class.
class VigemController {
public:
    // Default constructor: initializes internal handles to nullptr.
    VigemController();

    // Destructor: ensures any registered controller is removed and client is freed.
    ~VigemController();

    // Initialize the ViGEm client.
    // Returns true on success, false on failure.
    bool Initialize();

    // Allocate and register a virtual Xbox 360 controller.
    // Returns true on success, false on failure.
    bool RegisterX360();

    // Unregister the virtual controller (if any) and free associated resources.
    void Unregister();

    // Disconnect and free the ViGEm client (if initialized).
    void Cleanup();

    // Static constant: how many seconds to wait before cleanup in main().
    static const int WAIT_SECONDS = 20;

    // Check if client is currently connected.
    bool IsClientValid() const { return client_ != nullptr; }

    // Check if a virtual controller is currently registered.
    bool IsControllerValid() const { return target_ != nullptr; }

private:
    // Pointer to the ViGEm client instance
    PVIGEM_CLIENT client_;

    // Pointer to the virtual Xbox 360 controller instance
    PVIGEM_TARGET target_;

    // Disable copy-construction and assignment
    VigemController(const VigemController&) = delete;
    VigemController& operator=(const VigemController&) = delete;
};
