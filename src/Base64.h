//
// Created by ZV on 29.11.2018.
//

#ifndef BASE64_BASE64_H
#define BASE64_BASE64_H


#include <iostream>
#include <array>
#include <vector>
#include <limits>

class Base64 {
public:
    using Byte = unsigned char;

    static constexpr size_t ENCODE_BUFFER_SIZE = 3;
    static constexpr size_t DECODE_BUFFER_SIZE = 4;

public:
    static std::string encodeString(const std::string& str);

    static std::string decodeString(const std::string& str);

    static void encodeStream(std::istream& is, std::ostream& os);

    static void decodeStream(std::istream& is, std::ostream& os);

private:

    static inline bool isBase64(Byte byte);

    static bool isValid(const std::array<Byte, DECODE_BUFFER_SIZE>& buf);

    static bool isValid(const std::string& str, size_t pos);

    static Byte decodeByte(Byte byte);

private:
    static constexpr char toBase64[] =  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                        "abcdefghijklmnopqrstuvwxyz"
                                        "0123456789+/";
};


#endif //BASE64_BASE64_H
