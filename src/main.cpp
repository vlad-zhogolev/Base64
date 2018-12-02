#include <iostream>
#include <fstream>
#include "Base64.h"

using namespace std;

//#define BASE64_STRING
#define BASE64_STREAM

int main()
{
    ifstream fin("input.txt");
    fstream encoded("encoded.txt");
    ofstream decoded("decoded.txt");
#ifdef  BASE64_STRING
    string str = "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust\n"
            "of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds\n"
            "the short vehemence of any carnal pleasure.";
    encoded << Base64::encodeString(str);
#endif

#ifdef BASE64_STREAM
    Base64::encodeStream(fin, encoded);
#endif

    encoded.close();
    encoded.open("encoded.txt");

#ifdef  BASE64_STRING
    string enc;
    encoded >> noskipws >> enc;
    decoded << Base64::decodeString(enc);
#endif

#ifdef BASE64_STREAM
    Base64::decodeStream(encoded, decoded);
#endif

    decoded.close();
    encoded.close();
    fin.close();
    return 0;
}