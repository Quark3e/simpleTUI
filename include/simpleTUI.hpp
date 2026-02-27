#pragma once
#ifndef HPP__SIMPLETUI
#define HPP__SIMPLETUI


#include <mutex>
#include <thread>
#include <atomic>

#include <string>
#include <cstring>
#include <iostream>
#include <sstream>

#include <initializer_list>
#include <vector>
#include <map>
#include <functional>

#include <cassert>
#include <memory>
#include <chrono>

#include <Pos2d.hpp>

#ifndef NOMINMAX
# define NOMINMAX
#endif
// #include <windows.h>
#undef max


#define DEBUGPRINT(str) ANSIec::Print(0, 0, str, true, ANSIec::PrintAxisMethod::absolute, ANSIec::PrintAxisMethod::relative);
#define DEBUGPAUSE(ms)  std::this_thread::sleep_for(std::chrono::milliseconds(ms));



#pragma warning(disable: 4267)


namespace simpleTUI {

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

    namespace KeyHandler {
        /// @brief Retrieves the key codes from user input.
        /// @return A vector of integers representing the key codes pressed by the user.
        std::vector<size_t> helper_getKeyCode();
        
        enum KEY {
            BACKSPACE   = 8,
            ENTER   = 13,
            ESC     = 27,
            SPACE   = 32
        };
                
        struct __keyPressHandler_keyDetails {
            std::chrono::steady_clock::time_point startTime; // time since press start
            bool isPressed; // whether the key is actually being pressed
            bool active; // whether this key is to give a signal as being pressed.


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

        };
    };
    

    namespace core {
        class Cell;
        class Region;
        class Table;
        class Menu;
    };

    using type_cellFunc = std::function<void(core::Table*)>;

    enum class cell_types {
        null    = 0,
        text    = 1,
        function= 2,
        menu    = 3
    };

    // struct cell_singleTempHolder {
    // };

    struct cellTypeContent_null {

    };
    struct cellTypeContent_text {
        bool rule_followDelimiter{true};
    };
    struct cellTypeContent_function {

    };
    struct cellTypeContent_menu {
        
    };

    class  core::Cell {
        private:
        
        Table*  tablePtr{nullptr};

        cell_types      cellType{cell_types::null};
        Pos2d<int>      pos;

        // std::string     text;

        /**
         * Container for the text that's stored in the cell. Each element/string represents one line of the total string box,
         *  meaning no newline characters are allowed
         */
        std::vector<std::string>    text;
        type_cellFunc   function;
        //cell stores a menu object entirely in itself, by accessing it through a pointer
        std::unique_ptr<Menu>   menu;
        

        cellTypeContent_null        cellContent_null;
        cellTypeContent_text        cellContent_text;
        cellTypeContent_function    cellContent_function;
        cellTypeContent_menu        cellContent_menu;

        bool isDefined__pos{false};
        bool isDefined__text{false};
        bool isDefined__function{false};
        bool isDefined__menu{false}; //this way of checking definition is currently used just in case it's changed from pointer later on.'

        bool isModified__tablePtr{true};
        bool isModified__cellType{true};
        bool isModified__pos{true};
        bool isModified__text{true};
        bool isModified__function{true};
        bool isModified__menu{true};
        
        void cell_resetModificationFlag();

        friend class Table;
        friend class Menu;

        public:
    
        
        Cell(cell_types _cellType=cell_types::null);
        Cell(std::string _text, cell_types _cellType=cell_types::text);
        Cell(std::string _text, type_cellFunc _func, cell_types _cellType=cell_types::function);
        Cell(std::string _text, std::unique_ptr<Menu> _menu, cell_types _cellType=cell_types::menu);
        
        Cell(const Cell& _toCopy);
        Cell(Cell&& _toMove);
        ~Cell();
        Cell& operator=(const Cell& _toCopy);
        Cell& operator=(Cell&& _toMove);
        
        void call();
        
        int set_tablePtr(Table* _tablePtr);
        int set_pos(Pos2d<int> _pos);
        int set_text(std::string _text);
        int set_function(type_cellFunc _func);
        int set_menu(std::unique_ptr<Menu> _menu);
        
        int setContent_null(cellTypeContent_null _newContent);
        int setContent_text(cellTypeContent_text _newContent);
        int setContent_function(cellTypeContent_function _newContent);
        int setContent_menu(cellTypeContent_menu _newContent);


        void change_type(cell_types _newType);
        void change_type(cell_types _newType, std::string _text);
        void change_type(cell_types _newType, std::string _text, type_cellFunc _func);
        void change_type(cell_types _newType, type_cellFunc _func);
        void change_type(cell_types _newType, std::string _text, std::unique_ptr<Menu> _menu);
        void change_type(cell_types _newType, std::unique_ptr<Menu> _menu);

        Table*          get_tablePtr() const;
        Pos2d<int>      get_pos() const;
        std::string     get_text() const;
        type_cellFunc   get_function() const;
        // Menu            get_menu() const;
        cellTypeContent_null    get_cellContent_null() const;
        cellTypeContent_text    get_cellContent_text() const;
        cellTypeContent_function    get_cellContent_function() const;
        cellTypeContent_menu    get_cellContent_menu() const;
        cell_types  get_type() const;

        // Cell clone();

        bool isModified();
        bool isModified_tablePtr();
        bool isModified_pos();
        bool isModified_text();
        bool isModified_function();
        bool isModified_menu();
        
    };

    
    class core::Region {
        private:
        
            
        protected:
        
        Region();
        Region(const Region& _toCopy);
        Region(Region&& _toMove);
        ~Region();
        
        Region& operator=(const Region& _toCopy);
        Region& operator=(Region&& _toMove);
        
        
        
    };
    

    enum class style_axisCellScalingMethod {
        cellWidth,      // scale every cell_axis_size accordingly to their longest core::cell and likely have the axis go out of bounds or vice-versa to the TUI core::menu screen width. NOTE: will not include axes with only null cells
        fitMenuAxis_even,       // scale every cell_axis_size so they fit within the core::menu's axis evenly. Can cause clipping of text between column delimiters.
        // fitMenuAXis_adjusted    // scale every cell_axis_size so they fit within the core::menu's axis accordingly to an ratio of the size of each cell for that axis.
    };
    
    class core::Table {
        protected:
        /**
         * access index: [row][column] ([y][x])
         *  {
         *      {[0, 0], [0, 1], [0, 2]},
         *      {[1, 0], [1, 1], [1, 2]}
         *  }
         */
        std::vector<std::vector<Cell>> tableOfCells;
        
        /**
         * User defined character-size max dimensions of core::table. -1 means use dimensions of terminal
         * 
         */
        Pos2d<int> dimSize_table{-1, -1};

        /// @brief Maximum character-size widths of each column
        std::vector<size_t> maxSize_columnWidths;
        /// @brief Maximum character-size heights of each row
        std::vector<size_t> maxSize_rowHeights;

        
        /// \brief Loads a 2D matrix of cells from an initializer list.
        /// 
        /// This function populates the internal cell matrix using the provided
        /// nested initializer list. Each inner initializer list represents a row
        /// of cells in the matrix.
        /// 
        /// \param _matrixInput A nested initializer list of Cell objects, where each
        ///                     inner list represents a row in the matrix.
        /// 
        /// \note The dimensions of the matrix are determined by the input dimensions.
        ///       Ensure that all rows have consistent column counts for proper matrix structure.
        void func_loadInitialiserCellMatrix(std::initializer_list<std::initializer_list<Cell>> _matrixInput);


        void update__maxSize_axisVectors();

        /// @brief  Get [y][x] matrix location to the first character for the given cell for the text in
        ///         PrintableStringVectorMatrix.
        Pos2d<size_t> helper_PSVmatrixLocator(Pos2d<size_t> _cellPos);
        
        /// @brief Updates the table pointer stored in all table cells.
        /// 
        /// This function iterates through all cells in the table and updates their
        /// internal references to point to the current table object. This is typically
        /// called after table restructuring operations to ensure cells maintain valid
        /// pointers to their parent table.
        void helper_updateTablePtrInCells();
        
        /// @brief Separates a string into multiple lines based on a delimiter.
        /// @param _toSep The string to be separated into lines.
        /// @param _delim The delimiter used to separate lines. Defaults to newline character "\n".
        /// @return A vector of strings, each representing a separated line.
        std::vector<std::string> help__separateLines(std::string _toSep, std::string _delim="\n");
        
        /// @brief Updates the string table with current data
        /// @details Refreshes and synchronizes the string table contents to reflect
        ///          the latest state of the application data
        void update__PSVmatrix();
        /// @brief Final string containing the core::table in full
        std::string string_table{""};

        /// @brief Current character-size 2d dimensions of the PrintableStringVector matrix.
        Pos2d<size_t> current_PSVmatrix_size{0, 0};
        /**
         * string vector that represents the rows for the printable table.
         * 
         * [row][column]
         * 
         */
        std::vector<std::string> PrintableStringVectorMatrix;

        style_axisCellScalingMethod scalMethod_columns{style_axisCellScalingMethod::fitMenuAxis_even};
        style_axisCellScalingMethod scalMethod_rows{style_axisCellScalingMethod::fitMenuAxis_even};
        
        /// Member variables that store strings as symbols for the different aspects of the Terminal User Interface.
        ///     the contents are not allowed to be completely empty and must at least hold a single space to separate the different cells.
        ///     for now we ignore specific corner symbols
        // std::string symb_delimiter_columns{"|"};
        // std::string symb_delimiter_rows{"-"};
        // std::string symb_border_column{"|"};
        // std::string symb_border_row{"-"};
        // std::string symb_border_corner{"*"};
        std::string symb_delimiter_columns{" "};
        std::string symb_delimiter_rows{" "};
        std::string symb_border_column{" "};
        std::string symb_border_row{" "};
        std::string symb_border_corner{" "};
        std::string rowSeparator{"\n"}; // currently not used
        

        std::atomic<bool> bool_driverRunning{false};

        public:
        
        Table();
        Table(const Table& _toCopy);
        Table(Table&& _toMove);
        ~Table();
        Table& operator=(const Table& _toCopy);
        Table& operator=(Table&& _toMove);
        
        Table(std::initializer_list<std::initializer_list<Cell>> _matrixInput);
        Table& operator=(std::initializer_list<std::initializer_list<Cell>> _matrixInput);
        
        std::vector<Cell>& operator[](size_t _i);
        std::vector<Cell>  operator[](size_t _i) const;
        std::vector<Cell>& at(size_t _i);
        std::vector<Cell>  at(size_t _i) const;

        // Cell& get_cell(size_t _x, size_t _y);
        // Cell  get_cell(size_t _x, size_t _y) const;
        // Cell& get_cell(Pos2d<int> _pos);
        // Cell  get_cell(Pos2d<int> _pos) const;

        size_t rows();
        size_t columns();
        size_t size();
        
        std::vector<Cell> continuous(bool _includeNullCells=true);

        // int addCell(size_t _x, size_t _y);
        // int addCell(Pos2d<int> _pos);
        // int moveCell(size_t _current_x, size_t _current_y, size_t _new_x, size_t _new_y);
        // int moveCell(Pos2d<int> _current_pos, Pos2d<int> _new_pos);
        // int eraseCell(size_t _x, size_t _y);
        // int eraseCell(Pos2d<int> _pos);

        int call(size_t _x, size_t _y);
        int call(Pos2d<int> _pos);

    };

    /**
     * A user defiend type that holds the info of the core::menu of the current core::menu window consisting of core::table and core::cell user defiend type.
     * can be called from core::cell type.
     */
    class core::Menu: public Table {
        private:

        Pos2d<int> pos_selectedCell;
        
        
        void helper_drawSelectedCell(Pos2d<int> _selectPos);

        public:

        Menu(std::initializer_list<std::initializer_list<Cell>> _matrixInput);
        Menu(const Table& _tableToCopy);
        Menu(Table&& _tableToCopy);
        
        Menu();
        Menu(const Menu& _toCopy);
        Menu(Menu&& _toMove);
        ~Menu();
        Menu& operator=(const Menu& _toCopy);
        Menu& operator=(Menu&& _toMove);
        
        enum MenuDriver_returnType {
            success = 0,
            error   = 1,

        };

        MenuDriver_returnType Driver();
    
    };



};



#endif
