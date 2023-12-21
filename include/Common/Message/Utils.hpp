#include <string>
class Utils
{
public:
    static std::size_t getBodyLength(const std::string &_str);
    static std::string getChecksum(const std::string &_str);
};
