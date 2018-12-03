//
// Created by ZV on 29.11.2018.
//

#include "Base64.h"

using namespace std;

constexpr char Base64::toBase64[];

std::string Base64::encodeString(const std::string& str)
{
    string encoded;
    size_t range = str.size() - str.size() % ENCODE_BUFFER_SIZE;
    for (size_t i = 0; i < range; i += ENCODE_BUFFER_SIZE)
    {
        encoded += toBase64[(str[i] & 0xfcu) >> 2];
        encoded += toBase64[((str[i] & 0x03u) << 4) + ((str[i + 1] & 0xf0u) >> 4)];
        encoded += toBase64[((str[i + 1] & 0x0fu) << 2) + ((str[i + 2] & 0xc0u) >> 6)];
        encoded += toBase64[str[i + 2] & 0x3fu];
    }

    if (str.size() == range)
        return encoded;
    encoded += toBase64[(str[range] & 0xfcu) >> 2];
    if (str.size() - range == 1)
    {
        encoded += toBase64[(str[range] & 0x03u) << 4];
        encoded += "==";
    }
    else if (str.size() - range == 2)
    {
        encoded += toBase64[((str[range] & 0x03u) << 4) + ((str[range + 1] & 0xf0u) >> 4)];
        encoded += toBase64[(str[range + 1] & 0x0fu) << 2];
        encoded += '=';
    }
    return encoded;
}

std::string Base64::decodeString(const std::string& str)
{
    if (str.empty())
        return "";
    if (str.size() % DECODE_BUFFER_SIZE != 0)
        throw std::invalid_argument("String didn't contain right amount of characters to decode");

    string decoded;
    size_t range = str[str.size() - 1] == '=' ? str.size() - DECODE_BUFFER_SIZE : str.size();
    for (size_t i = 0; i < range; i += DECODE_BUFFER_SIZE)
    {
        if (!isValid(str, i))
            throw std::invalid_argument("String contents don't satisfy Base64");

        decoded += static_cast<Byte>((decodeByte((str[i])) << 2) + (decodeByte((str[i + 1])) >> 4));
        decoded += static_cast<Byte>((decodeByte((str[i + 1])) << 4) + (decodeByte((str[i + 2])) >> 2));
        decoded += static_cast<Byte>((decodeByte((str[i + 2])) << 6) + decodeByte((str[i + 3])));
    }

    if (str.size() == range)
        return decoded;

    if (!(isBase64(str[range]) && isBase64(str[range + 1]) && (isBase64(str[range + 2]) || str[range + 2] == '=')))
        throw std::invalid_argument("String contents don't satisfy Base64");

    decoded += static_cast<Byte>((decodeByte(str[range]) << 2) + (decodeByte(str[range + 1]) >> 4));
    if (str[range + 2] != '=')
        decoded += static_cast<Byte>((decodeByte(str[range + 1]) << 4) + (decodeByte(str[range + 2]) >> 2));
    return decoded;
}

void Base64::encodeStream(std::istream& is, std::ostream& os)
{
    size_t charsRead = 0;
    array<Byte, ENCODE_BUFFER_SIZE> buf{{}};


    while (charsRead < ENCODE_BUFFER_SIZE && /*is >> buf[charsRead]*/ is.get(buf[charsRead]))
        ++charsRead;
    while (charsRead == ENCODE_BUFFER_SIZE)
    {
        //TODO: leave comment about what happens here
        os << toBase64[(buf[0] & 0xfcu) >> 2]
           << toBase64[((buf[0] & 0x03u) << 4) + ((buf[1] & 0xf0u) >> 4)]
           << toBase64[((buf[1] & 0x0fu) << 2) + ((buf[2] & 0xc0u) >> 6)]
           << toBase64[buf[2] & 0x3fu];

        charsRead = 0;
        while (charsRead < ENCODE_BUFFER_SIZE && /*is >> buf[charsRead]*/is.get(buf[charsRead]))
            ++charsRead;
    }

    if (charsRead == 0)
        return;

    os << toBase64[(buf[0] & 0xfcu) >> 2];
    if (charsRead == 1)
        os << toBase64[(buf[0] & 0x03u) << 4] << "==";
    else if (charsRead == 2)
    {
        os << toBase64[((buf[0] & 0x03u) << 4) + ((buf[1] & 0xf0u) >> 4)]
           << toBase64[(buf[1] & 0x0fu) << 2]
           << '=';
    }
}

void Base64::decodeStream(std::istream& is, std::ostream& os)
{
    array<Byte, DECODE_BUFFER_SIZE> buf{{}};
    size_t charsRead = 0;

    while (charsRead < DECODE_BUFFER_SIZE && is.get(buf[charsRead]))
        ++charsRead;
    while (charsRead == DECODE_BUFFER_SIZE && buf[DECODE_BUFFER_SIZE - 1] != '=')
    {
        // Check read bytes for validity
        if (!isValid(buf))
            throw std::invalid_argument("Passed input stream contents didn't satisfy Base64");

        // TODO: leave comment about what happens here
        os << static_cast<Byte>((decodeByte(buf[0]) << 2) + (decodeByte(buf[1]) >> 4))
           << static_cast<Byte>((decodeByte(buf[1]) << 4) + (decodeByte(buf[2]) >> 2))
           << static_cast<Byte>((decodeByte(buf[2]) << 6) + decodeByte(buf[3]));

        charsRead = 0;
        while (charsRead < DECODE_BUFFER_SIZE && is.get(buf[charsRead]))
            ++charsRead;
    }

    // Check that we have something to write and it's valid
    if (charsRead == 0)
        return;

    if (charsRead < DECODE_BUFFER_SIZE)
        throw std::invalid_argument("Passed input stream didn't contain enough characters to decode stream");

    if (!(isBase64(buf[0]) && isBase64(buf[1]) && (isBase64(buf[2]) || buf[2] == '=')))
        throw std::invalid_argument("String contents don't satisfy Base64");

    os << static_cast<Byte>((decodeByte(buf[0]) << 2) + (decodeByte(buf[1]) >> 4));
    if (buf[2] != '=')
        os << static_cast<Byte>((decodeByte(buf[1]) << 4) + (decodeByte(buf[2]) >> 2));
}

bool Base64::isBase64(Byte byte) { return isalnum(byte) || byte == '+' || byte == '/'; }

bool Base64::isValid(const std::array<Byte, DECODE_BUFFER_SIZE>& buf)
{
    for(const auto& byte:buf)
    {
        if(!isBase64(byte))
            return false;
    }
    return true;
}

//TODO:: change this so it doesn't allow "==" and "=" in string
bool Base64::isValid(const std::string& str, size_t pos)
{
    size_t end = pos + DECODE_BUFFER_SIZE;
    for (pos; pos < end; ++pos)
    {
        if (!isBase64(str[pos]))
            return false;
    }
    return true;
}

Base64::Byte Base64::decodeByte(Byte byte)
{
    if ('A' <= byte && byte <= 'Z')
        return byte - 'A';
    if ('a' <= byte && byte <= 'z')
        return byte + 26 - 'a';
    if ('0' <= byte && byte <= '9')
        return byte + 52 - '0';
    if (byte == '+')
        return 62;
    if (byte == '/')
        return 63;
    throw std::invalid_argument("Passed byte isn't Base64");
}