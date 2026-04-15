
#include <iostream>
#include <simpleTUI2.hpp>


using simpleTUI2;

int main(int argc, char** argv) {
    core::Window windowObj{
	{ /// one core::Group
	    {{Item_types::null}, {"Item (1,0)"}, {Item_types::null}}, /// one row in core::Group
	    {{"Item (0,1)"}, {Item_types::null}, {"Item (2,1)"}},
	    {{Item_types::null}, {"Item (1,2)"}, {"Item (2,2)"}}
	}
    };

    windowObj.Driver();


    return 0;
}
