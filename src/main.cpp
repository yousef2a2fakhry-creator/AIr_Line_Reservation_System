#include <iostream>
#include <bits/stdc++.h>
#include "../inc/UI.h"
#include "../inc/System.h"
using namespace std;

signed main()
{

    System sys;
    System &test = sys;
    bool running = true;
    UI test_UI(test);
    while (running)
    {
        try
        {
            test_UI.interact();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            running = false;
        }
    }
    return 0;
}