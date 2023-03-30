#define PRIORITY1 PrintPriority<1>()
#ifndef FATROPPRINTERINCLUDED
#define FATROPPRINTERINCLUDED
#include <iostream>
namespace fatrop
{
    static std::ostream nullstream(nullptr);
    template <int Priority>
    struct PrintPriority
    {
        PrintPriority(){};
        const int priority = Priority;
    };
    template<int priority>
    std::ostream &operator<<(std::ostream &os, const PrintPriority<priority> &p)
    {
        if (p.priority >= -1)
            return os;
        return nullstream;
    }
} // namespace fatrop
#endif //  FATROPITERATIONDATAINCLUDED