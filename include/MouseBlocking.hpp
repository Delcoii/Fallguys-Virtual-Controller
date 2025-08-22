#include <Windows.h>

class MouseBlocker {
private:
    HHOOK mouse_hook_;
    bool is_blocking_;
    
    // Static mouse procedure
    static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode >= 0) {
            // Block all mouse move, click, and wheel events
            switch (wParam) {
                case WM_MOUSEMOVE:
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                case WM_MOUSEWHEEL:
                    return 1; // Block input
            }
        }
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    }
    
public:
    MouseBlocker() : mouse_hook_(nullptr), is_blocking_(false) {}
    
    bool StartBlocking() {
        if (is_blocking_) return false;
        
        mouse_hook_ = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, GetModuleHandle(NULL), 0);
        if (mouse_hook_) {
            is_blocking_ = true;
            return true;
        }
        return false;
    }
    
    void StopBlocking() {
        if (mouse_hook_) {
            UnhookWindowsHookEx(mouse_hook_);
            mouse_hook_ = nullptr;
            is_blocking_ = false;
        }
    }
    
    ~MouseBlocker() {
        StopBlocking();
    }
};