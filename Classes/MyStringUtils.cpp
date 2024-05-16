#include "MyStringUtils.h"

USING_NS_CC;

std::vector<std::string> MyStringUtils::line_split(const std::string &source, const char *delimeter) {
    std::vector<std::string> ret;
    std::string word;
    
    if (!source.empty()) {
        for(size_t i = 0; i < source.size(); ++i){
            if(source.substr(i, 1)== delimeter){
                ret.emplace_back(word);
                word.clear();
            } else {
                word += source[i];
            }
        }
        if (!word.empty()) ret.emplace_back(word);
    }
    
    return ret;
}

std::vector<std::string> MyStringUtils::column_split(const std::string &source, const char *delimeter) {
    std::stringstream ss(source);
    std::vector<std::string> ret;
    std::string word;
    std::string to;
    
    if (!source.empty()) {
        while (std::getline(ss, to)) {
            if (to == delimeter) {
                ret.emplace_back(word);
                word.clear();
            } else {
                word += to;
            }
        }
        if (!word.empty()) ret.emplace_back(word);
    }
    
    return ret;
}

std::string MyStringUtils::split_line_into_2_best_way(const std::string line)
{
    auto split = MyStringUtils::line_split(line, " ");
    if (split.size() <= 1) {
        return line;
    }
    std::string firstLineString;
    std::string secondLineString;
    auto allLength = 0;
    for (int i = 0; i < split.size(); ++i) {
        allLength += split.at(i).length();
    }
    auto firstLineLength = 0;
    auto splitIndex = 0;
    for (int i = 0; i < split.size(); ++i) {
        auto tempLength = split.at(i).length();
        if (firstLineLength + tempLength >= allLength / 2) {
            if (allLength - firstLineLength - tempLength > firstLineLength) splitIndex = i;
            else splitIndex = i - 1;
            break;
        }
        firstLineLength += tempLength;
    }
    firstLineString = split.at(0);
    for (int i = 1; i <= splitIndex; ++i) {
        firstLineString += " " + split.at(i);
    }
    secondLineString = split.at(splitIndex + 1);
    for (int i = splitIndex + 2; i < split.size(); ++i) {
        secondLineString += " " + split.at(i);
    }
    return firstLineString + "\n" + secondLineString;
}

MyStringUtils* MyStringUtils::getInstance()
{
    static MyStringUtils sm_pSharedInstance;
    return &sm_pSharedInstance;
}

MyStringUtils::MyStringUtils()
{
}

MyStringUtils::~MyStringUtils()
{
}
