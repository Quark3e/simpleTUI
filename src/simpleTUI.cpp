
#include <simpleTUI.hpp>

#include <string>
#include <cstring>
#include <windows.h>



namespace simpleTUI {

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

    namespace KeyHandler {
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
                        /// if startTime is set to current time (meaning just pressed) or if the set startTime - currentTime is bigger than decay time

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

    };


    core::Cell::Cell(cell_types _cellType): cellType(_cellType) {

    }
    core::Cell::Cell(std::string _text, cell_types _cellType): cellType(_cellType), isDefined__text(true) {
        this->text = helper_parseStringForNewlines(_text);
    }
    core::Cell::Cell(std::string _text, type_cellFunc _func, cell_types _cellType): cellType(_cellType), function(_func), isDefined__text(true), isDefined__function(true) {
        this->text = helper_parseStringForNewlines(_text);
    }
    core::Cell::Cell(std::string _text, std::unique_ptr<Menu> _menu, cell_types _cellType): cellType(_cellType), menu(std::move(_menu)), isDefined__text(true), isDefined__menu(true) {
        this->text = helper_parseStringForNewlines(_text);
    }

    core::Cell::Cell(const Cell& _toCopy): cellType(_toCopy.cellType) {
        // NOTE!!! A lot of missing variables
        tablePtr = _toCopy.tablePtr;
        if(_toCopy.isDefined__pos) pos = _toCopy.pos;
        if(_toCopy.isDefined__text) text = _toCopy.text;
        if(_toCopy.isDefined__function) function = _toCopy.function;
        // if(_toCopy.isDefined__menu) menu = _toCopy.menu;
        // if(_toCopy.__isDefinedcellContent_null) cellContent_null = _toCopy.cellContent_null;
        // if(_toCopy.__isDefinedcellContent_text) cellContent_text = _toCopy.cellContent_text;
        // if(_toCopy.__isDefinedcellContent_function) cellContent_function = _toCopy.cellContent_function;

    }
    core::Cell::Cell(Cell&& _toMove): cellType(std::move(_toMove.cellType)), tablePtr(std::move(_toMove.tablePtr)) {
        
        if(_toMove.isDefined__pos) std::swap(pos, _toMove.pos);
        if(_toMove.isDefined__text) std::swap(text, _toMove.text);
        if(_toMove.isDefined__function) std::swap(function, _toMove.function);
        if(_toMove.isDefined__menu) std::swap(menu, _toMove.menu);
        // if(_toMove.__isDefinedcellContent_null) std::swap(cellContent_null, _toMove.cellContent_null);
        // if(_toMove.__isDefinedcellContent_text) std::swap(cellContent_text, _toMove.cellContent_text);
        // if(_toMove.__isDefinedcellContent_function) std::swap(cellContent_function,  _toMove.cellContent_function);

    }
    core::Cell::~Cell() {

    }
    core::Cell& core::Cell::operator=(const Cell& _toCopy) {
        cellType = _toCopy.cellType;
        tablePtr = _toCopy.tablePtr;
        if(_toCopy.isDefined__pos) pos = _toCopy.pos;
        if(_toCopy.isDefined__text) text = _toCopy.text;
        if(_toCopy.isDefined__function) function = _toCopy.function;
        // if(_toCopy.isDefined__menu) menu = _toCopy.menu;
        // if(_toCopy.__isDefinedcellContent_null) cellContent_null = _toCopy.cellContent_null;
        // if(_toCopy.__isDefinedcellContent_text) cellContent_text = _toCopy.cellContent_text;
        // if(_toCopy.__isDefinedcellContent_function) cellContent_function = _toCopy.cellContent_function;

        return *this;
    }
    core::Cell& core::Cell::operator=(Cell&& _toMove) {
        std::swap(cellType, _toMove.cellType);
        
        std::swap(tablePtr, _toMove.tablePtr);
        if(_toMove.isDefined__pos) std::swap(pos, _toMove.pos);
        if(_toMove.isDefined__text) std::swap(text, _toMove.text);
        if(_toMove.isDefined__function) std::swap(function, _toMove.function);
        if(_toMove.isDefined__menu) std::swap(menu, _toMove.menu);
        // if(_toMove.__isDefinedcellContent_null) std::swap(cellContent_null, _toMove.cellContent_null);
        // if(_toMove.__isDefinedcellContent_text) std::swap(cellContent_text, _toMove.cellContent_text);
        // if(_toMove.__isDefinedcellContent_function) std::swap(cellContent_function,  _toMove.cellContent_function);

        return *this;
    }

    void core::Cell::call() {
        switch (cellType) {
        case cell_types::function:
            if(!isDefined__function) { throw std::runtime_error("ERROR: simpleTUI::cell::call() : a call function has not been defined."); }
            else {
                if(!tablePtr) { throw std::runtime_error("ERROR: simpleTUI::cell::call() : tablePtr has not been defined."); }
                function(tablePtr);
            }
            break;
        case cell_types::menu:
            if(!isDefined__menu) { throw std::runtime_error("ERROR: simpleTUI::cell::call() : a call menu has not been defined."); }
            else {
                menu->Driver();
            }
            break;
        default:
            throw std::runtime_error("ERROR: simpleTUI::cell::call() : call() member called without the cell type being a callable type.");
            break;
        }
    }

    int core::Cell::set_tablePtr(Table* _tablePtr) {
        tablePtr = _tablePtr;
        isModified__tablePtr = true;
        return 0;
    }

    int core::Cell::set_pos(Pos2d<int> _pos) {
        pos = _pos;
        isDefined__pos = true;
        isModified__pos = true;
        return 0;
    }
    int core::Cell::set_text(std::string _text) {
        this->text = helper_parseStringForNewlines(_text);
        isDefined__text = true;
        isModified__text = true;
        return 0;
    }
    int core::Cell::set_function(type_cellFunc _func) {
        function = _func;
        isDefined__function = true;
        isModified__function = true;
        return 0;
    }
    int core::Cell::set_menu(std::unique_ptr<Menu> _menu) {
        menu = std::move(_menu);
        isDefined__menu = true;
        isModified__menu = true;
        return 0;
    }
    int core::Cell::setContent_null(cellTypeContent_null _newContent) {
        cellContent_null = _newContent;
        // __isDefinedcellContent_null = true;

        return 0;
    }
    int core::Cell::setContent_text(cellTypeContent_text _newContent) {
        cellContent_text = _newContent;
        // __isDefinedcellContent_text = true;

        return 0;
    }
    int core::Cell::setContent_function(cellTypeContent_function _newContent) {
        cellContent_function = _newContent;
        // __isDefinedcellContent_function = true;

        return 0;
    }
    int core::Cell::setContent_menu(cellTypeContent_menu _newContent) {
        cellContent_menu = _newContent;
        
        return 0;
    }
    void core::Cell::change_type(cell_types _newType) {
        cellType = _newType;
        isModified__cellType = true;
    }
    void core::Cell::change_type(cell_types _newType, std::string _text) {
        this->change_type(_newType);
        this->text = helper_parseStringForNewlines(_text);
        isModified__text = true;
    }
    void core::Cell::change_type(cell_types _newType, std::string _text, type_cellFunc _func) {
        this->change_type(_newType, _text);
        function = _func;
        isModified__function = true;
    }
    void core::Cell::change_type(cell_types _newType, type_cellFunc _func) {
        this->change_type(_newType);
        function = _func;
        isModified__function = true;
    }
    void core::Cell::change_type(cell_types _newType, std::string _text, std::unique_ptr<Menu> _menu) {
        this->change_type(_newType, _text);
        menu = std::move(_menu);
        isModified__menu = true;
    }
    void core::Cell::change_type(cell_types _newType, std::unique_ptr<Menu> _menu) {
        this->change_type(_newType);
        menu = std::move(_menu);
        isModified__menu = true;
    }

    core::Table* core::Cell::get_tablePtr() const {
        return tablePtr;
    }
    Pos2d<int> core::Cell::get_pos() const {
        if(!isDefined__pos) throw std::runtime_error("cell::get_pos() const : member called when pos has not been defined.");
        return pos;
    }
    std::string core::Cell::get_text() const {
        if(!isDefined__text) throw std::runtime_error("cell::get_text() const : member called when text has not been defined.");
        std::string fullString = "";
        for(auto str : this->text) fullString+=str;
        return fullString;
    }
    type_cellFunc core::Cell::get_function() const {
        if(!isDefined__function) throw std::runtime_error("cell::get_function() const : member called when function has not been defined.");
        return function;
    }
    // core::Menu core::Cell::get_menu() const {
    //     if(!isDefined__menu) throw std::runtime_error("cell::get_menu() const : member called when menu* has not been defined.");
    //     return menu;
    // }
    cellTypeContent_null core::Cell::get_cellContent_null() const {
        // if(!__isDefinedcellContent_null) throw std::runtime_error("cell::get_cellContent_null() const : member has been called when cellContent for null has not been defined.");
        return cellContent_null;
    }
    cellTypeContent_text core::Cell::get_cellContent_text() const {
        // if(!__isDefinedcellContent_text) throw std::runtime_error("cell::get_cellContent_text() const : member has been called when cellContent for text has not been defined.");
        return cellContent_text;
    }
    cellTypeContent_function core::Cell::get_cellContent_function() const {
        // if(!__isDefinedcellContent_function) throw std::runtime_error("cell::get_cellContent_function() const : member has been called when cellContent for function has not been defined.");
        return cellContent_function;
    }
    cellTypeContent_menu core::Cell::get_cellContent_menu() const {
        return cellContent_menu;
    }
    cell_types core::Cell::get_type() const {
        return cellType;
    }
    
    // core::Cell core::Cell::clone() {
    // }

    void core::Cell::cell_resetModificationFlag() {
        isModified__tablePtr = false;
        isModified__pos = false;
        isModified__text = false;
        isModified__function = false;

    }
    bool core::Cell::isModified() {
        return (isModified__tablePtr || isModified__pos || isModified__text || isModified__function);
    }
    bool core::Cell::isModified_tablePtr() {
        return isModified__tablePtr;
    }
    bool core::Cell::isModified_pos() {
        return isModified__pos;
    }
    bool core::Cell::isModified_text() {
        return isModified__text;
    }
    bool core::Cell::isModified_function() {
        return isModified__function;
    }
    bool core::Cell::isModified_menu() {
        return isModified__menu;
    }


    void core::Table::func_loadInitialiserCellMatrix(std::initializer_list<std::initializer_list<core::Cell>> _matrixInput) {

        // DEBUGPRINT("---------- func_loadInitialiserCellMatrix ----------");
        // DEBUGPRINT(std::string("_matrixInput dimensions: ")+std::string(Pos2d<size_t>{_matrixInput.begin()->size(), _matrixInput.size()}));

        Pos2d<int>  limMatrix_min(0, 0);
        Pos2d<int>  limMatrix_max(0, 0);
        Pos2d<bool> limDefined_min(false, false);
        Pos2d<bool> limDefined_max(false, false);
        Pos2d<int>  lineCountedDim(0, _matrixInput.size());
        /// Define min-max limits of the tableOfCells
        for(auto itr_row=_matrixInput.begin(); itr_row!=_matrixInput.end(); ++itr_row) {
            for(auto itr_cell=itr_row->begin(); itr_cell!=itr_row->end(); ++itr_cell) {
                if(itr_cell->isDefined__pos) {
                    if(itr_cell->pos.x > limMatrix_max.x || !limDefined_max.x) {
                        limMatrix_max.x = itr_cell->pos.x;
                        limDefined_max.x = true;
                    }
                    else if(itr_cell->pos.x < limMatrix_min.x || !limDefined_min.x) {
                        limMatrix_min.x = itr_cell->pos.x;
                        limDefined_min.x = true;
                    }
                    if(itr_cell->pos.y > limMatrix_max.y || !limDefined_max.y) {
                        limMatrix_max.y = itr_cell->pos.y;
                        limDefined_max.y = true;
                    }
                    else if(itr_cell->pos.y < limMatrix_min.y || !limDefined_min.y) {
                        limMatrix_min.y = itr_cell->pos.y;
                        limDefined_min.y = true;
                    }

                }
                else {

                }
            }
            if(itr_row->size() > lineCountedDim.x) lineCountedDim.x = itr_row->size();
        }
        if(limDefined_min.x) assert(limDefined_max.x && " how the fuck is minimum defined but not maximum for x");
        if(limDefined_min.y) assert(limDefined_max.y && " how the fuck is minimum defined but not maximum for y");
        if(lineCountedDim.x > limMatrix_max.x-limMatrix_min.x) { //the defined limits are smaller than the existing number of cells for a row
            limMatrix_max.x = lineCountedDim.x;
        }
        if(lineCountedDim.y > limMatrix_max.y-limMatrix_min.y) {
            limMatrix_max.y = lineCountedDim.y;
        }

        tableOfCells = std::vector<std::vector<core::Cell>>(
            limMatrix_max.y-limMatrix_min.y, std::vector<core::Cell>(
                limMatrix_max.x-limMatrix_min.x, {cell_types::null}
            )
        );
        
        Pos2d<int> lineCountedPos = limMatrix_min;
        for(auto itr_row=_matrixInput.begin(); itr_row!=_matrixInput.end(); ++itr_row) {
            lineCountedPos.x = limMatrix_min.x;
            for(auto itr_cell=itr_row->begin(); itr_cell!=itr_row->end(); ++itr_cell) {

                if(itr_cell->isDefined__pos) tableOfCells[itr_cell->pos.y][itr_cell->pos.x] = *itr_cell;
                else {
                    tableOfCells[lineCountedPos.y][lineCountedPos.x] = *itr_cell;
                }

                // if(itr_cell->cellType!=cell_types::null) {
                //     if(itr_cell->isDefined__pos) tableOfCells[itr_cell->pos.y][itr_cell->pos.x] = *itr_cell;
                //     else { /// If the currently holding cell doesn't have a defined position
                //         if(tableOfCells[lineCountedPos.y][lineCountedPos.x].cellType==cell_types::null) {
                //             tableOfCells[itr_cell->pos.y][itr_cell->pos.x] = *itr_cell;
                //         }
                //     }
                // }
                lineCountedPos.x++;
            }
            lineCountedPos.y++;
        }


        // DEBUGPRINT(" ");
        // system("pause");
    }

    void core::Table::update__maxSize_axisVectors() {
        
        /// If either of the axis in the cell matrix is empty, exit the function because there is nothing to update.
        if(tableOfCells.size()==0) {
            throw std::runtime_error("ERROR: void core::Table::update__maxSize_axisVectors() : tableOfCells.size()==0");
        }
        if(tableOfCells.at(0).size()==0) {
            throw std::runtime_error("ERROR: void core::Table::update__maxSize_axisVectors() : tableOfCells.at(0).size()==0");
        }

        /// xy variable that holds the character-count axis dimensions of the table, i.e. how many characters wide/high
        Pos2d<int> tableDimensions(dimSize_table);
        if(dimSize_table.x==-1 || dimSize_table.y==-1) {
            Pos2d<int> dimSize_terminal{0, 0};
            
            /// NOTE: currently windows only: change/rewrite if implementing cross-platform
            auto consoleDims = helper_getConsoleDimensions();
            if(dimSize_table.x==-1) tableDimensions.x = consoleDims.x;
            if(dimSize_table.y==-1) tableDimensions.y = consoleDims.y;
        }
        
        /// Local containers holding the boggest size for that axis column/row
        std::vector<size_t> maxSize_columns(tableOfCells.at(0).size(), 0);
        std::vector<size_t> maxSize_rows(tableOfCells.size(), 0);
        
        /// If any of the axis has a scalMethod that is cellWidth, go through every cell and get the maximum cell's width/height for that column/row.
        if(scalMethod_columns==style_axisCellScalingMethod::cellWidth || scalMethod_rows==style_axisCellScalingMethod::cellWidth) {
            /// iterate through every cell that isn't null or doesn't follow delimiter separations to get the biggest cell's text dimensions
            for(size_t _y=0; _y<tableOfCells.size(); _y++) { // iterate over every row in table
                for(size_t _x=0; _x<tableOfCells.at(0).size(); _x++) { // iterate over every cell in row
                    
                    auto cell = tableOfCells.at(_y).at(_x);
                    switch (cell.get_type()) {
                    case cell_types::null:
                        continue;
                        break;
                    case cell_types::text:
                        if(!cell.get_cellContent_text().rule_followDelimiter) continue;
                        break;
                    case cell_types::function:
                        break;
                    default:
                        assert(false && "Something is wrong where an undefined enum value was entered [1].");
                        break;
                    }
                    
                    Pos2d<size_t> textBoxMaxDim(0, 0);
                    /// Get the max width and height of the texts in the text cell
                    size_t textWidth = 0;
                    for(size_t i=0; i<cell.get_text().size(); i++) {
                        if(cell.get_text().at(i)=='\n') {
                            textBoxMaxDim.y++;
                            if(textWidth > textBoxMaxDim.x) textBoxMaxDim.x = textWidth;
                            textWidth = 0;
                        }
                        else {
                            textWidth++;
                        }
                    }

    
                    if(textBoxMaxDim.x>maxSize_columns.at(_x))  maxSize_columns.at(_x)   = textBoxMaxDim.x;
                    if(textBoxMaxDim.y>maxSize_rows.at(_y))     maxSize_rows.at(_y)      = textBoxMaxDim.y;
    
                }
            }
        }
        /**
         * If cellScaling method for columns/x-axis is fitMenuAxis_even then adjust the sizes for the cellSize values in maxSize_columnWidths to an evenly divided cell width,
         *  that takes into account borderSymb size.
         * 
         */
        if(scalMethod_columns==style_axisCellScalingMethod::fitMenuAxis_even) {
            /**
             * Cell width solved from evenly dividing the table width
             * 
             * size_of_cell = total_size_of_all_cells( width_of_table - symbol_size_of_borderColumn*2 - (number of column delimiters)*size_of_column_delimiter ) / number_of_cells
             */
            int avgCellWidth = (tableDimensions.x-symb_border_column.size()*2-(maxSize_columns.size()-1)*symb_delimiter_columns.size())/maxSize_columns.size();
            for(auto& _el : maxSize_columns) {
                _el = (avgCellWidth < 0? 0 : static_cast<size_t>(avgCellWidth));
            }
        }
        /**
         * If cellScaling method for rows/y-axis is fitMenuAxis_even then adjust the sizes for the cellSize values in maxSize_rowHeights to an evenly divided cell height,
         *  that takes into account borderSymb size.
         * 
         */
        if(scalMethod_rows==style_axisCellScalingMethod::fitMenuAxis_even) {
            int avgCellHeight = (tableDimensions.y-symb_border_row.size()*2-(maxSize_rows.size()-1)*symb_delimiter_rows.size())/maxSize_rows.size();
            for(auto& _el : maxSize_rows) {
                _el = (avgCellHeight < 0? 0 : static_cast<size_t>(avgCellHeight));
            }
        }
        
        this->maxSize_columnWidths  = maxSize_columns;
        this->maxSize_rowHeights    = maxSize_rows;
    }

    Pos2d<size_t> core::Table::helper_PSVmatrixLocator(Pos2d<size_t> _cellPos) {
        if(tableOfCells.size()==0) throw std::runtime_error("Pos2d<size_t> core::Table::helper_PSVmatrixLocator(Pos2d<size_t>) : tableOfCells.size()==0");
        if(tableOfCells.at(0).size()==0) throw std::runtime_error("Pos2d<size_t> core::Table::helper_PSVmatrixLocator(Pos2d<size_t>) : tableOfCells.at(0).size()==0");
    
        /**
         * Using the currently defined maxSize_{axis label} size vectors, figure out the 2d matrix coordinate to the first char in the PRV-matrix (PrintableStringVectorMatrix)
         * 
         * 
         */

        Pos2d<size_t> posInMatrix(0, 0);
        posInMatrix.y+=symb_border_row.size();
        posInMatrix.y+=symb_delimiter_rows.size()*_cellPos.y;
        for(size_t _i=0; _i<_cellPos.y; _i++) {
            if(maxSize_rowHeights.at(_i)!=0) posInMatrix.y+=maxSize_rowHeights.at(_i);
        }
        
        posInMatrix.x+=symb_border_column.size();
        posInMatrix.x+=symb_delimiter_columns.size()*_cellPos.x;
        for(size_t _i=0; _i<_cellPos.x; _i++) {
            if(maxSize_columnWidths.at(_i)!=0) posInMatrix.x+=maxSize_columnWidths.at(_i);
        }

        return posInMatrix;
    }
    void core::Table::helper_updateTablePtrInCells() {
        /// Iterate through every cell in the table matrix and set their dedicated Table pointer to this tables's address
        for(size_t _y=0; _y<tableOfCells.size(); _y++) {
            for(size_t _x=0; _x<tableOfCells.at(_y).size(); _x++) {
                Cell& cell = tableOfCells.at(_y).at(_x);
                cell.tablePtr = this;
            }
        }
    }
    std::vector<std::string> core::Table::help__separateLines(std::string _toSep, std::string _delim) {
        if(_toSep.size()==0) throw std::runtime_error("table::help__separateLines(std::string, std::string) : _toSep value cannot be empty.");
        if(_delim.size()==0) throw std::runtime_error("table::help__separateLines(std::string, std::string) : _delim value cannot be empty.");
        std::vector<std::string> returVec;
        size_t ignPos = 0;
        for(size_t _c=0; _c<_toSep.size()-_delim.size()+1; _c++) {
            if(_toSep.substr(_c, _delim.size())==_delim) {
                returVec.push_back(_toSep.substr(ignPos, _c-ignPos));
                _c+=_delim.size();
                ignPos = _c;
            }
        }
        if(ignPos!=_toSep.size()) returVec.push_back(_toSep.substr(ignPos, std::string::npos));

        return returVec;
    }
    void core::Table::update__PSVmatrix() {
        if(tableOfCells.size()==0) return;
        if(tableOfCells.at(0).size()==0) return;
        
        DEBUGPRINT("test[0.0]");

        DEBUGPRINT(std::string("tableOfCells dim's: ")+std::string(Pos2d<size_t>{tableOfCells.at(0).size(), tableOfCells.size()}));

        DEBUGPRINT("test[0.1]");
        DEBUGPAUSE(500);
        this->update__maxSize_axisVectors();

        DEBUGPRINT("test[0.2]");
        
        /**
         * @brief Contains char size dimensions of the terminal
         * 
         */
        Pos2d<int> tableDimensions(dimSize_table);
        if(dimSize_table.x==-1 || dimSize_table.y==-1) {
            Pos2d<int> dimSize_terminal{0, 0};
            
            /// NOTE: currently windows only: change/rewrite if implementing cross-platform
            auto consoleDims = helper_getConsoleDimensions();
            if(dimSize_table.x==-1) tableDimensions.x = consoleDims.x;
            if(dimSize_table.y==-1) tableDimensions.y = consoleDims.y;
        }
        
        DEBUGPRINT(fmtToStr("test[0.3]",11,0,"left")+": "+std::string(CURRENT_CONSOLE_DIMENSIONS));
        DEBUGPAUSE(500);

        /// NOTE! To-do: Add a modification check to each cell so all of this can be optimised and potentially avoided
        /// as in, change the current method for writing to the "final string" so that instead of creating a copy, edit the existing "string" container.

        /// Resizing of PSV matrix (PrintableStringVectorMatrix)_
        ///     check new size of tableDimensions (which gets updated either by member functions or resizing of console/terminal window) and update the PSVmatrix accordingly
        Pos2d<int> diffCount(
            static_cast<int>(tableDimensions.x)-static_cast<int>(current_PSVmatrix_size.x),
            static_cast<int>(tableDimensions.y)-static_cast<int>(current_PSVmatrix_size.y)
        );
        DEBUGPRINT(std::string("test[0.3.1]: diffCount")+std::string(diffCount));
        DEBUGPRINT(std::string("           : old PSVm size:")+std::string(current_PSVmatrix_size));
        DEBUGPAUSE(1000);
        if(diffCount.y!=0) { /// update row count
            if(diffCount.y<0) { /// new tableDimensions.y is smaller than current size
                auto itr_erasePos = PrintableStringVectorMatrix.begin();
                std::advance(itr_erasePos, static_cast<int>(PrintableStringVectorMatrix.size())+diffCount.y);
                PrintableStringVectorMatrix.erase(itr_erasePos, PrintableStringVectorMatrix.end());
            }
            else { /// new tableDimensions.y is bigger than current size
                PrintableStringVectorMatrix.insert(PrintableStringVectorMatrix.end(), diffCount.y, std::string(current_PSVmatrix_size.x, ' '));
            }
        }
        DEBUGPRINT("test[0.3.2]");
        DEBUGPAUSE(100);
        if(diffCount.x!=0) { /// update column count
            for(auto itr_rowVec=PrintableStringVectorMatrix.begin(); itr_rowVec!=PrintableStringVectorMatrix.end(); ++itr_rowVec) {
                if(diffCount.x<0) { /// new tableDimensions.x is smaller than current size
                    auto itr_erasePos = itr_rowVec->begin();
                    std::advance(itr_erasePos, static_cast<int>(itr_rowVec->size())+diffCount.x);
                    itr_rowVec->erase(itr_erasePos, itr_rowVec->end());
                }
                else { /// new tableDimensions.x is bigger than current size
                    itr_rowVec->insert(itr_rowVec->end(), diffCount.x, ' ');
                }

            }
        }
        DEBUGPAUSE(1000);

        current_PSVmatrix_size.x = (PrintableStringVectorMatrix.size()==0? 0 : (PrintableStringVectorMatrix.at(0).size()));
        current_PSVmatrix_size.y = PrintableStringVectorMatrix.size();
        
        DEBUGPRINT(fmtToStr("test[0.4]",11,0,"left")+std::string(": new PSVm size:")+std::string(current_PSVmatrix_size));
        DEBUGPRINT(std::string("maxSize_columnWidths")+fmtCont(maxSize_columnWidths, 2, 0));
        DEBUGPRINT(std::string("maxSize_rowHeights")+fmtCont(maxSize_rowHeights, 2, 0));

        for(size_t _y=0; _y<tableOfCells.size(); _y++) {
            for(size_t _x=0; _x<tableOfCells.at(_y).size(); _x++) {
                auto& _cellRef = tableOfCells.at(_y).at(_x);
                if(_cellRef.isDefined__text) {
                    ANSIec::Print(50+5*_x, 50+5*_y, std::string("\"")+fmtToStr(_cellRef.text.front(), 5, 0, "left")+"\"");
                }
            }
        }
        DEBUGPAUSE(2000);



        /// ----------------- REWRITE THE CONTENTS BELOW THIS TO BE UPDATES ---------------------------
        
        /// Local cursorPos variable for editing the characters where .x is a std::strings character(column), .y is a std::vector's element (row)
        /// NOTE: Start at 0. Need to index by 1 when the coordinates are used in ansii positions since the console print coordinates start at (1,1)
        Pos2d<int> cursorPos_edit(symb_border_column.size(), 1);

        for(size_t _cellMat_y=0; _cellMat_y<tableOfCells.size(); _cellMat_y++) { /// iterate over each row of cells

            for(size_t _row_y=0; _row_y<maxSize_rowHeights.at(_cellMat_y); _row_y++) { /// iterate over each row/character-height of each cell-row

                cursorPos_edit.x=1; /// set start indexing pos at new row's first character after border column
                for(size_t _cellMat_x=0; _cellMat_x<tableOfCells.at(0).size(); _cellMat_x++) { /// iterate over each column of cells
                    core::Cell& cellRef = tableOfCells.at(_cellMat_y).at(_cellMat_x);

                    if(cellRef.isModified__text && cellRef.cellContent_text.rule_followDelimiter) {
                        if(cellRef.text.size()>_row_y) {
                            std::string textLine = cellRef.text.at(_row_y);
                            if(textLine.size()<=maxSize_columnWidths.at(_cellMat_x)) {
                                textLine+=std::string(maxSize_columnWidths.at(_cellMat_x)-textLine.size(), ' ');
                            }
                            else {
                                textLine.erase(maxSize_columnWidths.at(_cellMat_x));
                            }
                            assert(textLine.size()==maxSize_columnWidths.at(_cellMat_x));
                            

                            memcpy(&PrintableStringVectorMatrix.at(cursorPos_edit.y+_row_y).at(cursorPos_edit.x), &textLine.at(0), textLine.size());
                            // PrintableStringVectorMatrix.at(cursorPos_edit.y).replace(cursorPos_edit.x, textLine.size(), &textLine.at(0));
                            // std::string _tempStr(maxSize_columnWidths.at(_cellMat_x), '*');
                            // memcpy(&PrintableStringVectorMatrix.at(cursorPos_edit.y).at(cursorPos_edit.x), &_tempStr.at(0), _tempStr.size());
                        }
                        else {
                            std::string tempStr_emptyCellLine(maxSize_columnWidths.at(_cellMat_x), ' ');
                            // memcpy(&PrintableStringVectorMatrix.at(cursorPos_edit.y).at(cursorPos_edit.x), &tempStr_emptyCellLine.at(0), tempStr_emptyCellLine.size());
                            // PrintableStringVectorMatrix.at(cursorPos_edit.y).replace(cursorPos_edit.x, tempStr_emptyCellLine.size(), &tempStr_emptyCellLine.at(0));
                        }
                    }
                    
                    cursorPos_edit.x+=maxSize_columnWidths.at(_cellMat_x);
                    cursorPos_edit.x+=symb_delimiter_columns.size(); /// skip over PSV matrix's column delimiter
                }

            }
            cursorPos_edit.y+=maxSize_rowHeights.at(_cellMat_y);
            cursorPos_edit.y+=symb_delimiter_rows.size(); /// skip over PSV matrix's row delimiter
        }

        DEBUGPRINT(fmtToStr("test[0.5]",11,0,"left")+":");
        DEBUGPAUSE(1000);

        /// Draw border/background symbols and related:
        ////     because the editing of cell text content displayed on the PSVmatrix is handled by the previous section, the border drawing can be toggled to only be printed during resizing of dim

        Pos2d<size_t> _tempPos_xy{symb_border_column.size(), 0};
        for(size_t _cells_y=0; _cells_y<tableOfCells.size(); _cells_y++) {
            for(size_t _temp_y=0; _temp_y<maxSize_rowHeights.at(_cells_y); _temp_y++) {
                _tempPos_xy.x = 0;
                for(size_t _cellX_idx=0; _cellX_idx<maxSize_columnWidths.size()-1; _cellX_idx++) {
                    _tempPos_xy.x+=maxSize_columnWidths.at(_cellX_idx)+1;
                    memcpy(&PrintableStringVectorMatrix.at(_tempPos_xy.y+_temp_y+1).at(_tempPos_xy.x), &symb_delimiter_columns.at(0), symb_delimiter_columns.size());
                }
            }

            _tempPos_xy.y+=maxSize_rowHeights.at(_cells_y);
            if(_cells_y<tableOfCells.size()-1) {
                for(size_t _colFilled=symb_border_column.size(); _colFilled<tableDimensions.x-symb_delimiter_rows.size(); _colFilled+=symb_delimiter_rows.size()) {
                    memcpy(&PrintableStringVectorMatrix.at(_tempPos_xy.y+1).at(_colFilled), &symb_delimiter_rows.at(0), symb_delimiter_rows.size());
                }
            }
            _tempPos_xy+=1;
        }

        /// Draw ceiling/floor border rows
        size_t rowColumnsFilled = 0;
        for(; rowColumnsFilled<tableDimensions.x-symb_border_row.size(); rowColumnsFilled+=this->symb_border_row.size()) {
            memcpy(&PrintableStringVectorMatrix.at(0).at(rowColumnsFilled),   &symb_border_row.at(0), symb_border_row.size());    /// ceiling border
            memcpy(&PrintableStringVectorMatrix.back().at(rowColumnsFilled),  &symb_border_row.at(0), symb_border_row.size());    /// floor border
        }
        memcpy(&PrintableStringVectorMatrix.at(0).at(tableDimensions.x-symb_border_row.size()), &symb_border_row.at(0), (tableDimensions.x-rowColumnsFilled));
        memcpy(&PrintableStringVectorMatrix.back().at(tableDimensions.x-symb_border_row.size()),&symb_border_row.at(0), tableDimensions.x-rowColumnsFilled);
        
        /// Draw left/right border columns
        for(size_t _y=1; _y<tableDimensions.y-1; _y++) {
            memcpy(&PrintableStringVectorMatrix.at(_y).at(0), &symb_border_column.at(0), symb_border_column.size());
            memcpy(&PrintableStringVectorMatrix.at(_y).at(tableDimensions.x-symb_border_column.size()), &symb_border_column.at(0), symb_border_column.size());
        }

        /// Draw corners
        memcpy(&PrintableStringVectorMatrix.at(0).at(0),    &symb_border_corner.at(0), symb_border_corner.size());
        memcpy(&PrintableStringVectorMatrix.at(0).back(),   &symb_border_corner.at(0), symb_border_corner.size());
        memcpy(&PrintableStringVectorMatrix.back().at(0),   &symb_border_corner.at(0), symb_border_corner.size());
        memcpy(&PrintableStringVectorMatrix.back().back(),  &symb_border_corner.at(0), symb_border_corner.size());

        

        
        /// Draw text that ignores/goes-over border/delimiter symbols

        cursorPos_edit = Pos2d<int>(symb_border_column.size(), symb_border_row.size());
        for(size_t _cellMat_y=0; _cellMat_y<tableOfCells.size(); _cellMat_y++) { /// iterate over each row of cells

            for(size_t _row_y=0; _row_y<maxSize_rowHeights.at(_cellMat_y); _row_y++) { /// iterate over each row/character-height of each cell-row

                cursorPos_edit.x=1; /// set start indexing pos at new row's first character after border column
                for(size_t _cellMat_x=0; _cellMat_x<tableOfCells.at(0).size(); _cellMat_x++) { /// iterate over each column of cells
                    core::Cell& cellRef = tableOfCells.at(_cellMat_y).at(_cellMat_x);

                    if(cellRef.isModified__text && !cellRef.cellContent_text.rule_followDelimiter) {
                        if(cellRef.text.size()>_row_y) { /// !NOTE: Will need further revision to account for strings shorter than previous iterations
                            std::string textLine = cellRef.text.at(_row_y);
                            if(cursorPos_edit.x+textLine.size()>=tableDimensions.x) textLine.erase(tableDimensions.x-cursorPos_edit.x);
                            
                            memcpy(&PrintableStringVectorMatrix.at(cursorPos_edit.y).at(cursorPos_edit.x), &textLine.at(0), maxSize_columnWidths.at(_cellMat_x));
                        }
                        else {
                            std::string tempStr_emptyCellLine(maxSize_columnWidths.at(_cellMat_x), ' ');
                            memcpy(&PrintableStringVectorMatrix.at(cursorPos_edit.y).at(cursorPos_edit.x), &tempStr_emptyCellLine.at(0), maxSize_columnWidths.at(_cellMat_x));
                        }
                    }
                    
                    cursorPos_edit.x+=maxSize_columnWidths.at(_cellMat_x);
                    cursorPos_edit.x+=symb_delimiter_columns.size(); /// skip over PSV matrix's column delimiter
                }

            }
            cursorPos_edit.y+=maxSize_rowHeights.at(_cellMat_y);
            cursorPos_edit.y+=symb_delimiter_rows.size(); /// skip over PSV matrix's row delimiter
        }

        
        // DEBUGPRINT(fmtToStr("test[0.7]",11,0,"left")+":");
        // DEBUGPAUSE(1000);

    }

    core::Table::Table(std::initializer_list<std::initializer_list<Cell>> _matrixInput) {
        this->func_loadInitialiserCellMatrix(_matrixInput);
    
    }

    core::Table::Table() {

    }
    core::Table::Table(const Table& _toCopy): 
        tableOfCells(_toCopy.tableOfCells), dimSize_table(_toCopy.dimSize_table), maxSize_columnWidths(_toCopy.maxSize_columnWidths), maxSize_rowHeights(_toCopy.maxSize_rowHeights), string_table(_toCopy.string_table),
        scalMethod_columns(_toCopy.scalMethod_columns), scalMethod_rows(_toCopy.scalMethod_rows), rowSeparator(_toCopy.rowSeparator),
        symb_delimiter_columns(_toCopy.symb_delimiter_columns), symb_delimiter_rows(_toCopy.symb_delimiter_rows), symb_border_column(_toCopy.symb_border_column), symb_border_row(_toCopy.symb_border_row)
    {
        this->helper_updateTablePtrInCells();
        
        
        
    }
    core::Table::Table(Table&& _toMove):
        tableOfCells(std::move(_toMove.tableOfCells)), dimSize_table(std::move(_toMove.dimSize_table)), maxSize_columnWidths(std::move(_toMove.maxSize_columnWidths)), maxSize_rowHeights(std::move(_toMove.maxSize_rowHeights)), string_table(std::move(_toMove.string_table)),
        scalMethod_columns(std::move(_toMove.scalMethod_columns)), scalMethod_rows(std::move(_toMove.scalMethod_rows)), rowSeparator(std::move(_toMove.rowSeparator)),
        symb_delimiter_columns(std::move(_toMove.symb_delimiter_columns)), symb_delimiter_rows(std::move(_toMove.symb_delimiter_rows)), symb_border_column(std::move(_toMove.symb_border_column)), symb_border_row(std::move(_toMove.symb_border_row))
    {
        this->helper_updateTablePtrInCells();
        
    }
    core::Table::~Table() {

    }
    core::Table& core::Table::operator=(const Table& _toCopy) {
        tableOfCells  = _toCopy.tableOfCells;
        dimSize_table = _toCopy.dimSize_table;
        maxSize_columnWidths = _toCopy.maxSize_columnWidths; 
        maxSize_rowHeights   = _toCopy.maxSize_rowHeights;
        string_table  = _toCopy.string_table;
        scalMethod_columns    = _toCopy.scalMethod_columns;
        scalMethod_rows   = _toCopy.scalMethod_rows;
        symb_delimiter_columns = _toCopy.symb_delimiter_columns;
        symb_delimiter_rows    = _toCopy.symb_delimiter_rows;
        symb_border_column = _toCopy.symb_border_column;
        symb_border_row    = _toCopy.symb_border_row;
        rowSeparator  = _toCopy.rowSeparator;
        
        return *this;
    }
    core::Table& core::Table::operator=(Table&& _toMove) {
        std::swap(tableOfCells, _toMove.tableOfCells);
        std::swap(tableOfCells, _toMove.tableOfCells);
        std::swap(dimSize_table, _toMove.dimSize_table);
        std::swap(maxSize_columnWidths, _toMove.maxSize_columnWidths); 
        std::swap(maxSize_rowHeights, _toMove.maxSize_rowHeights);
        std::swap(string_table, _toMove.string_table);
        std::swap(scalMethod_columns, _toMove.scalMethod_columns);
        std::swap(scalMethod_rows, _toMove.scalMethod_rows);
        std::swap(symb_delimiter_columns, _toMove.symb_delimiter_columns);
        std::swap(symb_delimiter_rows, _toMove.symb_delimiter_rows);
        std::swap(symb_border_column, _toMove.symb_border_column);
        std::swap(symb_border_row,  _toMove.symb_border_row);
        std::swap(rowSeparator,  _toMove.rowSeparator);
        
        return *this;
    }
    core::Table& core::Table::operator=(std::initializer_list<std::initializer_list<core::Cell>> _matrixInput) {
        this->func_loadInitialiserCellMatrix(_matrixInput);

        this->helper_updateTablePtrInCells();
        
        return *this;
    }
    
    std::vector<core::Cell>& core::Table::operator[](size_t _i) {
        return tableOfCells[_i];
    }
    std::vector<core::Cell>  core::Table::Table::operator[](size_t _i) const {
        return tableOfCells[_i];
    }
    std::vector<core::Cell>& core::Table::at(size_t _i) {
        if(tableOfCells.size()==0)        throw std::runtime_error("at(size_t) : table is empty of cells.");
        if(_i>=tableOfCells.size())       throw std::runtime_error("at(size_t) : _i argument is bigger than existing rows in table.");
        

        return tableOfCells.at(_i);
    }
    std::vector<core::Cell>  core::Table::at(size_t _i) const {
        if(tableOfCells.size()==0)        throw std::runtime_error("at(size_t) const : table is empty of cells.");
        if(_i>=tableOfCells.size())       throw std::runtime_error("at(size_t) const : _i argument is bigger than existing rows in table.");
        

        return tableOfCells.at(_i);
    }
    // table_row  at(size_t _i) const;

    // core::Cell& core::Table::get_cell(size_t _x, size_t _y) {
    //     /// NOTE!: Need to be changed somewhat or removed since this'll cause a runtime stackoverflow
    //     return tableOfCells.at(_x).at(_y);
    // }
    // core::Cell core::Table::get_cell(size_t _x, size_t _y) const {
    //     /// NOTE!: Need to be changed somewhat or removed since this'll cause a runtime stackoverflow
    //     return get_cell(_x, _y);
    // }
    // core::Cell& core::Table::get_cell(Pos2d<int> _pos) {
    //     return get_cell(_pos.x, _pos.y);
    // }
    // core::Cell core::Table::get_cell(Pos2d<int> _pos) const {
    //     return get_cell(_pos);
    // }

    size_t core::Table::rows() {
        return tableOfCells.size();
    }
    size_t core::Table::columns() {
        return (tableOfCells.size()>0? tableOfCells.at(0).size() : 0);
    }
    size_t core::Table::size() {
        return tableOfCells.size() * (tableOfCells.size()>0? tableOfCells.at(0).size() : 0);
    }
    std::vector<core::Cell> core::Table::continuous(bool _includeNullCells) {
        std::vector<core::Cell> returVec;
        for(auto row : tableOfCells) {
            for(auto cell : row) {
                if(!_includeNullCells && cell.cellType==cell_types::null) continue;
                returVec.push_back(cell);
            }
        }
        
        return returVec;
    }


    // int table::addCell(size_t _x, size_t _y) {
    // }
    // int table::addCell(Pos2d<int> _pos) {
    // }
    // int table::moveCell(size_t _current_x, size_t _current_y, size_t _new_x, size_t _new_y) {
    // }
    // int table::moveCell(Pos2d<int> _current_pos, Pos2d<int> _new_pos) {
    // }
    // int table::eraseCell(size_t _x, size_t _y) {
    // }
    // int table::eraseCell(Pos2d<int> _pos) {
    // }

    int core::Table::call(size_t _x, size_t _y) {
        if(tableOfCells.size()==0)        throw std::runtime_error("call(size_t, size_t) : table is empty of cells.");
        if(_y>=tableOfCells.size())       throw std::runtime_error("call(size_t, size_t) : _y argument is bigger than existing rows in table.");
        if(_x>=tableOfCells.at(0).size()) throw std::runtime_error("call(size_t, size_t) : _x argument is bigger than existing columns in table.");
        if(tableOfCells.at(_y).at(_x).cellType!=cell_types::function) throw std::runtime_error(
            std::string("call(size_t, size_t) : called cell at row:")+std::to_string(_y)+" column:"+std::to_string(_x)+" is not a function cell but defined as a "+(tableOfCells.at(_y).at(_x).cellType==cell_types::null? "null" : "text")+" type cell."
        );
        if(!tableOfCells.at(_y).at(_x).isDefined__function) throw std::runtime_error(std::string("call(size_t, size_t) : cell at [")+std::to_string(_y)+"]["+std::to_string(_x)+"] doesn't have a defined function to call.");

        tableOfCells.at(_y).at(_x).call();

        return 0;
    }
    int core::Table::call(Pos2d<int> _pos) {
        if(tableOfCells.size()==0)        throw std::runtime_error("call(Pos2d<int>) : table is empty of cells.");
        if(_pos.y>=tableOfCells.size())       throw std::runtime_error("call(Pos2d<int>) : _y argument is bigger than existing rows in table.");
        if(_pos.x>=tableOfCells.at(0).size()) throw std::runtime_error("call(Pos2d<int>) : _x argument is bigger than existing columns in table.");
        if(tableOfCells.at(_pos.y).at(_pos.x).cellType!=cell_types::function) throw std::runtime_error(
            std::string("call(Pos2d<int>) : called cell at row:")+std::to_string(_pos.y)+" column:"+std::to_string(_pos.x)+" is not a function cell but defined as a "+(tableOfCells.at(_pos.y).at(_pos.x).cellType==cell_types::null? "null" : "text")+" type cell."
        );
        if(!tableOfCells.at(_pos.y).at(_pos.x).isDefined__function) throw std::runtime_error(std::string("call(Pos2d<int>) : cell at [")+std::to_string(_pos.y)+"]["+std::to_string(_pos.x)+"] doesn't have a defined function to call.");

        tableOfCells.at(_pos.y).at(_pos.x).call();

        return 0;
    }

    
    core::Menu::Menu(std::initializer_list<std::initializer_list<core::Cell>> _matrixInput) {
        this->func_loadInitialiserCellMatrix(_matrixInput);

        
    }
    core::Menu::Menu(const Table& _tableToCopy) {

    }
    core::Menu::Menu(Table&& _tableToMove) {

    }
    
    core::Menu::Menu() {
        
    }
    core::Menu::Menu(const Menu& _toCopy) {

    }
    core::Menu::Menu(Menu&& _toMove) {
        
    }
    core::Menu::~Menu() {
        
    }

    core::Menu& core::Menu::operator=(const Menu& _toCopy) {

        return *this;
    }
    core::Menu& core::Menu::operator=(Menu&& _toMove) {

        return *this;
    }

    core::Menu::MenuDriver_returnType core::Menu::Driver() {
        /**
         * Deals only with the TUI aspects as this namespace will be moved into a separate dedicated external library later on.
         * 
         * Will just keep track of cursorpos, move acounts and call functions based on key inputs. Will prob handle with return code's e.t.c and so
         * I will need to figure out how to pass member functions as core::Cell objects' functions'.
         * 
         * Will prob include a frame-rate manager to set the framerate to a locked amount.
         * 
         */

        /// initialise variables

        ANSIec::clearScreen();

        // if(PrintableStringVectorMatrix.size()==0) {
        //     ANSIec::Print(0, 0, "ERROR: PrintableStringVectorMatrix.size()==0", true, ANSIec::PrintAxisMethod::absolute, ANSIec::PrintAxisMethod::relative);
        //     return MenuDriver_returnType::error;
        // }
        // if(PrintableStringVectorMatrix.at(0).size()==0) {
        //     ANSIec::Print(0, 0, "ERROR: PrintableStringVectorMatrix.at(0).size()==0", true, ANSIec::PrintAxisMethod::absolute, ANSIec::PrintAxisMethod::relative);
        //     return MenuDriver_returnType::error;
        // }

        // DEBUGPRINT("test[0]");
        
        this->update__PSVmatrix();

        // DEBUGPRINT(std::string("test[1] : PrintableStringVectorMatrix dim: ")+std::string(Pos2d<size_t>{PrintableStringVectorMatrix.at(0).size(), PrintableStringVectorMatrix.size()}));
        // DEBUGPAUSE(1000);

        KeyHandler::keyPressHandler keyObj;
        // keyObj.updateKeys();

        Pos2d<size_t> TUI_cursorPos;
        

        const float limit_maxFPS = 60;
        std::chrono::steady_clock::time_point timeP_1 = std::chrono::steady_clock::now(), timeP_2 = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> intervalDuration_ms = std::chrono::duration<double, std::milli>(0.001);
        std::chrono::duration<double, std::milli> intervalDuration_ms_refrDur(1000.0/60);
        double measured_fps = 0;


        for(size_t _y=0; _y<PrintableStringVectorMatrix.size(); _y++) {
            // ANSIec::Print(1, _y+1, PrintableStringVectorMatrix.at(_y),true,ANSIec::PrintAxisMethod::absolute,ANSIec::PrintAxisMethod::absolute);
            ANSIec::setCursorPos(1, _y+1);
            std::cout << PrintableStringVectorMatrix.at(_y);
            // ANSIec::Print(CURRENT_CONSOLE_DIMENSIONS.x-5,CURRENT_CONSOLE_DIMENSIONS.y-10, std::string("y:")+std::to_string(_y));
            // DEBUGPAUSE(50);
        }
        // ANSIec::Print(0, 0, std::string(PrintableStringVectorMatrix.at(0).size(), '-'),true,ANSIec::PrintAxisMethod::absolute,ANSIec::PrintAxisMethod::relative);
        std::cout.flush();
        ANSIec::setCursorPos(0, 0);
        

        bool exitLoopCalled = false;
        
        bool_driverRunning = true;
        while(bool_driverRunning.load()) {
            /// ----------------------------------------
            
            keyObj.updateKeys();
            
            if(keyObj.isPressed(27)) {
                exitLoopCalled = true;
            }


            // ANSIec::Print(15, 11, fmtToStr(fmtCont(keyObj.getActiveKeys(), 0, 0), CURRENT_CONSOLE_DIMENSIONS.x-15, 0, "left"));

            ANSIec::Print(5, 10, "[test string]:");

            // /// ----------------------------------------
            // timeP_2 = std::chrono::steady_clock::now();
            // intervalDuration_ms = timeP_2 - timeP_1;
            // if(intervalDuration_ms < intervalDuration_ms_refrDur) {
            //     std::this_thread::sleep_for(intervalDuration_ms_refrDur-intervalDuration_ms);
            // }
            // measured_fps = 1.0/(0.001*intervalDuration_ms.count());
            // timeP_1 = std::chrono::steady_clock::now();


            if(exitLoopCalled) break;
        }
        bool_driverRunning = false;


        return MenuDriver_returnType::success;
    }
    
};

