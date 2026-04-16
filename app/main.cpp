
#include <simpleTUI2.hpp>
#include <iostream>


int main(int argc, char** argv) {
    // simpleTUI::core::Menu testMenu({
    //     {{"test"}, {simpleTUI::cell_types::null}, {"test2"}},
    //     {{simpleTUI::cell_types::null}, {"test3\nsecond line\nthird line"}, {simpleTUI::cell_types::null}},
    //     {{"test4"}, {simpleTUI::cell_types::null}, {"test5"}}
    // });
    
    // try {
    //     if(testMenu.Driver()==simpleTUI::core::Menu::MenuDriver_returnType::error) {
    //         throw std::runtime_error("ERROR: simpleTUI::core::Menu::Driver() return error.");
    //     }
        
    // }
    // catch(const std::exception& e) {
    //     std::cerr << e.what() << '\n';
    //     return 1;
    // }

    using namespace simpleTUI2;
    
    try {
        core::Window testWindow{
            {
                {{"test string, TL group uno."}},
                {{[](core::Window* _winPtr) {}, "test func0"},  {Item_types::null}, {"test2"}},
                {{Item_types::null}, {[](core::Window* _winPtr) {}, "test3 func1\nsecond line\nthird line"}, {"center test"}, {[](core::Window* _winPtr) {}, std::string("test\nsecond line\nthird line\n")+std::string(50,'0')+"\nfunc2"}},
                {{[](core::Window* _winPtr) {}, std::string(50,'4')+"test4 func3"}, {Item_types::null}, {"test5"}},
                {{[](core::Window* _winPtr) {}, "func4 testing 69"}, {Item_types::null}, {"ligma"}, {"sigundus seximus"}},
                {{[](core::Window* _winPtr) {}, "func5"}, core::Item([](core::Window* _winPtr) {}, "func2")}
            }, {
                {{"test string, TL group deux"}},
                {{Item_types::null}, {"second row, second column, not a func"}, {[](core::Window* _winPtr) {}, "first func in row[1],column[2]"}}
            }, {
                {{[](core::Window* _winPtr) {}, "test string, TL group trois"}}
            }
        };

        
        if(testWindow.Driver()!=0) {
            throw std::runtime_error("ERROR: simpleTUI2::core::Menu::Driver() returned non-zero value.");
        }
        
    }
    catch(const std::exception& e) {
        std::cout << e.what() << '\n';
        return 1;
    }
    
    

    return 0;
}
