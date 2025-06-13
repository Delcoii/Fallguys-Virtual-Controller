#pragma once

#include <Windows.h>            // Windows type definitions (VOID, USHORT, etc.)
#include <ViGEm/ViGEmClient.h>
#include <iostream>

// VigemController
// - Contains functions for initialization (Initialize), virtual X360 controller registration (RegisterX360),
//   report sending (SendX360Report), and resource cleanup (Cleanup)
class VigemController {
public:
    VigemController();              // Constructor: Initializes client_ and target_ to nullptr
    ~VigemController();             // Destructor: Automatically frees controller or client if they remain

    bool Initialize();              // Connects to the ViGEm client
    bool RegisterX360();            // Registers a virtual Xbox 360 controller
    void Unregister();              // Removes the controller
    void Cleanup();                 // Disconnects the client and frees memory

    // Sends the XUSB_REPORT to the virtual controller
    // Returns false if client_ or target_ is nullptr or if the update fails
    bool SendX360Report(const XUSB_REPORT& report);

    bool IsClientValid() const    { return client_ != nullptr; }
    bool IsControllerValid() const{ return target_ != nullptr; }

    bool IsRegistered() const {
        return (client_ != nullptr) && (target_ != nullptr);
    }

    static const int WAIT_SECONDS = 20;  // not used now

    // Disables copy construction and copy assignment
    VigemController(const VigemController&) = delete;
    VigemController& operator=(const VigemController&) = delete;

private:
    PVIGEM_CLIENT client_;   // Handle for the connection to the ViGEm bus
    PVIGEM_TARGET  target_;  // Handle for the registered virtual X360 controller
};
