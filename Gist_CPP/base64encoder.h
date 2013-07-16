#ifndef BASE64ENCODER
#define BASE64ENCODER

#include <string>

using namespace std;

string Base64Encode(const unsigned char* Data,int DataByte);

string Base64Decode(const char* Data,int DataByte);
#endif //BASE64ENCODER