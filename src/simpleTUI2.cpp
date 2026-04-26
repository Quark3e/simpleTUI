
#include <simpleTUI2.hpp>

namespace simpleTUI2 {

    
    int core::Item::callItem(core::Window* _originWindowPtr) {
        const std::string _infoStr{"int core::Item::callItem(core::Window*)"};
        std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));

        switch(itemType) {
        case Item_types::null:
            throw std::logic_error(_infoStr+" : an Item_type::null type Item was called.");
            break;
        case Item_types::text:
            throw std::logic_error(_infoStr+" : an Item_type::text type Item was called.");
            break;
        case Item_types::function:
            if(!isDefined__function) {
                throw std::runtime_error(_infoStr+" : isDefined__function==false yet this Item's type is window and it has been called.");
            }
            this->itemFunction(_originWindowPtr);
            break;
        case Item_types::window:
            if(!isDefined__window) {
                throw std::runtime_error(_infoStr+" : isDefined__window==false yet this Item's type is window and it has been called.");
            }
            this->itemWindow->Driver(_originWindowPtr);
            break;
        default:
            // assert(false && "int core::Item::callItem(core::Window*) : What the fuck. Somehow this core::Item object was called without a type having been specified... which is illegal.");
            throw std::runtime_error(_infoStr+" : What the fuck. Somehow this core::Item object was called without a type having been specified... which is illegal.");
            
        }
        
        return 0;
    }
    int core::Item::set_parentGroupPtr(core::Group* _newParentGroupPtr) {
        std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));

        parentGroupPtr = _newParentGroupPtr;

        if(_newParentGroupPtr==nullptr) isDefined__parentGroupPtr = false;
        else isDefined__parentGroupPtr = true;

        isModified__parentGroupPtr = true;

        return 0;
    }
    int core::Item::set_posInParentGroup(Pos2d<size_t> _newPos) {
        std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));

        posInParentGroup.x = static_cast<int>(_newPos.x);
        posInParentGroup.y = static_cast<int>(_newPos.y);

        isDefined__pos  = true;
        isModified__pos = true;

        return 0;
    }
    int core::Item::nullify() {
        std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));
        
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
    core::Item::Item(const char* _text, Item_types _itemType): itemType(_itemType) {
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
    core::Item::Item(const Item& _toCopy) {
        itemType    = _toCopy.itemType;
        isModified__itemType= true;
        
        posInParentGroup    = _toCopy.posInParentGroup;
        isDefined__pos      = !(posInParentGroup.x==std::string::npos || posInParentGroup.y==std::string::npos);
        isModified__pos     = true;

        if(_toCopy.isDefined__text) {
            itemText            = _toCopy.itemText;
            isDefined__text     = true;
            isModified__text    = true;
        }
        if(_toCopy.isDefined__function) {
            itemFunction        = _toCopy.itemFunction;
            isDefined__function = true;
            isModified__function= true;
        }
        if(_toCopy.isDefined__window) {
            /// the window object at the moment cannot be copied. Need to create a member method that makes a copy of all the stored groups and types, recursively, with a depth limit.
        }

        parentGroupPtr = _toCopy.parentGroupPtr;
        isDefined__parentGroupPtr   = (parentGroupPtr!=nullptr);
        isModified__parentGroupPtr  = true;
        
        itemStyleInfo = _toCopy.itemStyleInfo;
        
        // itemContent_null    = _toCopy.itemContent_null;
        // itemContent_text    = _toCopy.itemContent_text;
        // itemContent_function= _toCopy.itemContent_function;
        // itemContent_window  = _toCopy.itemContent_window;

    }
    core::Item::Item(Item&& _toMove) {
        itemType    = std::move(_toMove.itemType);
        isModified__itemType= true;
        
        posInParentGroup    = std::move(_toMove.posInParentGroup);
        isDefined__pos      = !(posInParentGroup.x==std::string::npos || posInParentGroup.y==std::string::npos);
        isModified__pos     = true;

        if(_toMove.isDefined__text) {
            itemText        = std::move(_toMove.itemText);
            isDefined__text = true;
            isModified__text= true;
        }
        if(_toMove.isDefined__function) {
            itemFunction        = std::move(_toMove.itemFunction);
            isDefined__function = true;
            isModified__function= true;
        }
        if(_toMove.isDefined__window) {
            itemWindow          = std::move(_toMove.itemWindow);
            isDefined__window   = true;
            isModified__window  = true;
        }

        parentGroupPtr  = std::move(_toMove.parentGroupPtr);
        isDefined__parentGroupPtr   = (parentGroupPtr!=nullptr);
        isModified__parentGroupPtr  = true;
        
        itemStyleInfo   = std::move(_toMove.itemStyleInfo);

        // itemContent_null    = std::move(_toMove.itemContent_null);
        // itemContent_text    = std::move(_toMove.itemContent_text);
        // itemContent_function= std::move(_toMove.itemContent_function);
        // itemContent_window  = std::move(_toMove.itemContent_window);

    }
    core::Item::~Item() {
        
    }
    core::Item& core::Item::operator=(const Item& _toCopy) {
        std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));
        isModified__itemType    = (itemType!=_toCopy.itemType);
        itemType        = _toCopy.itemType;
        
        isModified__pos = (posInParentGroup!=_toCopy.posInParentGroup);
        posInParentGroup= _toCopy.posInParentGroup;
        isDefined__pos  = !(posInParentGroup.x==std::string::npos || posInParentGroup.y==std::string::npos);

        if(_toCopy.isDefined__text) {
            isModified__text= (itemText!=_toCopy.itemText);
            itemText        = _toCopy.itemText;
            isDefined__text = true;
        }
        if(_toCopy.isDefined__function) {
            isModified__function= true;
            itemFunction        = _toCopy.itemFunction;
            isDefined__function = true;
        }

        isModified__parentGroupPtr  = (parentGroupPtr!=_toCopy.parentGroupPtr);
        parentGroupPtr              = _toCopy.parentGroupPtr;
        isDefined__parentGroupPtr   = (parentGroupPtr!=nullptr);

        itemStyleInfo = _toCopy.itemStyleInfo;

        // itemContent_null    = _toCopy.itemContent_null;
        // itemContent_text    = _toCopy.itemContent_text;
        // itemContent_function= _toCopy.itemContent_function;
        // itemContent_window  = _toCopy.itemContent_window;

        return *this;
    }
    core::Item& core::Item::operator=(Item&& _toMove) {
        std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));
        isModified__itemType    = (itemType!=_toMove.itemType);
        itemType        = std::move(_toMove.itemType);

        isModified__pos = (posInParentGroup!=_toMove.posInParentGroup);
        posInParentGroup= std::move(_toMove.posInParentGroup);
        isDefined__pos  = !(posInParentGroup.x==std::string::npos || posInParentGroup.y==std::string::npos);

        if(_toMove.isDefined__text) {
            isModified__text= (itemText!=_toMove.itemText);
            itemText        = std::move(_toMove.itemText);
            isDefined__text = true;
        }
        if(_toMove.isDefined__function) {
            isModified__function= true;
            itemFunction        = std::move(_toMove.itemFunction);
            isDefined__function = true;
        }
        if(_toMove.isDefined__window) {
            itemWindow          = std::move(_toMove.itemWindow);
            isDefined__window   = true;
            isModified__window  = true;
        }

        isModified__parentGroupPtr  = (parentGroupPtr!=_toMove.parentGroupPtr);
        parentGroupPtr              = std::move(_toMove.parentGroupPtr);
        isDefined__parentGroupPtr   = (parentGroupPtr!=nullptr);
        
        itemStyleInfo   = std::move(_toMove.itemStyleInfo);

        // itemContent_null    = std::move(_toMove.itemContent_null);
        // itemContent_text    = std::move(_toMove.itemContent_text);
        // itemContent_function= std::move(_toMove.itemContent_function);
        // itemContent_window  = std::move(_toMove.itemContent_window);

        return *this;
    }
    //bool core::Item::operator==(const Item& _toCompare) {
    //    std::unique_lock<std::mutex> u_lck_accssContent
    //}
    // int core::Item::setContent_null(ItemContent_null _newContent) {
    //     std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));
    //     itemContent_null    = _newContent;
    //     return 0;
    // }
    // int core::Item::setContent_text(ItemContent_text _newContent) {
    //     std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));
    //     itemContent_text    = _newContent;
    //     return 0;
    // }
    // int core::Item::setContent_function(ItemContent_function _newContent) {
    //     std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));
    //     itemContent_function= _newContent;
    //     return 0;
    // }
    // int core::Item::setContent_window(ItemContent_window _newContent) {
    //     std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));
    //     itemContent_window  = _newContent;
    //     return 0;
    // }
    core::Group*    core::Item::get_parentWindowPtr() const {
        std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));
        return parentGroupPtr;
    }
    Pos2d<size_t>   core::Item::get_posInParentGroup() const {
        std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));
        return posInParentGroup;
    }
    std::vector<std::string> core::Item::get_text() const {
        std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));
        return itemText;
    }
    std::vector<std::string> core::Item::get_text(size_t _maxWidth) const {
        const std::string _infoStr{"std::vector<std::string> core::Item::get_text(size_t) const"};
        if(_maxWidth==0) throw std::invalid_argument(_infoStr+" : _maxWidth value cannot be 0.");
        std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));
        std::vector<std::string> modif_itemText{itemText};
        u_lck_accsContent.unlock();

        for(size_t _i=0; _i<modif_itemText.size(); _i++) {
            if(modif_itemText.at(_i).size()>_maxWidth) {
                // std::string& strRef = modif_itemText.at(_i);
                switch (itemStyleInfo.cont_text.rule_oe_textLineSol) {
                case style::ItemCont_text::enum_solution_widthOE::ignore_oe_region:
                    modif_itemText.at(_i).erase(_maxWidth,std::string::npos);
                    break;
                case style::ItemCont_text::enum_solution_widthOE::transfer_to_new:
                    // std::string tempStr = modif_itemText.at(_i).substr(_maxWidth, std::string::npos);
                    modif_itemText.insert(modif_itemText.begin()+_i, modif_itemText.at(_i).substr(_maxWidth, std::string::npos));
                    modif_itemText.at(_i).erase(_maxWidth, std::string::npos);
                    break;
                case style::ItemCont_text::enum_solution_widthOE::transfer_to_next:
                    if(_i<modif_itemText.size()-1) {
                        // modif_itemText.insert(modif_itemText.begin()+_i, modif_itemText.at(_i).substr(_maxWidth, std::string::npos));

                        modif_itemText.at(_i+1).insert(0, modif_itemText.at(_i).substr(_maxWidth, std::string::npos));
                        modif_itemText.at(_i).erase(_maxWidth, std::string::npos);
                    }
                    else {
                        modif_itemText.push_back(modif_itemText.at(_i).substr(_maxWidth, std::string::npos));
                        modif_itemText.at(_i).erase(_maxWidth, std::string::npos);
                    }
                    break;
                default:
                    break;
                }

            }
        }

        return modif_itemText;
    }
    int core::Item::set_text(std::string _newStr) {
        const std::string _infoStr("int core::Item::set_text(std::string)");
        
        std::unique_lock<std::mutex> u_lck_accssContent(mtx_access_otherMembers);
        itemText = helper_parseStringForNewlines(_newStr);
        isDefined__text = true;
        isModified__text = true;
        
        return 0;
    }
    int core::Item::set_text(std::vector<std::string> _newStr) {
        const std::string _infoStr("int core::Item::set_text(std::vector<std::string>)");

        std::unique_lock<std::mutex> u_lck_accssContent(mtx_access_otherMembers);
        itemText = _newStr;
        isDefined__text = true;
        isModified__text = true;

        return 0;
    }
    Type_ItemFunc   core::Item::get_itemFunc() const {
        if(!isDefined__function) throw std::runtime_error("Type_ItemFunc core::Item::get_itemFunc() const : a function has not been defined before this member call.");
        std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));
        return itemFunction;
    }
    Item_types      core::Item::get_itemType() const {
        std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));
        return itemType;
    }
    int core::Item::set_itemType(Item_types _newType) {
        std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));

        itemType = _newType;
        isModified__itemType = true;

        return 0;
    }
    int core::Item::set_itemType(std::string _text, Item_types _newType) {
        std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));

        itemText = helper_parseStringForNewlines(_text);
        isDefined__text = true;
        isModified__text = true;

        itemType = _newType;
        isModified__itemType = true;

        return 0;
    }
    int core::Item::set_itemType(Type_ItemFunc _func, Item_types _newType) {
        std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));

        itemFunction = _func;
        isDefined__function = true;
        isModified__function = true;

        itemType = _newType;
        isModified__itemType = true;

        return 0;
    }
    int core::Item::set_itemType(core::Window&& _window, Item_types _newType) {
        std::unique_lock<std::mutex> u_lck_accsContent(const_cast<std::mutex&>(mtx_access_otherMembers));

        itemWindow = std::unique_ptr<core::Window>(new core::Window(_window));
        isDefined__window = true;
        isModified__window = true;

        itemType = _newType;
        isModified__itemType = true;

        return 0;
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

    core::Group::result_moveNavCursor core::Group::func_moveNavCursor(Pos2d<int> _moveSteps) {
        const std::string _infoStr{"core::Group::result_moveNavCursor core::Group::func_moveNavCursor(Pos2d<int>)"};
        std::unique_lock<std::mutex> u_lck_accss_otherMembers(mtx_access_otherMembers, std::defer_lock);
        std::unique_lock<std::mutex> u_lck_accss_groupItemMatrix(mtx_access_groupItemMatrix, std::defer_lock);
        
        u_lck_accss_groupItemMatrix.lock();
        if(groupItemMatrix.size()==0) throw std::logic_error(_infoStr+" : groupItemMatrix.size()==0.");
        if(groupItemMatrix.at(0).size()==0) throw std::logic_error(_infoStr+" : groupItemMatrix.at(0).size()==0.");

        if(winNavCursorPos.x==std::string::npos || winNavCursorPos.y==std::string::npos) {
            if(last_winNavCursorPos.x==std::string::npos || last_winNavCursorPos.y==std::string::npos) {
                std::vector<Pos2d<size_t>> newPos = matrixSearch_2D<core::Item>(
                    core::Group::groupItemMatrix, core::Item{Item_types::null},
                    [](core::Item matrixElement, core::Item toSearch) {

                        return (matrixElement.get_itemType()==Item_types::function);
                    }, {0, 0}, 1
                );
                if(newPos.size()==0) {

                    // throw std::runtime_error(_infoStr+" : no previous nav cursor positions exist and no function types exist.");
                    return result_moveNavCursor::no_options_available;
                }
                else {
                }
                // exit(0);

                winNavCursorPos = newPos.at(0);
            }
            else {
                winNavCursorPos = last_winNavCursorPos;
            }
            
        }
        u_lck_accss_groupItemMatrix.unlock();
        /// If `last_winNavCursorPos` wasn't initialised and the new move step values are both 0, then that means this member function was used to initialise
        /// a that variables, which is something allowed.
        if(_moveSteps.x==0 && _moveSteps.y==0) return result_moveNavCursor::normal;

        // Pos2d<int> refrNewPos = winNavCursorPos.cast<int>()+_moveSteps;
        // while(refrNewPos.x>=groupItemMatrix.at(0).size()) {
        //     refrNewPos.x-=groupItemMatrix.at(0).size();
        // }
        // while(refrNewPos.y>=groupItemMatrix.size()) {
        //     refrNewPos.y-=groupItemMatrix.size();
        // }
        
        /// @brief position of the current cursor used as a reference for placing the new cursorpos after final movements
        Pos2d<int> refrNewPos = winNavCursorPos.cast<int>();

        Pos2d<int> moveDirection{
            (_moveSteps.x==0? 0 : _moveSteps.x / std::abs(_moveSteps.x)),
            (_moveSteps.y==0? 0 : _moveSteps.y / std::abs(_moveSteps.y))
        };

        Pos2d<size_t> groupDim{groupItemMatrix.at(0).size(), groupItemMatrix.size()};

        /// ---------- NEW METHOD ----------

        /**
         * Move the new cursor refr pos by given movesteps, and if there are no core::Item's in in the new cell along one axis, then this method will look for the
         * first available core::Item along the new axis (axis perpendicular to the moveStep axis) closest to the expected position, with the newfound pos' delta value stored in a "ghost" pos.
         * 
         * Move to new pos from given _moveSteps without jumping steps, then if there are no core::Item's there, search for the closest core::Item, prioritising "closest" as the first along
         * moveDirection moveset.
         * 
         */

        // if(_moveSteps==winNavCursorPos_deltaGhostOffset) winNavCursorPos_deltaGhostOffset = Pos2d<int>{0, 0};
        refrNewPos += _moveSteps;
        // refrNewPos -= winNavCursorPos_deltaGhostOffset;
        if(!(refrNewPos.x>=0 && refrNewPos.x<groupDim.x)) {
            if(!windowOptions.whenCursorOutOfBoundsReEnter) return result_moveNavCursor::out_of_bounds;
            refrNewPos.x = (refrNewPos.x<0? groupDim.x-1 : 0);
        }
        if(!(refrNewPos.y>=0 && refrNewPos.y<groupDim.y)) {
            if(!windowOptions.whenCursorOutOfBoundsReEnter) return result_moveNavCursor::out_of_bounds;
            refrNewPos.y = (refrNewPos.y<0? groupDim.y-1 : 0);
        }
        
        if(groupItemMatrix.at(refrNewPos.y).at(refrNewPos.x).get_itemType()!=Item_types::window) {
            // winNavCursorPos_deltaGhostOffset = refrNewPos;
            Pos2d<int> xtemp_refrNewPos = refrNewPos;
            Pos2d<int> ytemp_refrNewPos = refrNewPos;

            Pos2d<size_t> stepsMoved{std::string::npos, std::string::npos}; /// hypotenus distance values of the found pos for respective axis search.

            if(moveDirection.x!=0) {
                int temp_x = xtemp_refrNewPos.x;
                bool funcFound = false;
                for(size_t _i=0; _i<groupDim.x; _i++) {
                    for(int _j=0; _j<groupDim.y; _j++) {
                        if(xtemp_refrNewPos.y-_j>=0) {
                            if(groupItemMatrix.at(xtemp_refrNewPos.y-_j).at(temp_x).get_itemType()==Item_types::function) {
                                xtemp_refrNewPos.x = temp_x;
                                xtemp_refrNewPos.y-= _j;
                                stepsMoved.x = xtemp_refrNewPos.getHypotenuse();
                                funcFound = true;
                                break;
                            }
                        }
                        if(xtemp_refrNewPos.y+_j<groupDim.y) {
                            if(groupItemMatrix.at(xtemp_refrNewPos.y+_j).at(temp_x).get_itemType()==Item_types::function) {
                                xtemp_refrNewPos.x = temp_x;
                                xtemp_refrNewPos.y+= _j;
                                stepsMoved.x = xtemp_refrNewPos.getHypotenuse();
                                funcFound = true;
                                break;
                            }
                        }
                    }
                    if(funcFound) break;
                    
                    temp_x+=moveDirection.x;
                    if(!(temp_x>=0 && temp_x<groupDim.x)) {
                        if(!windowOptions.whenCursorOutOfBoundsReEnter) return result_moveNavCursor::out_of_bounds;
                        temp_x = (temp_x<0? groupDim.x-1 : 0);
                    }
                }
            }
            if(moveDirection.y!=0) {
                int temp_y = ytemp_refrNewPos.y;
                bool funcFound = false;
                for(size_t _i=0; _i<groupDim.y; _i++) {
                    for(int _j=0; _j<groupDim.x; _j++) {
                        if(ytemp_refrNewPos.x-_j>=0) {
                            if(groupItemMatrix.at(temp_y).at(ytemp_refrNewPos.x-_j).get_itemType()==Item_types::function) {
                                ytemp_refrNewPos.y = temp_y;
                                ytemp_refrNewPos.x-= _j;
                                stepsMoved.y = ytemp_refrNewPos.getHypotenuse();
                                funcFound = true;
                                break;
                            }
                        }
                        if(ytemp_refrNewPos.x+_j<groupDim.x) {
                            if(groupItemMatrix.at(temp_y).at(ytemp_refrNewPos.x+_j).get_itemType()==Item_types::function) {
                                ytemp_refrNewPos.y = temp_y;
                                ytemp_refrNewPos.x+= _j;
                                stepsMoved.y = ytemp_refrNewPos.getHypotenuse();
                                funcFound = true;
                                break;
                            }
                        }
                    }
                    if(funcFound) break;

                    temp_y+=moveDirection.y;
                    if(!(temp_y>=0 && temp_y<groupDim.y)) {
                        if(!windowOptions.whenCursorOutOfBoundsReEnter) return result_moveNavCursor::out_of_bounds;
                        temp_y = (temp_y<0? groupDim.y-1 : 0);
                    }
                }
            }

            // if(moveDirection.x!=0 && moveDirection.y!=0) {
            // }
            
            if(stepsMoved==Pos2d<size_t>(std::string::npos, std::string::npos)) throw std::runtime_error(_infoStr+" : stepsMoved==std::string::npos. No new valid location found.");
            
            Pos2d<int> tempStepsMoved{0, 0};
            if(stepsMoved.x<stepsMoved.y) tempStepsMoved = xtemp_refrNewPos;
            else tempStepsMoved = ytemp_refrNewPos;

            // winNavCursorPos_deltaGhostOffset = tempStepsMoved - refrNewPos;
            refrNewPos = tempStepsMoved;
            
        }
        if(refrNewPos.x<0) throw std::logic_error(_infoStr+" : refrNewPos.x<0");
        if(refrNewPos.y<0) throw std::logic_error(_infoStr+" : refrNewPos.y<0");
        

        winNavCursorPos = refrNewPos.cast<size_t>();

        groupItemMatrix.at(winNavCursorPos.y).at(winNavCursorPos.x).isModified__text = true;
        // u_lck_accss_groupItemMatrix.unlock();

        // system("pause");
        
        
        return result_moveNavCursor::normal;
    }
    void core::Group::callItem() {
        const std::string _infoStr{"void core::Group::callItem()"};
        if(winNavCursorPos.x==std::string::npos) throw std::logic_error(_infoStr+" : winNavCursorPos.x==std::string::npos.");
        if(winNavCursorPos.y==std::string::npos) throw std::logic_error(_infoStr+" : winNavCursorPos.y==std::string::npos.");
        
        std::unique_lock<std::mutex> u_lck_accss_groupItemMatrix(mtx_access_groupItemMatrix);

        if(winNavCursorPos.y>=groupItemMatrix.size()) throw std::logic_error(_infoStr+" : winNavCursorPos.y>=groupItemMatrix.size().");
        if(winNavCursorPos.x>=groupItemMatrix.at(0).size()) throw std::logic_error(_infoStr+" : winNavCursorPos.x>=groupItemMatrix.at(0).size().");
        
        if(groupItemMatrix.at(winNavCursorPos.y).at(winNavCursorPos.x).get_itemType()!=Item_types::function) throw std::logic_error(_infoStr+" : core::Item at given winNavCursorPos"+std::string(winNavCursorPos)+" is not Item_types::function type.");
        
        
        groupItemMatrix.at(winNavCursorPos.y).at(winNavCursorPos.x).callItem(parentWindowPtr);
        
    }
    void core::Group::update_axisMaxSizeDefVectors() {
        const std::string _infoStr{"void core::Group::update_axisMaxSizeDefVectors()"};
        std::unique_lock<std::mutex> u_lck_accss_groupItemMatrix(mtx_access_groupItemMatrix, std::defer_lock);

        u_lck_accss_groupItemMatrix.lock();
        if(groupItemMatrix.size()==0) {
            throw std::logic_error(_infoStr+" : groupItemMatrix.size() is not allowed to be 0.");
        }
        if(groupItemMatrix.at(0).size()==0) {
            throw std::logic_error(_infoStr+" : groupItemMatrix.at(0).size() is not allowed to be 0.");
        }

        std::vector<size_t> maxSize_rows(groupItemMatrix.size(), 0);
        std::vector<size_t> maxSize_columns(groupItemMatrix[0].size(), 0);
        
        for(size_t _y=0; _y<groupItemMatrix.size(); _y++) {
            for(size_t _x=0; _x<groupItemMatrix.at(_y).size(); _x++) {
                const core::Item& _item = groupItemMatrix.at(_y).at(_x);
                if(_item.get_itemType()!=Item_types::text) continue;
                std::vector<std::string> _itemTextBox =_item.get_text();
                Pos2d<size_t> textBoxDim{0, 0};
                for(std::string _line : _itemTextBox) {
                    if(_line.size()>textBoxDim.x) textBoxDim.x=_line.size();
                }
                if(textBoxDim.x>0) textBoxDim.y = _itemTextBox.size();

                if(textBoxDim.x>maxSize_columns.at(_x)) maxSize_columns.at(_x) = textBoxDim.x;
                if(textBoxDim.y>maxSize_rows.at(_y)) maxSize_rows.at(_y) = textBoxDim.y;
            }
        }


        axisMaxSizeDef.at(0) = maxSize_columns;
        axisMaxSizeDef.at(1) = maxSize_rows;

    }
    void core::Group::update_axisMaxSizeAdjVectors() {
        const std::string _infoStr{"void core::Group::update_axisMaxSizeAdjVectors()"};
        std::unique_lock<std::mutex> u_lck_accss_groupItemMatrix(mtx_access_groupItemMatrix, std::defer_lock);

        
        this->update_axisMaxSizeDefVectors();

        u_lck_accss_groupItemMatrix.lock();
        if(groupItemMatrix.size()==0) {
            throw std::logic_error(_infoStr+" : groupItemMatrix.size() is not allowed to be 0.");
        }
        if(groupItemMatrix.at(0).size()==0) {
            throw std::logic_error(_infoStr+" : groupItemMatrix.at(0).size() is not allowed to be 0.");
        }
        
        if(groupStyleInfo.posDim.get_dim().x==0) {
            throw std::logic_error(_infoStr+" : groupStyleInfo.posDim.get_dim().x is not allowed to be 0.");
        }
        if(groupStyleInfo.posDim.get_dim().y==0) {
            throw std::logic_error(_infoStr+" : groupStyleInfo.posDim.get_dim().y is not allowed to be 0");
        }


        std::vector<size_t> maxSize_rows(groupItemMatrix.size(),0);
        std::vector<size_t> maxSize_columns(groupItemMatrix.at(0).size(),0);

        Pos2d<size_t>   totItemSize{0, 0};
        Pos2d<float>    avgItemSize{0, 0};

        if(groupStyleInfo.posDim.get_dim().x==std::string::npos || groupStyleInfo.posDim.get_dim().y==std::string::npos) {

            for(const auto& _lineOfItems : groupItemMatrix) {
                for(const core::Item& _item : _lineOfItems) {
                    size_t maxStrLen=0;
                    for(std::string _line : _item.get_text()) {
                        if(_line.size()>maxStrLen) maxStrLen = _line.size();
                    }
                    totItemSize.x+=maxStrLen;
                    totItemSize.y+=_item.get_text().size();
                }
            }

            totItemSize.x+=groupStyleInfo.symbs.border_column.size()*2+groupStyleInfo.symbs.delimiter_columns.size()*(groupItemMatrix.at(0).size()-1);
            totItemSize.y+=1*2+1*(groupItemMatrix.size()-1);

            avgItemSize.x = totItemSize.x/groupItemMatrix.at(0).size();
            avgItemSize.y = totItemSize.y/groupItemMatrix.size();
            
        }
        else {
            
            totItemSize.x = static_cast<size_t>(groupStyleInfo.posDim.get_dim().x-groupStyleInfo.symbs.border_column.size()*2-(maxSize_columns.size()-1)*groupStyleInfo.symbs.delimiter_columns.size());
            totItemSize.y = static_cast<size_t>(groupStyleInfo.posDim.get_dim().y-(1)*2-(groupItemMatrix.size()-1)*groupStyleInfo.symbs.delimiter_rows.size());

            avgItemSize.x = static_cast<int>(
                totItemSize.x
            )/static_cast<int>(
                maxSize_columns.size()
            );
            avgItemSize.y = static_cast<int>(
                totItemSize.y
            )/static_cast<int>(
                groupItemMatrix.size()
            );

        }
        
        u_lck_accss_groupItemMatrix.unlock();
        

        if(avgItemSize.x<0) {
            maxSize_columns = std::vector<size_t>(maxSize_columns.size(), 0);
        }
        else {
            for(size_t _i=0; _i<maxSize_columns.size()-1; _i++) maxSize_columns.at(_i) = (avgItemSize.x<0? 0 : avgItemSize.x);
            maxSize_columns.back() = (totItemSize.x-(maxSize_columns.size()-1)*avgItemSize.x);
        }
        if(avgItemSize.y<0) {
            maxSize_rows = std::vector<size_t>(maxSize_rows.size(), 0);
        }
        else {
            for(size_t _i=0; _i<maxSize_rows.size()-1; _i++) maxSize_rows.at(_i) = (avgItemSize.y<0? 0 : avgItemSize.y);
            maxSize_rows.back() = (totItemSize.y-(maxSize_rows.size()-1)*avgItemSize.y);

        }

        // axisMaxSizeAdj = axisMaxSizeDef; ///temporary solution
        // return;

        axisMaxSizeAdj.at(0) = maxSize_columns;
        axisMaxSizeAdj.at(1) = maxSize_rows;
        
        
        isModified__axisMaxSize = true;

    }
    void core::Group::update_groupPtrInItems() {
        for(auto& _row : groupItemMatrix) {
            for(auto& _item : _row) _item.parentGroupPtr = this;
        }
    }
    void core::Group::update_PSVmatrix() {
        const std::string _infoStr{"void core::Group::update_PSVmatrix"};
        
        if(groupItemMatrix.size()==0) return;
        if(groupItemMatrix.at(0).size()==0) return;

        std::unique_lock<std::mutex> u_lck_accss_groupItemMatrix(mtx_access_groupItemMatrix, std::defer_lock);
        std::unique_lock<std::mutex> u_lck_accss_PSVmatrix(mtx_access_PSVmatrix, std::defer_lock);

        
        // this->update_axisMaxSizeAdjVectors();
        if(CONSOLE_DIMENSIONS_MODIFIED) this->update_axisMaxSizeAdjVectors();

        
        u_lck_accss_PSVmatrix.lock();
        if(groupStyleInfo.posDim.get_dim().x==std::string::npos || groupStyleInfo.posDim.get_dim().y==std::string::npos) {
            // groupStyleInfo.posDim.get_dim() = Pos2d<size_t>{0, 0};
            Pos2d<size_t> tempPos{0, 0};
            for(auto column_width   : axisMaxSizeAdj.at(0)) tempPos.x+=column_width;
            for(auto row_height     : axisMaxSizeAdj.at(1)) tempPos.y+=row_height;

            tempPos.x+=2*groupStyleInfo.symbs.border_column.size()+(axisMaxSizeAdj.at(0).size()-1)*groupStyleInfo.symbs.delimiter_columns.size();
            tempPos.y+=2+(axisMaxSizeAdj.at(1).size()-1)*1;

            groupStyleInfo.posDim.set_TL({0, 0});
            groupStyleInfo.posDim.set_dim(tempPos);
        }
        
        
        Pos2d<int> diffCount{
            static_cast<int>(groupStyleInfo.posDim.get_dim().x)-static_cast<int>(previousGroupDimensions.x),
            static_cast<int>(groupStyleInfo.posDim.get_dim().y)-static_cast<int>(previousGroupDimensions.y)
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


        // groupStyleInfo.posDim.get_dim().x = (PrintableStringVectorMatrix.size()==0? 0 : PrintableStringVectorMatrix.at(0).size());
        // groupStyleInfo.posDim.get_dim().y = PrintableStringVectorMatrix.size();

        if(diffCount.x!=0 || diffCount.y!=0) PSVmatrixCleared = true;

        if(PSVmatrixCleared) {
            for(std::string& _line : PrintableStringVectorMatrix) {
                _line = std::string(_line.size(), ' ');
            }
        }
        

        previousGroupDimensions.x = (PrintableStringVectorMatrix.size()==0? 0 : PrintableStringVectorMatrix.at(0).size());
        previousGroupDimensions.y = PrintableStringVectorMatrix.size();
        u_lck_accss_PSVmatrix.unlock();

        /// Text rendering of the Items' text's stored in this Group object. Only modified text is updated.
        
        Pos2d<size_t> cursorPos_edit(groupStyleInfo.symbs.border_column.size(), 1);
        u_lck_accss_groupItemMatrix.lock();
        for(size_t item_y=0; item_y<groupItemMatrix.size(); item_y++) {
            for(size_t char_y=0; char_y<axisMaxSizeAdj[1].at(item_y); char_y++) {
                cursorPos_edit.x=groupStyleInfo.symbs.border_column.size();
                for(size_t item_x=0; item_x<groupItemMatrix.at(0).size(); item_x++) {
                    core::Item& itemRef = groupItemMatrix.at(item_y).at(item_x);
                    const size_t& itemColMaxSize = axisMaxSizeAdj[0].at(item_x);
                    
                    if(
                        itemRef.isDefined__text &&
                        (itemRef.isModified__text || PSVmatrixCleared) && itemRef.itemStyleInfo.cont_text.rule_followDelimiter
                    ) {
                        std::string textLine{""};

                        std::vector<std::string> item_text = itemRef.get_text(itemColMaxSize);
                        if(item_text.size()>char_y) {
                            textLine += item_text.at(char_y);
                            if(textLine.size()<=itemColMaxSize) {
                                textLine+=std::string(itemColMaxSize-textLine.size(), ' ');
                            }
                            else {
                                textLine.erase(itemColMaxSize);
                            }
                            // assert(textLine.size()==itemColMaxSize);
                            // memcpy(&PrintableStringVectorMatrix.at(cursorPos_edit.y+char_y).at(cursorPos_edit.x), &textLine.at(0), textLine.size());
                            
                            // if(item_text.size()<=char_y+1) itemRef.isModified__text = false;
                        }
                        else {
                            // std::string tempStr_emptyItemLine(itemColMaxSize, ' ');
                            // memcpy(&PrintableStringVectorMatrix.at(cursorPos_edit.y+char_y).at(cursorPos_edit.x), &tempStr_emptyItemLine.at(0), tempStr_emptyItemLine.size());
                            textLine += std::string(itemColMaxSize, ' ');
                            
                        }

                        u_lck_accss_PSVmatrix.lock();
                        try {
                            memcpy(&PrintableStringVectorMatrix.at(cursorPos_edit.y+char_y).at(cursorPos_edit.x), &textLine.at(0), textLine.size());
                            
                        }
                        catch(const std::exception& e) {
                            DEBUGPRINT1(std::string("[check 2]:char_y:")+fmtToStr(char_y,0,0))
                            DEBUGPRINT1(std::string("[check 2]:cursorPos_edit.y+char_y:")+fmtToStr(cursorPos_edit.y+char_y,0,0))
                            DEBUGPRINT1(std::string("[check 2]:PrintableStringVectorMatrix.size():")+fmtToStr(PrintableStringVectorMatrix.size(),0,0))
                            throw std::runtime_error(_infoStr+" : "+e.what());
                        }
                        
                        u_lck_accss_PSVmatrix.unlock();

                        isModified__PSVmatrix = true;
                    }

                    cursorPos_edit.x+=itemColMaxSize;
                    cursorPos_edit.x+=groupStyleInfo.symbs.delimiter_columns.size();
                    
                }
            }
            cursorPos_edit.y+=axisMaxSizeAdj[1].at(item_y);
            cursorPos_edit.y+=groupStyleInfo.symbs.delimiter_rows.size();
        }
        // system("pause");
        
        /// Draw border/delimiter symbols

        Pos2d<size_t> _tempPos_xy(groupStyleInfo.symbs.border_column.size(), 0);
        u_lck_accss_PSVmatrix.lock();
        for(size_t item_y=0; item_y<groupItemMatrix.size(); item_y++) {
            /// Draw column delimiters
            for(size_t rowHeight_y=0; rowHeight_y<axisMaxSizeAdj[1].at(item_y); rowHeight_y++) {
                _tempPos_xy.x = 0;
                for(size_t item_x=0; item_x<axisMaxSizeAdj[0].size()-1; item_x++) {
                    _tempPos_xy.x+=axisMaxSizeAdj[0].at(item_x)+1;
                    memcpy(&PrintableStringVectorMatrix.at(_tempPos_xy.y+rowHeight_y+1).at(_tempPos_xy.x), &groupStyleInfo.symbs.delimiter_columns.at(0), groupStyleInfo.symbs.delimiter_columns.size());
                }
            }

            /// Draw row delimiters
            _tempPos_xy.y+=axisMaxSizeAdj[1].at(item_y);
            if(item_y<groupItemMatrix.size()-1) {
                for(size_t _colFilled=groupStyleInfo.symbs.border_column.size(); _colFilled<groupStyleInfo.posDim.get_dim().x-groupStyleInfo.symbs.delimiter_rows.size(); _colFilled+=groupStyleInfo.symbs.delimiter_rows.size()) {
                    memcpy(&PrintableStringVectorMatrix.at(_tempPos_xy.y+1).at(_colFilled), &groupStyleInfo.symbs.delimiter_rows.at(0), groupStyleInfo.symbs.delimiter_rows.size());
                }
            }
            _tempPos_xy.y+=1;
        }
        u_lck_accss_PSVmatrix.unlock();
        u_lck_accss_groupItemMatrix.unlock();

        /// Draw ceiling/floor border rows

        size_t rowColumnsFilled = 0;
        u_lck_accss_PSVmatrix.lock();
        for(; rowColumnsFilled<groupStyleInfo.posDim.get_dim().x-groupStyleInfo.symbs.border_row.size(); rowColumnsFilled+=this->groupStyleInfo.symbs.border_row.size()) {
            memcpy(&PrintableStringVectorMatrix.at(0).at(rowColumnsFilled),   &groupStyleInfo.symbs.border_row.at(0), groupStyleInfo.symbs.border_row.size());    /// ceiling border
            memcpy(&PrintableStringVectorMatrix.back().at(rowColumnsFilled),  &groupStyleInfo.symbs.border_row.at(0), groupStyleInfo.symbs.border_row.size());    /// floor border
        }
        memcpy(&PrintableStringVectorMatrix.at(0).at(groupStyleInfo.posDim.get_dim().x-groupStyleInfo.symbs.border_row.size()), &groupStyleInfo.symbs.border_row.at(0), (groupStyleInfo.posDim.get_dim().x-rowColumnsFilled));
        memcpy(&PrintableStringVectorMatrix.back().at(groupStyleInfo.posDim.get_dim().x-groupStyleInfo.symbs.border_row.size()),&groupStyleInfo.symbs.border_row.at(0), groupStyleInfo.posDim.get_dim().x-rowColumnsFilled);

        /// Draw left/right border columns

        for(size_t _y=1; _y<groupStyleInfo.posDim.get_dim().y-1; _y++) {
            memcpy(&PrintableStringVectorMatrix.at(_y).at(0), &groupStyleInfo.symbs.border_column.at(0), groupStyleInfo.symbs.border_column.size());
            memcpy(&PrintableStringVectorMatrix.at(_y).at(groupStyleInfo.posDim.get_dim().x-groupStyleInfo.symbs.border_column.size()), &groupStyleInfo.symbs.border_column.at(0), groupStyleInfo.symbs.border_column.size());
        }

        
        /// Draw corners
        memcpy(&PrintableStringVectorMatrix.at(0).at(0),    &groupStyleInfo.symbs.border_corner.at(0), groupStyleInfo.symbs.border_corner.size());
        memcpy(&PrintableStringVectorMatrix.at(0).back(),   &groupStyleInfo.symbs.border_corner.at(0), groupStyleInfo.symbs.border_corner.size());
        memcpy(&PrintableStringVectorMatrix.back().at(0),   &groupStyleInfo.symbs.border_corner.at(0), groupStyleInfo.symbs.border_corner.size());
        memcpy(&PrintableStringVectorMatrix.back().back(),  &groupStyleInfo.symbs.border_corner.at(0), groupStyleInfo.symbs.border_corner.size());
        u_lck_accss_PSVmatrix.unlock();
        
        /// Draw text that ignores/goes-over border/delimiter symbols


        cursorPos_edit = Pos2d<size_t>(groupStyleInfo.symbs.border_column.size(), groupStyleInfo.symbs.border_row.size());

        u_lck_accss_groupItemMatrix.lock();
        for(size_t item_y=0; item_y<groupItemMatrix.size(); item_y++) { /// iterate over each row of cells

            for(size_t _row_y=0; _row_y<axisMaxSizeAdj[1].at(item_y); _row_y++) { /// iterate over each row/character-height of each cell-row

                cursorPos_edit.x=1; /// set start indexing pos at new row's first character after border column
                for(size_t item_x=0; item_x<groupItemMatrix.at(0).size(); item_x++) { /// iterate over each column of cells
                    core::Item& itemRef = groupItemMatrix.at(item_y).at(item_x);
                    
                    if(
                        itemRef.isDefined__text &&
                        (itemRef.isModified__text || PSVmatrixCleared) && !itemRef.itemStyleInfo.cont_text.rule_followDelimiter
                    ) {
                        std::string textLine{""};
                        std::vector<std::string> item_text = itemRef.get_text();
                        if(item_text.size()>_row_y) { /// !NOTE: Will need further revision to account for strings shorter than previous iterations
                            textLine = item_text.at(_row_y);
                            if(cursorPos_edit.x+textLine.size()>=groupStyleInfo.posDim.get_dim().x) textLine.erase(groupStyleInfo.posDim.get_dim().x-cursorPos_edit.x);
                            
                            // memcpy(&PrintableStringVectorMatrix.at(cursorPos_edit.y).at(cursorPos_edit.x), &textLine.at(0), axisMaxSizeAdj[0].at(item_x));
                        }
                        else {
                            // std::string tempStr_emptyCellLine(axisMaxSizeAdj[0].at(item_x), ' ');
                            textLine = std::string(axisMaxSizeAdj[0].at(item_x), ' ');
                        }
                        u_lck_accss_PSVmatrix.lock();
                        memcpy(&PrintableStringVectorMatrix.at(cursorPos_edit.y).at(cursorPos_edit.x), &textLine.at(0), axisMaxSizeAdj[0].at(item_x));
                        u_lck_accss_PSVmatrix.unlock();

                        isModified__PSVmatrix = true;
                    }
                    
                    itemRef.isModified__text = false;

                    cursorPos_edit.x+=axisMaxSizeAdj[0].at(item_x);
                    cursorPos_edit.x+=groupStyleInfo.symbs.delimiter_columns.size(); /// skip over PSV matrix's column delimiter
                }

            }
            cursorPos_edit.y+=axisMaxSizeAdj[1].at(item_y);
            cursorPos_edit.y+=groupStyleInfo.symbs.delimiter_rows.size(); /// skip over PSV matrix's row delimiter
        }
        u_lck_accss_groupItemMatrix.unlock();

        PSVmatrixCleared = false;

        this->update_groupPtrInItems();
        
    }
    /// !!!NOTE: This needs to be changed to acount for the new simpleTUI2::style types and overloads.
    void core::Group::resize_groupItemMatrix(int axisDiff_x, int axisDiff_y) {
        std::unique_lock<std::mutex> u_lck_accss_groupItemMatrix(mtx_access_groupItemMatrix);

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
    void core::Group::LoadInitialiserItemMatrix(std::initializer_list<std::initializer_list<core::Item>>& _matrixInput) {
        const std::string _infoStr{"void core::Group::LoadInitialiserItemMatrix(std::initializer_list<std::initializer_list<core::Item>>&)"};
        
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
        if(limDefined_min.x && !limDefined_max.x) {
            // assert(limDefined_max.x && " how the fuck is minimum defined but not maximum for x");
            throw std::logic_error(_infoStr+" : limDefined_min.x && !limDefined_max.x");
        }
        if(limDefined_min.y && !limDefined_max.y) {
            // assert(limDefined_max.y && " how the fuck is minimum defined but not maximum for y");
            throw std::logic_error(_infoStr+" : limDefined_min.y && !limDefined_max.y");
        }
        if(lineCountedDim.x > limMatrix_max.x-limMatrix_min.x) { //the defined limits are smaller than the existing number of cells for a row
            limMatrix_max.x = lineCountedDim.x;
        }
        if(lineCountedDim.y > limMatrix_max.y-limMatrix_min.y) {
            limMatrix_max.y = lineCountedDim.y;
        }

        std::unique_lock<std::mutex> u_lck_accss_groupItemMatrix(mtx_access_groupItemMatrix);
        groupItemMatrix = std::vector<std::vector<core::Item>>(
            limMatrix_max.y, std::vector<core::Item>(
                limMatrix_max.x, {Item_types::null}
            )
        );
        
        
        Pos2d<size_t> lineCountedPos = limMatrix_min;
        
        for(auto itr_row=_matrixInput.begin(); itr_row!=_matrixInput.end(); ++itr_row) {
            lineCountedPos.x = limMatrix_min.x;
            for(auto itr_item=itr_row->begin(); itr_item!=itr_row->end(); ++itr_item) {

                if(itr_item->isDefined__pos) {
                    groupItemMatrix[itr_item->posInParentGroup.y][itr_item->posInParentGroup.x] = *itr_item;
                }
                else {
                    groupItemMatrix[lineCountedPos.y][lineCountedPos.x] = *itr_item;
                    groupItemMatrix[lineCountedPos.y][lineCountedPos.x].set_posInParentGroup(lineCountedPos);
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
        for(size_t _y=0; _y<groupItemMatrix.size(); _y++) {
            for(size_t _x=0; _x<groupItemMatrix.at(_y).size(); _x++) {
                core::Item& itemRef = groupItemMatrix.at(_y).at(_x);
                if(!itemRef.isDefined__pos) itemRef.set_posInParentGroup({_x, _y});
            }
        }

    }
    std::string core::Group::print_insertPrintSettings(std::string& _toPrintLine, size_t _currentPSVmatrixLine) {
        const std::string _infoStr{"std::string core::Group::print_insertPrintSettings(std::string& _toPrintLine, size_t _currentPSVmatrixLine)"};

        if(PrintableStringVectorMatrix.size()==0) throw std::runtime_error(_infoStr+" : PSVmatrix.size()==0.");
        if(PrintableStringVectorMatrix.at(0).size()==0) throw std::runtime_error(_infoStr+" : PSVmatrix.at(0).size()==0.");
        if(_currentPSVmatrixLine>PrintableStringVectorMatrix.size()-1) throw std::invalid_argument(_infoStr+" : _currentPSVmatrixLine>PSVmatrix.size()-1.");
        // if(_currentPSVmatrixLine==1&&groupStyleInfo.symbs.border_row.size()>0) throw std::runtime_error(_infoStr+" : _currentPSVmatrixLine is <= than border_row symbol size.");

        if((winNavCursorPos.x==std::string::npos || winNavCursorPos.y==std::string::npos) && !winNav_isSelected) return _toPrintLine;
        //if(_currentPSVmatrixLine==(0)&&groupStyleInfo.symbs.border_row.size()>0) return _toPrintLine;
        
        //assert(winNavCursorPos.y<groupItemMatrix.size());

        bool highlightBorder = (winNav_isSelected && (winNavCursorPos.x==std::string::npos || winNavCursorPos.y==std::string::npos));
        
        Pos2d<size_t> itemPos{std::string::npos, std::string::npos};
        std::string ansiInsertStr_activCol{ANSIec::esc_code+groupStyleInfo.ANSI.highlighted_textColour+";"+groupStyleInfo.ANSI.highlighted_backgroundColour+"m"};
        std::string ansiInsertStr_closeCol{ANSIec::esc_code+groupStyleInfo.ANSI.default_textColour+";"+groupStyleInfo.ANSI.default_backgroundColour+"m"};

        /*
            TL.y = groupStyleInfo.symbs.border_row + (item_y)*groupStyleInfo.symbs.delimiter_row + summation(axisMaxSize.y(item_y-1)) 
            
             => TL.y - groupStyleInfo.symbs.border_row = (item_y)*groupStyleInfo.symbs.delimiter_row + summation(axisMaxSize.y(item_y-1)) 
             => TL.y - groupStyleInfo.symbs.border_row - groupStyleInfo.symbs.delimiter_row = summation[_i=0; _i<item_y-1] { axisMaxSize.y(_i)+groupStyleInfo.symbs.delimiter_row }
        */

        // const auto lambda_vectorElemSum = [](std::vector<size_t>& _vecRef, size_t _upToIdx) {
        //     size_t sumRowHeights=0;
        //     for(size_t _i=0; _i<_upToIdx; _i++) sumRowHeights+=_vecRef.at(_i);
        //     return sumRowHeights;
        // };


        /// ---------- New method ----------
        
        auto hlString = [&](std::string& _strToInsert, size_t _start, size_t _num) {
            _strToInsert.insert(_start, ansiInsertStr_activCol);
            if(_num==std::string::npos) _num = _strToInsert.size();
            else _num += ansiInsertStr_activCol.size();
            _strToInsert.insert(_start+_num, ansiInsertStr_closeCol);
            return _strToInsert;
        };
        
        size_t len_aCol=ansiInsertStr_activCol.size();
        size_t len_cCol=ansiInsertStr_closeCol.size();
        
        size_t len_delSymbCol=groupStyleInfo.symbs.delimiter_columns.size();
        size_t len_delSymbRow=groupStyleInfo.symbs.delimiter_rows.size();
        size_t len_bordSymbCol=groupStyleInfo.symbs.border_column.size();
        size_t len_bordSymbRow=groupStyleInfo.symbs.border_row.size();
        
        size_t travelled_y_dist = 0;
        size_t travelled_x_dist = 0;
        std::string lineCopy{_toPrintLine};
        if(_currentPSVmatrixLine==travelled_y_dist || _currentPSVmatrixLine==PrintableStringVectorMatrix.size()-(1)) {
            if(highlightBorder) {
                hlString(lineCopy, 0, lineCopy.size());
                return lineCopy;
            }
            return lineCopy;
        }
        travelled_y_dist+=1; // char-height of the row border symbol.
        
        if(_currentPSVmatrixLine<travelled_y_dist+axisMaxSizeAdj[1].at(0)+(1)) {
            if(highlightBorder) {
                hlString(lineCopy, lineCopy.size()-len_bordSymbCol, len_bordSymbCol); // insert highlight ansi code at the end border column
                hlString(lineCopy, 0, len_bordSymbCol); // insert highlight ansi code at the start border column
                return lineCopy;
            }
            if(winNavCursorPos.y!=0 || _currentPSVmatrixLine>=travelled_y_dist+axisMaxSizeAdj[1].at(0)) return _toPrintLine;
            itemPos.y = 0;
        }
        else {
            travelled_y_dist+=axisMaxSizeAdj[1].at(0);
            if(highlightBorder && (_currentPSVmatrixLine==(1))) { // first delimiter row highlight.
                hlString(lineCopy, lineCopy.size()-len_bordSymbCol, len_bordSymbCol);
                hlString(lineCopy, 0, len_bordSymbCol);
                return lineCopy;
            }
            travelled_y_dist+=(1);
            for(size_t _i=1; _i<groupItemMatrix.size(); _i++) {
                if(_currentPSVmatrixLine<=travelled_y_dist+axisMaxSizeAdj[1].at(_i)) {
                    if(highlightBorder) {
                        hlString(lineCopy, lineCopy.size()-len_bordSymbCol, len_bordSymbCol);
                        hlString(lineCopy, 0, len_bordSymbCol);
                        return lineCopy;
                    }
                    if(winNavCursorPos.y!=_i || _currentPSVmatrixLine>=travelled_y_dist+axisMaxSizeAdj[1].at(_i)) return _toPrintLine;
                    itemPos.y = _i;
                    break;
                }
                else {
                    travelled_y_dist+=axisMaxSizeAdj[1].at(_i);
                    if(highlightBorder) {
                        hlString(lineCopy, lineCopy.size()-len_bordSymbCol, len_bordSymbCol);
                        hlString(lineCopy, 0, len_bordSymbCol);
                        return lineCopy;
                    }
                    if(_i<groupItemMatrix.size()-1) travelled_y_dist+=(1); //char height of row delimiter.
                    else travelled_y_dist+=(1); //char height of bottom row border
                }
            }
        }
        
        if(itemPos.y==std::string::npos) {
            throw std::runtime_error("invalid itemPos.y situation.");
        }
        
        travelled_x_dist = len_bordSymbCol;
        for(size_t _i=0; _i<groupItemMatrix.at(0).size(); _i++) {
            if(winNavCursorPos.x==_i) {
                hlString(lineCopy, travelled_x_dist, axisMaxSizeAdj[0].at(_i));
                return lineCopy;
            }
            travelled_x_dist+=axisMaxSizeAdj[0].at(_i);
            travelled_x_dist+=len_delSymbCol;
        }
        return lineCopy;
        
        /*
        /// ---------- Old method ----------
        
        size_t travelled_y_dist=1; //default groupStyleInfo.symbs.border_row

        if(_currentPSVmatrixLine>=travelled_y_dist && _currentPSVmatrixLine<travelled_y_dist+axisMaxSizeAdj[1].at(0)+(1)) { /// lineIdx is bigger than top row border, but smaller than next row delimiter idx.
            if(winNavCursorPos.y!=0 || _currentPSVmatrixLine>=travelled_y_dist+axisMaxSizeAdj[1].at(0)) return _toPrintLine;
            itemPos.y = 0;
        }
        else { 
            travelled_y_dist+=axisMaxSizeAdj[1].at(0)+(1);
            for(size_t _i=1; _i<groupItemMatrix.size(); _i++) {

                if(_currentPSVmatrixLine>=travelled_y_dist && _currentPSVmatrixLine<travelled_y_dist+axisMaxSizeAdj[1].at(_i)+(1)) {
                    if(winNavCursorPos.y!=_i || _currentPSVmatrixLine>=travelled_y_dist+axisMaxSizeAdj[1].at(_i)) return _toPrintLine;
                    assert(_i!=0);
                    
                    itemPos.y = _i;
    
                    break;
                }
                else {
                    
                    travelled_y_dist+=axisMaxSizeAdj[1].at(_i)+(1);
                }
            }
        }
        if(itemPos.y==std::string::npos) {
            if(winNavCursorPos.y!=groupItemMatrix.size()-1) return _toPrintLine;
            itemPos.y = groupItemMatrix.size()-1;
        }

        size_t travelled_x_dist=groupStyleInfo.symbs.border_column.size();
        std::string _tempStr{_toPrintLine};
        for(size_t _i=0; _i<groupItemMatrix.at(0).size(); _i++) {
            if(winNavCursorPos.x==_i) {
                _tempStr.insert(travelled_x_dist, ansiInsertStr_activCol);
                _tempStr.insert(travelled_x_dist+axisMaxSizeAdj[0].at(_i)+ansiInsertStr_activCol.size(), ansiInsertStr_closeCol);
                // _tempStr.insert(travelled_x_dist+44, ansiInsertStr_closeCol);

                // _tempStr.insert(travelled_x_dist+axisMaxSizeAdj[0].at(_i), "&");
                break;
            }
            travelled_x_dist+=axisMaxSizeAdj[0].at(_i);
            travelled_x_dist+=groupStyleInfo.symbs.delimiter_columns.size();
        }

        return _tempStr;*/
    }
    Pos2d<size_t> core::Group::get_PSVmatrix_dim() {
        std::unique_lock<std::mutex> u_lck_accss_groupItemMatrix(mtx_access_PSVmatrix);

        return Pos2d<size_t>{(PrintableStringVectorMatrix.size()>0? PrintableStringVectorMatrix.at(0).size() : 0), PrintableStringVectorMatrix.size()};
    }
    Pos2d<size_t> core::Group::get_groupItemMatrix_dim() {
        std::unique_lock<std::mutex> u_lck_accss_groupItemMatrix(mtx_access_groupItemMatrix);

        return Pos2d<size_t>{(groupItemMatrix.size()>0? groupItemMatrix.at(0).size() : 0), groupItemMatrix.size()};
    }
    std::string core::Group::get_PSVmatrix_line(Pos2d<size_t> _TLpos, size_t _lineLen) {
        const std::string _infoStr{"std::string core::Group::get_PSVmatrix_line(Pos2d<size_t>, size_t)"};
        std::unique_lock<std::mutex> u_lck_accss_PSVmatrix(mtx_access_PSVmatrix, std::defer_lock);

        if(PrintableStringVectorMatrix.size()==0) throw std::logic_error(_infoStr+" : PrintableStringVectorMatrix.size()==0.");
        if(PrintableStringVectorMatrix.at(0).size()==0) throw std::logic_error(_infoStr+" : PrintableStringVectorMatrix.at(0).size()==0.");
        if(_TLpos.y>=PrintableStringVectorMatrix.size()) throw std::invalid_argument(_infoStr+" : _TLpos.y>=PrintableStringVectorMatrix.size()   ("+std::to_string(_TLpos.y)+">="+std::to_string(PrintableStringVectorMatrix.size())+").");
        if(_TLpos.x>=PrintableStringVectorMatrix.at(0).size()) throw std::invalid_argument(_infoStr+" : _TLpos.x>=PrintableStringVectorMatrix.at(0).size()   ("+std::to_string(_TLpos.x)+">="+std::to_string(PrintableStringVectorMatrix.at(0).size())+").");
        if(_lineLen==0) throw std::invalid_argument(_infoStr+" : _lineLen==0.");
        if(_lineLen!=std::string::npos && (_TLpos.x+_lineLen>PrintableStringVectorMatrix.at(0).size())) throw std::invalid_argument(_infoStr+" : _TLpos.x+_lineLen>PrintableStringVectorMatrix.at(0).size()   ("+std::to_string(_TLpos.x)+"+"+std::to_string(_lineLen)+">"+std::to_string(PrintableStringVectorMatrix.at(0).size())+").");

        std::string _line{""};
        
        _line = PrintableStringVectorMatrix.at(_TLpos.y).substr(_TLpos.x, _lineLen);

        return _line;
    }
    core::Group::Group(std::initializer_list<std::initializer_list<core::Item>> _matrixInput) {
        this->LoadInitialiserItemMatrix(_matrixInput);

    }
    core::Group& core::Group::operator=(std::initializer_list<std::initializer_list<core::Item>> _matrixInput) {
        this->LoadInitialiserItemMatrix(_matrixInput);

        return *this;
    }
    core::Group::Group() {

    }
    core::Group::Group(const Group& _toCopy):
        groupItemMatrix(_toCopy.groupItemMatrix), groupStyleInfo(_toCopy.groupStyleInfo), axisMaxSizeAdj(_toCopy.axisMaxSizeAdj),
        PrintableStringVectorMatrix(_toCopy.PrintableStringVectorMatrix)
    {

    }
    core::Group::Group(Group&& _toMove):
        groupItemMatrix(std::move(_toMove.groupItemMatrix)), groupStyleInfo(std::move(_toMove.groupStyleInfo)), axisMaxSizeAdj(std::move(_toMove.axisMaxSizeAdj)),
        PrintableStringVectorMatrix(std::move(_toMove.PrintableStringVectorMatrix))
    {
        
    }
    core::Group::~Group() {

    }
    core::Group& core::Group::operator=(const Group& _toCopy) {
        groupItemMatrix     = _toCopy.groupItemMatrix;
        groupStyleInfo      = _toCopy.groupStyleInfo;
        axisMaxSizeAdj         = _toCopy.axisMaxSizeAdj;
        PrintableStringVectorMatrix = _toCopy.PrintableStringVectorMatrix;

        return *this;
    }
    core::Group& core::Group::operator=(Group&& _toMove) {
        groupItemMatrix     = std::move(_toMove.groupItemMatrix);
        groupStyleInfo      = std::move(_toMove.groupStyleInfo);
        axisMaxSizeAdj         = std::move(_toMove.axisMaxSizeAdj);
        PrintableStringVectorMatrix = std::move(_toMove.PrintableStringVectorMatrix);
        
        return *this;
    }
    int core::Group::add(const core::Item& _ItemToAdd, Pos2d<size_t> _ItemPosition, flag_add_alreadyExists _posTaken) {
        const std::string _infoStr{"int core::Group::add(const core::Item&, Pos2d<size_t>, flag_add_alreadyExists)"};

        if(_ItemPosition==Pos2d<size_t>{std::string::npos, std::string::npos} && !_ItemToAdd.isDefined__pos) throw std::invalid_argument("int core::Group::add(const core::Item&) : _ItemPosition argument cannot be empty whilst the core::Item arg also doesn't have a position given.");

        std::unique_lock<std::mutex> u_lck_otherMembers(mtx_access_otherMembers, std::defer_lock);
        std::unique_lock<std::mutex> u_lck_groupItemMatrix(mtx_access_groupItemMatrix, std::defer_lock);
        
        Pos2d<size_t> item_pos = (
            (_ItemPosition.x==std::string::npos && _ItemPosition.y==std::string::npos)?
                _ItemToAdd.get_posInParentGroup().cast<size_t>() : _ItemPosition
        );
        int resizeAxis_x = 0;
        int resizeAxis_y = 0;

        u_lck_groupItemMatrix.lock();
        if(item_pos.y>groupItemMatrix.size())       resizeAxis_y = static_cast<int>(item_pos.y)-static_cast<int>(groupItemMatrix.size());
        if(item_pos.x>groupItemMatrix.at(0).size()) resizeAxis_x = static_cast<int>(item_pos.x)-static_cast<int>(groupItemMatrix.at(0).size());
        u_lck_groupItemMatrix.unlock();
        
        if(resizeAxis_x || resizeAxis_y) this->resize_groupItemMatrix(resizeAxis_x, resizeAxis_y);
        
        u_lck_groupItemMatrix.lock();
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
                            groupItemMatrix, core::Item{Item_types::null}, [](core::Item _matrixElement, core::Item _checkElement) {
                                if(_matrixElement.get_itemType()==Item_types::null) return true;
                                else return false;
                            }, item_pos, 1, flag_matrixSearch_method::radial
                        );
                        if(validPos.size()==0) throw std::logic_error(" error: matrixSearch_2D return 0 valid positions");
                        item_pos = validPos[0];
                        groupItemMatrix.at(item_pos.y).at(item_pos.x) = _ItemToAdd;
                        groupItemMatrix.at(item_pos.y).at(item_pos.x).set_posInParentGroup(item_pos);
                    }
                    break;
                case flag_add_alreadyExists::append_cont: {
                        auto validPos = matrixSearch_2D<core::Item>(
                            groupItemMatrix, core::Item{Item_types::null}, [](core::Item _matrixElement, core::Item _checkElement) {
                                if(_matrixElement.get_itemType()==Item_types::null) return true;
                                else return false;
                            }, item_pos, 1, flag_matrixSearch_method::axial
                        );
                        if(validPos.size()==0) throw std::logic_error(" error: matrixSearch_2D return 0 valid positions");
                        item_pos = validPos[0];
                        groupItemMatrix.at(item_pos.y).at(item_pos.x) = _ItemToAdd;
                        groupItemMatrix.at(item_pos.y).at(item_pos.x).set_posInParentGroup(item_pos);
                    }
                    break;
                default:
                    throw std::runtime_error(_infoStr+" : switch(_posTake) default case reached. Dunno how this happened.");
            }

        }
        else {
            groupItemMatrix.at(item_pos.y).at(item_pos.x) = _ItemToAdd;
            groupItemMatrix.at(item_pos.y).at(item_pos.x).set_posInParentGroup(item_pos);
        }
        u_lck_groupItemMatrix.unlock();

        
        return 0;
    }
    int core::Group::add(core::Item&& _ItemToAdd, Pos2d<size_t> _ItemPosition, flag_add_alreadyExists _posTaken) {
        if(_ItemPosition==Pos2d<size_t>{std::string::npos, std::string::npos} && !_ItemToAdd.isDefined__pos) throw std::invalid_argument("int core::Group::add(const core::Item&) : _ItemPosition argument cannot be empty whilst the core::Item arg also doesn't have a position given.");
        
        std::unique_lock<std::mutex> u_lck_otherMembers(mtx_access_otherMembers, std::defer_lock);
        std::unique_lock<std::mutex> u_lck_groupItemMatrix(mtx_access_groupItemMatrix, std::defer_lock);
        

        Pos2d<size_t> item_pos = (
            (_ItemPosition.x==std::string::npos && _ItemPosition.y==std::string::npos)?
                _ItemToAdd.get_posInParentGroup().cast<size_t>() : _ItemPosition
        );
        int resizeAxis_x = 0;
        int resizeAxis_y = 0;
        u_lck_groupItemMatrix.lock();
        if(item_pos.y>groupItemMatrix.size())       resizeAxis_y = static_cast<int>(item_pos.y)-static_cast<int>(groupItemMatrix.size());
        if(item_pos.x>groupItemMatrix.at(0).size()) resizeAxis_x = static_cast<int>(item_pos.x)-static_cast<int>(groupItemMatrix.at(0).size());
        u_lck_groupItemMatrix.unlock();

        if(resizeAxis_x || resizeAxis_y) this->resize_groupItemMatrix(resizeAxis_x, resizeAxis_y);
        
        u_lck_groupItemMatrix.lock();
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
                            groupItemMatrix, core::Item{Item_types::null}, [](core::Item _matrixElement, core::Item _checkElement) {
                                if(_matrixElement.itemType==Item_types::null) return true;
                                else return false;
                            }, item_pos, 1, flag_matrixSearch_method::radial
                        );
                        if(validPos.size()==0) throw std::logic_error(" error: matrixSearch_2D return 0 valid positions");
                        item_pos = validPos[0];
                        groupItemMatrix.at(item_pos.y).at(item_pos.x) = std::move(_ItemToAdd);
                        groupItemMatrix.at(item_pos.y).at(item_pos.x).set_posInParentGroup(item_pos);
                    }
                    break;
                case flag_add_alreadyExists::append_cont: {
                        auto validPos = matrixSearch_2D<core::Item>(
                            groupItemMatrix, core::Item{Item_types::null}, [](core::Item _matrixElement, core::Item _checkElement) {
                                if(_matrixElement.itemType==Item_types::null) return true;
                                else return false;
                            }, item_pos, 1, flag_matrixSearch_method::axial
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
        u_lck_groupItemMatrix.unlock();
        
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
        
        std::unique_lock<std::mutex> u_lck_otherMembers(mtx_access_otherMembers, std::defer_lock);
        std::unique_lock<std::mutex> u_lck_groupItemMatrix(mtx_access_groupItemMatrix, std::defer_lock);
        
        u_lck_groupItemMatrix.lock();
        if(_posToItem.y>=groupItemMatrix.size())        throw std::invalid_argument(_infoStr+" : _posToItem.y value is larger than existing columns ("+std::to_string(groupItemMatrix.size())+")");
        if(_posToItem.x>=groupItemMatrix.at(0).size())  throw std::invalid_argument(_infoStr+" : _posToItem.x value is larger than existing columns ("+std::to_string(groupItemMatrix.at(0).size())+")");
        
        if(groupItemMatrix.at(_posToItem.y).at(_posToItem.x).get_itemType()==Item_types::null) throw std::invalid_argument(_infoStr+" : Item at given location is a Item_types::null");

        groupItemMatrix.at(_posToItem.y).at(_posToItem.x).nullify();
        u_lck_groupItemMatrix.unlock();

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

        for(size_t _i=0; _i<windowGroups.size(); _i++) {
            if(CONSOLE_DIMENSIONS_MODIFIED.load()) windowGroups.at(_i).isModified__axisMaxSize = true;
            windowGroups.at(_i).update_PSVmatrix();
        }

        static Pos2d<size_t> previousWindowDimensions{0, 0};
        Pos2d<int> windowDiffCount{
            static_cast<int>(CURRENT_CONSOLE_DIMENSIONS.x)-static_cast<int>(previousWindowDimensions.x),
            static_cast<int>(CURRENT_CONSOLE_DIMENSIONS.y)-static_cast<int>(previousWindowDimensions.y)
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
        for(size_t _i=0; _i<windowGroups.size(); _i++) {
            core::Group& _groupRef = windowGroups.at(_i);
            
            if(!_groupRef.isModified__PSVmatrix) continue;
            Pos2d<size_t> corner_TL = _groupRef.groupStyleInfo.posDim.TL_pos();
            Pos2d<size_t> corner_BR = _groupRef.groupStyleInfo.posDim.BR_pos();
            std::unique_lock<std::mutex> u_lck_accss_PSVmatrix(_groupRef.mtx_access_PSVmatrix, std::defer_lock);
            u_lck_accss_PSVmatrix.lock();
            Pos2d<size_t> boxDim_groupPSVmatrix{
                (_groupRef.PrintableStringVectorMatrix.at(0).size() >(corner_BR.x-corner_TL.x)? (corner_BR.x-corner_TL.x) : _groupRef.PrintableStringVectorMatrix.at(0).size()),
                (_groupRef.PrintableStringVectorMatrix.size()       >(corner_BR.y-corner_TL.y)? (corner_BR.y-corner_TL.y) : _groupRef.PrintableStringVectorMatrix.size())
            };
            
            for(size_t _y=0; _y<boxDim_groupPSVmatrix.y; _y++) {
                memcpy(&PrintableStringVectorMatrix.at(corner_TL.y+_y).at(corner_TL.x), &_groupRef.PrintableStringVectorMatrix.at(_y).at(0), boxDim_groupPSVmatrix.x);
            }

            u_lck_accss_PSVmatrix.unlock();
            
            _groupRef.isModified__PSVmatrix = false;

            this->isModified__PSVmatrix = true;
        }
        

    }
    std::string core::Window::print_insertPrintSettings(std::string& _toPrintLine, size_t _currentPSVmatrixLine) {
        const std::string _infoStr{"std::string core::Window::print_insertPrintSettings(std::string& _toPrintLine, size_t _currentPSVmatrixLine)"};
        if(windowGroups.size()==0) throw std::runtime_error(_infoStr+" : windowGroups.size()==0.");
        // if(windowGroups.size()==0) throw std::runtime_error(_infoStr+" : windowGroups.size()==0.");
        if(PrintableStringVectorMatrix.size()==0) throw std::runtime_error(_infoStr+" : PSVmatrix.size()==0.");
        if(PrintableStringVectorMatrix.at(0).size()==0) throw std::runtime_error(_infoStr+" : PSVmatrix.at(0).size()==0.");

        if(_currentPSVmatrixLine>=PrintableStringVectorMatrix.size()) throw std::invalid_argument(_infoStr+" : _currentPSVmatrixLine>=PSVmatrix.size().");
        if(idx_selectedGroup==std::string::npos) return _toPrintLine;

        // DEBUGPRINT2(0,1,std::string(CURRENT_CONSOLE_DIMENSIONS),absolute,absolute)
        // // DEBUGPRINT2(" ")
        // // DEBUGPRINT1(std::string(CURRENT_CONSOLE_DIMENSIONS))
        // for(size_t _n=0; _n<windowGroups.size(); _n++) {
        //     DEBUGPRINT1(fmtToStr(
        //         std::string("_i:")+fmtToStr(_n)+" | "+std::string(posOfGroupsInWindow.at(_n)[0])+":"+std::string(posOfGroupsInWindow.at(_n)[1])+" | "+
        //         std::string(Pos2d<size_t>{windowGroups.at(_n).PrintableStringVectorMatrix.at(0).size(), windowGroups.at(_n).PrintableStringVectorMatrix.size()})
        //         ,CURRENT_CONSOLE_DIMENSIONS.x,0,"left"
        //     ))
        // }

        std::string substringToGroup{""};
        std::string _returStr = _toPrintLine;
        for(size_t _i=0; _i<windowGroups.size(); _i++) {
            core::Group& groupRef = windowGroups.at(_i);
            std::vector<Pos2d<size_t>> cornerPos_ref = {groupRef.groupStyleInfo.posDim.TL_pos(), groupRef.groupStyleInfo.posDim.BR_pos()};
            // std::vector<Pos2d<size_t>> cornerPos_ref = posOfGroupsInWindow.at(_i);; 
            if(_currentPSVmatrixLine>=cornerPos_ref[1].y || _currentPSVmatrixLine<cornerPos_ref[0].y) continue;

            substringToGroup = _returStr.substr(cornerPos_ref[0].x,cornerPos_ref[1].x-cornerPos_ref[0].x);
            substringToGroup = groupRef.print_insertPrintSettings(substringToGroup, _currentPSVmatrixLine-cornerPos_ref[0].y);

            // memcpy(&_returStr.at(cornerPos_ref[0].x),&substringToGroup.at(0),substringToGroup.size());
            _returStr = _returStr.substr(0, cornerPos_ref[0].x) + substringToGroup + _returStr.substr(cornerPos_ref[1].x,std::string::npos);

            // _toPrintLine = substringToGroup;
        }

        return _returStr;
    }
    void core::Window::prep_solveNewGroupPosInWindow() {
        const std::string _infoStr{"void core::Window::prep_solveNewGroupPosInWindow()"};
        if(windowGroups.size()==0) throw std::logic_error(_infoStr+" : windowGroups is empty.");
        // if(windowGroups.size()==windowGroups.size()) throw std::logic_error(_infoStr+" : windowGroups.size()==windowGroups.size() which means there are no new core::Group to find a position for.");
        if(windowGroups.size()<windowGroups.size())  throw std::logic_error(_infoStr+" : windowGroups.size()<windowGroups.size() which is not allowed.");

        /// TEMPORARY SOLUTION! Need to be changed for a more proper method later on.
        /// Current method of placing the TL corners of each core::Group::PrintableStringVectorMatrix is to just divide the longer axis of the terminal/console space and place the
        /// core::Group's along that axis side.
        ///
        /// Future idea on setting positions.

        /// NOTE: For now, made it so it overwrites any and all currently set positions for the sake of just getting over it and have it work at the start. In a rough patch with
        /// progress for some reason.


        /// !!!!Need to make it so this function actually implements the fact that total dimensions is to be equal to current console dimensions,
        /// then from there on divide and assign dimensions and corner-positions to each group.

        posOfGroupsInWindow = std::vector<std::vector<Pos2d<size_t>>>(windowGroups.size(), {{0, 0}, {0, 0}});
        

        Pos2d<size_t> consoleDims = CURRENT_CONSOLE_DIMENSIONS;
        size_t axisToDrawOn = 0; ///< 0-width; 1-height;
        axisToDrawOn = (consoleDims.y>consoleDims.x*0.5? 1 : 0);


        float gapLen = static_cast<float>((axisToDrawOn==0? consoleDims.x : consoleDims.y))/static_cast<float>(windowGroups.size());
        for(size_t _i=0; _i<windowGroups.size(); _i++) {
            std::vector<Pos2d<size_t>> _tempPos{{0, 0}, {0, 0}};
            switch (axisToDrawOn) {
            case 0:
                _tempPos = {{static_cast<size_t>(_i*gapLen), 0}, {static_cast<size_t>((_i+1)*gapLen), consoleDims.y}};
                if(_i==windowGroups.size()-1) {
                    _tempPos.at(1).x+=(consoleDims.x-(static_cast<float>(windowGroups.size())*gapLen));
                    
                }
                break;
            case 1:
                _tempPos = {{0, static_cast<size_t>(_i*gapLen)}, {consoleDims.x, static_cast<size_t>((_i+1)*gapLen)}};
                if(_i==windowGroups.size()-1) {
                    _tempPos.at(1).y+=(consoleDims.y-(static_cast<float>(windowGroups.size())*gapLen));
                }
                break;
            default:
                break;
            }
            
            posOfGroupsInWindow.at(_i) = _tempPos;
            windowGroups.at(_i).groupStyleInfo.posDim.set_pos(posOfGroupsInWindow.at(_i)[0].cast<double>(),posOfGroupsInWindow.at(_i)[1].cast<double>()); //!???
            // windowGroups.at(_i).groupStyleInfo.posDim.set_pos(_tempPos[0].cast<double>(), _tempPos[0].cast<double>());
        }
        
        // size_t idx_startSolve = windowGroups.size();
        size_t idx_startSolve = 0;

        // Pos2d<size_t> totalAxis_PSVmatrixLen{0, 0};

        for(size_t _i=idx_startSolve; _i<windowGroups.size(); _i++) {
            core::Group& _groupRef = windowGroups.at(_i);
            
            // Pos2d<size_t> psvMatrixDim = _groupRef.groupStyleInfo.posDim.get_dim();
            // Pos2d<size_t> psvMatrixDim = _groupRef.groupStyleInfo.posDim.BR_pos()-_groupRef.groupStyleInfo.posDim.TL_pos();
            Pos2d<size_t> psvMatrixDim = posOfGroupsInWindow.at(_i).at(1)-posOfGroupsInWindow.at(_i).at(0);
            DEBUGPRINT1(std::string("get_dim:")+std::string(psvMatrixDim)+" | "+std::string(_groupRef.groupStyleInfo.posDim.get_dim())+" {"+std::string(_groupRef.groupStyleInfo.posDim.TL_pos())+", "+std::string(_groupRef.groupStyleInfo.posDim.BR_pos())+"}")
            DEBUGPAUSE(1000)
            _groupRef.groupStyleInfo.posDim.set_dim(psvMatrixDim);
            
            _groupRef.update_PSVmatrix();
            

            // totalAxis_PSVmatrixLen.y+=_groupRef.PrintableStringVectorMatrix.size();
            // totalAxis_PSVmatrixLen.x+=_groupRef.PrintableStringVectorMatrix.at(0).size();
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
        const std::string _infoStr{"int core::Window::Driver(core::Window*)"};

        keyHandler::keyPressHandler keyHandlerObj;
        /*
        if(idx_selectedGroup==std::string::npos) {
            for(size_t _i_group=0; _i_group<windowGroups.size(); _i_group++) {
                if(windowGroups.at(_i_group).func_moveNavCursor({0, 0})!=core::Group::result_moveNavCursor::no_options_available) {
                    idx_selectedGroup = _i_group;
                    break;
                }
                else {

                }
            }
            /// We allow a situation where not a single core::Item has been selected/hovered over.

            if(idx_selectedGroup==std::string::npos) {
                // DEBUGPRINT1(std::string("moveNavCursor not found."));
            }
        }*/

        if(idx_selectedGroup!=std::string::npos) {
            Pos2d<size_t> winNavPos_inGroup = windowGroups.at(idx_selectedGroup).winNavCursorPos;
            if(winNavPos_inGroup.x==std::string::npos || windowCursorPos.y==std::string::npos) throw std::runtime_error(_infoStr+" : winNavPos_inGroup has a member ==std::string::npos.");
            windowGroups.at(idx_selectedGroup).groupItemMatrix.at(winNavPos_inGroup.y).at(winNavPos_inGroup.x).isModified__text = true;
        }

        // for(auto& _groupRef : windowGroups) {
        //     for(auto& _itemRow : _groupRef.groupItemMatrix) {
        //         for(auto& _item : _itemRow) _item.isModified__text = true;
        //     }
        // }
        

        bool_DriverRunning = true;

        /// Driver loop iteration fps cap management.
        ///    - for now, it uses a default 60fps cap

        double fps_cap   = 120;
        double fps_read  =  0;
        double fps_readTot   = 0;
        std::chrono::duration<double, std::milli> fps_capTotDur_ms(double(1'000)/double(fps_cap)); ///< Total duration in milliseconds for how long one iteration should be for it to reach the fps cap.
        auto fps_timePoint_1 = std::chrono::steady_clock::now();
        auto fps_timePoint_1_temp = fps_timePoint_1;
        auto fps_timePoint_2 = fps_timePoint_1;
        std::chrono::duration<double, std::milli> fps_intervalDur_ms(0.0); ///< How many milliseconds the current iteration of processes has taken up until fps_timePoint_2.
        std::chrono::duration<double, std::milli> fps_totIntervalDur_ms(0.0); ///< How many milliseconds the current iteration of processes has taken up in total.
        std::chrono::duration<double, std::milli> fps_fixSleepDur_ms(0.0); ///< How long to sleep for to reach fps_cap total iteration duration.

        
        std::unordered_map<std::string, timeStruct> timeStructurs;

        while(bool_DriverRunning.load()) {
            if(TAKE_TIME) timeStructurs["updateKeys"].set_t1();
            keyHandlerObj.updateKeys();
            if(TAKE_TIME) timeStructurs["updateKeys"].set_t2();
            // helper_getConsoleDimensions(false);
            
            if(CONSOLE_DIMENSIONS_MODIFIED.load()) {
                ANSIec::clearScreen();
                this->isModified__PSVmatrix = true;
                this->prep_solveNewGroupPosInWindow(); // !!!NOTE: Need to find a solution to clearing spaces in core::Group::PSVmatrix for removing old char's
            }
            
            if(TAKE_TIME) timeStructurs["arrow_key_check"].set_t1();
            Pos2d<int> moveSteps{0, 0};
            if(keyHandlerObj.isActivated(keyHandler::KEY::arrow_LEFT)   || keyHandlerObj.isActivated(keyHandler::KEY::letter_A, false)) moveSteps.x-=1;
            if(keyHandlerObj.isActivated(keyHandler::KEY::arrow_RIGHT)  || keyHandlerObj.isActivated(keyHandler::KEY::letter_D, false)) moveSteps.x+=1;
            if(keyHandlerObj.isActivated(keyHandler::KEY::arrow_UP)     || keyHandlerObj.isActivated(keyHandler::KEY::letter_W, false)) moveSteps.y-=1;
            if(keyHandlerObj.isActivated(keyHandler::KEY::arrow_DOWN)   || keyHandlerObj.isActivated(keyHandler::KEY::letter_S, false)) moveSteps.y+=1; 
            if(TAKE_TIME) timeStructurs["arrow_key_check"].set_t2();


            if(idx_selectedGroup!=std::string::npos) windowGroups.at(idx_selectedGroup).winNav_isSelected = false;
            
            if(!(moveSteps.x==0 && moveSteps.y==0)) {
                if(depth_selected==0) { ///need to create a flag for "selectable core::Group"'s. '
                    
                }
                else if(depth_selected==1) { // moving between core::Group
                    /// For now any movement will just index to the next core::Group since no real matrix of locations for the groups have been established yet.
                    
                    idx_selectedGroup+=1;
                    if(idx_selectedGroup>=windowGroups.size()) {
                        idx_selectedGroup = 0;
                    }
                    size_t numSearched = 0;
                    while(windowGroups.at(idx_selectedGroup).func_moveNavCursor({0,0})==core::Group::result_moveNavCursor::no_options_available) {
                        idx_selectedGroup+=1;
                        numSearched+=1;
                        if(idx_selectedGroup>=windowGroups.size()) {
                            idx_selectedGroup = 0;
                        }
                        if(numSearched>windowGroups.size()) throw std::runtime_error("moveSteps -> depth_selected==1: no valid group found. This is an illegal state where user managed to navigate into a core::Group that doesn't hold any selectable core::Item's.");
                    }
                    windowGroups.at(idx_selectedGroup).winNavCursorPos = Pos2d<size_t>{std::string::npos, std::string::npos};
                    this->isModified__PSVmatrix = true;
                }
                else if(depth_selected==2) { // moving between core::Item's'
                    if(idx_selectedGroup!=std::string::npos) {
                        if(TAKE_TIME) timeStructurs["moveNavCursor"].set_t1();
                        auto moveNavCursorReturnVal = windowGroups.at(idx_selectedGroup).func_moveNavCursor(moveSteps);
                        this->isModified__PSVmatrix = true;
                        if(TAKE_TIME) timeStructurs["moveNavCursor"].set_t2();
                    }
                    else {
                        throw std::runtime_error("invalid scenario where depth_selected==2 but idx_selectedGroup==std::string::npos");
                    }
                }
            }
            
            if(keyHandlerObj.isActivated(keyHandler::KEY::ENTER)) {
                //if(windowGroups.at(idx_selectedGroup).get_itemType()==Item_types::Window) depth_selected=3; ///need to add this flag modification from core::Item::callItem()
                
                if(depth_selected==0) { // ->core::Group
                    if(windowGroups.size()==0) throw std::runtime_error("Enter called in core::Window::Driver when depth_selected==0 but no groups exist in this window.");
                    
                    for(size_t _i_group=0; _i_group<windowGroups.size(); _i_group++) {
                        if(windowGroups.at(_i_group).func_moveNavCursor({0, 0})!=core::Group::result_moveNavCursor::no_options_available) {
                            idx_selectedGroup = _i_group;
                            depth_selected = 1;
                            windowGroups.at(_i_group).winNavCursorPos = Pos2d<size_t>{std::string::npos, std::string::npos};
                            this->isModified__PSVmatrix = true;
                            
                            break;
                        }
                        else {}
                    }
                    /// We allow a situation where not a single core::Item has been selected/hovered over.

                    if(idx_selectedGroup==std::string::npos) { //Does nothing as this scenario is just ignored.
                        // DEBUGPRINT1(std::string("moveNavCursor not found."));
                    
                        //idx_selectedGroup = 0;
                        throw std::runtime_error("no groups found after enter called.");
                    }
                }
                else if(depth_selected==1) { // core::Group->core::Item
                    if(windowGroups.at(idx_selectedGroup).func_moveNavCursor({0, 0})!=core::Group::result_moveNavCursor::no_options_available) {
                        depth_selected = 2;
                        this->isModified__PSVmatrix = true;
                    }
                    else {
                        throw std::runtime_error("depth_selected==1 and it's about to enter into a core::Item selection but no valid core::Item's exist.");
                    }
                
                }
                else if(depth_selected==2) {
                    windowGroups.at(idx_selectedGroup).callItem();
                }
            }
            
            if(keyHandlerObj.isActivated(keyHandler::KEY::ESCAPE) && keyHandlerObj.__active_keys.size()==1) {
                if(depth_selected==0) {
                    ANSIec::Print(0, CURRENT_CONSOLE_DIMENSIONS.y, fmtToStr(" ", CURRENT_CONSOLE_DIMENSIONS.x,0));
                    ANSIec::setCursorPos(0, CURRENT_CONSOLE_DIMENSIONS.y,true);
                    
                    bool_DriverRunning = false;
                    depth_selected = std::string::npos;
                    break;
                }
                else if(depth_selected==1) { // core::Group->core::Window
                    idx_selectedGroup = std::string::npos;
                    depth_selected = 0;
                    this->isModified__PSVmatrix = true;
                }
                else if(depth_selected==2) {
                    windowGroups.at(idx_selectedGroup).winNavCursorPos = Pos2d<size_t>{std::string::npos, std::string::npos};
                    depth_selected = 1;
                    this->isModified__PSVmatrix = true;
                }
            }
            
            if(idx_selectedGroup!=std::string::npos) windowGroups.at(idx_selectedGroup).winNav_isSelected = true;
            
            
            windowGroups.at(1).groupItemMatrix.at(5).at(3).set_text(std::string(fmtCont(keyHandlerObj.__active_keys,3,0)));
            // windowGroups.at(1).groupItemMatrix.at(5).at(2).set_text(fmtToStr(std::string("FPS:")+fmtToStr(fps_readTot,6,1)));
            //windowGroups.at(0).groupItemMatrix.at(5).at(3).isModified__text = true;
            //windowGroups.at(0).groupItemMatrix.at(5).at(2).isModified__text = true;
            // windowGroups.at(0).isModified__PSVmatrix = true;
            // isModified__PSVmatrix = true;
            
            if(TAKE_TIME) timeStructurs["updatePSVmatrix"].set_t1();
            this->update_PSVmatrix();
            if(TAKE_TIME) timeStructurs["updatePSVmatrix"].set_t2();
            
            if(this->isModified__PSVmatrix) {
                
                // assert(PrintableStringVectorMatrix.size()>0);
                // assert(PrintableStringVectorMatrix.at(0).size()>0);
                
                if(!(PrintableStringVectorMatrix.size()>0)) throw std::logic_error(_infoStr+" : !(PrintableStringVectorMatrix.size()>0)");
                if(!(PrintableStringVectorMatrix.at(0).size()>0)) throw std::logic_error(_infoStr+" : !(PrintableStringVectorMatrix.at(0).size()>0)");
                
                
                if(TAKE_TIME) timeStructurs["print rows"].set_t1();
                for(size_t _row=0; _row<this->PrintableStringVectorMatrix.size(); _row++) {
                    ANSIec::Print(0, _row+1, print_insertPrintSettings(PrintableStringVectorMatrix.at(_row), _row), false);
                }
                std::cout.flush();
                this->isModified__PSVmatrix = false;
                if(TAKE_TIME) timeStructurs["print rows"].set_t2();
            }
            

            /// Driver loop fps cap management section

            /*
            if(keyHandlerObj.__active_keys.size()>0) {
                DEBUGPRINT2(0, 0, fmtToStr(
                    std::string("{__active_keys: ")+fmtCont(keyHandlerObj.__active_keys,3,0)+" | "+
                    "depth_selected:"+fmtToStr(depth_selected)
                    ,CURRENT_CONSOLE_DIMENSIONS.x-1,0,"left"
                )+"}", absolute, absolute)
            }
            */
                
            
            fps_timePoint_2 = std::chrono::steady_clock::now();
            fps_intervalDur_ms = (fps_timePoint_2 - fps_timePoint_1)*0.05+fps_intervalDur_ms*0.95;
            fps_fixSleepDur_ms = (fps_capTotDur_ms - fps_intervalDur_ms)*0.05+fps_fixSleepDur_ms*0.95;
            fps_read = fps_read*0.99+(1.0/(fps_intervalDur_ms/1000.0).count())*0.01;

            ANSIec::Print(
                0, CURRENT_CONSOLE_DIMENSIONS.y-1,
                fmtToStr(std::string("FPS:")+fmtToStr(fps_readTot,6,1))+" | tot:"+fmtToStr(fps_totIntervalDur_ms.count(),5,1)+"ms"+
                " | read:"+fmtToStr(fps_intervalDur_ms.count(),5,1)+"ms"+
                " | exp :"+fmtToStr(fps_capTotDur_ms.count(), 5, 1)+"ms"+
                " | fix :"+fmtToStr(fps_fixSleepDur_ms.count(),5,1)+"ms"
            );
            
            
            if(TAKE_TIME) timeStructurs["print time stats"].set_t1();
            if(fps_fixSleepDur_ms.count()>0) {
                // ANSIec::Print(0, CURRENT_CONSOLE_DIMENSIONS.y-2, fmtToStr(std::string("program iter. time:")+fmtToStr(fps_intervalDur_ms.count(), 7)+"ms",0,0,"left"));

                std::this_thread::sleep_until(fps_timePoint_2+fps_fixSleepDur_ms);
                // std::this_thread::sleep_for(fps_fixSleepDur_ms);
                // ANSIec::clearScreen();
            }
            if(TAKE_TIME) timeStructurs["print time stats"].set_t2();
            fps_timePoint_1_temp = std::chrono::steady_clock::now();
            fps_totIntervalDur_ms = (fps_timePoint_1_temp - fps_timePoint_1);
            fps_readTot = (1.0/(fps_totIntervalDur_ms/1000.0).count())*0.01+fps_readTot*0.99;
            fps_timePoint_1 = fps_timePoint_1_temp;
            
            
            //if(keyHandlerObj.isActivated(keyHandler::KEY::ESCAPE) && keyHandlerObj.__active_keys.size()==1) {
            //    ANSIec::Print(0, CURRENT_CONSOLE_DIMENSIONS.y, fmtToStr(" ", CURRENT_CONSOLE_DIMENSIONS.x,0));
            //    ANSIec::setCursorPos(0, CURRENT_CONSOLE_DIMENSIONS.y,true);  
            //    bool_DriverRunning = false;
            //    break;
            //}
        
            helper_getConsoleDimensions(false);
        }
        

        return 0;
    }

    namespace style {
        
        GroupS::GroupS():
            posDim(), symbs(), ANSI()
        {

        }
        GroupS::GroupS(Group_posDim _Group_posDim):
            posDim(_Group_posDim), symbs(), ANSI()
        {

        }
        

        Group_posDim::Group_posDim():
            scalingMethod(screen_ratio)
        {


        }
        Group_posDim::Group_posDim(axisScalingMethod _scalMeth, Pos2d<size_t> _cornerTL_pos, Pos2d<size_t> _cornerBR_pos):
            scalingMethod(_scalMeth)
        {
            // helper_getConsoleDimensions(false);
            switch (_scalMeth) {
            case screen_ratio:
                corner_TL = _cornerTL_pos.cast<double>() / CURRENT_CONSOLE_DIMENSIONS.cast<double>();
                corner_BR = _cornerBR_pos.cast<double>() / CURRENT_CONSOLE_DIMENSIONS.cast<double>();
                break;
            case fixed_value:
                corner_TL = _cornerTL_pos.cast<double>();
                corner_BR = _cornerBR_pos.cast<double>();
                break;
            default:
                throw std::runtime_error("invalid axisScalingMethod value.");
                break;
            }

        }
        Group_posDim::Group_posDim(axisScalingMethod _scalMeth, Pos2d<double> _cornerTL_ratio, Pos2d<double> _cornerBR_ratio):
            scalingMethod(_scalMeth)
        {
            const std::string _infoStr{"Group_posDim::Group_posDim(axisScalingMethod, Pos2d<double>, Pos2d<double>)"};
            if(_cornerTL_ratio.x>1)     throw std::invalid_argument(_infoStr+" : _cornerTL_ratio.x>1.");
            else if(_cornerTL_ratio.x<0)throw std::invalid_argument(_infoStr+" : _cornerTL_ratio.x<0.");
            if(_cornerTL_ratio.y>1) throw std::invalid_argument(_infoStr+" : _cornerTL_ratio.y>1.");
            else if(_cornerTL_ratio.y<0)throw std::invalid_argument(_infoStr+" : _cornerTL_ratio.y<0.");
            if(_cornerBR_ratio.x>1) throw std::invalid_argument(_infoStr+" : _cornerBR_ratio.x>1.");
            else if(_cornerBR_ratio.x<0)throw std::invalid_argument(_infoStr+" : _cornerBR_ratio.x<0.");
            if(_cornerBR_ratio.y>1) throw std::invalid_argument(_infoStr+" : _cornerBR_ratio.y>1.");
            else if(_cornerBR_ratio.y<0)throw std::invalid_argument(_infoStr+" : _cornerBR_ratio.y<0.");
            
            // helper_getConsoleDimensions(false);
            switch (_scalMeth) {
            case screen_ratio:
                corner_TL = _cornerTL_ratio;
                corner_BR = _cornerBR_ratio;
                break;
            case fixed_value:
                corner_TL = CURRENT_CONSOLE_DIMENSIONS.cast<double>()*_cornerTL_ratio;
                corner_BR = CURRENT_CONSOLE_DIMENSIONS.cast<double>()*_cornerBR_ratio;
                break;
            default:
                throw std::runtime_error("invalid axisScalingMethod value.");
                break;
            }

        }
        int Group_posDim::set_TL(Pos2d<double> _newTL) {
            if(_newTL.x<0 && _newTL.y<0) throw std::invalid_argument("both new corner position values cannot bed defined -1.");

            switch(scalingMethod) {
                case screen_ratio:
                    if(_newTL.x>1) corner_TL.x = _newTL.x / static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x);
                    else corner_TL.x = _newTL.x;
                    if(_newTL.y>1) corner_TL.y = _newTL.y / static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y);
                    else corner_TL.y = _newTL.y;
                    break;
                case fixed_value:
                    if(_newTL.x<=1) corner_TL.x = _newTL.x * static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x);
                    else corner_TL.x = _newTL.x;
                    if(_newTL.y<=1) corner_TL.y = _newTL.y * static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y);
                    else corner_TL.y = _newTL.y;
                    break;
            default:
                assert(false && "How the fuck is the default called here.");
            }

            return 0;
        }
        int Group_posDim::set_TL(Pos2d<double> _newTL, axisScalingMethod _newSclMeth) {
            scalingMethod = _newSclMeth;

            return this->set_TL(_newTL);
        }
        int Group_posDim::set_BR(Pos2d<double> _newBR) {
            if(_newBR.x<0 && _newBR.y<0) throw std::invalid_argument("both new corner position values cannot bed defined -1.");

            switch(scalingMethod) {
                case screen_ratio:
                    if(_newBR.x>1) corner_TL.x = _newBR.x / static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x);
                    else corner_TL.x = _newBR.x;
                    if(_newBR.y>1) corner_TL.y = _newBR.y / static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y);
                    else corner_TL.y = _newBR.y;
                    break;
                case fixed_value:
                    if(_newBR.x<=1) corner_TL.x = _newBR.x * static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x);
                    else corner_TL.x = _newBR.x;
                    if(_newBR.y<=1) corner_TL.y = _newBR.y * static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y);
                    else corner_TL.y = _newBR.y;
                    break;
            default:
                assert(false && "How the fuck is the default called here.");
            }

            return 0;
        }
        int Group_posDim::set_BR(Pos2d<double> _newBR, axisScalingMethod _newSclMeth) {
            scalingMethod = _newSclMeth;

            return this->set_BR(_newBR);
        }
        int Group_posDim::set_pos(Pos2d<double> _newTL, Pos2d<double> _newBR, axisScalingMethod _sclMeth) {
            if(_newTL<Pos2d<double>{0, 0} && _newBR<Pos2d<double>{0, 0}) throw std::invalid_argument("both new corner parameters cannot be defined to default ({-1, -1})");

            scalingMethod = screen_ratio;

            this->set_TL(_newTL);
            this->set_BR(_newBR);

            return 0;
        }
        int Group_posDim::set_dim(Pos2d<double> _newDim, int _xPivot, int _yPivot) {
            if(_newDim.x<0 && _newDim.y<0) throw std::invalid_argument("both values of _newDim cannot be defined to default value (-1).");
            
            if(_xPivot!=0 && _xPivot!=1) throw std::invalid_argument("invalid _xPivot argument.");
            if(_yPivot!=0 && _yPivot!=1) throw std::invalid_argument("invalid _yPivot argument.");


            if(_newDim.x>=0) {
                switch (_xPivot) {
                case 0:
                    if(corner_TL.x<0) throw std::runtime_error("corner_TL.x<0 when _newDim.x>=0 && xPivot==0");
                    switch (scalingMethod) {
                    case screen_ratio:
                        corner_BR.x = (corner_TL.x*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x)+_newDim.x) / CURRENT_CONSOLE_DIMENSIONS.x;
                        break;
                    case fixed_value:
                        corner_BR.x = corner_TL.x+_newDim.x;
                        break;
                    }
                    break;
                case 1:
                    if(corner_BR.x<0) throw std::runtime_error("corner_BR.x<0 when _newDim.x>=0 && xPivot==1");
                    switch (scalingMethod) {
                    case screen_ratio:
                        corner_TL.x = (corner_BR.x*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x)>_newDim.x? 0 : corner_BR.x*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x)-_newDim.x) / CURRENT_CONSOLE_DIMENSIONS.x;
                        break;
                    case fixed_value:
                        corner_TL.x = (corner_TL.x>_newDim.x? 0 : corner_TL.x-_newDim.x);
                        break;
                    }
                    break;
                }
            }
            if(_newDim.y>=0) {
                switch (_yPivot) {
                case 0:
                    if(corner_TL.y<0) throw std::runtime_error("corner_TL.y<0 when _newDim.y>=0 && yPivot==0");
                    switch (scalingMethod) {
                    case screen_ratio:
                        corner_BR.y = (corner_TL.y*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y)+_newDim.y) / CURRENT_CONSOLE_DIMENSIONS.y;
                        break;
                    case fixed_value:
                        corner_BR.y = corner_TL.y+_newDim.y;
                        break;
                    }
                    break;
                case 1:
                    if(corner_BR.y<0) throw std::runtime_error("corner_BR.y<0 when _newDim.y>=0 && yPivot==1");
                    switch (scalingMethod) {
                    case screen_ratio:
                        corner_TL.y = ((corner_BR.y*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y)>_newDim.y? 0 : corner_BR.y*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y)-_newDim.y)) / CURRENT_CONSOLE_DIMENSIONS.y;
                        break;
                    case fixed_value:
                        corner_TL.y = (corner_TL.y>_newDim.y? 0 : corner_TL.y-_newDim.y);
                        break;
                    }
                    break;
                }
            }

            return 0;
        }
        int Group_posDim::set_dim(Pos2d<size_t> _newDim, int _xPivot, int _yPivot) {
            return this->set_dim(_newDim.cast<double>(), _xPivot, _yPivot);
        }
        Pos2d<size_t> Group_posDim::get_dim() {
            
            Pos2d<size_t> returnPos{std::string::npos, std::string::npos};
            switch (scalingMethod) {
            case screen_ratio:
                if(corner_TL.x<0) returnPos.x = std::string::npos;
                else if(corner_BR.x<0) returnPos.x = CURRENT_CONSOLE_DIMENSIONS.x-static_cast<size_t>(static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x)*corner_TL.x);
                else returnPos.x = static_cast<size_t>(std::floor(corner_BR.x*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x)-corner_TL.x*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x)));
            
                if(corner_TL.y<0) returnPos.y = std::string::npos;
                else if(corner_BR.y<0) returnPos.y = CURRENT_CONSOLE_DIMENSIONS.y-static_cast<size_t>(static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y)*corner_TL.y);
                else returnPos.y = static_cast<size_t>(std::floor(corner_BR.y*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y)-corner_TL.y*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y)));
                break;
            case fixed_value:
                if(corner_TL.x<0) returnPos.x = std::string::npos;
                else if(corner_BR.x<0) returnPos.x = CURRENT_CONSOLE_DIMENSIONS.x-corner_TL.x;
                else returnPos.x = corner_BR.x-corner_TL.x;
            
                if(corner_TL.y<0) returnPos.y = std::string::npos;
                else if(corner_BR.y<0) returnPos.y = CURRENT_CONSOLE_DIMENSIONS.y-corner_TL.y;
                else returnPos.y = corner_BR.y-corner_TL.y;
                break;
            default:
                break;
            }

            return returnPos;
        }
        int Group_posDim::set_axisScalMeth(axisScalingMethod _newMeth, Pos2d<double> _newTL, Pos2d<double> _newBR) {

            switch (_newMeth) {
            case screen_ratio:
                if(_newTL.x>=0) {
                    if(_newTL.x<=1) corner_TL.x = _newTL.x;
                    else corner_TL.x = _newTL.x/static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x);
                }
                else if(corner_TL.x>1) corner_TL.x = corner_TL.x/static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x);
                if(_newTL.y>=0) {
                    if(_newTL.y<=1) corner_TL.y = _newTL.y;
                    else corner_TL.y = _newTL.y/static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y);
                }
                else if(corner_TL.y>1) corner_TL.y = corner_TL.y/static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y);
                if(_newBR.x>=0) {
                    if(_newBR.x<=1) corner_BR.x = _newBR.x;
                    else corner_BR.x = _newBR.x/static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x);
                }
                else if(corner_BR.x>1) corner_BR.x = corner_BR.x/static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x);
                if(_newBR.y>=0) {
                    if(_newBR.y<=1) corner_BR.y = _newBR.y;
                    else corner_BR.y = _newBR.y/static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y);
                }
                else if(corner_BR.y>1) corner_BR.y = corner_BR.y/static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y);
                break;
            case fixed_value:
                if(_newTL.x>=0) {
                    if(_newTL.x>1) corner_TL.x = _newTL.x;
                    else corner_TL.x = _newTL.x*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x);
                }
                else if(corner_TL.x<=1) corner_TL.x = corner_TL.x*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x);
                if(_newTL.y>=0) {
                    if(_newTL.y>1) corner_TL.y = _newTL.y;
                    else corner_TL.y = _newTL.y*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y);
                }
                else if(corner_TL.y<=1) corner_TL.y = corner_TL.y*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y);
                if(_newBR.x>=0) {
                    if(_newBR.x>1) corner_BR.x = _newBR.x;
                    else corner_BR.x = _newBR.x*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x);
                }
                else if(corner_BR.x<=1) corner_BR.x = corner_BR.x*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x);
                if(_newBR.y>=0) {
                    if(_newBR.y>1) corner_BR.y = _newBR.y;
                    else corner_BR.y = _newBR.y*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y);
                }
                else if(corner_BR.y<=1) corner_BR.y = corner_BR.y*static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y);
                break;
            default:
                break;
            }
            scalingMethod = _newMeth;

            return 0;
        }
        Group_posDim::axisScalingMethod Group_posDim::get_axisScalMeth() const {
            return scalingMethod;
        }
        Pos2d<double> Group_posDim::TL_raw() const {
            return corner_TL;
        }
        Pos2d<double> Group_posDim::BR_raw() const {
            return corner_BR;
        }
        Pos2d<double> Group_posDim::TL_ratio() const {
            Pos2d<double> returValue{-2, -2};

            switch (scalingMethod) {
            case screen_ratio:
                returValue = corner_TL;
                break;
            case fixed_value:
                if(corner_TL.x>=0) returValue.x = corner_TL.x/static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x);
                if(corner_TL.y>=0) returValue.y = corner_TL.y/static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y);
                break;
            default:
                assert(false && "invalid scalingMethod enum value stored.");
            }
            
            return returValue;
        }
        Pos2d<double> Group_posDim::BR_ratio() const {
            Pos2d<double> returValue{-2, -2};

            switch (scalingMethod) {
            case screen_ratio:
                returValue = corner_BR;
                break;
            case fixed_value:
                if(corner_BR.x>=0) returValue.x = corner_BR.x/static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.x);
                if(corner_BR.y>=0) returValue.y = corner_BR.y/static_cast<double>(CURRENT_CONSOLE_DIMENSIONS.y);
                break;
            default:
                assert(false && "invalid scalingMethod enum value stored.");
            }
            
            return returValue;
        }
        Pos2d<size_t> Group_posDim::TL_pos() const {
            Pos2d<size_t> returValue{std::string::npos, std::string::npos};

            switch (scalingMethod) {
            case screen_ratio:
                if(corner_TL.x>=0) returValue.x = static_cast<size_t>(std::round(corner_TL.x*CURRENT_CONSOLE_DIMENSIONS.x));
                if(corner_TL.y>=0) returValue.y = static_cast<size_t>(std::round(corner_TL.y*CURRENT_CONSOLE_DIMENSIONS.y));
                break;
            case fixed_value:
                if(corner_TL.x>=0) returValue.x = static_cast<size_t>(corner_TL.x);
                if(corner_TL.y>=0) returValue.y = static_cast<size_t>(corner_TL.y);
                break;
            default:
                break;
            }

            return returValue;
        }
        Pos2d<size_t> Group_posDim::BR_pos() const {
            Pos2d<size_t> returValue{std::string::npos, std::string::npos};

            switch (scalingMethod) {
            case screen_ratio:
                if(corner_BR.x>=0) returValue.x = static_cast<size_t>(std::round(corner_BR.x*CURRENT_CONSOLE_DIMENSIONS.x));
                if(corner_BR.y>=0) returValue.y = static_cast<size_t>(std::round(corner_BR.y*CURRENT_CONSOLE_DIMENSIONS.y));
                break;
            case fixed_value:
                if(corner_BR.x>=0) returValue.x = static_cast<size_t>(corner_BR.x);
                if(corner_BR.y>=0) returValue.y = static_cast<size_t>(corner_BR.y);
                break;
            default:
                break;
            }

            return returValue;
        }

    };

}
