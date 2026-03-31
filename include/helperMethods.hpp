#pragma once
#ifndef HPP_SIMPLETUI__HELPERMETHODS
#define HPP_SIMPLETUI__HELPERMETHODS


#include <string>
#include <vector>

#include <iostream>
#include <functional>

#include <Pos2d.hpp>

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#endif


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

    /// @brief Formats a value to a string with customizable width, precision, and alignment.
    /// @tparam _castType The type of the value to be formatted.
    /// @param _value The value to be formatted.
    /// @param _width The width of the formatted string (default: 0).
    /// @param _precision The decimal precision for floating-point values (default: 1).
    /// @param _align The alignment direction - "left" or "right" (default: "right").
    /// @param _numberFill Whether to fill numeric values with leading zeros (default: false).
    /// @param _formatCommas Whether to insert thousands separators (default: false).
    /// @return A formatted string representation of the value.
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

    /// @brief Formats a container of values as a formatted string representation.
    /// @tparam T The container type (must be iterable).
    /// @param _container The container of values to be formatted.
    /// @param _strWidth The width of each formatted value within the container.
    /// @param _varPrecision The decimal precision for each value.
    /// @param _align The alignment direction - "left" or "right" (default: "right").
    /// @param _numberFill Whether to fill values with leading zeros (default: false).
    /// @param _openSymb The opening symbol for the container (default: '{').
    /// @param _closeSymb The closing symbol for the container (default: '}').
    /// @param _sepSymb The separator symbol between values (default: ',').
    /// @return A formatted string representation of the container.
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

        /// @brief Sets the cursor position to absolute coordinates.
        /// @param _x The x-coordinate (column).
        /// @param _y The y-coordinate (row).
        /// @param _flushEnd Whether to flush the output buffer (default: false).
        void setCursorPos(size_t _x, size_t _y, bool _flushEnd=false);
        
        /// @brief Sets the cursor position using a Pos2d object.
        /// @param _pos A Pos2d<size_t> object containing the x and y coordinates.
        /// @param _flushEnd Whether to flush the output buffer (default: false).
        void setCursorPos(Pos2d<size_t> _pos, bool _flushEnd=false);
        
        /// @brief Clears the entire console screen.
        /// @param _flushEnd Whether to flush the output buffer (default: true).
        void clearScreen(bool _flushEnd=true);
        
        /// @brief Enumeration for cursor positioning method.
        enum class PrintAxisMethod {
            absolute,   ///< Set the coordinate position for that axis as absolute value.
            relative    ///< Set the coordinate position for that axis as relative from the previously set value (held as static).
        };

        /// @brief Prints text at specified coordinates with support for absolute/relative positioning.
        /// @param _x The x-coordinate (column).
        /// @param _y The y-coordinate (row).
        /// @param _text The text to print.
        /// @param _flushEnd Whether to flush the output buffer (default: true).
        /// @param _x_method The positioning method for the x-axis (default: absolute).
        /// @param _y_method The positioning method for the y-axis (default: absolute).
        /// @param _initClearScreen Whether to clear the screen before printing (default: false).
        void Print(
            size_t _x, size_t _y,
            std::string _text,
            bool _flushEnd=true,
            PrintAxisMethod _x_method=PrintAxisMethod::absolute,
            PrintAxisMethod _y_method=PrintAxisMethod::absolute,
            bool _initClearScreen=false
        );
    };
    
    
    /// @brief Enumeration for matrix search method.
    enum flag_matrixSearch_method {
        radial, ///< Search in expanding rectangular rings from the start position.
        axial   ///< Search along horizontal and vertical axes from the start position.
    };

    /// @brief Type alias for the comparison function used in 2D matrix searches.
    /// @tparam _searchType The type of elements being searched.
    /// @note First argument is to be reserved for the matrices element and the second argument for the `_toFind` object of _searchType type.
    template<typename _searchType>
    using checkFuncType_matrixSearch_2D = std::function<bool(_searchType, _searchType)>;
    
    /// @brief Searches a 2D matrix using a radial (concentric rings) pattern.
    /// @tparam _searchType The type of elements in the matrix.
    /// @param _foundPosRef Output vector to store found positions.
    /// @param _matrixRef The 2D matrix to search through.
    /// @param _findRef The value to search for.
    /// @param _startPos The starting position for the radial search. If npos, defaults to (0,0).
    /// @param _funcRef Comparison function to determine matches between elements.
    /// @param _extractAtNumFound Stop searching after finding this many matches (0 = find all).
    /// @details Expands outward in rectangular rings from the start position.
    template<typename _searchType>
    inline void matrixSearch_2D__method_radial(
        std::vector<Pos2d<size_t>>& _foundPosRef,
        std::vector<std::vector<_searchType>>& _matrixRef,
        _searchType& _findRef,
        Pos2d<size_t> _startPos,
        checkFuncType_matrixSearch_2D<_searchType>& _funcRef,
        const size_t& _extractAtNumFound
    ) {
        if(_startPos.x==std::string::npos) _startPos.x = 0;
        if(_startPos.y==std::string::npos) _startPos.y = 0;

        Pos2d<size_t> corner_TL = _startPos;
        Pos2d<size_t> corner_BR = _startPos;
        
        Pos2d<size_t> matrixSize{_matrixRef.at(0).size(), _matrixRef.size()};
    
        std::vector<bool> outOfBounds(4, false);

        bool _verbose = false;
        if(_funcRef(_matrixRef.at(_startPos.y).at(_startPos.x), _findRef)) _foundPosRef.push_back(_startPos);
        for(size_t radius=1; radius<(_matrixRef.size()>_matrixRef.at(0).size()? _matrixRef.size() : _matrixRef.at(0).size()); radius++) {
            ///corner_TL.x = (radius>_startPos.x? std::string::npos : _startPos.x-radius);
            ///corner_TL.y = (radius>_startPos.y? std::string::npos : _startPos.y-radius);
            ///corner_BR.x = (_startPos.x+radius>=matrixSize.x? std::string::npos : _startPos.x+radius);
            ///corner_BR.y = (_startPos.y+radius>=matrixSize.y? std::string::npos : _startPos.y+radius);

            if(radius>_startPos.x) {
                corner_TL.x = 0;
                outOfBounds[0] = true;
            }
            else corner_TL.x = _startPos.x-radius;
            
            if(radius>_startPos.y) {
                corner_TL.y = 0;
                outOfBounds[1] = true;
            }
            else corner_TL.y = _startPos.y-radius;
            
            if(_startPos.x+radius>=matrixSize.x) {
                corner_BR.x = matrixSize.x-1;
                outOfBounds[2] = true;
            }
            else corner_BR.x = _startPos.x+radius;
            
            if(_startPos.y+radius>=matrixSize.y) {
                corner_BR.y = matrixSize.y-1;
                outOfBounds[3] = true;
            }
            else corner_BR.y = _startPos.y+radius;
            

            if(_verbose) std::cout << "corners: {"<<corner_TL.x<<","<<corner_TL.y<<"} {"<<corner_BR.x<<","<<corner_BR.y<<"} | ";

            
            if(!outOfBounds[1]) {
                for(Pos2d<size_t> _pos=corner_TL; _pos.x<corner_BR.x; _pos.x++) {
                    if(_funcRef(_matrixRef.at(_pos.y).at(_pos.x), _findRef)) {
                        _foundPosRef.push_back(_pos);
                        if(_verbose) std::cout << "[0] ";
                        if(_foundPosRef.size()>=_extractAtNumFound) break;
                    }
                }
                if(_foundPosRef.size()>=_extractAtNumFound) break;
            }
            if(!outOfBounds[2]) {
                for(Pos2d<size_t> _pos={corner_BR.x, corner_TL.y}; _pos.y<corner_BR.y; _pos.y++) {
                    if(_funcRef(_matrixRef.at(_pos.y).at(_pos.x), _findRef)) {
                        _foundPosRef.push_back(_pos);
                        if(_verbose) std::cout << "[1] ";
                        if(_foundPosRef.size()>=_extractAtNumFound) break;
                    }
                }
                if(_foundPosRef.size()>=_extractAtNumFound) break;
            }
            if(!outOfBounds[3]) {
                for(Pos2d<size_t> _pos=corner_BR; _pos.x>corner_TL.x; _pos.x--) {
                    if(_funcRef(_matrixRef.at(_pos.y).at(_pos.x), _findRef)) {
                        _foundPosRef.push_back(_pos);
                        if(_verbose) std::cout << "[2] ";
                        if(_foundPosRef.size()>=_extractAtNumFound) break;
                    }
                }
                if(_foundPosRef.size()>=_extractAtNumFound) break;
            }
            if(!outOfBounds[0]) {
                for(Pos2d<size_t> _pos={corner_TL.x, corner_BR.y}; _pos.y>corner_TL.y; _pos.y--) {
                    if(_funcRef(_matrixRef.at(_pos.y).at(_pos.x), _findRef)) {
                        _foundPosRef.push_back(_pos);
                        if(_verbose) std::cout << "[3] ";
                        if(_foundPosRef.size()>=_extractAtNumFound) break;
                    }
                }
                if(_foundPosRef.size()>=_extractAtNumFound) break;
            }
            
            if(_verbose) std::cout << "\n";
        }
        
    }
    /// @brief Searches a 2D matrix using an axial (cross) pattern.
    /// @tparam _searchType The type of elements in the matrix.
    /// @param _foundPosRef Output vector to store found positions.
    /// @param _matrixRef The 2D matrix to search through.
    /// @param _findRef The value to search for.
    /// @param _startPos The starting position for the axial search.
    /// @param _funcRef Comparison function to determine matches between elements.
    /// @param _extractAtNumFound Stop searching after finding this many matches (0 = find all).
    /// @details Searches along horizontal and vertical axes extending from the start position.
    template<typename _searchType>
    inline void matrixSearch_2D__method_axial(
        std::vector<Pos2d<size_t>>& _foundPosRef,
        std::vector<std::vector<_searchType>>& _matrixRef,
        _searchType& _findRef,
        Pos2d<size_t> _startPos,
        std::function<bool(_searchType, _searchType)>& _funcRef,
        const size_t& _extractAtNumFound
    ) {

    }

    /// @brief Performs a 2D matrix search with selectable search algorithm (radial or axial).
    /// @tparam _searchType The type of elements in the matrix.
    /// @param _matrixToSearch The 2D matrix to search through (must not be empty).
    /// @param _toFind The value to search for.
    /// @param _startPos The starting position for the search. If both coordinates are npos, defaults to (0,0).
    /// @param _matchCheckFunction Custom comparison function for element matching. If null, uses equality operator (default: nullptr).
    /// @param _extractAtNumFound Maximum number of matches to find. If 0, finds all matches (default: 0).
    /// @param _methodOfSearch The search algorithm to use - radial or axial (default: radial).
    /// @return A vector of Pos2d<size_t> containing the positions of all found matches.
    /// @throws std::invalid_argument if _matrixToSearch is empty.
    template<typename _searchType>
    inline std::vector<Pos2d<size_t>> matrixSearch_2D(
        std::vector<std::vector<_searchType>> _matrixToSearch,
        _searchType _toFind,
        Pos2d<size_t> _startPos = Pos2d<size_t>{std::string::npos, std::string::npos},
        checkFuncType_matrixSearch_2D<_searchType> _matchCheckFunction = nullptr,
        size_t _extractAtNumFound = 0,
        flag_matrixSearch_method _methodOfSearch = flag_matrixSearch_method::radial
    ) {
        // static std::string _infoStr_function{"template<_searchType> inline Pos2d<size_t> matrixSearch_2D(std::vector<std::vector<_searchType>>, _searchType, Pos2d<size_t>, std::function<bool(_searchType, _searchType)>, flag_matrixSearch_method)"};
        static std::string _infoStr_function{"[...] Pos2d<size_t> matrixSearch_2D([...])"};
        if(_matrixToSearch.size()==0) throw std::invalid_argument(_infoStr_function+" : the argument for matrix to be searched `_matrixToSearch` is not allowed to be empty.");

        std::vector<Pos2d<size_t>> foundMatrixPos;

        if(!_matchCheckFunction) _matchCheckFunction = [](_searchType _compareA, _searchType _compareB) {
            return (_compareA==_compareB);
        };

        switch (_methodOfSearch) {
        case flag_matrixSearch_method::radial:
            matrixSearch_2D__method_radial(foundMatrixPos, _matrixToSearch, _toFind, _startPos, _matchCheckFunction, _extractAtNumFound);
            break;
        case flag_matrixSearch_method::axial:
            matrixSearch_2D__method_axial(foundMatrixPos, _matrixToSearch, _toFind, _startPos, _matchCheckFunction, _extractAtNumFound);
            break;
        default:
            break;
        }


        return foundMatrixPos;
    }

};


#endif //HPP_SIMPLETUI__HELPERMETHODS
