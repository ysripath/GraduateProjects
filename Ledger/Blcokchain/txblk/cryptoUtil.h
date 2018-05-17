#ifndef _CRYPTO_UTIL_H_
#define _CRYPTO_UTIL_H_

#include <string>
#include <cstring>
#include <iostream>
#include <openssl/sha.h>
#include <openssl/rsa.h>

#include <fstream>

#include <streambuf>



// Encode in base64 format
void Base64Encode( const unsigned char* buffer, size_t length, char** base64Text); 



// Obtain the private key data structure from the key string
RSA* createPrivateRSA(std::string key); 

// Function to sign the message using RSA private key
bool RSASign( RSA* rsa, const unsigned char* Msg, size_t MsgLen, unsigned char** EncMsg, size_t* MsgLenEnc); 


// Utility functions for Digital signature verification 

// Obtain the padding length
size_t calcDecodeLength(const char* b64input);

// Decode from the Base64 format
void Base64Decode(char* b64message, unsigned char** buffer, size_t* length);


// Obtain public key data structure from the key string
RSA* createPublicRSA(std::string key);

// Verify using RSA public key
bool RSAVerifySignature( RSA* rsa, unsigned char* MsgHash, size_t MsgHashLen, const char* Msg, size_t MsgLen, bool* Authentic);


// Check if verification is true or false
bool verifySignature(std::string publicKey, std::string plainText, std::string signatureBase64);


/*int main()
{
  std::string plainText = "Test message";
  //std::string privateKey = "";

  //ifstream infile { "private.pem" };
   //std::string privateKey { istreambuf_iterator<char>(infile), istreambuf_iterator<char>() };

std::ifstream t("private.pem");
std::string privateKey((std::istreambuf_iterator<char>(t)),
                 std::istreambuf_iterator<char>());


  RSA* privateRSA = createPrivateRSA(privateKey);
  unsigned char* encMessage;
  char* base64Text;
  size_t encMessageLength;
  RSASign(privateRSA, (unsigned char*) plainText.c_str(), plainText.length(), &encMessage, &encMessageLength);
  Base64Encode(encMessage, encMessageLength, &base64Text);
  free(encMessage);
  //return base64Text;
        std::cout<<"Base64-> "<<base64Text<<"\n";

  std::ifstream t1("public.pem");
std::string publicKey((std::istreambuf_iterator<char>(t1)),
                 std::istreambuf_iterator<char>());

 if (verifySignature(publicKey, plainText, base64Text))
   std::cout<<"Verification OK\n";
 else 
  std::cout<<"Verification fail\n";
  return 0;
}*/
#endif // !_CRYPTO_UTIL_H_
