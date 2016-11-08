#include "value.h"

Value::Value(){};
Value::Value(string v) :type(vStr), strv(v){};
Value::Value(float v) :type(vNum), numv(v){};
Value::Value(bool v) :type(vBol), boolv(v){};
Value::Value(const Value& v){
    if (v.type == vNum)
    {
        type = vNum;
        numv = v.numv;
    }
    else if (v.type == vBol)
    {
        type = vBol;
        boolv = v.boolv;
    }
    else
    {
        type = vStr;
        strv = v.strv;
    }
};

bool Value::isNum(){ return type == vNum; };
bool Value::isStr(){ return type == vStr; };
bool Value::isBol(){ return type == vBol; };
Value::Vtype Value::getType(){ return type; };

Value Value::operator + (const Value& r)
{
    if (type == vNum && r.type == vNum)
        return numv + r.numv;
    else if (type == vStr && r.type == vStr)
        return strv + r.strv;
    else
    {
        printf("ERROR: Can't add values with different type\n");
        return (float)0;
    }
}

Value Value::operator * (const Value& r)
{
    if (type == vNum && r.type == vNum)
        return numv * r.numv;
    else
    {
        printf("ERROR: Can't multiply none number values \n");
        return (float)0;
    }
}

Value Value::operator / (const Value& r)
{
    if (type == vNum && r.type == vNum)
        return numv / r.numv;
    else
    {
        printf("ERROR: Can't divide none number values \n");
        return (float)0;
    }
}

Value Value::operator - (const Value& r)
{
    if (type == vNum && r.type == vNum)
        return numv - r.numv;
    else
    {
        printf("ERROR: Can't minus none number values \n");
        return (float)0;
    }
}

Value Value::operator == (const Value& r)
{
    if (type == vNum && r.type == vNum)
        return numv == r.numv;
    else if (type == vBol && r.type == vBol)
        return boolv == r.boolv;
    {
        printf("ERROR: Can't minus none number values \n");
        return (float)0;
    }
}

Value Value::operator > (const Value& r)
{
    if (type == vNum && r.type == vNum)
    {
        return numv > r.numv;
    }
    else
    {
        printf("ERROR: Unable to compare none number values\n");
        return (float)0;
    }
}

Value Value::operator < (const Value& r)
{
    if (type == vNum && r.type == vNum)
    {
        return numv < r.numv;
    }
    else
    {
        printf("ERROR: Unable to compare none number values\n");
        return (float)0;
    }
}

Value Value::operator >= (const Value& r)
{
    if (type == vNum && r.type == vNum)
    {
        return numv >= r.numv;
    }
    else
    {
        printf("ERROR: Unable to compare none number values\n");
        return (float)0;
    }
}

Value Value::operator <= (const Value& r)
{
    if (type == vNum && r.type == vNum)
    {
        return numv <= r.numv;
    }
    else
    {
        printf("ERROR: Unable to compare none number values\n");
        return (float)0;
    }
}

Value Value::operator && (const Value& r)
{
    if (type == vBol && r.type == vBol)
    {
        return boolv && r.boolv;
    }
    else
    {
        printf("ERROR: Unable to compare none number values\n");
        return (float)0;
    }
}

Value Value::operator || (const Value& r)
{
    if (type == vBol && r.type == vBol)
    {
        return boolv || r.boolv;
    }
    else
    {
        printf("ERROR: Unable to compare none number values\n");
        return (float)0;
    }
}

bool Value::getBoolean()
{
    return boolv;
}

float Value::getFloat()
{
    return numv;
}

std::string Value::getString()
{
    return strv;
}

std::string Value::toString()
{
    if (type == vBol)
        return boolv ? "true" : "false";
    else if (type == vNum)
        return floatToStr(numv);
    else
        return strv;
}

//Value Value::not()
//{
//    if (type == vBol)
//    {
//        return !boolv;
//    }
//    else if (type == vNum)
//    {
//        return numv == 0 ? true : false;
//    }
//    else
//    {
//        return true;
//    }
//}
//
//Value Value::negtive()
//{
//    if (type == vNum)
//    {
//        return -numv;
//    }
//    // error type
//}

Value Value::operator !()
{
    if (type == vBol)
    {
        return !boolv;
    }
    else if (type == vNum)
    {
        return numv == 0 ? true : false;
    }
    else
    {
        return true;
    }
}

Value Value::operator -()
{
    if (type == vNum)
    {
        return -numv;
    }
}

void Value::operator *= (const Value& r)
{
    if (type == vNum)
    {
        numv *= r.numv;
    }
    // error type
}

void Value::operator /= (const Value& r)
{
    if (type == vNum)
    {
        numv /= r.numv;
    }
}

void Value::operator += (const Value& r)
{
    if (type == vNum)
    {
        numv += r.numv;
    }
}

void Value::operator -= (const Value& r)
{
    if (type == vNum)
    {
        numv -= r.numv;
    }
}

void Value::operator = (const Value& v)
{
    if (v.type == vNum)
    {
        type = vNum;
        numv = v.numv;
    }
    else if (v.type == vBol)
    {
        type = vBol;
        boolv = v.boolv;
    }
    else
    {
        type = vStr;
        strv = v.strv;
    }
}