// src\test.cpp


#include <Windows.h>         // 반드시 ViGEm/Client.h 이전에 포함
#include <iostream>
#include <thread>
#include <chrono>
#include <ViGEm/ViGEmClient.h>    // ViGEmClient 헤더

int main()
{
    // 1) ViGEmClient 할당 및 연결
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

    // 2) 가상 Xbox 360 컨트롤러 할당 및 등록
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

    // 3) 3초간 대기
    std::cout << "[Info] Waiting 20 seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(20));

    // 4) 컨트롤러 제거 및 정리
    vigem_target_remove(client, target);
    vigem_target_free(target);
    vigem_disconnect(client);
    vigem_free(client);
    std::cout << "[Info] Virtual controller removed and client freed\n";

    return 0;
}
