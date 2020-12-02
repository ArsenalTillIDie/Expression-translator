#include <iostream>
#include "uttranslator.h"


int main()
{
    std::string exprStr;
    std::cin >> exprStr;
    Expression expr(exprStr);
    std::cout << expr.solve();
}
//---------------------------------------------------------------------------
