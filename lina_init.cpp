/*
    This file has one and only one thing: Lina constructor
    Internally, Lina initialize its keywords to their appropriate functions
*/

#include "lina.h"

Lina::Lina():_CurrentLanguage("EN-en")
{
    using namespace std::placeholders;
    using namespace std;

    /*
        Binds keywords to functions
    */

    _Commands.insert({string("exit"), 
                      function<int(vector<string> &)>(bind(&Lina::Exit, this, _1)) 
                      });
   
    _Commands.insert({string("help"), 
                      function<int(vector<string> &)>(bind(&Lina::Help, this, _1))
                      });

    _Commands.insert({string("create"), 
                      function<int(vector<string> &)>(bind(&Lina::Create, this, _1))
                      });

    _Commands.insert({string("show"), 
                      function<int(vector<string> &)>(bind(&Lina::Show, this, _1))
                      });
   _Commands.insert({string("change"), 
                      function<int(vector<string> &)>(bind(&Lina::Change, this, _1))
                      });
    /*
   _Commands.insert({string(""), });*/

    /*
        Reserved keywords
    */

    _Keywords.insert("det");
    _Keywords.insert("inv");
    // "i" as in Identity (Matrix) (Yeah, i need to work on that real soon)
    _Keywords.insert("i");
    _Keywords.insert("language");
    _Keywords.insert("exit");
    _Keywords.insert("help");
    _Keywords.insert("create");
    _Keywords.insert("show");
    _Keywords.insert("change");
    
    // _Keywords.insert("");
    
}