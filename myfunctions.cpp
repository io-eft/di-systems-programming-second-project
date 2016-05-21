#include "myfunctions.hpp"

int getLastNumber(std::string s)    //get the number at the end of a string as int
{
    int i;
    for(i = s.length(); i > 0; i--)
    {
        if(s.at(i-1) == ' ')
            break;
    }
    std::stringstream ss;
    ss << s.substr(i);
    if(ss >> i)
        return i;
    else
        return 0;
}

int getSecondToLastNumber(std::string s)    //get the second to end number of a string as int
{
    int i;
    for(i = s.length(); i > 0; i--)
    {
        if(s.at(i-1) == ' ')
            break;
    }
    for(i = i - 1; i > 0; i--)
    {
        if(s.at(i-1) == ' ')
            break;
    }
    std::stringstream ss;
    ss << s.substr(i);
    if(ss >> i)
        return i;
    else
        return 0;
}
