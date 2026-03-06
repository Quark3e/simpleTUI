#pragma once
#ifndef HPP__SIMPLETUI2
#define HPP__SIMPLETUI2


// #include <mutex>
// #include <thread>
// #include <atomic>

// #include <string>
// #include <cstring>
// #include <iostream>
// #include <sstream>

// #include <initializer_list>
// #include <vector>
// #include <map>
// #include <functional>

// #include <cassert>
// #include <memory>
// #include <chrono>

#include <iostream>
#include <sstream>

#include <cstring>
#include <string>

#include <initializer_list>
#include <vector>
#include <unordered_map>

#include <memory>
#include <functional>

#include <cassert>

#include <Pos2d.hpp>

#include <helperMethods.hpp>
#include <keyHandler.hpp>



#define DEBUGPRINT(str) ANSIec::Print(0, 0, str, true, ANSIec::PrintAxisMethod::absolute, ANSIec::PrintAxisMethod::relative);
#define DEBUGPAUSE(ms)  std::this_thread::sleep_for(std::chrono::milliseconds(ms));



#pragma warning(disable: 4267)


namespace simpleTUI2 {
    /// @namespace simpleTUI2
    /// @brief Root namespace for the simpleTUI2 text UI framework.
    ///
    /// All public types, functions, and utilities are declared within this
    /// namespace. The `core` sub-namespace contains fundamental classes like
    /// Item, Group, and Window.
    using namespace helperMethods;


    namespace core {
        class Item;
        class Group;
        class Window;
    };

    /// @enum Item_types
    /// @brief Enumeration defining the types of items that can be contained within groups and windows.
    /// 
    /// This enum specifies the various content types an Item can represent in the simpleTUI2 framework.
    enum class Item_types {
        null    = 0,  ///< Null type; represents an empty or uninitialized item
        text    = 1,  ///< Text type; item contains text content
        function= 2,  ///< Function type; item contains an executable function callback
        window  = 3   ///< Window type; item represents a nested window
    };

    /// @brief Alias for item callback functions.
    ///
    /// Functions matching this signature are stored in items of type
    /// `Item_types::function`. The pointer argument refers to the window
    /// containing the item.
    using Type_ItemFunc = std::function<void(core::Window*)>;


    /// @enum ItemContent_text__textAlign
    /// @brief Horizontal text alignment options for `ItemContent_text`.
    enum ItemContent_text__textAlign {
        left    = -1, ///< Left-aligned text.
        center  =  0, ///< Centered text.
        right   =  1  ///< Right-aligned text.
    };

    /// @struct ItemContent_null
    /// @brief Placeholder struct used when an item has no additional data.
    struct ItemContent_null {
    };

    /// @struct ItemContent_text
    /// @brief Holds configuration for text-type items.
    struct ItemContent_text {
        bool rule_followDelimiter{true}; ///< Obey delimiter rules when rendering.

        ItemContent_text__textAlign style_textAlign{left}; ///< Alignment style.
    };

    /// @struct ItemContent_function
    /// @brief Placeholder for function-type items (no extra fields yet).
    struct ItemContent_function {
    };

    /// @struct ItemContent_window
    /// @brief Placeholder for window-type items (no extra fields yet).
    struct ItemContent_window {
    };
    
    /// @class core::Item
    /// @brief Represents an element within a group or window.
    ///
    /// An Item may contain text, a callback function, or even a nested window.
    /// It tracks modification flags for efficient redraw logic.
    class core::Item {
        protected:
        core::Group*    parentGroupPtr{nullptr}; ///< Pointer to parent core::Group object.

        Pos2d<int>  posInParentGroup{-1, -1}; ///< Position of this item within parent group.

        // content storage ----------------------------------------------------
        std::vector<std::string>        itemText{""}; ///< Text lines (if any).
        Type_ItemFunc                   itemFunction; ///< Callback function.
        Item_types                      itemType{Item_types::null}; ///< Current type.
        std::unique_ptr<core::Window>   itemWindow; ///< Nested window (if type=window).


        ItemContent_null        itemContent_null{};
        ItemContent_text        itemContent_text{};
        ItemContent_function    itemContent_function{};
        ItemContent_window      itemContent_window{};

        // defined flags ------------------------------------------------------

        bool isDefined__pos{false};
        bool isDefined__text{false};
        bool isDefined__function{false};
        bool isDefined__window{false};

        // modification flags -------------------------------------------------

        bool isModified__parentGroupPtr{false};
        bool isModified__pos{false};
        bool isModified__text{false};
        bool isModified__function{false};
        bool isModified__window{false};
        

        friend core::Group;
        friend core::Window;

        public:
        /// @name Constructors / rule of five
        ///@{
        /// @brief Default/null item.
        Item(Item_types _itemType=Item_types::null);
        /// @brief Construct a text item.
        Item(std::string _text, Item_types _itemType=Item_types::text);
        /// @brief Construct a function item with optional text label.
        Item(Type_ItemFunc _func, std::string _text="", Item_types _itemType=Item_types::null);
        /// @brief Construct a window item by moving in a Window object.
        Item(core::Window&& _window, std::string _text="", Item_types _itemType=Item_types::null);

        Item();
        Item(const Item& _toCopy);  // Copy Constructor
        Item(Item&& _toMove);       // Move Constructor
        ~Item();
        Item& operator=(const Item& _toCopy);   // Copy Assignment Operator
        Item& operator=(Item&& _toMove);        // Move Assignment Operator
        ///@}

        /// @name Content setters
        ///@{
        int setContent_null(ItemContent_null _newContent);
        int setContent_text(ItemContent_text _newContent);
        int setContent_function(ItemContent_function _newContent);
        int setContent_window(ItemContent_window _newContent);
        ///@}

        /// @name Accessors
        ///@{
        core::Group*    get_parentWindowPtr() const;
        Pos2d<int>      get_posInParentGroup() const;
        std::vector<std::string>    get_text() const;
        Type_ItemFunc   get_itemFunc() const;
        Item_types      get_itemType() const;
        ///@}

        /// @name Modification queries
        ///@{
        bool    isModified();
        bool    isModified_parentGroupPtr();
        bool    isModified_pos();
        bool    isModified_text();
        bool    isModified_function();
        bool    isModified_window();
        ///@}
    };
    /// @class core::Group
    /// @brief Two-dimensional grid of Item objects.
    ///
    /// Groups manage layout, sizing and provide printing support for their items.
    class core::Group {
        protected:

        std::vector<std::vector<core::Item>> groupItemMatrix; ///< Items organized by rows and columns.

        Pos2d<size_t> posInParentWindow; ///< Location of group inside parent window.
        Pos2d<size_t> groupDimension;   ///< Dimensions of the group in terminal/console character dimensions (char-size)

        std::vector<std::vector<size_t>> axisMaxSize{{}, {}}; ///< Maximum width/height per axis. [0]: x-axis, [1]: y-axis.

        /// @brief Updates the maximum size's of each axis' widths/heights.
        /// 
        /// @note This function at the moment of writing only divides the spaces for items evenly along both axes and it doesn't have any adjusted scaling
        void update_axisMaxSizeVectors();

        /// @brief Update the `core::Group*` value's of every Item stored in this Group.
        void update_groupPtrInItems();

        /// @brief Recomputes PrintableStringVector matrix for layout.
        void update_PSVmatrix();


        void LoadInitializerItemMatrix(std::initializer_list<std::initializer_list<core::Item>>& _matrixInput);
        
        std::vector<std::string> PrintableStringVectorMatrix; ///< Cached strings for printing.
        std::string symb_delimiter_columns{"|"}; ///< Column delimiter symbol.
        std::string symb_delimiter_rows{"-"}; ///< Row delimiter symbol.
        std::string symb_border_column{"|"}; ///< Border column symbol.
        std::string symb_border_row{"-"}; ///< Border row symbol.
        std::string symb_border_corner{"*"}; ///< Corner symbol.
        std::string symb_rowSeparator{"\n"}; ///< Row separator string.


        public:

        /// @brief Construct group from initializer list of rows.
        Group(std::initializer_list<std::initializer_list<core::Item>> _matrixInput);
        /// @brief Assign new contents from initializer list.
        Group& operator=(std::initializer_list<std::initializer_list<core::Item>> _matrixInput);

        Group();
        Group(const Group& _toCopy);                   // Copy Constructor
        Group(Group&& _toMove);               // Move Constructor
        ~Group();
        Group& operator=(const Group& _toCopy);        // Copy Assignment Operator
        Group& operator=(Group&& _toMove);    // Move Assignment Operator


    };
    /// @class core::Window
    /// @brief High-level container that holds groups and handles input.
    ///
    /// Windows are the top-level structures rendered by the simpleTUI system.
    class core::Window {
        protected:

        std::vector<core::Group> windowGroups; ///< Groups contained within the window.

        Pos2d<size_t> windowCursorPos; ///< Current cursor position inside window.


        public:

        /// @brief Construct window from initializer list of groups.
        Window(std::initializer_list<core::Group> _groupInput);
        /// @brief Assign new groups from initializer list.
        Window& operator=(std::initializer_list<core::Group> _groupInput);

        Window();
        Window(const Window& _toCopy);                 // Copy Constructor
        Window(Window&& _toMove);             // Move Constructor
        ~Window();
        Window& operator=(const Window& _toCopy);      // Copy Assignment Operator
        Window& operator=(Window&& _toMove);  // Move Assignment Operator

        /// @brief Primary driver loop for the window.
        int Driver();

    };    

};


#endif //HPP__SIMPLETUI2