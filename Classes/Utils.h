#ifndef Utils_h
#define Utils_h

#include <string>

class Utils {
public:
    static void printTime(const std::string &className, const std::string &message);
    static void makeToast(const std::string &message);
};

#endif /* Utils_h */
