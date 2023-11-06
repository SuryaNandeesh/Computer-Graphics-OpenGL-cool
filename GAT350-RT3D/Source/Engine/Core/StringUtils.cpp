#include "StringUtils.h"
// used chatGPT to help me with figureing out what to help here (specifically the cctype)
#include <cctype> // for to upper and to lower

unsigned int StringUtils::uniqueCounter = 0;

//uppy
std::string StringUtils::ToUpper(const std::string& str) {
    std::string result = str;
    //takes the characters from the string and changes then to uppercase
    for (char& c : result) {
        c = std::toupper(c);
    }
    return result;
}

//to make it a little guy, little fella!
std::string StringUtils::ToLower(const std::string& str) {
    std::string result = str;
    //takes the characters from the string and changes then to lowercase
    for (char& c : result) {
        c = std::tolower(c);
    }
    return result;
}

//twostring case thigny majig
bool StringUtils::IsEqualIgnoreCase(const std::string& str1, const std::string& str2) {
    if (str1.length() != str2.length()) { //if the strings are not the same, return false
        return false;
    }

    for (size_t i = 0; i < str1.length(); i++) { // if the 1st sting is not equal to thje 2nd one, return false
        if (std::tolower(str1[i]) != std::tolower(str2[i])) {
            return false;
        }
    }

    return true;
}

std::string StringUtils::CreateUnique(const std::string& str) {
    // Append the uniqueCounter to the input string so i know which string is which
    std::string result = str + std::to_string(uniqueCounter);
    uniqueCounter++;
    return result;
}
