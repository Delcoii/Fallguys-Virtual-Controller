#include "VigemController.hpp"
#include "LeftStickControl.hpp"
#include <iostream>
#include <thread>


int main() {
    std::string mainpage0 = " _______   _______  __        ______   ______    __   __  \n";
    std::string mainpage1 = "|       \\ |   ____||  |      /      | /  __  \\  |  | |  | \n";
    std::string mainpage2 = "|  .--.  ||  |__   |  |     |  ,----'|  |  |  | |  | |  | \n";
    std::string mainpage3 = "|  |  |  ||   __|  |  |     |  |     |  |  |  | |  | |  | \n";
    std::string mainpage4 = "|  '--'  ||  |____ |  `----.|  `----.|  `--'  | |  | |  | \n";
    std::string mainpage5 = "|_______/ |_______||_______| \\______| \\______/  |__| |__|\n\n\n";

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Show the main page
    std::cout << mainpage0 << mainpage1 << mainpage2 << mainpage3 << mainpage4 << mainpage5;

    VigemController controller;

    // Initialize ViGEm client
    if (!controller.Initialize()) {
        return -1;
    }

    // Register virtual Xbox360 controller
    if (!controller.RegisterX360()) {
        // If registration fails, immediately cleanup and exit
        controller.Cleanup();
        return -1;
    }

    // Wait for a moment until the virtual controller is fully created
    std::cout << "[Info] Waiting 1 second before starting LeftStickControl...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Execute LeftStickControl
    LeftStickControl leftStick(controller);
    leftStick.Run();

    // After the loop ends, unregister controller and release client
    controller.Cleanup();
    return 0;
}
