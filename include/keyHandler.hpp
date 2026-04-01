#pragma once
#ifndef HPP_SIMPLETUI__KEYHANDLER
#define HPP_SIMPLETUI__KEYHANDLER

#include <unordered_map>
#include <vector>

#include <chrono>

#include <string>
#include <sstream>

#include <iostream>
#include <iomanip>

#include <Windows.h>



namespace keyHandler {
    /// @brief Retrieves the key codes from user input.
    /// @return A vector of integers representing the key codes pressed by the user.
    std::vector<size_t> helper_getKeyCode();
    
    enum KEY {
        BACKSPACE   = 8,
        ENTER       = 13,
        ESCAPE      = 27,
        SPACE       = 32,
        arrow_LEFT  = 37,
        arrow_UP    = 38,
        arrow_RIGHT = 39,
        arrow_DOWN  = 40,
        letter_A    = 65,
        letter_D    = 68,
        letter_S    = 83,
        letter_W    = 87
    };
            
    struct __keyPressHandler_keyDetails {
        std::chrono::steady_clock::time_point startTime; // time since press start
        bool isPressed; // whether the key is actually being pressed
        bool active;    // whether this key is to give a signal as being pressed.


        operator std::string();

        friend auto operator<<(std::ostream &os, __keyPressHandler_keyDetails const& m) -> std::ostream& {
            os << m.startTime.time_since_epoch().count() << "," << std::boolalpha << m.isPressed <<  ","<< m.active;

            return os;
        }
    };

    class keyPressHandler {
    private:
        // std::vector<int> __keys_pressed;
        // std::vector<std::chrono::steady_clock::time_point> __keys_timePressed;
        std::unordered_map<size_t, __keyPressHandler_keyDetails> __pressed_keys;

        /// @brief Container of every active key.
        std::vector<size_t> __active_keys;

    public:
        
        /// \brief Duration in seconds for the decay of key press type difference.
        /// 
        /// Represents the time interval (0.3 seconds) after which the accumulated
        /// difference in key press types will decay or reset. This is used to track
        /// and manage the decay of input state changes in the terminal user interface.
        std::chrono::duration<double> pressTypeDifrDecayDur_seconds = std::chrono::duration<double>(0.3);

        keyPressHandler();
        keyPressHandler(const keyPressHandler& _toCopy);
        keyPressHandler(keyPressHandler&& _toSwap);
        ~keyPressHandler();
        keyPressHandler& operator=(const keyPressHandler& _toCopy);
        keyPressHandler& operator=(keyPressHandler&& _toMove);

        const std::vector<size_t>& updateKeys();

        const std::vector<size_t>& getActiveKeys();
        __keyPressHandler_keyDetails getKey(size_t _key);
        std::unordered_map<size_t, __keyPressHandler_keyDetails> getAllKeyDetails();

        bool isPressed(size_t _key);
        bool isActivated(size_t _key);

    };

};

#endif //HPP_SIMPLETUI__KEYHANDLER