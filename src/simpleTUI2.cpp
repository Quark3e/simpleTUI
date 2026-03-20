
#include <simpleTUI2.hpp>

namespace simpleTUI2 {

    
    int core::Item::callItem(core::Window* _originWindowPtr) {
        switch(itemType) {
        case Item_types::null:
            throw std::logic_error("int core::Item::callItem(core::Window*) : an Item_type::null type Item was called.");
            break;
        case Item_types::text:
            throw std::logic_error("int core::Item::callItem(core::Window*) : an Item_type::text type Item was called.");
            break;
        case Item_types::function:
            if(!isDefined__function) {
                throw std::runtime_error("int core::Item::callItem(core::Window*) : isDefined__function==false yet this Item's type is window and it has been called.");
            }
            this->itemFunction(_originWindowPtr);
            break;
        case Item_types::window:
            if(!isDefined__window) {
                throw std::runtime_error("int core::Item::callItem(core::Window*) : isDefined__window==false yet this Item's type is window and it has been called.");
            }
            this->itemWindow->Driver(_originWindowPtr);
            break;
        default:
            assert(false && "int core::Item::callItem(core::Window*) : What the fuck. Somehow this core::Item object was called without a type having been specified... which is illegal.");
        }
        
        return 0;
    }
    int core::Item::set_parentGroupPtr(core::Group* _newParentGroupPtr) {
        parentGroupPtr = _newParentGroupPtr;

        if(_newParentGroupPtr==nullptr) isDefined__parentGroupPtr = false;
        else isDefined__parentGroupPtr = true;

        isModified__parentGroupPtr = true;

        return 0;
    }
    int core::Item::set_posInParentGroup(Pos2d<size_t> _newPos) {
        posInParentGroup.x = static_cast<int>(_newPos.x);
        posInParentGroup.y = static_cast<int>(_newPos.y);

        isDefined__pos  = true;
        isModified__pos = true;

        return 0;
    }
    int core::Item::nullify() {
        itemType = Item_types::null;
        isModified__itemType = true;

        itemWindow.reset();
        isDefined__window   = false;
        isModified__window  = true;
        
        return 0;
    }
    core::Item::Item(Item_types _itemType): itemType(_itemType) {
        isModified__itemType = true;
    }
    core::Item::Item(std::string _text, Item_types _itemType): itemType(_itemType) {
        isModified__itemType = true;
        itemText = helper_parseStringForNewlines(_text);
        isDefined__text = true;
        isModified__text = true;
        
    }
    core::Item::Item(Type_ItemFunc _func, std::string _text, Item_types _itemType): itemType(_itemType), itemFunction(_func) {
        isModified__itemType = true;
        itemText = helper_parseStringForNewlines(_text);
        isDefined__text = true;
        isDefined__function = true;

    }
    core::Item::Item(core::Window&& _window, std::string _text, Item_types _itemType): itemType(_itemType), itemWindow(std::make_unique<core::Window>(std::move(_window))) {
        isModified__itemType = true;
        itemText = helper_parseStringForNewlines(_text);
        isDefined__text = true;
        isDefined__window = true;

    }
    core::Item::Item() {

    }
    core::Item::Item(const Item& _toCopy): posInParentGroup(_toCopy.posInParentGroup), itemType(_toCopy.itemType), itemText(_toCopy.itemText) {
        isModified__itemType = true;
        if(_toCopy.isDefined__function) {
            itemFunction        = _toCopy.itemFunction;
            isDefined__function = true;
        }

        itemContent_null    = _toCopy.itemContent_null;
        itemContent_text    = _toCopy.itemContent_text;
        itemContent_function= _toCopy.itemContent_function;
        itemContent_window  = _toCopy.itemContent_window;

    }
    core::Item::Item(Item&& _toMove): posInParentGroup(std::move(_toMove.posInParentGroup)), itemType(std::move(_toMove.itemType)), itemText(std::move(_toMove.itemText)) {
        if(_toMove.isDefined__text) {
            itemText        = std::move(_toMove.itemText);
            isDefined__text = std::move(_toMove.isDefined__text);
        }
        if(_toMove.isDefined__function) {
            itemFunction        = std::move(_toMove.itemFunction);
            isDefined__function = std::move(_toMove.isDefined__function);
        }
        if(_toMove.isDefined__window) {
            itemWindow          = std::move(_toMove.itemWindow);
            isDefined__window   = std::move(_toMove.isDefined__window);
        }
        
        itemContent_null    = std::move(_toMove.itemContent_null);
        itemContent_text    = std::move(_toMove.itemContent_text);
        itemContent_function= std::move(_toMove.itemContent_function);
        itemContent_window  = std::move(_toMove.itemContent_window);

    }
    core::Item::~Item() {
        
    }
    core::Item& core::Item::operator=(const Item& _toCopy) {
        posInParentGroup= _toCopy.posInParentGroup;
        itemType        = _toCopy.itemType;
        itemText        = _toCopy.itemText;
        if(_toCopy.isDefined__text) {
            itemText        = _toCopy.itemText;
            isDefined__text = true;
        }
        if(_toCopy.isDefined__function) {
            itemFunction        = _toCopy.itemFunction;
            isDefined__function = true;
        }

        itemContent_null    = _toCopy.itemContent_null;
        itemContent_text    = _toCopy.itemContent_text;
        itemContent_function= _toCopy.itemContent_function;
        itemContent_window  = _toCopy.itemContent_window;

        return *this;
    }
    core::Item& core::Item::operator=(Item&& _toMove) {
        posInParentGroup= std::move(_toMove.posInParentGroup);
        itemType        = std::move(_toMove.itemType);
        itemText        = std::move(_toMove.itemText);

        if(_toMove.isDefined__text) {
            itemText        = std::move(_toMove.itemText);
            isDefined__text = std::move(_toMove.isDefined__text);
        }
        if(_toMove.isDefined__function) {
            itemFunction        = std::move(_toMove.itemFunction);
            isDefined__function = std::move(_toMove.isDefined__function);
        }
        if(_toMove.isDefined__window) {
            itemWindow          = std::move(_toMove.itemWindow);
            isDefined__window   = std::move(_toMove.isDefined__window);
        }
        
        itemContent_null    = std::move(_toMove.itemContent_null);
        itemContent_text    = std::move(_toMove.itemContent_text);
        itemContent_function= std::move(_toMove.itemContent_function);
        itemContent_window  = std::move(_toMove.itemContent_window);

        return *this;
    }
    int core::Item::setContent_null(ItemContent_null _newContent) {
        itemContent_null    = _newContent;

        return 0;
    }
    int core::Item::setContent_text(ItemContent_text _newContent) {
        itemContent_text    = _newContent;

        return 0;
    }
    int core::Item::setContent_function(ItemContent_function _newContent) {
        itemContent_function= _newContent;

        return 0;
    }
    int core::Item::setContent_window(ItemContent_window _newContent) {
        itemContent_window  = _newContent;

        return 0;
    }
    core::Group*    core::Item::get_parentWindowPtr() const {
        return parentGroupPtr;
    }
    Pos2d<int>      core::Item::get_posInParentGroup() const {
        return posInParentGroup;
    }
    std::vector<std::string> core::Item::get_text() const {
        return itemText;
    }
    Type_ItemFunc   core::Item::get_itemFunc() const {
        return itemFunction;
    }
    Item_types      core::Item::get_itemType() const {
        return itemType;
    }
    bool    core::Item::isModified() {
        return (
            isModified__parentGroupPtr ||
            isModified__itemType ||
            isModified__pos ||
            isModified__text ||
            isModified__function ||
            isModified__window
        );
    }
    bool    core::Item::isModified_parentGroupPtr() {
        return isModified__parentGroupPtr;
    }
    bool    core::Item::isModified_itemType() {
        return isModified__itemType;
    }
    bool    core::Item::isModified_pos() {
        return isModified__pos;
    }
    bool    core::Item::isModified_text() {
        return isModified__text;
    }
    bool    core::Item::isModified_function() {
        return isModified__function;
    }
    bool    core::Item::isModified_window() {
        return isModified__window;
    }

    void core::Group::callItem() {
        const std::string _infoStr{"void core::Group::callItem()"};
        if(parentWindowNavCursorPos.x==std::string::npos) throw std::logic_error(_infoStr+" : parentWindowNavCursorPos.x==std::string::npos.");
        if(parentWindowNavCursorPos.y==std::string::npos) throw std::logic_error(_infoStr+" : parentWindowNavCursorPos.y==std::string::npos.");
        
        if(parentWindowNavCursorPos.y>=groupItemMatrix.size()) throw std::logic_error(_infoStr+" : parentWindowNavCursorPos.y>=groupItemMatrix.size().");
        if(parentWindowNavCursorPos.x>=groupItemMatrix.at(0).size()) throw std::logic_error(_infoStr+" : parentWindowNavCursorPos.x>=groupItemMatrix.at(0).size().");
        
        if(groupItemMatrix.at(parentWindowNavCursorPos.y).at(parentWindowNavCursorPos.x).get_itemType()!=Item_types::function) throw std::logic_error(_infoStr+" : core::Item at given parentWindowNavCursorPos"+std::string(parentWindowNavCursorPos)+" is not Item_types::function type.");
        
        
        groupItemMatrix.at(parentWindowNavCursorPos.y).at(parentWindowNavCursorPos.x).callItem(parentWindowPtr);
        
        
    }
    void core::Group::update_axisMaxSizeVectors() {
        if(groupItemMatrix.size()==0) {
            throw std::invalid_argument("void core::Group::update_axisMaxSizeVectors() : groupItemMatrix.size() is not allowed to be 0.");
        }
        if(groupItemMatrix.at(0).size()==0) {
            throw std::invalid_argument("void core::Group::update_axisMaxSizeVectors() : groupItemMatrix.at(0).size() is not allowed to be 0.");
        }

        if(groupDimension.x==0) {
            throw std::invalid_argument("void core::Group::update_axisMaxSizeVectors() : groupDimension.x is not allowed to be 0.");
        }
        if(groupDimension.y==0) {
            throw std::invalid_argument("void core::Group::udpate_axisMaxSizeVectors() : groupDimension.y is not allowed to be 0");
        }

        std::vector<size_t> maxSize_rows(groupItemMatrix.size());
        std::vector<size_t> maxSize_columns(groupItemMatrix.at(0).size());

        Pos2d<int> avgItemSize{
            (groupDimension.x-symb_border_column.size()*2-(maxSize_columns.size()-1)*symb_delimiter_columns.size())/maxSize_columns.size(),
            (groupDimension.y-symb_border_row.size()*2-(maxSize_rows.size()-1)*symb_delimiter_rows.size())/maxSize_rows.size()
        };
        
        for(auto& _col : maxSize_columns)   _col = (avgItemSize.x < 0? 0 : avgItemSize.x);
        for(auto& _row : maxSize_rows)      _row = (avgItemSize.y < 0? 0 : avgItemSize.y);

        axisMaxSize.at(0) = maxSize_columns;
        axisMaxSize.at(1) = maxSize_rows;

    }
    void core::Group::update_groupPtrInItems() {
        for(auto& _row : groupItemMatrix) {
            for(auto& _item : _row) _item.parentGroupPtr = this;
        }
    }
    void core::Group::update_PSVmatrix() {
        if(groupItemMatrix.size()==0) return;
        if(groupItemMatrix.at(0).size()==0) return;

        this->update_axisMaxSizeVectors();

        static Pos2d<size_t> previousGroupDimensions{0, 0};
        Pos2d<int> diffCount{
            static_cast<int>(groupDimension.x)-static_cast<int>(previousGroupDimensions.x),
            static_cast<int>(groupDimension.y)-static_cast<int>(previousGroupDimensions.y)
        };

        if(diffCount.y<0) {
            auto itr_erasePos = PrintableStringVectorMatrix.begin();
            std::advance(itr_erasePos, static_cast<int>(PrintableStringVectorMatrix.size())+diffCount.y);
            PrintableStringVectorMatrix.erase(itr_erasePos, PrintableStringVectorMatrix.end());
        }
        else if(diffCount.y>0) {
            PrintableStringVectorMatrix.insert(PrintableStringVectorMatrix.end(), diffCount.y, std::string(previousGroupDimensions.x, ' '));
        }
        if(diffCount.x!=0) {
            for(auto itr_rowVec=PrintableStringVectorMatrix.begin(); itr_rowVec!=PrintableStringVectorMatrix.end(); ++itr_rowVec) {
                if(diffCount.x<0) {
                    auto itr_erasePos = itr_rowVec->begin();
                    std::advance(itr_erasePos, static_cast<int>(itr_rowVec->size())+diffCount.x);
                    itr_rowVec->erase(itr_erasePos, itr_rowVec->end());
                }
                else {
                    itr_rowVec->insert(itr_rowVec->end(), diffCount.x, ' ');
                }
            }
        }

        if(diffCount.x!=0 || diffCount.y!=0) isModified__PSVmatrix = true;

        previousGroupDimensions.x = (PrintableStringVectorMatrix.size()==0? 0 : PrintableStringVectorMatrix.at(0).size());
        previousGroupDimensions.y = PrintableStringVectorMatrix.size();


        /// Text rendering of the Items' text's stored in this Group object. Only modified text is updated.
        
        Pos2d<size_t> cursorPos_edit(symb_border_column.size(), 1);
        for(size_t item_y=0; item_y<groupItemMatrix.size(); item_y++) {
            for(size_t char_y=0; char_y<axisMaxSize[1].size(); char_y++) {
                cursorPos_edit.x=symb_border_column.size();
                for(size_t item_x=0; item_x<groupItemMatrix.at(0).size(); item_x++) {
                    core::Item& itemRef = groupItemMatrix.at(item_y).at(item_x);
                    const size_t& itemColMaxSize = axisMaxSize[0].at(item_x);
                    
                    if(itemRef.isModified__text && itemRef.itemContent_text.rule_followDelimiter) {
                        if(itemRef.itemText.size()>char_y) {
                            std::string textLine = itemRef.itemText.at(char_y);
                            if(textLine.size()<=itemColMaxSize) {
                                textLine+=std::string(itemColMaxSize-textLine.size(), ' ');
                            }
                            else {
                                textLine.erase(itemColMaxSize);
                            }
                            assert(textLine.size()==itemColMaxSize);

                            memcpy(&PrintableStringVectorMatrix.at(cursorPos_edit.y+char_y).at(cursorPos_edit.x), &textLine.at(0), textLine.size());
                            
                        }
                        else {
                            std::string tempStr_emptyItemLine(itemColMaxSize, ' ');
                            
                            memcpy(&PrintableStringVectorMatrix.at(cursorPos_edit.y+char_y).at(cursorPos_edit.x), &tempStr_emptyItemLine.at(0), tempStr_emptyItemLine.size());
                        }

                        isModified__PSVmatrix = true;
                    }

                    cursorPos_edit.x+=itemColMaxSize;
                    cursorPos_edit.x+=symb_delimiter_columns.size();
                }
            }
            cursorPos_edit.y+=axisMaxSize[1].at(item_y);
            cursorPos_edit.y+=symb_delimiter_rows.size();
        }


        /// Draw border/delimiter symbols

        Pos2d<size_t> _tempPos_xy(symb_border_column.size(), 0);
        for(size_t item_y=0; item_y<groupItemMatrix.size(); item_y++) {
            for(size_t rowHeight_y=0; rowHeight_y<axisMaxSize[1].at(item_y); rowHeight_y++) {
                _tempPos_xy.x = 0;
                for(size_t item_x=0; item_x<axisMaxSize[0].size()-1; item_x++) {
                    _tempPos_xy.x+=axisMaxSize[0].at(item_x)+1;
                    memcpy(&PrintableStringVectorMatrix.at(_tempPos_xy.y+item_y+1).at(_tempPos_xy.x), &symb_delimiter_columns.at(0), symb_delimiter_columns.size());
                }
            }

            _tempPos_xy.y+=axisMaxSize[1].at(item_y);
            if(item_y<groupItemMatrix.size()-1) {
                for(size_t _colFilled=symb_border_column.size(); _colFilled<groupDimension.x-symb_delimiter_rows.size(); _colFilled+=symb_delimiter_rows.size()) {
                    memcpy(&PrintableStringVectorMatrix.at(_tempPos_xy.y+1).at(_colFilled), &symb_delimiter_rows.at(0), symb_delimiter_rows.size());
                }
            }
            _tempPos_xy+=1;
        }

        /// Draw ceiling/floor border rows
        size_t rowColumnsFilled = 0;
        for(; rowColumnsFilled<groupDimension.x-symb_border_row.size(); rowColumnsFilled+=this->symb_border_row.size()) {
            memcpy(&PrintableStringVectorMatrix.at(0).at(rowColumnsFilled),   &symb_border_row.at(0), symb_border_row.size());    /// ceiling border
            memcpy(&PrintableStringVectorMatrix.back().at(rowColumnsFilled),  &symb_border_row.at(0), symb_border_row.size());    /// floor border
        }
        memcpy(&PrintableStringVectorMatrix.at(0).at(groupDimension.x-symb_border_row.size()), &symb_border_row.at(0), (groupDimension.x-rowColumnsFilled));
        memcpy(&PrintableStringVectorMatrix.back().at(groupDimension.x-symb_border_row.size()),&symb_border_row.at(0), groupDimension.x-rowColumnsFilled);
        
        /// Draw left/right border columns
        for(size_t _y=1; _y<groupDimension.y-1; _y++) {
            memcpy(&PrintableStringVectorMatrix.at(_y).at(0), &symb_border_column.at(0), symb_border_column.size());
            memcpy(&PrintableStringVectorMatrix.at(_y).at(groupDimension.x-symb_border_column.size()), &symb_border_column.at(0), symb_border_column.size());
        }

        /// Draw corners
        memcpy(&PrintableStringVectorMatrix.at(0).at(0),    &symb_border_corner.at(0), symb_border_corner.size());
        memcpy(&PrintableStringVectorMatrix.at(0).back(),   &symb_border_corner.at(0), symb_border_corner.size());
        memcpy(&PrintableStringVectorMatrix.back().at(0),   &symb_border_corner.at(0), symb_border_corner.size());
        memcpy(&PrintableStringVectorMatrix.back().back(),  &symb_border_corner.at(0), symb_border_corner.size());

        
        /// Draw text that ignores/goes-over border/delimiter symbols

        cursorPos_edit = Pos2d<size_t>(symb_border_column.size(), symb_border_row.size());
        for(size_t item_y=0; item_y<groupItemMatrix.size(); item_y++) { /// iterate over each row of cells

            for(size_t _row_y=0; _row_y<axisMaxSize[1].at(item_y); _row_y++) { /// iterate over each row/character-height of each cell-row

                cursorPos_edit.x=1; /// set start indexing pos at new row's first character after border column
                for(size_t item_x=0; item_x<groupItemMatrix.at(0).size(); item_x++) { /// iterate over each column of cells
                    core::Item& itemRef = groupItemMatrix.at(item_y).at(item_x);

                    if(itemRef.isModified__text && !itemRef.itemContent_text.rule_followDelimiter) {
                        if(itemRef.itemText.size()>_row_y) { /// !NOTE: Will need further revision to account for strings shorter than previous iterations
                            std::string textLine = itemRef.itemText.at(_row_y);
                            if(cursorPos_edit.x+textLine.size()>=groupDimension.x) textLine.erase(groupDimension.x-cursorPos_edit.x);
                            
                            memcpy(&PrintableStringVectorMatrix.at(cursorPos_edit.y).at(cursorPos_edit.x), &textLine.at(0), axisMaxSize[0].at(item_x));
                        }
                        else {
                            std::string tempStr_emptyCellLine(axisMaxSize[0].at(item_x), ' ');
                            memcpy(&PrintableStringVectorMatrix.at(cursorPos_edit.y).at(cursorPos_edit.x), &tempStr_emptyCellLine.at(0), axisMaxSize[0].at(item_x));
                        }
                    }
                    
                    cursorPos_edit.x+=axisMaxSize[0].at(item_x);
                    cursorPos_edit.x+=symb_delimiter_columns.size(); /// skip over PSV matrix's column delimiter
                }

            }
            cursorPos_edit.y+=axisMaxSize[1].at(item_y);
            cursorPos_edit.y+=symb_delimiter_rows.size(); /// skip over PSV matrix's row delimiter
        }


        this->update_groupPtrInItems();
        
    }
    void core::Group::resize_groupItemMatrix(int axisDiff_x, int axisDiff_y) {
        if(axisDiff_x==0 && axisDiff_y==0) throw std::invalid_argument("void core::Group::resize_groupItemMatrix(int, int) : both arguments for the resizing differences cannot be empty.");
        if(groupItemMatrix.size()==0) throw std::runtime_error("void core::Group::resize_groupItemMatrix(int, int) : object error: groupItemMatrix.size()==0.");
        
        if(axisDiff_y<0 && std::abs(axisDiff_y)>groupItemMatrix.size())         throw std::logic_error("void core::Group::resize_groupItemMatrix(int, int) : axisDiff_y subtraction value cannot be bigger than the number of existing rows.");
        if(axisDiff_x<0 && std::abs(axisDiff_x)>groupItemMatrix.at(0).size())   throw std::logic_error("void core::Group::resize_groupItemMatrix(int, int) : axisDiff_x subtraction value cannot be bigger than the number of existing columns.");
        
        if(axisDiff_y<0) {
            auto rowItr = groupItemMatrix.begin();
            std::advance(rowItr, static_cast<int>(groupItemMatrix.size())+axisDiff_y);
            groupItemMatrix.erase(rowItr, groupItemMatrix.end());
        }
        else if(axisDiff_y>0) {
            for(size_t _i=0; _i<axisDiff_y; _i++) groupItemMatrix.push_back(std::vector<core::Item>(groupItemMatrix.at(0).size(), {Item_types::null}));
        }
        
        if(axisDiff_x<0) {
            for(auto rowItr=groupItemMatrix.begin(); rowItr!=groupItemMatrix.end(); ++rowItr) {
                auto cutoffItr = rowItr->begin();
                std::advance(cutoffItr, static_cast<int>(groupItemMatrix.at(0).size())+axisDiff_x);
                rowItr->erase(cutoffItr, rowItr->end());
            }
        }
        
        
    }
    void core::Group::LoadInitializerItemMatrix(std::initializer_list<std::initializer_list<core::Item>>& _matrixInput) {
        
        Pos2d<size_t>   limMatrix_min(0, 0);
        Pos2d<size_t>   limMatrix_max(0, 0);
        Pos2d<bool>     limDefined_min(false, false);
        Pos2d<bool>     limDefined_max(false, false);
        Pos2d<size_t>   lineCountedDim(0, _matrixInput.size());

        for(auto itr_row=_matrixInput.begin(); itr_row!=_matrixInput.end(); ++itr_row) {
            for(auto itr_item=itr_row->begin(); itr_item!=itr_row->end(); ++itr_item) {
                if(itr_item->isDefined__pos) {
                    if(itr_item->posInParentGroup.x > limMatrix_max.x || !limDefined_max.x) {
                        limMatrix_max.x = itr_item->posInParentGroup.x;
                        limDefined_max.x = true;
                    }
                    else if(itr_item->posInParentGroup.x < limMatrix_min.x || !limDefined_min.x) {
                        limMatrix_min.x = itr_item->posInParentGroup.x;
                        limDefined_min.x = true;
                    }
                    if(itr_item->posInParentGroup.y > limMatrix_max.y || !limDefined_max.y) {
                        limMatrix_max.y = itr_item->posInParentGroup.y;
                        limDefined_max.y = true;
                    }
                    else if(itr_item->posInParentGroup.y < limMatrix_min.y || !limDefined_min.y) {
                        limMatrix_min.y = itr_item->posInParentGroup.y;
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

        groupItemMatrix = std::vector<std::vector<core::Item>>(
            limMatrix_max.y-limMatrix_min.y, std::vector<core::Item>(
                limMatrix_max.x-limMatrix_min.x, {Item_types::null}
            )
        );
        
        Pos2d<size_t> lineCountedPos = limMatrix_min;
        for(auto itr_row=_matrixInput.begin(); itr_row!=_matrixInput.end(); ++itr_row) {
            lineCountedPos.x = limMatrix_min.x;
            for(auto itr_item=itr_row->begin(); itr_item!=itr_row->end(); ++itr_item) {

                if(itr_item->isDefined__pos) groupItemMatrix[itr_item->posInParentGroup.y][itr_item->posInParentGroup.x] = *itr_item;
                else {
                    groupItemMatrix[lineCountedPos.y][lineCountedPos.x] = *itr_item;
                }

                // if(itr_item->cellType!=cell_types::null) {
                //     if(itr_item->isDefined__pos) tableOfCells[itr_item->pos.y][itr_item->pos.x] = *itr_item;
                //     else { /// If the currently holding cell doesn't have a defined position
                //         if(tableOfCells[lineCountedPos.y][lineCountedPos.x].cellType==cell_types::null) {
                //             tableOfCells[itr_item->pos.y][itr_item->pos.x] = *itr_item;
                //         }
                //     }
                // }
                lineCountedPos.x++;
            }
            lineCountedPos.y++;
        }
    }
    core::Group::Group(std::initializer_list<std::initializer_list<core::Item>> _matrixInput) {
        this->LoadInitializerItemMatrix(_matrixInput);

    }
    core::Group& core::Group::operator=(std::initializer_list<std::initializer_list<core::Item>> _matrixInput) {
        this->LoadInitializerItemMatrix(_matrixInput);

    }
    core::Group::Group() {

    }
    core::Group::Group(const Group& _toCopy):
        groupItemMatrix(_toCopy.groupItemMatrix), posInParentWindow(_toCopy.posInParentWindow), groupDimension(_toCopy.groupDimension), axisMaxSize(_toCopy.axisMaxSize),
        PrintableStringVectorMatrix(_toCopy.PrintableStringVectorMatrix),
        symb_delimiter_columns(_toCopy.symb_delimiter_columns), symb_delimiter_rows(_toCopy.symb_delimiter_rows), symb_border_column(_toCopy.symb_border_column), symb_border_row(_toCopy.symb_border_row),
        symb_border_corner(_toCopy.symb_border_corner), symb_rowSeparator(_toCopy.symb_rowSeparator)
    {

    }
    core::Group::Group(Group&& _toMove):
        groupItemMatrix(std::move(_toMove.groupItemMatrix)), posInParentWindow(std::move(_toMove.posInParentWindow)), groupDimension(std::move(_toMove.groupDimension)), axisMaxSize(std::move(_toMove.axisMaxSize)),
        PrintableStringVectorMatrix(std::move(_toMove.PrintableStringVectorMatrix)),
        symb_delimiter_columns(std::move(_toMove.symb_delimiter_columns)), symb_delimiter_rows(std::move(_toMove.symb_delimiter_rows)), symb_border_column(std::move(_toMove.symb_border_column)), symb_border_row(std::move(_toMove.symb_border_row)),
        symb_border_corner(std::move(_toMove.symb_border_corner)), symb_rowSeparator(std::move(_toMove.symb_rowSeparator))
    {
        
    }
    core::Group::~Group() {

    }
    core::Group& core::Group::operator=(const Group& _toCopy) {
        groupItemMatrix     = _toCopy.groupItemMatrix;
        posInParentWindow   = _toCopy.posInParentWindow;
        groupDimension      = _toCopy.groupDimension;
        axisMaxSize         = _toCopy.axisMaxSize;
        PrintableStringVectorMatrix = _toCopy.PrintableStringVectorMatrix;
        symb_delimiter_columns  = _toCopy.symb_delimiter_columns;
        symb_delimiter_rows = _toCopy.symb_delimiter_rows;
        symb_border_column  = _toCopy.symb_border_column;
        symb_border_row     = _toCopy.symb_border_row;
        symb_border_corner  = _toCopy.symb_border_corner;
        symb_rowSeparator   = _toCopy.symb_rowSeparator;

    }
    core::Group& core::Group::operator=(Group&& _toMove) {
        groupItemMatrix     = std::move(_toMove.groupItemMatrix);
        posInParentWindow   = std::move(_toMove.posInParentWindow);
        groupDimension      = std::move(_toMove.groupDimension);
        axisMaxSize         = std::move(_toMove.axisMaxSize);
        PrintableStringVectorMatrix = std::move(_toMove.PrintableStringVectorMatrix);
        symb_delimiter_columns  = std::move(_toMove.symb_delimiter_columns);
        symb_delimiter_rows = std::move(_toMove.symb_delimiter_rows);
        symb_border_column  = std::move(_toMove.symb_border_column);
        symb_border_row     = std::move(_toMove.symb_border_row);
        symb_border_corner  = std::move(_toMove.symb_border_corner);
        symb_rowSeparator   = std::move(_toMove.symb_rowSeparator);
        
    }
    int core::Group::add(const core::Item& _ItemToAdd, Pos2d<size_t> _ItemPosition, flag_add_alreadyExists _posTaken) {

        if(_ItemPosition==Pos2d<size_t>{std::string::npos, std::string::npos} && !_ItemToAdd.isDefined__pos) throw std::invalid_argument("int core::Group::add(const core::Item&) : _ItemPosition argument cannot be empty whilst the core::Item arg also doesn't have a position given.");
        
        Pos2d<size_t> item_pos = (
            (_ItemPosition.x==std::string::npos && _ItemPosition.y==std::string::npos)?
                _ItemToAdd.get_posInParentGroup().cast<size_t>() : _ItemPosition
        );
        int resizeAxis_x = 0;
        int resizeAxis_y = 0;
        if(item_pos.y>groupItemMatrix.size())       resizeAxis_y = static_cast<int>(item_pos.y)-static_cast<int>(groupItemMatrix.size());
        if(item_pos.x>groupItemMatrix.at(0).size()) resizeAxis_x = static_cast<int>(item_pos.x)-static_cast<int>(groupItemMatrix.at(0).size());
        
        if(resizeAxis_x || resizeAxis_y) this->resize_groupItemMatrix(resizeAxis_x, resizeAxis_y);
        
        if(groupItemMatrix.at(item_pos.y).at(item_pos.x).get_itemType()!=Item_types::null) {
            switch (_posTaken) {
                case flag_add_alreadyExists::throwExcept:
                    throw std::invalid_argument(std::string("int core::Group::add(const core::Item&, Pos2d<size_t>, flag_add_alreadyExists) : An non-null Item at pos:")+std::string(item_pos)+" already exists.");
                    break;
                case flag_add_alreadyExists::replace:
                    groupItemMatrix.at(item_pos.y).at(item_pos.x) = _ItemToAdd;
                    if(!_ItemToAdd.isDefined__pos) groupItemMatrix.at(item_pos.y).at(item_pos.x).set_posInParentGroup(item_pos);
                    break;
                case flag_add_alreadyExists::skip:
                    break;
                case flag_add_alreadyExists::append_abs: {
                        auto validPos = matrixSearch_2D<core::Item>(
                            groupItemMatrix, core::Item{Item_types::null}, item_pos, [](core::Item _matrixElement, core::Item _checkElement) {
                                if(_matrixElement.get_itemType()==Item_types::null) return true;
                                else return false;
                            }, 1, flag_matrixSearch_method::radial
                        );
                        if(validPos.size()==0) throw std::logic_error(" error: matrixSearch_2D return 0 valid positions");
                        item_pos = validPos[0];
                        groupItemMatrix.at(item_pos.y).at(item_pos.x) = _ItemToAdd;
                        groupItemMatrix.at(item_pos.y).at(item_pos.x).set_posInParentGroup(item_pos);
                    }
                    break;
                case flag_add_alreadyExists::append_cont: {
                        auto validPos = matrixSearch_2D<core::Item>(
                            groupItemMatrix, core::Item{Item_types::null}, item_pos, [](core::Item _matrixElement, core::Item _checkElement) {
                                if(_matrixElement.get_itemType()==Item_types::null) return true;
                                else return false;
                            }, 1, flag_matrixSearch_method::axial
                        );
                        if(validPos.size()==0) throw std::logic_error(" error: matrixSearch_2D return 0 valid positions");
                        item_pos = validPos[0];
                        groupItemMatrix.at(item_pos.y).at(item_pos.x) = _ItemToAdd;
                        groupItemMatrix.at(item_pos.y).at(item_pos.x).set_posInParentGroup(item_pos);
                    }
                    break;
            }

        }
        else {
            groupItemMatrix.at(item_pos.y).at(item_pos.x) = _ItemToAdd;
            groupItemMatrix.at(item_pos.y).at(item_pos.x).set_posInParentGroup(item_pos);
        }

        
        return 0;
    }
    int core::Group::add(core::Item&& _ItemToAdd, Pos2d<size_t> _ItemPosition, flag_add_alreadyExists _posTaken) {
        
        if(_ItemPosition==Pos2d<size_t>{std::string::npos, std::string::npos} && !_ItemToAdd.isDefined__pos) throw std::invalid_argument("int core::Group::add(const core::Item&) : _ItemPosition argument cannot be empty whilst the core::Item arg also doesn't have a position given.");
        
        Pos2d<size_t> item_pos = (
            (_ItemPosition.x==std::string::npos && _ItemPosition.y==std::string::npos)?
                _ItemToAdd.get_posInParentGroup().cast<size_t>() : _ItemPosition
        );
        int resizeAxis_x = 0;
        int resizeAxis_y = 0;
        if(item_pos.y>groupItemMatrix.size())       resizeAxis_y = static_cast<int>(item_pos.y)-static_cast<int>(groupItemMatrix.size());
        if(item_pos.x>groupItemMatrix.at(0).size()) resizeAxis_x = static_cast<int>(item_pos.x)-static_cast<int>(groupItemMatrix.at(0).size());
        
        if(resizeAxis_x || resizeAxis_y) this->resize_groupItemMatrix(resizeAxis_x, resizeAxis_y);
        
        if(groupItemMatrix.at(item_pos.y).at(item_pos.x).itemType!=Item_types::null) {
            switch (_posTaken) {
                case flag_add_alreadyExists::throwExcept:
                    throw std::invalid_argument(std::string("int core::Group::add(const core::Item&, Pos2d<size_t>, flag_add_alreadyExists) : An non-null Item at pos:")+std::string(item_pos)+" already exists.");
                    break;
                case flag_add_alreadyExists::replace:
                    groupItemMatrix.at(item_pos.y).at(item_pos.x) = std::move(_ItemToAdd);
                    if(!_ItemToAdd.isDefined__pos) groupItemMatrix.at(item_pos.y).at(item_pos.x).set_posInParentGroup(item_pos);
                    break;
                case flag_add_alreadyExists::skip:
                    break;
                case flag_add_alreadyExists::append_abs: {
                        auto validPos = matrixSearch_2D<core::Item>(
                            groupItemMatrix, core::Item{Item_types::null}, item_pos, [](core::Item _matrixElement, core::Item _checkElement) {
                                if(_matrixElement.itemType==Item_types::null) return true;
                                else return false;
                            }, 1, flag_matrixSearch_method::radial
                        );
                        if(validPos.size()==0) throw std::logic_error(" error: matrixSearch_2D return 0 valid positions");
                        item_pos = validPos[0];
                        groupItemMatrix.at(item_pos.y).at(item_pos.x) = std::move(_ItemToAdd);
                        groupItemMatrix.at(item_pos.y).at(item_pos.x).set_posInParentGroup(item_pos);
                    }
                    break;
                case flag_add_alreadyExists::append_cont: {
                        auto validPos = matrixSearch_2D<core::Item>(
                            groupItemMatrix, core::Item{Item_types::null}, item_pos, [](core::Item _matrixElement, core::Item _checkElement) {
                                if(_matrixElement.itemType==Item_types::null) return true;
                                else return false;
                            }, 1, flag_matrixSearch_method::axial
                        );
                        if(validPos.size()==0) throw std::logic_error(" error: matrixSearch_2D return 0 valid positions");
                        item_pos = validPos[0];
                        groupItemMatrix.at(item_pos.y).at(item_pos.x) = std::move(_ItemToAdd);
                        groupItemMatrix.at(item_pos.y).at(item_pos.x).set_posInParentGroup(item_pos);
                    }
                    break;
            }

        }
        else {
            groupItemMatrix.at(item_pos.y).at(item_pos.x) = std::move(_ItemToAdd);
            groupItemMatrix.at(item_pos.y).at(item_pos.x).set_posInParentGroup(item_pos);
        }

        
        return 0;
    }
    int core::Group::add(std::vector<core::Item> _ItemsToAdd, std::vector<Pos2d<size_t>> _ItemPositions, flag_add_alreadyExists _posTaken){
        const std::string _infoStr{"int core::Group::add(std::vector<core::Item>, std::vector<Pos2d<size_t>>, flag_add_alreadyExists)"};
        if(_ItemsToAdd.size()==0)       throw std::invalid_argument(_infoStr+" : _ItemsToAdd argument cannot be empty.");
        if(_ItemPositions.size()==0)    throw std::invalid_argument(_infoStr+" : _ItemPositions argument cannot be empty.");
        if(_ItemsToAdd.size()!=_ItemPositions.size()) throw std::invalid_argument(_infoStr+" : _ItemsToAdd.size()!=_ItemPositions.");

        try {
            for(size_t _i=0; _i<_ItemsToAdd.size(); _i++) {
                this->add(_ItemsToAdd.at(_i), _ItemPositions.at(_i), _posTaken);
            }
        }
        catch(const std::exception& e) {
            throw std::runtime_error(_infoStr+" : "+e.what());
        }
        

        return 0;
    }
    int core::Group::erase(Pos2d<size_t> _posToItem) {
        const std::string _infoStr{"int core::Group::erase(Pos2d<size_t>)"};

        if(_posToItem.y>=groupItemMatrix.size())        throw std::invalid_argument(_infoStr+" : _posToItem.y value is larger than existing columns ("+std::to_string(groupItemMatrix.size())+")");
        if(_posToItem.x>=groupItemMatrix.at(0).size())  throw std::invalid_argument(_infoStr+" : _posToItem.x value is larger than existing columns ("+std::to_string(groupItemMatrix.at(0).size())+")");
        
        if(groupItemMatrix.at(_posToItem.y).at(_posToItem.x).get_itemType()==Item_types::null) throw std::invalid_argument(_infoStr+" : Item at given location is a Item_types::null");

        groupItemMatrix.at(_posToItem.y).at(_posToItem.x).nullify();

        return 0;
    }
    int core::Group::erase(std::vector<Pos2d<size_t>> _posToItemsToErase) {
        const std::string _infoStr{"int core::Group::erase(std::vector<Pos2d<size_t>>)"};

        if(_posToItemsToErase.size()==0) throw std::invalid_argument(_infoStr+" : _posToItemsToErase container cannot be empty.");

        try {
            for(auto toErasePos : _posToItemsToErase) {
                this->erase(toErasePos);
            }
        }
        catch(const std::exception& e) {
            throw std::runtime_error(_infoStr+" : "+e.what());
        }

        return 0;
    }

    void core::Window::update_PSVmatrix() {
        const std::string _infoStr{"void core::Window::update_PSVmatrix()"};
        if(windowGroups.size()==0) throw std::logic_error(_infoStr+" : windowGroups container cannot be empty.");

        for(core::Group& groupRef : windowGroups) groupRef.update_PSVmatrix();

        static Pos2d<size_t> previousWindowDimensions{0, 0};
        helperMethods::helper_getConsoleDimensions();
        Pos2d<int> windowDiffCount{
            static_cast<int>(helperMethods::CURRENT_CONSOLE_DIMENSIONS.x)-static_cast<int>(previousWindowDimensions.x),
            static_cast<int>(helperMethods::CURRENT_CONSOLE_DIMENSIONS.y)-static_cast<int>(previousWindowDimensions.y)
        };

        if(windowDiffCount.y<0) {
            auto itr_erasePos = PrintableStringVectorMatrix.begin();
            std::advance(itr_erasePos, static_cast<int>(PrintableStringVectorMatrix.size())+windowDiffCount.y);
            PrintableStringVectorMatrix.erase(itr_erasePos, PrintableStringVectorMatrix.end());
        }
        else if(windowDiffCount.y>0) {
            PrintableStringVectorMatrix.insert(PrintableStringVectorMatrix.end(), windowDiffCount.y, std::string(previousWindowDimensions.x, ' '));
        }
        if(windowDiffCount.x!=0) {
            for(auto itr_rowVec=PrintableStringVectorMatrix.begin(); itr_rowVec!=PrintableStringVectorMatrix.end(); ++itr_rowVec) {
                if(windowDiffCount.x<0) {
                    auto itr_erasePos = itr_rowVec->begin();
                    std::advance(itr_erasePos, static_cast<int>(itr_rowVec->size())+windowDiffCount.x);
                    itr_rowVec->erase(itr_erasePos, itr_rowVec->end());
                }
                else {
                    itr_rowVec->insert(itr_rowVec->end(), windowDiffCount.x, ' ');
                }
            }
        }

        previousWindowDimensions.x = (PrintableStringVectorMatrix.size()==0? std::string::npos : PrintableStringVectorMatrix.at(0).size());
        previousWindowDimensions.y = PrintableStringVectorMatrix.size();

        
        Pos2d<size_t> cursorPos_edit{0, 0};
        for(size_t _i=0; _i<posOfGroupsInWindow.size(); _i++) {
            core::Group& _groupRef = windowGroups.at(_i);
            if(!_groupRef.isModified__PSVmatrix) continue;

            Pos2d<size_t> corner_TL = posOfGroupsInWindow.at(_i).at(0);
            Pos2d<size_t> corner_BR = posOfGroupsInWindow.at(_i).at(1);

            Pos2d<size_t> boxDim_groupPSVmatrix{
                (_groupRef.PrintableStringVectorMatrix.at(0).size()>(corner_BR.x-corner_TL.x)? (corner_BR.x-corner_TL.x) : _groupRef.PrintableStringVectorMatrix.at(0).size()),
                (_groupRef.PrintableStringVectorMatrix.at(1).size()>(corner_BR.y-corner_TL.y)? (corner_BR.y-corner_TL.y) : _groupRef.PrintableStringVectorMatrix.at(1).size())
            };

            

        }
        
    }
    void core::Window::prep_solveNewGroupPosInWindow() {
        const std::string _infoStr{"void core::Window::prep_solveNewGroupPosInWindow()"};
        if(windowGroups.size()==0) throw std::logic_error(_infoStr+" : windowGroups is empty.");
        if(windowGroups.size()==posOfGroupsInWindow.size()) throw std::logic_error(_infoStr+" : windowGroups.size()==posOfGroupsInWindow.size() which means there are no new core::Group to find a position for.");
        if(windowGroups.size()<posOfGroupsInWindow.size())  throw std::logic_error(_infoStr+" : windowGroups.size()<posOfGroupsInWindow.size() which is not allowed.");

        /// TEMPORARY SOLUTION! Need to be changed for a more proper method later on.
        /// Current method of placing the TL corners of each core::Group::PrintableStringVectorMatrix is to just divide the longer axis of the terminal/console space and place the
        /// core::Group's along that axis side.
        ///
        /// Future idea on setting positions.

        /// NOTE: For now, made it so it overwrites any and all currently set positions for the sake of just getting over it and have it work at the start. In a rough patch with
        /// progress for some reason.

        // size_t idx_startSolve = posOfGroupsInWindow.size();
        size_t idx_startSolve = 0;

        Pos2d<size_t> totalAxis_PSVmatrixLen{0, 0};
        size_t axisToDrawOn = 0; ///< 0-width; 1-height;

        for(size_t _i=idx_startSolve; _i<windowGroups.size(); _i++) {
            core::Group& _groupRef = windowGroups.at(_i);
            _groupRef.update_PSVmatrix();

            totalAxis_PSVmatrixLen.y+=_groupRef.PrintableStringVectorMatrix.size();
            totalAxis_PSVmatrixLen.x+=_groupRef.PrintableStringVectorMatrix.at(0).size();
        }


        posOfGroupsInWindow = std::vector<std::vector<Pos2d<size_t>>>(windowGroups.size(), {{0, 0}, {0, 0}});

        Pos2d<size_t> consoleDims = helperMethods::helper_getConsoleDimensions();
        axisToDrawOn = (consoleDims.y>consoleDims.x? 1 : 0);

        float gapLen = static_cast<float>((axisToDrawOn==0? consoleDims.x : consoleDims.y))/static_cast<float>(windowGroups.size());
        for(size_t _i=0; _i<windowGroups.size(); _i++) {
            switch (axisToDrawOn) {
            case 0:
                posOfGroupsInWindow.at(_i) = {{_i*gapLen, 0}, {(_i+1)*gapLen, consoleDims.y}};
                break;
            case 1:
                posOfGroupsInWindow.at(_i) = {{0, _i*gapLen}, {consoleDims.x, (_i+1)*gapLen}};
                break;
            default:
                break;
            }
        }
        

    }
    void core::Window::prep_windowInit() {
        
        for(size_t _i=0; _i<windowGroups.size(); _i++) {
            core::Group& groupRef = windowGroups.at(_i);

            groupRef.parentWindowPtr = this;
            // groupRef.update_PSVmatrix();
        }
        prep_solveNewGroupPosInWindow();

    }
    core::Window::Window(std::initializer_list<core::Group> _groupInput):
        windowGroups(_groupInput)
    {
        prep_windowInit();
        
    }
    core::Window& core::Window::operator=(std::initializer_list<core::Group> _groupInput) {
        windowGroups = _groupInput;
        prep_windowInit();


        return *this;
    }
    core::Window::Window(): windowGroups({}) {


    }
    core::Window::Window(const Window& _toCopy):
        windowGroups(_toCopy.windowGroups), posOfGroupsInWindow(_toCopy.posOfGroupsInWindow),
        idx_selectedGroup(_toCopy.idx_selectedGroup), PrintableStringVectorMatrix(_toCopy.PrintableStringVectorMatrix),
        windowCursorPos(_toCopy.windowCursorPos)
    {
        prep_windowInit();

    }
    core::Window::Window(Window&& _toMove):
        windowGroups(std::move(_toMove.windowGroups)), posOfGroupsInWindow(std::move(_toMove.posOfGroupsInWindow)),
        idx_selectedGroup(std::move(_toMove.idx_selectedGroup)), PrintableStringVectorMatrix(std::move(_toMove.PrintableStringVectorMatrix)),
        windowCursorPos(std::move(_toMove.windowCursorPos))
    {
        prep_windowInit();

    }
    core::Window::~Window() {

    }
    core::Window& core::Window::operator=(const Window& _toCopy) {
        windowGroups                = _toCopy.windowGroups;
        posOfGroupsInWindow         = _toCopy.posOfGroupsInWindow;
        idx_selectedGroup           = _toCopy.idx_selectedGroup;
        PrintableStringVectorMatrix = _toCopy.PrintableStringVectorMatrix;
        windowCursorPos             = _toCopy.windowCursorPos;

        prep_windowInit();


        return *this;
    }
    core::Window& core::Window::operator=(Window&& _toMove) {
        windowGroups                = std::move(_toMove.windowGroups);
        posOfGroupsInWindow         = std::move(_toMove.posOfGroupsInWindow);
        idx_selectedGroup           = std::move(_toMove.idx_selectedGroup);
        PrintableStringVectorMatrix = std::move(_toMove.PrintableStringVectorMatrix);
        windowCursorPos             = std::move(_toMove.windowCursorPos);
        
        prep_windowInit();


        return *this;
    }
    int core::Window::Driver(core::Window* _originPtr) {

        

    }

}
