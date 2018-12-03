//
// Created by ZV on 02.12.2018.
//

#include <fstream>
#include "gtest/gtest.h"
#include "Base64.h"

using namespace std;

string text =
        "Man is distinguished, not only by his reason, "
                "but by this singular passion from other animals, "
                "which is a lust of the mind, that by a perseverance "
                "of delight in the continued and indefatigable generation "
                "of knowledge, exceeds the short vehemence of any carnal pleasure.";

string encoded_text =
        "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0"
                "aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1"
                "c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0"
                "aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdl"
                "LCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=";

string resourcesPath = "../../../tests/base64_tests/resources/";
string encodedPath = "encoded/";
string decodedPath = "decoded/";

TEST(Base64StringEncoding, Test_01)
{
    EXPECT_EQ(Base64::encodeString(""), "");
}

TEST(Base64StringEncoding, Test_02)
{
    EXPECT_EQ(Base64::encodeString("Man"), "TWFu");
}

TEST(Base64StringEncoding, Test_03)
{
    EXPECT_EQ(Base64::encodeString("Hello"), "SGVsbG8=");
}

TEST(Base64StringEncoding, Test_04)
{
    EXPECT_EQ(Base64::encodeString("Hello, world!"), "SGVsbG8sIHdvcmxkIQ==");
}

TEST(Base64StringEncoding, Test_05)
{
    EXPECT_EQ(Base64::encodeString(text), encoded_text);
}

TEST(Base64StringDecoding, Test_01)
{
    EXPECT_EQ(Base64::decodeString(""), "");
}

TEST(Base64StringDecoding, Test_02)
{
    EXPECT_EQ(Base64::decodeString("TWFu"), "Man");
}

TEST(Base64StringDecoding, Test_03)
{
    EXPECT_EQ(Base64::decodeString("SGVsbG8="), "Hello");
}

TEST(Base64StringDecoding, Test_04)
{
    EXPECT_EQ(Base64::decodeString("SGVsbG8sIHdvcmxkIQ=="), "Hello, world!");
}

TEST(Base64StringDecoding, Test_05)
{
    EXPECT_EQ(Base64::decodeString(encoded_text), text);
}

TEST(Base64StringDecoding, Test_06)
{
    EXPECT_THROW((Base64::decodeString("A")), invalid_argument);
}

TEST(Base64StringDecoding, Test_07)
{
    EXPECT_THROW(Base64::decodeString("Ha=a"), invalid_argument);
}

TEST(Base64StringDecoding, Test_08)
{
    EXPECT_THROW(Base64::decodeString("\nasf"), invalid_argument);
    EXPECT_THROW(Base64::decodeString("0\tfv"), invalid_argument);
    EXPECT_THROW(Base64::decodeString("kq[f"), invalid_argument);
    EXPECT_THROW(Base64::decodeString("qfa]"), invalid_argument);
}

TEST(Base64Stream, Test_01)
{
    ifstream input;
    fstream encoded;
    ofstream decoded;

    input.open(resourcesPath + "text.txt", ios_base::binary);
    encoded.open(encodedPath + "text.base64", ios_base::binary | ios_base::out | ios_base::trunc);
    Base64::encodeStream(input, encoded);
    encoded.close();
    input.close();

    encoded.open(encodedPath + "text.base64", ios_base::binary | ios_base::in);
    decoded.open(decodedPath + "decoded_text.txt", ios_base::binary | ios_base::trunc);
    Base64::decodeStream(encoded, decoded);
    decoded.close();
    encoded.close();
}

TEST(Base64Stream, Test_02)
{
    ifstream input;
    fstream encoded;
    ofstream decoded;

    input.open(resourcesPath + "cat.jpg", ios_base::binary);
    encoded.open(encodedPath + "cat.base64", ios_base::binary | ios_base::out | ios_base::trunc);
    Base64::encodeStream(input, encoded);
    encoded.close();
    input.close();

    encoded.open(encodedPath + "cat.base64", ios_base::binary | ios_base::in);
    decoded.open(decodedPath + "decoded_cat.jpg", ios_base::binary | ios_base::trunc);
    Base64::decodeStream(encoded, decoded);
    decoded.close();
    encoded.close();
}

TEST(Base64Stream, Test_03)
{
    ifstream input;
    fstream encoded;
    ofstream decoded;

    input.open(resourcesPath + "screaming_cat.png", ios_base::binary);
    encoded.open(encodedPath + "screaming_cat.base64", ios_base::binary | ios_base::out | ios_base::trunc);
    Base64::encodeStream(input, encoded);
    encoded.close();
    input.close();

    encoded.open(encodedPath + "screaming_cat.base64", ios_base::binary | ios_base::in);
    decoded.open(decodedPath + "decoded_screaming_cat.png", ios_base::binary | ios_base::trunc);
    Base64::decodeStream(encoded, decoded);
    decoded.close();
    encoded.close();
}