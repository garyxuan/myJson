//
//  main.cpp
//  myJson
//
//  Created by garyxuan on 2024/7/16.
//

#include <iostream>
#include "myJson.hpp"

using namespace std;
using namespace myJson;

void TestSetObject()
{
    myJson::object test = {};
    Json jsonobject(test);
    
    jsonobject.addToObject("key1", "hello");
    jsonobject.addToObject("key2", 1);
    jsonobject.addToObject("key3", true);
    jsonobject.addToObject("key4", {});
    jsonobject.addToObject("key5", nullptr);
    
    myJson::object test1 = {};
    Json jsonobject1(test1);
    jsonobject1.addToObject("kkk", "3121");
    jsonobject.addToObject("key5",jsonobject1);

    cout << "before TestSetObject:\n"
         << jsonobject.dump() << endl;
}

void TestSetArray()
{
    myJson::array test = {};
    Json jsonarray(test);

    jsonarray.addToArray(nullptr);
    jsonarray.addToArray({});
    jsonarray.addToArray(1);
    jsonarray.addToArray(true);
    jsonarray.addToArray("hello");
    
    myJson::object test1 = {};
    Json jsonobject1(test1);
    jsonobject1.addToObject("kkk", "3121");
    jsonobject1.addToObject("kkk1", "13121");
    jsonarray.addToArray(jsonobject1);
    
    cout << "before TestSetArray:\n"
         << jsonarray.dump() << endl;

    for (auto iter = jsonarray.arrayBegin(); iter != jsonarray.arrayEnd(); ++iter)
    {
        if (iter->is_number() && iter->getNumber() == 1)
        {
            cout << "find number :" << iter->getNumber() << endl;
            iter->setNumber(123);
        }
        
        if (iter->is_null())
        {
            cout << "find null :" << endl;
        }
        
        if (iter->is_bool())
        {
            cout << "find bool :" << (iter->getBool()?"true":"false") << endl;
        }
        
        if (iter->is_string())
        {
            cout << "find string :" << iter->getString() << endl;
        }
    }
    cout << "after TestSetArray:\n"
         << jsonarray.dump() << endl;
}

void TestSetNumber()
{
    Json num = {1234};
    cout << "setNumber before num:" << num.getNumber() << endl;

     num.setNumber(99.111);

    cout << "setNumber after num:" << num.getNumber() << endl;
}

void TestparseLiteral(const std::string& str)
{
    try {
        Json j = parse(str);
        cout << j.dump() << endl;
        for (auto i = j.objectBegin(); i != j.objectEnd();i++)
        {
            cout << i->second.dump() << endl;;
        }
    } catch (const myJsonException &e) {
        cout << e.what() << endl;
    }
}

void TestLiteral()
{
//    TestparseLiteral(" ");
//    TestparseLiteral("");
//    TestparseLiteral("true");
//    TestparseLiteral("false");
//    TestparseLiteral("null");

    TestparseLiteral("{ \"key1\":\"\\\"value\\\"\" , \"key2\": true , \"key3\":[ null , true , false , 1E+10] , \"key4\" :{ \"key1\":\"value1\" , \"key2\": true , \"key3\":[ null , true , false , 1.12E10] }}");





    //TestparseLiteral("\"Hello\\u0000World\"");
    //TestparseLiteral("\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
}

int main(int argc, const char * argv[])
{
    //TestSetNumber();
    //TestSetArray();
    //TestSetObject();
    TestLiteral();
    
    
    return 0;
}
