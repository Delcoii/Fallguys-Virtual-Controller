#pragma once
#ifndef __MENU_BUTTON_CONTROL_HPP__
#define __MENU_BUTTON_CONTROL_HPP__

// os
#include <Windows.h>
#include <iostream>
#include <chrono>

// xbox controller emulation
#include <ViGEm/ViGEmClient.h>
#include "VigemController.hpp"

// utils
#include "KeyTable.hpp"        // Key mapping table
#include "ini_parser.h"


class MenuButtonControl {
public:
    MenuButtonControl();
    ~MenuButtonControl();
    XUSB_REPORT SetCheckPointResetReport(bool& is_in_checkpoint_reset);


private:
    
    
    
    // XUSB_REPORT SetImmediateExitReport(bool& is_op_end);
    void ProcessINI();


    std::chrono::steady_clock::time_point checkpoint_reset_cmd_start_time_
        = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point immediate_exit_cmd_start_time_
        = std::chrono::steady_clock::now();



    bool is_checkpoint_started_ = false;
    bool is_immediate_started_ = false;
    
    // TODO : default should be KNOCKOUT
    // unsigned char mode_ = CREATIVE;
    

    // config parameter
    double cmd_input_interval_s_ = 0.1;
    double last_keydown_time_s_ = 1.0;
    
    IniParser ini_parser_;

};
#endif // __MENU_BUTTON_CONTROL_HPP__