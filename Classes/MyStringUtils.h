#ifndef MOPCHIK_IOHELPER_H
#define MOPCHIK_IOHELPER_H

#include <cocos2d.h>

class MyStringUtils {

public:

    static std::vector<std::string> line_split(const std::string &source, const char *delimeter);
    static std::vector<std::string> column_split(const std::string &source, const char *delimeter);
    static std::string split_line_into_2_best_way(const std::string line);

    MyStringUtils(MyStringUtils const&) = delete;
    void operator=(MyStringUtils const&) = delete;
    static MyStringUtils* getInstance();

CC_CONSTRUCTOR_ACCESS:
    MyStringUtils();

protected:
    ~MyStringUtils();
};

#endif //MOPCHIK_IOHELPER_H
