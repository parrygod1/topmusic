#ifndef PARSEFUNC_H
#define PARSEFUNC_H
#include <string>
#include <cstring>
#include <vector>

inline std::string getCmdkey(std::string command)
{
    char *p = strtok(&command[0], " ");
    if(p)
        return std::string(p);
    else
        return "";
}

inline void getCmdArgs(std::vector<std::string> &vector, std::string args, int count=0)
{
    if(args[0]==' ')
        args.erase(0,1);

    char *c_args = new char[args.size()];
    strcpy(c_args, args.c_str());
    char *p = strtok(c_args, " ");
    if(p)
        vector.push_back(std::string(p));

    if(count>0)
    {
        for(count; count > 0; count--)
        {   
            p = strtok (NULL, " ");
            if(p)   
            {   
                vector.push_back(std::string(p));
            } 
            else
            {
                vector.push_back(std::string(""));
            }  
        }
    }
    else
    {
        while(p)
        {
            p = strtok (NULL, " ");
            if(p)   
            {   
                vector.push_back(std::string(p));
            }
        }
    }
    
}

inline void getCmdStrings(std::vector<std::string> &vector, std::string args)
{
    if(args[0]==' ')
        args.erase(0,1);
    
    std::string temp;
    bool reading_chars = false;
    for(int i=0; i < args.size(); i++)
    {
        if((args[i]==' ' && args[i+1]=='\"') || (i==0 && args[i]=='\"'))
        {
            if(i!=0)
                i++;
            reading_chars = true;
            temp = "";
            continue;
        }
        if(reading_chars)
        {
            if(args[i]=='\"' && (args[i+1]==' ' || args[i+1] == '\0'))
            {
                reading_chars = false;
                vector.push_back(temp);
                continue;
            }
            temp+=args[i];
        }
    }
}

#endif