#include "fraction.h"
#include <iostream>
#include <iomanip>
#include "matrix.h"
#include "block.h"
#include "lina.h"

int main()
{   
    Lina lina;
    //lina.Start();
    
    std::vector<std::string> one{"create", "abc", "1x2"};
    lina.Create(one);
    decltype(one) two{"change", "abc"};
    lina.Change(two);

    one={"create", "def", "2x2"};
    lina.Create(one);
    two={"change", "def"};
    lina.Change(two);

    one={"create", "ghi", "2x2"};
    lina.Create(one);
    two={"change", "ghi"};
    lina.Change(two);

    

    auto m = lina.Calculate("abc + abc*(ghi+def)*def + abc");
    std::cout<<m;
    
    return 0;   
}

