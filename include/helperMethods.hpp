#pragma once
#ifndef HPP_SIMPLETUI__HELPERMETHODS
#define HPP_SIMPLETUI__HELPERMETHODS


#include <string>
#include <vector>

#include <iostream>

#include <Pos2d.hpp>
#include <Windows.h>


namespace helperMethods {

    /**
     * @brief Container for the last gotten character-size xy dimensions of the console.
     * 
     */
    extern Pos2d<size_t> CURRENT_CONSOLE_DIMENSIONS;


    /// @brief Retrieves the current console window dimensions.
    /// @return A Pos2d<size_t> object containing the console width and height.
    /// @details This helper function queries the console window to determine its current
    ///          size in terms of columns (width) and rows (height).
    Pos2d<size_t> helper_getConsoleDimensions();
    
    /**
     * @brief Parses a string and splits it into multiple strings at newline characters.
     * 
     * @param _fullString The input string to be parsed for newline characters.
     * 
     * @return A vector of strings, where each element represents a line from the input
     *         string that was separated by newline characters.
     */
    std::vector<std::string> helper_parseStringForNewlines(std::string _fullString);

    template<class _castType>
    inline std::string fmtToStr(
        _castType   _value,
        size_t      _width          = 0,
        int         _precision      = 1,
        std::string _align          = "right",
        bool        _numberFill     = false,
        bool        _formatCommas   = false
    ) {
        std::stringstream outStream, _temp;
        std::string _final;
        int fillZeros = 0;
        if(_formatCommas) {
            outStream.imbue(std::locale(""));
        }
        if(_numberFill && _align=="right") {
            _temp << std::fixed;
            _temp << std::setprecision(_precision) << _value;
            if(static_cast<int>(_temp.str().length()) < _width) fillZeros = static_cast<int>(_width) - static_cast<int>(_temp.str().length());
        }
        outStream << std::fixed;
        outStream << std::boolalpha;
        if(_align=="left") outStream<<std::left;
        else if(_align=="right") outStream<<std::right;
        outStream << std::setw(_width - fillZeros);
        if(_numberFill && _align=="right") outStream << std::string(fillZeros, '0');
        // outStream << (align=="left"? std::left : std::right);
        outStream << std::setprecision(_precision) << _value;

        return outStream.str();
    }

    template<class T>
    inline std::string fmtCont(
        T _container,
        int _strWidth,
        int _varPrecision,
        std::string _align = "right",
        bool _numberFill = false,
        char _openSymb   = '{',
        char _closeSymb  = '}',
        char _sepSymb   = ','
    ) {
        std::string _out{_openSymb};
        for(auto itr=_container.begin(); itr!=_container.end(); ++itr) {
            _out += fmtToStr(*itr, _strWidth, _varPrecision, _align, _numberFill);
            if(itr!=--_container.end()) _out += _sepSymb;
        }
        return _out + _closeSymb;
    }


    /**
     * @brief ANSI escape code related helper methods.
     * 
     */
    namespace ANSIec {
        const std::string esc_code = "\x1B[";

        void setCursorPos(size_t _x, size_t _y, bool _flushEnd=false);
        void setCursorPos(Pos2d<size_t> _pos, bool _flushEnd=false);
        
        void clearScreen(bool _flushEnd=true);
        
        enum class PrintAxisMethod {
            absolute,   // Set the coordinate position for that axis as absolute value.
            relative    // Set the coordinate position for that axis as a relative value from the previously set value. NOTE! The previous position is hold as a static variable.
        };

        void Print(
            size_t _x, size_t _y,
            std::string _text,
            bool _flushEnd=true,
            PrintAxisMethod _x_method=PrintAxisMethod::absolute,
            PrintAxisMethod _y_method=PrintAxisMethod::absolute,
            bool _initClearScreen=false
        );
    };
    

};


#endif //HPP_SIMPLETUI__HELPERMETHODS