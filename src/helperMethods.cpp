#include <helperMethods.hpp>


namespace helperMethods {

    Pos2d<size_t> CURRENT_CONSOLE_DIMENSIONS{0, 0};
    
        
    Pos2d<size_t> helper_getConsoleDimensions() {
        Pos2d<size_t> console_dimensions(-1, -1);

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        console_dimensions.x = csbi.srWindow.Right  - csbi.srWindow.Left    + 1;
        console_dimensions.y = csbi.srWindow.Bottom - csbi.srWindow.Top     + 1;

        CURRENT_CONSOLE_DIMENSIONS = console_dimensions;

        return console_dimensions;
    }

    std::vector<std::string> helper_parseStringForNewlines(std::string _fullString) {
        std::vector<std::string> parsed_string;
        
        size_t newlinePos = _fullString.find('\n');
        while(newlinePos!=std::string::npos) {
            parsed_string.push_back(_fullString.substr(0, newlinePos));
            _fullString.erase(0, newlinePos+1);
            newlinePos = _fullString.find('\n');
        }
        if(_fullString.size()>0) parsed_string.push_back(_fullString);
        
        return parsed_string;
    }


    namespace ANSIec {
        void setCursorPos(size_t _x, size_t _y, bool _flushEnd) {
            std::cout << esc_code << _y << ";" << _x << "H";
            if(_flushEnd) std::cout.flush();
        }
        void setCursorPos(Pos2d<size_t> _pos, bool _flushEnd) {
            setCursorPos(_pos.x, _pos.y, _flushEnd);
        };

        void clearScreen(bool _flushEnd) {
            std::cout << esc_code << "2J";
            if(_flushEnd) std::cout.flush();
        }

        void Print(
            size_t _x, size_t _y,
            std::string _text,
            bool _flushEnd,
            PrintAxisMethod _x_method,
            PrintAxisMethod _y_method,
            bool _initClearScreen
        ) {
            static Pos2d<size_t> prevPos{0, 0};

            if(_x_method==PrintAxisMethod::relative) _x = prevPos.x+_x;
            if(_y_method==PrintAxisMethod::relative) _y = prevPos.y+_y;

            if(_initClearScreen) {
                clearScreen(false);
            }

            if(_text.size()==0) {
                return;
            }
            else {
                std::vector<std::string> splitString = helper_parseStringForNewlines(_text);
                Pos2d<size_t> parsed2Dsizes{splitString.back().size(), splitString.size()};
                for(size_t pos_y=0; pos_y<parsed2Dsizes.y; pos_y++) {
                    setCursorPos(_x, _y+pos_y, false);
                    std::cout << splitString.at(pos_y);
                }
                prevPos.x = _x + parsed2Dsizes.x;
                prevPos.y = _y + parsed2Dsizes.y;
            }

            if(_flushEnd) std::cout.flush();

        }
    };

};