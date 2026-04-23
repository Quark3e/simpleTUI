#include <keyHandler.hpp>


#ifndef NOMINMAX
# define NOMINMAX
#endif
// #include <windows.h>
#undef max



namespace keyHandler {

    __keyPressHandler_keyDetails::operator std::string() {
        std::stringstream ss;
        ss << this->startTime.time_since_epoch().count() << "," << std::boolalpha << this->isPressed << "," << this->active;
        
        return ss.str();
    }



    std::vector<size_t> keyPressHandler::helper_getKeyCodes() {
        const std::string _infoStr{"std::vector<size_t> keyPressHandler::helper_getKeyCodess()"};
        std::vector<size_t> pressed_keyCodes;

#ifdef _WIN32
        for(int keyCode=0; keyCode<256; ++keyCode) {
            if(GetAsyncKeyState(keyCode) & 0x8000) {
                char keyChar = static_cast<char>(keyCode);
                pressed_keyCodes.push_back(static_cast<size_t>(keyCode));
            }
        }
#else
        if(!raw_mode_active) throw std::runtime_error(_infoStr+" : raw_mode_active is not active/enabled.");

        while(this->kbhit()) {
            char buffer[16] = {0};
            ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);

            if(bytes_read>0) {
                for(ssize_t _i=0; _i<bytes_read; _i++) {
                    pressed_keyCodes.push_back(static_cast<size_t>(static_cast<unsigned char>(buffer[_i])));
                }
            }
        }
#endif

        return pressed_keyCodes;
    }
#ifdef __unix__
    void keyPressHandler::set_raw_mode(struct termios* old_tio) {
        struct termios new_tio;
        tcgetattr(STDIN_FILENO, old_tio);
        new_tio = *old_tio;

        // Disable canonical mode (line buffering) and echo
        new_tio.c_lflag &= ~(ICANON | ECHO);
        // Set minimum characters to read to 0 (non-blocking)
        new_tio.c_cc[VMIN] = 0;
        // Set timeout to 0 (return immediately if no input)
        new_tio.c_cc[VTIME] = 0;

        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    }
    bool keyPressHandler::kbhit() {
        struct timeval tv = {0, 0};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
    }
    void keyPressHandler::enable_raw_mode() {
        if(!raw_mode_active) {
            set_raw_mode(&old_tio);
            raw_mode_active = true;
        }
    }
    void keyPressHandler::restore_mode() {
        if(raw_mode_active) {
            tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
            raw_mode_active = false;
        }
    }
#endif
    keyPressHandler::keyPressHandler() {
        for(size_t i=0; i<256; i++) {
            __pressed_keys[i] = __keyPressHandler_keyDetails{
                std::chrono::time_point<std::chrono::steady_clock>::max(),
                false,
                false
            };
        }

#ifdef __unix__
        if(!raw_mode_active) this->enable_raw_mode();
#endif

    }
    keyPressHandler::keyPressHandler(const keyPressHandler& _toCopy) {
        this->__pressed_keys = _toCopy.__pressed_keys;
        this->__active_keys  = _toCopy.__active_keys;


#ifdef __unix__
        this->old_tio = _toCopy.old_tio;
        this->raw_mode_active = _toCopy.raw_mode_active;

        if(!raw_mode_active) this->enable_raw_mode();
#endif

    }
    keyPressHandler::keyPressHandler(keyPressHandler&& _toMove) {
        std::swap(this->__pressed_keys, _toMove.__pressed_keys);
        std::swap(this->__active_keys,  _toMove.__active_keys);

#ifdef __unix__
        std::swap(this->old_tio, _toMove.old_tio);
        std::swap(this->raw_mode_active, _toMove.raw_mode_active);

        if(!raw_mode_active) this->enable_raw_mode();
#endif

    }
    keyPressHandler::~keyPressHandler() {
        
#ifdef _WIN32

#else
        if(raw_mode_active) restore_mode();
#endif

    }
    keyPressHandler& keyPressHandler::operator=(const keyPressHandler& _toCopy) {
        this->__pressed_keys = _toCopy.__pressed_keys;
        this->__active_keys  = _toCopy.__active_keys;

#ifdef __unix__
        this->old_tio = _toCopy.old_tio;
        this->raw_mode_active = _toCopy.raw_mode_active;
#endif

        return *this;
    }
    keyPressHandler& keyPressHandler::operator=(keyPressHandler&& _toMove) {
        std::swap(__pressed_keys,   _toMove.__pressed_keys);
        std::swap(__active_keys,    _toMove.__active_keys);

#ifdef __unix__
        std::swap(this->old_tio, _toMove.old_tio);
        std::swap(this->raw_mode_active, _toMove.raw_mode_active);
#endif

        return *this;
    }

    std::vector<size_t>& keyPressHandler::updateKeys() {
        std::vector<size_t> newKeys = helper_getKeyCodes();
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

    bool keyPressHandler::isPressed(size_t _key, bool _caseSensitive) {
#ifdef __unix__
        if(_key>=37 && _key<=40) {
            return (
                __pressed_keys.at(27).isPressed && __pressed_keys.at(91).isPressed && (
                    __pressed_keys.at(65).isPressed &&
                    __pressed_keys.at(66).isPressed &&
                    __pressed_keys.at(67).isPressed &&
                    __pressed_keys.at(68).isPressed
                )
            );
        }
#endif
        
        if(!_caseSensitive) {
            if(_key>=65 && _key<=90)    return (__pressed_keys.at(_key).isPressed || __pressed_keys.at(_key+32).isPressed);
            if(_key>=97 && _key<=122)   return (__pressed_keys.at(_key).isPressed || __pressed_keys.at(_key-32).isPressed);
        }
        
        return __pressed_keys.at(_key).isPressed;
    }
    bool keyPressHandler::isActivated(size_t _key, bool _caseSensitive) {
        
#ifdef __unix__
        if(__active_keys.size()==3 && __active_keys.at(0)==27 && __active_keys.at(2)==91) {
            if(_key>=37 && _key<=40) {
                switch (_key) {
                    case KEY::arrow_UP:     return (__active_keys.at(1)==65);
                    case KEY::arrow_DOWN:   return (__active_keys.at(1)==66);
                    case KEY::arrow_RIGHT:  return (__active_keys.at(1)==67);
                    case KEY::arrow_LEFT:   return (__active_keys.at(1)==68);
                }
            }
            return false;
        }
#endif
        
        if(!_caseSensitive) {
            if(_key>=65 && _key<=90)    return (__pressed_keys.at(_key).active || __pressed_keys.at(_key+32).active);
            if(_key>=97 && _key<=122)   return (__pressed_keys.at(_key).active || __pressed_keys.at(_key-32).active);
        }
        
        
        return this->__pressed_keys.at(_key).active;
    }

};

