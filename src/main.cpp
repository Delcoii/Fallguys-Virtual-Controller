#include "VigemController.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    VigemController controller;

    // 1) Initialize the ViGEm client
    if (!controller.Initialize()) {
        return -1;
    }

    // 2) Register the virtual Xbox 360 controller
    if (!controller.RegisterX360()) {
        // If registration fails, make sure to clean up before exiting
        controller.Cleanup();
        return -1;
    }

    // 3) Wait for the specified duration
    std::cout << "[Info] Waiting " << VigemController::WAIT_SECONDS << " seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(VigemController::WAIT_SECONDS));

    // 4) Unregister the controller and clean up the client
    controller.Unregister();
    controller.Cleanup();

    return 0;
}
