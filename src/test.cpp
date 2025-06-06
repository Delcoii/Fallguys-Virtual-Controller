// src\test.cpp

#include <Windows.h>         // Must be included before ViGEm/Client.h
#include <iostream>
#include <thread>
#include <chrono>
#include <ViGEm/ViGEmClient.h>    // ViGEmClient header

int main() {
    // 1) Allocate and connect ViGEmClient
    PVIGEM_CLIENT client = vigem_alloc();
    if (client == nullptr)
    {
        std::cerr << "vigem_alloc() failed\n";
        return -1;
    }
    if (!VIGEM_SUCCESS(vigem_connect(client)))
    {
        std::cerr << "vigem_connect() failed\n";
        vigem_free(client);
        return -1;
    }
    std::cout << "[Info] ViGEmBus connected\n";

    // 2) Allocate and register virtual Xbox 360 controller
    PVIGEM_TARGET target = vigem_target_x360_alloc();
    if (target == nullptr)
    {
        std::cerr << "vigem_target_x360_alloc() failed\n";
        vigem_disconnect(client);
        vigem_free(client);
        return -1;
    }
    if (!VIGEM_SUCCESS(vigem_target_add(client, target)))
    {
        std::cerr << "vigem_target_add() failed\n";
        vigem_target_free(target);
        vigem_disconnect(client);
        vigem_free(client);
        return -1;
    }
    std::cout << "[Info] Virtual Xbox 360 controller registered\n";

    // 3) Wait for 20 seconds
    std::cout << "[Info] Waiting 20 seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(20));

    // 4) Remove controller and clean up
    vigem_target_remove(client, target);
    vigem_target_free(target);
    vigem_disconnect(client);
    vigem_free(client);
    std::cout << "[Info] Virtual controller removed and client freed\n";

    return 0;
}
