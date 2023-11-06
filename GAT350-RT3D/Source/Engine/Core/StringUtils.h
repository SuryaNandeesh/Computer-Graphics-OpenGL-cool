#pragma once
#include <string>

class StringUtils {
public:
    // converting string to uppercase
    static std::string ToUpper(const std::string& str);

    // transformers into string to lowercase
    static std::string ToLower(const std::string& str);

    // compare two strings ignoring case
    static bool IsEqualIgnoreCase(const std::string& str1, const std::string& str2);

    // create unique string by appending a static unsigned integer
    static std::string CreateUnique(const std::string& str);

private:
    //mainly for testing if the code worked so that it doesnt replay the same string over and over
    static unsigned int uniqueCounter;
};