#ifndef DAPLUGPASS_H_INCLUDED
#define DAPLUGPASS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>

#include <openssl/sha.h>

#include <daplug/keyboard.h>
#include <daplug/DaplugDongle.h>
#include <daplug/sc.h>

#define TRUE 1
#define FALSE 0

#define ADMIN_KEYSET 0x01
#define ADMIN_KEYSET_VALUE "404142434445464748494A4B4C4D4E4F"
#define KEYSETS_DIR "3F00C00FC0DE0001"
#define GP_KEY_SIZE 16

#define AUTH_KEYSET 0xA7
#define GEN_KEYSET 0xB9
#define DAPLUG_PASS_DIR 0xD1A7
#define GEN_DIV_FILE_ID 0xD1B9
#define COMPUTER_LOGIN_FILE_ID 0xD1BA

#define DATA_TO_PASS_LEN 255
#define HMAC_PASS_LEN 20 //20 bytes
#define FINAL_PASS_LEN 16 //16 bytes

#define KEYBOARD_FILE_ID 0x0800
#define KEYBOARD_FILE_SIZE 500

#define SLEEP_BEFORE_PASS_GEN 5 //Duration to wait (in seconds) before generating password

int isDefinedKeyset(int keysetVersion);
int createAuthKeyset(const char *pass);
int deleteAuthKeyset(char *pass);
int createGeneratorKeyset(const char *pass, const char *recoveryKey_in, char *recoveryKey_out);
int deleteGeneratorKeyset(char *pass);
int daplugPassAuth(const char *authPass);
int doHmacSha1(Keyset hmacKeyset, char *inData, char *outData);
int configureKeyboard(char *text, int forComputerLogin);
int playPassword(int forComputerLogin);
void requestAccountInfo(char *userId, char *serviceName);
char *hexstrToModhexstr(const char *hexstr); //free the returned string after use
int isDefinedForcomputerLogin(const char *authPass); //the card is already configured for computer login?

int daplugPassDongleDetection();
int startDaplugPass(char *recoveryKey_in, char *pass);
int makePasssword(const char *authPass, const char * serviceName, const char *userId, int forComputerLogin);

void cleanDaplugPassCard(const char *password);

#ifdef __cplusplus
}
#endif

#endif // DAPLUGPASS_H_INCLUDED
