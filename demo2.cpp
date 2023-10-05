#include <iostream>
#include <cstdint>

union Number
{
    uint32_t value;
    uint8_t bytes[4];
};

int main()
{
    Number num;
    num.value = 0x12345678;

    if (num.bytes[0] == 0x12 && num.bytes[1] == 0x34 &&
        num.bytes[2] == 0x56 && num.bytes[3] == 0x78)
    {
        std::cout << "0x12345678 is stored as big-endian" << std::endl;
        std::cout << "a = 0x" << std::hex << static_cast<int>(num.bytes[0]) << std::endl;
        std::cout << "b = 0x" << std::hex << static_cast<int>(num.bytes[1]) << std::endl;
        std::cout << "c = 0x" << std::hex << static_cast<int>(num.bytes[2]) << std::endl;
        std::cout << "d = 0x" << std::hex << static_cast<int>(num.bytes[3]) << std::endl;
    }
    else if (num.bytes[0] == 0x78 && num.bytes[1] == 0x56 &&
             num.bytes[2] == 0x34 && num.bytes[3] == 0x12)
    {
        std::cout << "0x12345678 is stored as little-endian" << std::endl;
        std::cout << "a = 0x" << std::hex << static_cast<int>(num.bytes[0]) << std::endl;
        std::cout << "b = 0x" << std::hex << static_cast<int>(num.bytes[1]) << std::endl;
        std::cout << "c = 0x" << std::hex << static_cast<int>(num.bytes[2]) << std::endl;
        std::cout << "d = 0x" << std::hex << static_cast<int>(num.bytes[3]) << std::endl;
    }
    else
    {
        std::cout << "Unable to determine byte order" << std::endl;
    }

    return 0;
}