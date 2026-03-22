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

#include <cmath>

#include <initializer_list>
#include <vector>
#include <unordered_map>

#include <memory>
#include <functional>

#include <thread>
#include <atomic>
#include <chrono>

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
        private:
        /// @brief Pointer to the parent core::Group that contains this item.
        core::Group*    parentGroupPtr{nullptr};

        /// @brief Position of this item within its parent group (x, y coordinates).
        /// Initialized to (-1, -1) to indicate undefined position.
        Pos2d<int>  posInParentGroup{-1, -1};
        
        /// @brief The current type of content stored in this item.
        Item_types                      itemType{Item_types::null};
        
        /// @brief Text content; each string represents a line of text.
        /// Used when itemType is Item_types::text or Item_types::function
        std::vector<std::string>        itemText{""};
        
        /// @brief Callback function that executes when this item is activated.
        /// Used when itemType is Item_types::function.
        Type_ItemFunc                   itemFunction;
        
        /// @brief Nested window object owned by this item.
        /// Used when itemType is Item_types::window.
        std::unique_ptr<core::Window>   itemWindow;
        

        protected:

        /// @brief Configuration structure for null-type items.
        ItemContent_null        itemContent_null{};
        
        /// @brief Configuration structure for text-type items (alignment, delimiter rules, etc.).
        ItemContent_text        itemContent_text{};
        
        /// @brief Configuration structure for function-type items.
        ItemContent_function    itemContent_function{};
        
        /// @brief Configuration structure for window-type items.
        ItemContent_window      itemContent_window{};

        // defined flags ----------------------------------------------------------

        /// @brief Tracks whether the parent group pointer has been explicitly set.
        bool isDefined__parentGroupPtr{false};
        
        /// @brief Tracks whether the position within parent group has been explicitly set.
        bool isDefined__pos{false};
        
        /// @brief Tracks whether text content has been explicitly set.
        bool isDefined__text{false};
        
        /// @brief Tracks whether a function callback has been explicitly set.
        bool isDefined__function{false};
        
        /// @brief Tracks whether a nested window has been explicitly set.
        bool isDefined__window{false};

        // modification flags -------------------------------------------------------

        /// @brief Indicates the parent group pointer has been modified since last reset.
        bool isModified__parentGroupPtr{false};
        
        /// @brief Indicates the item type has been modified since last reset.
        bool isModified__itemType{false};
        
        /// @brief Indicates the position has been modified since last reset.
        bool isModified__pos{false};
        
        /// @brief Indicates the text content has been modified since last reset.
        bool isModified__text{false};
        
        /// @brief Indicates the function callback has been modified since last reset.
        bool isModified__function{false};
        
        /// @brief Indicates the nested window has been modified since last reset.
        bool isModified__window{false};
        
        
        /// @brief Executes this item's function callback if it contains one.
        /// @param _originWindowPtr Pointer to the window that originated the call.
        /// @return Status code indicating success or failure.
        int callItem(core::Window* _originWindowPtr);

        /// @brief Sets the parent group pointer and marks it as modified.
        /// @param _newParentGroupPtr Pointer to the new parent group.
        /// @return Status code indicating success or failure.
        int set_parentGroupPtr(core::Group* _newParentGroupPtr);
        
        /// @brief Updates this item's position within its parent group.
        /// @param _newPos New position coordinates (x, y).
        /// @return Status code indicating success or failure.
        int set_posInParentGroup(Pos2d<size_t> _newPos);

        /// @brief Resets all content and clears modification flags.
        /// @return Status code indicating success or failure.
        int nullify();

        friend core::Group;
        friend core::Window;

        public:
        /// @name Constructors / rule of five
        ///@{
        /// @brief Default/null item constructor.
        /// @param _itemType Type of item to create (defaults to null).
        Item(Item_types _itemType=Item_types::null);
        
        /// @brief Construct a text item.
        /// @param _text The text content for this item.
        /// @param _itemType Type of item (defaults to text).
        Item(std::string _text, Item_types _itemType=Item_types::text);
        
        /// @brief Construct a function item with optional text label.
        /// @param _func The callback function to execute.
        /// @param _text Optional text label for the item.
        /// @param _itemType Type of item (defaults to null, should be function).
        Item(Type_ItemFunc _func, std::string _text="", Item_types _itemType=Item_types::null);
        
        /// @brief Construct a window item by moving in a Window object.
        /// @param _window Rvalue reference to the window to embed.
        /// @param _text Optional text label for the item.
        /// @param _itemType Type of item (defaults to null, should be window).
        Item(core::Window&& _window, std::string _text="", Item_types _itemType=Item_types::null);

        /// @brief Default constructor.
        Item();
        
        /// @brief Copy constructor.
        /// @param _toCopy Item to copy from.
        Item(const Item& _toCopy);
        
        /// @brief Move constructor.
        /// @param _toMove Item to move from.
        Item(Item&& _toMove);
        
        /// @brief Destructor.
        ~Item();
        
        /// @brief Copy assignment operator.
        /// @param _toCopy Item to copy assign from.
        /// @return Reference to this item.
        Item& operator=(const Item& _toCopy);
        
        /// @brief Move assignment operator.
        /// @param _toMove Item to move assign from.
        /// @return Reference to this item.
        Item& operator=(Item&& _toMove);
        ///@}

        /// @name Content setters
        ///@{
        /// @brief Set this item as a null-type item with configuration.
        /// @param _newContent Configuration structure for null content.
        /// @return Status code indicating success or failure.
        int setContent_null(ItemContent_null _newContent);
        
        /// @brief Set this item as a text-type item with configuration.
        /// @param _newContent Configuration structure for text content.
        /// @return Status code indicating success or failure.
        int setContent_text(ItemContent_text _newContent);
        
        /// @brief Set this item as a function-type item with configuration.
        /// @param _newContent Configuration structure for function content.
        /// @return Status code indicating success or failure.
        int setContent_function(ItemContent_function _newContent);
        
        /// @brief Set this item as a window-type item with configuration.
        /// @param _newContent Configuration structure for window content.
        /// @return Status code indicating success or failure.
        int setContent_window(ItemContent_window _newContent);
        ///@}

        /// @name Accessors
        ///@{
        /// @brief Get the parent group pointer.
        /// @return Pointer to the parent group, or nullptr if not part of a group.
        core::Group*    get_parentWindowPtr() const;
        
        /// @brief Get the position of this item within its parent group.
        /// @return The (x, y) position coordinates.
        Pos2d<int>      get_posInParentGroup() const;
        
        /// @brief Get the text content of this item.
        /// @return Vector of text strings.
        std::vector<std::string>    get_text() const;
        
        /// @brief Get the function callback associated with this item.
        /// @return The function object, or empty function if none set.
        Type_ItemFunc   get_itemFunc() const;
        
        /// @brief Get the current item type.
        /// @return The Item_types enumeration value.
        Item_types      get_itemType() const;
        ///@}

        /// @name Modification queries
        ///@{
        /// @brief Check if any aspect of this item has been modified.
        /// @return True if any modification flag is set, false otherwise.
        bool    isModified();
        
        /// @brief Check if the parent group pointer was modified.
        /// @return True if parentGroupPtr has been modified since last reset.
        bool    isModified_parentGroupPtr();
        
        /// @brief Check if the item type was modified.
        /// @return True if itemType has been modified since last reset.
        bool    isModified_itemType();
        
        /// @brief Check if the position was modified.
        /// @return True if posInParentGroup has been modified since last reset.
        bool    isModified_pos();
        
        /// @brief Check if the text content was modified.
        /// @return True if itemText has been modified since last reset.
        bool    isModified_text();
        
        /// @brief Check if the function callback was modified.
        /// @return True if itemFunction has been modified since last reset.
        bool    isModified_function();
        
        /// @brief Check if the nested window was modified.
        /// @return True if itemWindow has been modified since last reset.
        bool    isModified_window();
        ///@}
    };
    /// @class core::Group
    /// @brief Two-dimensional grid of Item objects.
    ///
    /// Groups manage layout, sizing and provide printing support for their items.
    class core::Group {
        private:


        protected:

        core::Window* parentWindowPtr{nullptr};
        Pos2d<size_t> posInParentWindow; ///< Location of group inside parent window.
        
        /// @brief Current core::Item index position of the parent core::Window's navigation cursor. `std::string::npos` means the cursor doesn't have an active position.
        /// Used in update_PSVmatrix to highlight selected `core::Item`.
        Pos2d<size_t> winNavCursorPos{std::string::npos, std::string::npos};
        /// @brief Last core::Item index position of the parent core::Window's navigation cursor. If the values are `std::string::npos` then it means a last position hasn't 
        /// been defined and/or this variable is uninitialised.
        Pos2d<size_t> last_winNavCursorPos{std::string::npos, std::string::npos};

        enum class result_moveNavCursor {
            normal,
            out_of_bounds
        };

        /// @brief 
        /// @param _moveSteps 
        /// @return 
        result_moveNavCursor func_moveNavCursor(Pos2d<int> _moveSteps);

        /// @brief Call the currently selected core::Item from winNavCursorPos.
        void callItem();
        
        struct options_windowCustoms {
            bool allowCursorToNavOutOfGroup{false};
            /**
             * @brief Rule that says that when the cursor navigation cursor moves out of bounds
             * 
             */
            bool whenCursorOutOfBoundsReEnter{false};
            
        } windowOptions;

        /// @brief Indicates the PrintableStringVectorMatrix container has been modified since last reset.
        /// Used by `void core::Window::update_PSVmatrix()` to determine whether the region with this core::Group is to be re-drawn.
        bool isModified__PSVmatrix{true};


        std::vector<std::vector<core::Item>> groupItemMatrix; ///< Items organized by rows and columns.
        Pos2d<size_t> groupDimension;   ///< Dimensions of the group in terminal/console character dimensions (char-size). Does NOT say the dimension in elements.

        std::vector<std::vector<size_t>> axisMaxSize{{}, {}}; ///< Maximum width/height per axis. [0]: x-axis, [1]: y-axis.

        /// @brief Updates the maximum size's of each axis' widths/heights.
        /// 
        /// @note This function at the moment of writing only divides the spaces for items evenly along both axes and it doesn't have any adjusted scaling
        void update_axisMaxSizeVectors();

        /// @brief Update the `core::Group*` value's of every Item stored in this Group.
        void update_groupPtrInItems();

        /// @brief Recomputes PrintableStringVector matrix for layout. Not intented to be called by the user or this object but instead by the parent core::Window.
        void update_PSVmatrix();
        
        /// @brief Resize the `groupItemMatrix` 2d member variable with either the addition or removal of columns/rows.
        void resize_groupItemMatrix(int axisDiff_x, int axisDiff_y);


        void LoadInitializerItemMatrix(std::initializer_list<std::initializer_list<core::Item>>& _matrixInput);
        
        std::vector<std::string> PrintableStringVectorMatrix; ///< Cached strings for printing.
        std::string symb_delimiter_columns{"|"}; ///< Column delimiter symbol.
        std::string symb_delimiter_rows{"-"}; ///< Row delimiter symbol.
        std::string symb_border_column{"|"}; ///< Border column symbol.
        std::string symb_border_row{"-"}; ///< Border row symbol.
        std::string symb_border_corner{"*"}; ///< Corner symbol.
        std::string symb_rowSeparator{"\n"}; ///< Row separator string.

        friend core::Window;
        

        public:

        /// @brief Construct group from initializer list of rows.
        Group(std::initializer_list<std::initializer_list<core::Item>> _matrixInput);
        /// @brief Assign new contents from initializer list.
        Group& operator=(std::initializer_list<std::initializer_list<core::Item>> _matrixInput);

        Group();
        Group(const Group& _toCopy);            // Copy Constructor
        Group(Group&& _toMove);                 // Move Constructor
        ~Group();
        Group& operator=(const Group& _toCopy); // Copy Assignment Operator
        Group& operator=(Group&& _toMove);      // Move Assignment Operator

        /**
         * Dedicated enum flag for dealing with circumstances where the position given for a new item to be added, already holds an item.
         */
        enum class flag_add_alreadyExists{
            throwExcept,/// Throw `std::invalid_argument` exception that gives the position argument given in the function..
            replace,    /// Replace the existing core::Item with the newly given Item.
            skip,       /// Skip the action and move one, ignore that Item.
            append_abs, /// Append the new Item to the first empty coordinate/position after the given position argument, in an absolute closeest search, meaning it'll look for the closest empty location ignore axis order and instead closest in trigonometric sense: i.e. smallest `sqrt((x2-x1)`2+(y^2-y^1)^2)`.
            append_cont,/// Append the new Item to the first empty coordinate/position after the given position argument, in a continuous queue placement order, meaning  x->y so it'll look for the first empty location in x-axis, then y.
        };
        
        int add(const core::Item& _ItemToAdd, Pos2d<size_t> _ItemPosition=Pos2d<size_t>{std::string::npos, std::string::npos}, flag_add_alreadyExists _posTaken=flag_add_alreadyExists::replace);
        int add(core::Item&& _ItemToAdd, Pos2d<size_t> _ItemPosition=Pos2d<size_t>{std::string::npos, std::string::npos}, flag_add_alreadyExists _posTaken=flag_add_alreadyExists::replace);
        int add(std::vector<core::Item> _ItemsToAdd, std::vector<Pos2d<size_t>> _ItemPositions=std::vector<Pos2d<size_t>>(), flag_add_alreadyExists _posTaken=flag_add_alreadyExists::replace);
        
        int erase(Pos2d<size_t> _posToItem);
        int erase(std::vector<Pos2d<size_t>> _posToItemsToErase);
        

    };
    
    // enum class Window_rule_groupOverlapping {
    //     PrioritisePrior,
    // };
    
    /// @class core::Window
    /// @brief High-level container that holds groups and handles input.
    ///
    /// Windows are the top-level structures rendered by the simpleTUI system.
    class core::Window {
        private:

        /// @brief Main std::vector container member variable that holds all the core::Group objects for this window.
        ///
        /// The order of placement for each element/core::Group matters because during group PSV intersection, the latter element will be placed over the previous.
        std::vector<core::Group> windowGroups;
        /// @brief Terminal/Console location of each core::Group members' TL(Top Left) and BR(Bot Right) corners in this core::Window.
        std::vector<std::vector<Pos2d<size_t>>> posOfGroupsInWindow;
        size_t idx_selectedGroup{std::string::npos};

        std::vector<std::string> PrintableStringVectorMatrix;

        void update_PSVmatrix();

        protected:

        Pos2d<size_t> windowCursorPos{0, 0}; ///< Current cursor position inside window.

        std::atomic<bool> bool_DriverRunning{false};

        
        void prep_windowInit();
        /// @brief Check the existing core::Group objects' PSVmatrix dimensions and apply a fitting xy-location for the TopLeft corners of each PSVmatrix string vector
        /// in this->PrintableStringVectorMatrix. Will only solve for core::Group's without an already existing location/position.
        void prep_solveNewGroupPosInWindow();
        

        public:

        /// @brief Construct window from initializer list of groups.
        Window(std::initializer_list<core::Group> _groupInput);
        /// @brief Assign new groups from initializer list.
        Window& operator=(std::initializer_list<core::Group> _groupInput);

        Window();
        Window(const Window& _toCopy);  // Copy Constructor
        Window(Window&& _toMove);       // Move Constructor
        ~Window();
        Window& operator=(const Window& _toCopy);   // Copy Assignment Operator
        Window& operator=(Window&& _toMove);        // Move Assignment Operator

        /// @brief Primary driver loop for the window.
        int Driver(core::Window* _originPtr=nullptr);


    };

};


#endif //HPP__SIMPLETUI2
