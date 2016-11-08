#ifndef __VALUES_H
#define __VALUES_H
#include <string>
#include "util.h"

class Value
{
public:
    enum Vtype{
        vNum,
        vStr,
        vBol,
    };
    Value();
    Value(string v);
    Value(float v) ;
    Value(bool v);
    Value(const Value& v);
    bool isNum();
    bool isStr();
    bool isBol();
    Vtype getType();

    //Value not();
    //Value negtive();

    bool getBoolean();
    float getFloat();
    std::string getString();

    std::string toString();

    Value operator + (const Value& r);
    Value operator * (const Value& r);
    Value operator / (const Value& r);
    Value operator - (const Value& r);

    Value operator == (const Value& r);

    Value operator > (const Value& r);
    Value operator < (const Value& r);
    Value operator >= (const Value& r);
    Value operator <= (const Value& r);

    Value operator && (const Value& r);
    Value operator || (const Value& r);

    Value operator !();
    Value operator -();

    void operator *= (const Value& r);
    void operator /= (const Value& r);
    void operator += (const Value& r);
    void operator -= (const Value& r);

    void operator = (const Value& r);

protected:
    Vtype type;

    float numv;
    string strv;
    bool boolv;
};

#endif