#include <iostream>
#include <fstream>
#include "Base64.h"

using namespace std;

//#define BASE64_STRING
#define BASE64_STREAM

int main()
{
    ifstream fin("cat.jpg", ios_base::binary);
    ofstream outEncoded("inEncoded.txt", ios_base::binary);
#ifdef  BASE64_STRING
    string str = "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust\n"
            "of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds\n"
            "the short vehemence of any carnal pleasure.";
    outEncoded << Base64::encodeString(str);
#endif

#ifdef BASE64_STREAM
    Base64::encodeStream(fin, outEncoded);
#endif

    outEncoded.close();
    ifstream inEncoded("inEncoded.txt", ios_base::binary);
    ofstream decoded("decoded.jpg", ios_base::binary);

#ifdef  BASE64_STRING
    string enc;
    inEncoded >> noskipws >> enc;
    decoded << Base64::decodeString(enc);
#endif

#ifdef BASE64_STREAM
    Base64::decodeStream(inEncoded, decoded);
#endif

    decoded.close();
    outEncoded.close();
    fin.close();
    return 0;
}