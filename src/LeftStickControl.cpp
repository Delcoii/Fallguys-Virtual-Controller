#include "LeftStickControl.hpp"


// Constructor: store the reference and initialize should_exit_ to false.
LeftStickControl::LeftStickControl(VigemController& controller)
    : controller_(controller)
    , should_exit_(false),
    log_thread_(&LeftStickControl::LogWorker, this) {

    // Ini init: get exe folder path.
    char exePath[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    std::string path(exePath);
    // Extract the directory from the full exe path.
    size_t pos = path.find_last_of("\\/");
    if (pos != std::string::npos) {
        path = path.substr(0, pos);
    }

    // Build ini file path relative to exe directory.
    std::string iniFile = path + "\\key_settings.ini";
    ini_parser_.Init(iniFile.c_str());

    ProcessINI();
}

LeftStickControl::~LeftStickControl() {
    log_thread_running_ = false;
    log_cv_.notify_one();
    if (log_thread_.joinable()) {
        log_thread_.join();
    }
}


void LeftStickControl::Run() {
    // Verify that the VigemController is initialized and a controller is registered.
    if (!controller_.IsClientValid() || !controller_.IsControllerValid()) {
        std::cerr << "[Error] VigemController is not initialized or controller not registered.\n";
        return;
    }

    AddLog("[Info] Fallguys Jelly Control started. Press F12 to exit.");
    AddLog("[Info] Current Default Mode : FAST, Press toggle key to change mode!!");

    // desired period per loop.
    auto desired_period = std::chrono::microseconds(10);
    auto next_frame_time = std::chrono::high_resolution_clock::now();

    while (true) {
        total_loop_time_ = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - loop_start_time_); // calculate total loop time
        loop_start_time_ = std::chrono::high_resolution_clock::now(); // reset loop start time
        next_frame_time += desired_period; // schedule next loop time

        // If F12 is pressed, break out.
        if ((GetAsyncKeyState(VK_F12) & 0x8000) != 0) {
            AddLog("[Info] F12 detected. Exiting Program.");
            break;
        }
        if (should_exit_) {
            AddLog("[Info] LoopExit() called. Exiting Program.");
            break;
        }

        // Toggle mode with debounce handling.
        static bool toggle_mode_debounce = false;
        if ((GetAsyncKeyState(toggle_mode_key_) & 0x8000) != 0) {
            if (!toggle_mode_debounce) {
                if (moving_mode_ == MOVING_TYPE::FAST) {
                    moving_mode_ = MOVING_TYPE::NORMAL;
                    AddLog("[Info] Switched to NORMAL mode.");
                } else {
                    moving_mode_ = MOVING_TYPE::FAST;
                    AddLog("[Info] Switched to FAST mode.");
                }
                toggle_mode_debounce = true;
            }
        } else {
            toggle_mode_debounce = false;
        }

        // Toggle signal onoff 
        // Toggle signal onoff with debounce handling.
        static bool signal_onoff_debounce = false;
        if ((GetAsyncKeyState(controller_onoff_key_) & 0x8000) != 0) {
            if (!signal_onoff_debounce) {
                // Toggle the on/off signal.
                
                if (signal_on_ == true) {
                    // send neutral report and unregister the controller
                    XUSB_REPORT neutral = {};
                    if (!controller_.SendX360Report(neutral)) {
                        std::cerr << "[Error] Failed to send neutral X360 report.\n";
                        break;
                    }
                    // std::this_thread::sleep_for(std::chrono::milliseconds(100)); // wait for a moment
                    controller_.Unregister();
                }
                else if (signal_on_ == false) {
                    if (!controller_.RegisterX360()) {
                        std::cerr << "[Error] Failed to register controller.\n";
                        break;
                    }
                }

                signal_onoff_debounce = true;
                signal_on_ = !signal_on_;
            }
        } else {
            signal_onoff_debounce = false;
        }


        // Build and send the current Left Stick report.
        if (signal_on_ == true) {
            XUSB_REPORT report = BuildReportFromKeys(moving_mode_);
            std::chrono::steady_clock::time_point before_signal_send = std::chrono::high_resolution_clock::now();
            if (!controller_.SendX360Report(report)) {
                std::cerr << "[Error] Failed to send X360 report.\n";
                break;
            }
            signal_send_time_ = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - before_signal_send);    
        }
        
        // Sleep until next frame time.
        // std::this_thread::sleep_until(next_frame_time);
        // std::this_thread::sleep_for(desired_period);
        last_period_us_ = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - loop_start_time_);
    }

    // On exit, reset Left Stick to (0,0) to center it.
    XUSB_REPORT neutral = {};
    controller_.SendX360Report(neutral);
    AddLog("[Info] LeftStickControl loop exited.\n");
}

// Build an XUSB_REPORT from current key states.
XUSB_REPORT LeftStickControl::BuildReportFromKeys(const int moving_mode) {
    XUSB_REPORT report = {}; // All fields start at 0.

    // Xbox Left Stick range: -32768 .. +32767
    constexpr SHORT FULL     = 32767;
    constexpr SHORT NEG_FULL = -32768;

    SHORT x = before_stick_pos_x_;    // Left Stick X axis value
    SHORT y = before_stick_pos_y_;    // Left Stick Y axis value

    bool press_up = (GetAsyncKeyState(up_key_) & 0x8000) != 0;
    bool press_down = (GetAsyncKeyState(down_key_) & 0x8000) != 0;
    bool press_left = (GetAsyncKeyState(left_key_) & 0x8000) != 0;
    bool press_right = (GetAsyncKeyState(right_key_) & 0x8000) != 0;
    bool press_jump = (GetAsyncKeyState(jump_key_) & 0x8000) != 0;
    bool press_dive = (GetAsyncKeyState(dive_key_) & 0x8000) != 0;

    static int report_count = 0;

    // check initial press time
    if (press_up == true && before_press_state_up_ == false)  up_count_ = report_count;
    if (press_down == true && before_press_state_down_ == false)  down_count_ = report_count;
    if (press_left == true && before_press_state_left_ == false)  left_count_ = report_count;
    if (press_right == true && before_press_state_right_ == false)  right_count_ = report_count;

    // make setting as same as possible to origin virtual controller settings
    if (moving_mode == MOVING_TYPE::NORMAL) {
        // joystick X axis
        
        // left is pressed
        if (before_press_state_left_ == false && press_left == true) {
            if (x == FULL)      x -= FULL;      // make it 0
            else if (x == 0)    x += NEG_FULL;  // make it -32768
        }

        // right is pressed
        if (before_press_state_right_ == false && press_right == true) {
            if (x == NEG_FULL)  x += FULL+1;    // make it 0
            else if (x == 0)    x += FULL;      // make it 32767
        }

        // down is pressed
        if (before_press_state_down_ == false && press_down == true) {
            if (y == FULL)      y -= FULL;      // make it 0
            else if (y == 0)    y += NEG_FULL;  // make it -32768
        }

        // up is pressed
        if (before_press_state_up_ == false && press_up == true) {
            if (y == NEG_FULL)  y += FULL+1;    // make it 0
            else if (y == 0)    y += FULL;      // make it 32767
        }
        
        // left is released
        if (before_press_state_left_ == true && press_left == false) {
            if (x == NEG_FULL)  x += FULL+1;    // make it 0
            else if (x == 0)    x += FULL;      // make it 32767
        }

        // right is released
        if (before_press_state_right_ == true && press_right == false) {
            if (x == FULL)      x -= FULL;      // make it 0
            else if (x == 0)    x += NEG_FULL;  // make it -32768
        }

        // down is released
        if (before_press_state_down_ == true && press_down == false) {
            if (y == NEG_FULL)  y += FULL+1;    // make it 0
            else if (y == 0)    y += FULL;      // make it 32767
        }

        // up is released
        if (before_press_state_up_ == true && press_up == false) {
            if (y == FULL)      y -= FULL;      // make it 0
            else if (y == 0)    y += NEG_FULL;  // make it -32768
        }
    }

    else if (moving_mode == MOVING_TYPE::FAST) {
        x = 0;
        y = 0;
        
        // joystick X axis
        if (press_left == true && press_right == true) {        // pressed both left & right
            x = (left_count_ < right_count_) ? FULL : NEG_FULL; // input is last pressed key
        }
        else if (press_left == true && press_right == false) { // pressed left only
            x = NEG_FULL; // Left
        }
        else if (press_left == false && press_right == true) { // pressed right only
            x = FULL; // Right
        }

        // joystick Y axis
        if (press_up == true && press_down == true) {          // pressed both up & down
            y = (up_count_ < down_count_) ? NEG_FULL : FULL;     // input is last pressed key
        }
        else if (press_up == true && press_down == false) {     // pressed up only
            y = FULL; // Up
        }
        else if (press_up == false && press_down == true) {     // pressed down only
            y = NEG_FULL; // Down
        }    
    }
    


    // Assign the stick values.
    report.sThumbLX = x;
    report.sThumbLY = y;
    if (press_jump == true) {
        report.wButtons |= XUSB_GAMEPAD_A; 
    }
    if (press_dive == true) {
        report.wButtons |= XUSB_GAMEPAD_X; 
    }

    std::string output_x = (x == NEG_FULL) ? "<-" : (x == FULL) ? "->" : "o";
    std::string output_y = (y == NEG_FULL) ? "v" : (y == FULL) ? "^" : "o";
    std::string jump_state = (press_jump) ? "o" : "x";
    std::string dive_state = (press_dive) ? "o" : "x";
    
    
    // Keep triggers and right stick at zero (unused).
    report.bLeftTrigger  = 0;
    report.bRightTrigger = 0;
    report.sThumbRX      = 0;
    report.sThumbRY      = 0;


    before_press_state_down_ = press_down;
    before_press_state_up_ = press_up;
    before_press_state_left_ = press_left;
    before_press_state_right_ = press_right;

    before_stick_pos_x_ = x;
    before_stick_pos_y_ = y;

    report_count++;


    if (report_count % 15000 == 0) {         
        if ((press_up == true || press_down == true || press_left == true || press_right == true
            || press_jump == true || press_dive == true)   
            && signal_on_ == true) {
            // std::cout << "[Info] move   : " << x << "\t" << y << "\t" << jump_state <<
            //             "\tDuration : " << last_period_us_.count() << "us\t" <<
            //             "\tSignal Send Time: " << signal_send_time_.count() << "us\n";
            AddLog("[Info] move   : " + output_x + "\t" + output_y + "\t" + jump_state + " " + dive_state +
                   "\tDuration : " + std::to_string(last_period_us_.count()) + "us");
                // "\tSignal Send Time: " + std::to_string(signal_send_time_.count()) + "us");
        }
    }
    return report;
}

// automove the jelly
// left -> right -> up -> down in every 500 loops
void LeftStickControl::AutoMoveJelly() {
    // Verify that the VigemController is initialized and a controller is registered.
    if (!controller_.IsClientValid() || !controller_.IsControllerValid()) {
        std::cerr << "[Error] VigemController is not initialized or controller not registered.\n";
        return;
    }

    AddLog("[Info] Auto Jelly Control started. Press F12 to exit.");
    
    int count = 0;
    // Loop until F12 is pressed or should_exit_ is true.
    while (true) {
        // If F12 is pressed, break out.
        if ((GetAsyncKeyState(VK_F12) & 0x8000) != 0) {
            AddLog("[Info] F12 detected. Exiting Program.\n");
            break;
        }

        // If an external request to exit was made, break out.
        if (should_exit_) {
            AddLog("[Info] LoopExit() called. Exiting Program.\n");
            break;
        }

        // Change mode by get input of pgup or pgdn
        
        // Build and send the current Left Stick report based on T/F/H/G.
        XUSB_REPORT report;

        
        if (count % 100 == 0) {         // go left
            report.sThumbLX = -32768; 
            report.sThumbLY = 0;      
        }
        else if (count % 100 == 1) {    // go right
            report.sThumbLX = 32767; 
            report.sThumbLY = 0;      
        }
        else if (count % 100 == 2) {    // go up
            report.sThumbLX = 0; 
            report.sThumbLY = 32767;      
        }
        else if (count % 100 == 3) {    // go down
            report.sThumbLX = 0; 
            report.sThumbLY = -32768;      
        }

        report.bLeftTrigger  = 0;
        report.bRightTrigger = 0;
        report.sThumbRX      = 0;
        report.sThumbRY      = 0;

        if (!controller_.SendX360Report(report)) {
            std::cerr << "[Error] Failed to send X360 report.\n";
            break;
        }

        // Wait ~20ms (about 50 updates per second).
        count++;
        if (count == 400) {
            count = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }


    // On exit, reset Left Stick to (0,0) to center it.
    XUSB_REPORT neutral = {};
    controller_.SendX360Report(neutral);
    AddLog("[Info] LeftStickControl loop exited.\n");
}


void LeftStickControl::ProcessINI() {
    std::string up_key_string;
    std::string down_key_string;
    std::string left_key_string;
    std::string right_key_string;
    std::string jump_key_string;
    std::string dive_key_string;

    std::string toggle_mode_key_string;
    std::string temp_toggle_mode_key_string;

    std::string controller_onoff_key_string;

    ini_parser_.ParseConfig("Pad to Key", "up", up_key_string);
    ini_parser_.ParseConfig("Pad to Key", "down", down_key_string);
    ini_parser_.ParseConfig("Pad to Key", "left", left_key_string);
    ini_parser_.ParseConfig("Pad to Key", "right", right_key_string);
    ini_parser_.ParseConfig("Pad to Key", "jump", jump_key_string);
    ini_parser_.ParseConfig("Pad to Key", "dive", dive_key_string);

    ini_parser_.ParseConfig("Mode", "toggle", toggle_mode_key_string);

    ini_parser_.ParseConfig("System", "onoff", controller_onoff_key_string);

    
    // convert string input to key map
    try {
        up_key_ = KeyTable::map.at(KeyTable::toUpper(up_key_string));
        down_key_ = KeyTable::map.at(KeyTable::toUpper(down_key_string));
        left_key_ = KeyTable::map.at(KeyTable::toUpper(left_key_string));
        right_key_ = KeyTable::map.at(KeyTable::toUpper(right_key_string));
        jump_key_ = KeyTable::map.at(KeyTable::toUpper(jump_key_string));
        dive_key_ = KeyTable::map.at(KeyTable::toUpper(dive_key_string));

        toggle_mode_key_ = KeyTable::map.at(KeyTable::toUpper(toggle_mode_key_string));
        controller_onoff_key_ = KeyTable::map.at(KeyTable::toUpper(controller_onoff_key_string));

    } catch (const std::out_of_range &e) {
        std::cerr << "!!! Invalid Key input in key_settings.ini !!! : " << e.what() << std::endl;
        std::cerr << "!!! Invalid Key input in key_settings.ini !!! : " << e.what() << std::endl;
        std::cerr << "!!! Invalid Key input in key_settings.ini !!! : " << e.what() << std::endl;
        // errror handling: set default keys
    }
    
    AddLog("[Info] Key Setting Detected Successfully Detected!\n\n"
              "[Info] up key      : " + up_key_string + "\n"
              "[Info] left key    : " + left_key_string + "\n"
              "[Info] down key    : " + down_key_string + "\n"
              "[Info] right key   : " + right_key_string + "\n\n"
              "[Info] jump key    : " + jump_key_string + "\n"
              "[Info] dive key    : " + dive_key_string + "\n\n"
              "[Info] mode toggle : " + toggle_mode_key_string + "\n"
              "[Info] onoff       : " + controller_onoff_key_string + "\n\n\n\n");
    
}



// logging thread
void LeftStickControl::LogWorker() {
    while (log_thread_running_) {
        std::unique_lock<std::mutex> lock(log_mutex_);
        log_cv_.wait(lock, [this]() { return !log_queue_.empty() || !log_thread_running_; });

        while (!log_queue_.empty()) {
            std::string log_message = log_queue_.front();
            log_queue_.pop();
            lock.unlock();

            // log message!
            std::cout << log_message << std::endl;

            lock.lock();
        }
    }
}

void LeftStickControl::AddLog(const std::string& message) {
    std::lock_guard<std::mutex> lock(log_mutex_);
    log_queue_.push(message);
    log_cv_.notify_one();
}