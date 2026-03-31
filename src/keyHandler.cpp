#include <keyHandler.hpp>


#ifndef NOMINMAX
# define NOMINMAX
#endif
// #include <windows.h>
#undef max



namespace keyHandler {
    std::vector<size_t> helper_getKeyCode() {
        std::vector<size_t> pressed_keyCodes;
        for(int keyCode=0; keyCode<256; ++keyCode) {
            if(GetAsyncKeyState(keyCode) & 0x8000) {
                char keyChar = static_cast<char>(keyCode);
                pressed_keyCodes.push_back(static_cast<size_t>(keyCode));
            }
        }

        return pressed_keyCodes;
    }

    __keyPressHandler_keyDetails::operator std::string() {
        std::stringstream ss;
        ss << this->startTime.time_since_epoch().count() << "," << std::boolalpha << this->isPressed << "," << this->active;
        
        return ss.str();
    }


    keyPressHandler::keyPressHandler() {
        for(size_t i=0; i<256; i++) {
            __pressed_keys[i] = __keyPressHandler_keyDetails{
                std::chrono::time_point<std::chrono::steady_clock>::max(),
                false,
                false
            };
        }
    }
    keyPressHandler::keyPressHandler(const keyPressHandler& _toCopy) {
        this->__pressed_keys = _toCopy.__pressed_keys;
        this->__active_keys  = _toCopy.__active_keys;

    }
    keyPressHandler::keyPressHandler(keyPressHandler&& _toSwap) {
        std::swap(this->__pressed_keys, _toSwap.__pressed_keys);
        std::swap(this->__active_keys,  _toSwap.__active_keys);

    }
    keyPressHandler::~keyPressHandler() {}
    keyPressHandler& keyPressHandler::operator=(const keyPressHandler& _toCopy) {
        this->__pressed_keys = _toCopy.__pressed_keys;
        this->__active_keys  = _toCopy.__active_keys;

        return *this;
    }
    keyPressHandler& keyPressHandler::operator=(keyPressHandler&& _toMove) {
        std::swap(__pressed_keys,   _toMove.__pressed_keys);
        std::swap(__active_keys,    _toMove.__active_keys);

        return *this;
    }

    const std::vector<size_t>& keyPressHandler::updateKeys() {
        std::vector<size_t> newKeys = helper_getKeyCode();
        /// \brief Stores the current reference time point using a steady clock. The reference
        ///         is used to determine how much time has passed since a key was pressed.
        /// 
        /// This variable captures the current time from std::chrono::steady_clock,
        /// which is suitable for measuring elapsed time and is not affected by
        /// system clock adjustments. Used for timing operations that require
        /// monotonic time progression.
        /// 
        /// Does not store any 
        /// 
        std::chrono::steady_clock::time_point refrTime_now = std::chrono::steady_clock::now();

        for(size_t i=0; i<256; i++) __pressed_keys[i].isPressed = false;
        for(size_t i=0; i<newKeys.size(); i++) {
            __pressed_keys[newKeys.at(i)].isPressed = true;
            if(__pressed_keys[newKeys.at(i)].startTime==std::chrono::time_point<std::chrono::steady_clock>::max()) {
                /// If the `startTime` member of the key is set to maximum value then it means the startTime isn't defined yet.
                __pressed_keys[newKeys.at(i)].startTime = refrTime_now;
                // __pressed_keys[newKeys.at(i)].active = true;
            }
            else {

            }
        }
        __active_keys.clear();
        for(size_t i=0; i<256; i++) {
            __keyPressHandler_keyDetails& keyRef = __pressed_keys[i];

            if(!keyRef.isPressed) { /// If key isn't pressed
                keyRef.startTime = std::chrono::time_point<std::chrono::steady_clock>::max();
                keyRef.active = false;
            }
            else { /// If key is pressed
                if(keyRef.startTime==refrTime_now || (refrTime_now-keyRef.startTime)>pressTypeDifrDecayDur_seconds) {
                    /// If startTime is set to current time (meaning the key is pressed this cycle/iteration) or if the set startTime - currentTime is
                    /// bigger than the pre-determined decay time for differentiating between click and hold.

                    keyRef.active = true;
                    __active_keys.push_back(i);
                }
                else {
                    keyRef.active = false;
                }
            }

        }

        return __active_keys;
    }

    const std::vector<size_t>& keyPressHandler::getActiveKeys() {
        return __active_keys;
    }
    __keyPressHandler_keyDetails keyPressHandler::getKey(size_t _key) {
        return __pressed_keys.at(_key);
    }
    std::unordered_map<size_t, __keyPressHandler_keyDetails> keyPressHandler::getAllKeyDetails() {
        return __pressed_keys;
    }

    bool keyPressHandler::isPressed(size_t _key) {
        return this->__pressed_keys.at(_key).isPressed;
    }
    bool keyPressHandler::isActivated(size_t _key) {
        return this->__pressed_keys.at(_key).active;
    }

};

