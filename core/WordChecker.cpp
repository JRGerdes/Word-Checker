// WordChecker.cpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"


/*===== CONSTRUCTOR =====*/
WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}

/*=====IF WORD EXISTS IN DATA STRUCTURE=====*/
bool WordChecker::wordExists(const std::string& word) const
{
    if(words.contains(word))
        return true;
    else
        return false;
}

/*=====FIND WORD SUGGESTIONS FOR INVALID WORD=====*/
std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    std::vector<std::string> vect;
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    //SWAP
    for(int i = 0; i < word.size() - 1; i++)
    {
        std::string str = word;
        char ch = str[i];
        str[i] = str[i+1];
        str[i+1] = ch;
        if(wordExists(str))
        {
            if(std::find(vect.begin(), vect.end(), str) != vect.end())
            {}
            else
                vect.push_back(str);
        }
    }
    //INSERT
    for(int j = 0; j < word.size() + 1; j++)
    {
        for(int l = 0; l < alphabet.size(); l++)
        {
            std::string str = word;
            str.insert(j, 1, alphabet[l]);
            if(wordExists(str))
            {
                if(std::find(vect.begin(),vect.end(), str) != vect.end())
                {}
                else
                    vect.push_back(str);
            }
        }
    }
    //DELETE
    for(int m = 0; m < word.size(); m++)
    {
        std::string str = word;
        str.erase(str.begin()+m);
        if(wordExists(str))
        {   
            if(std::find(vect.begin(),vect.end(),str) != vect.end())
            {}
            else
                vect.push_back(str);
        }
    }
    //REPLACE
    for(int n = 0; n < word.size(); n++)
    {
        for(int o = 0; o < alphabet.size(); o++)
        {
            std::string str = word;
            str[n] = alphabet[o];
            if(wordExists(str))
            {
                if(std::find(vect.begin(),vect.end(),str) != vect.end())
                {}
                else
                    vect.push_back(str);
            }
        }
    }
    //SPLIT
    for(int p = 1; p < word.size(); p++)
    {
        std::string str = word;
        str.insert(p, 1, ' ');
        std::size_t pos = str.find(' ');
        std::string temp1 = str.substr(0,pos);
        std::string temp2 = str.substr(pos+1);
        if(wordExists(temp1) && wordExists(temp2))
        {
            if(std::find(vect.begin(),vect.end(),str) != vect.end())
            {}
            else
                vect.push_back(str);
        }
    }
    return vect;
}

