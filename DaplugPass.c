#include "DaplugPass.h"

char *hexstrToModhexstr(const char *hexstr){

    char *mapping[3] = {"0123456789ABCDEF",
                        "0123456789abcdef",
                        "CBDEFGHIJKLNRTUV"};
    int hexstrLen = 0;
    int i = 0, j = 0;

    hexstrLen = strlen(hexstr);
    char * modhexstr = NULL;
    modhexstr = (char*) calloc(hexstrLen + 1, sizeof(char));

    if(!isHexInput(hexstr)){
        fprintf(stderr, "\nhexstrToModhexstr(): Not a valid hex string : %s\n", hexstr);
        return NULL;
    }

    for(i=0;i<hexstrLen;i++){
        for(j=0;j<16;j++){
            if((hexstr[i] == mapping[0][j]) || (hexstr[i] == mapping[1][j])){
                modhexstr[i] = mapping[2][j];
                break;
            }
        }
    }

    return modhexstr;
}

int isDefinedKeyset(int keysetVersion){

    char keysetPath[255] = "";
    char keysetVersion_str[1*2+1] = "";

    sprintf(keysetVersion_str,"%02X",keysetVersion);
    strcat(keysetPath, KEYSETS_DIR);
    strcat(keysetPath, "10");
    strcat(keysetPath, keysetVersion_str);

    if(!Daplug_selectPath(keysetPath)){
        fprintf(stderr, "\nisDefinedKeyset() : Keyset 0x%02X does not exist !\n", keysetVersion);
        return FALSE;
    }

    return TRUE;

}

int createAuthKeyset(const char *pass){

    //Check if auth keyset already exist
    if(isDefinedKeyset(AUTH_KEYSET)){
        fprintf(stderr, "\ncreateAuthKeyset() : Cannot create authentication keyset : already exist !\n");
        return 0;
    }

    //SHA1
    unsigned char outBuf[20];
    SHA1((unsigned char *)pass, strlen(pass), outBuf);

    //Left 16
    char passKey[GP_KEY_SIZE*2+1] = "";
    bytesToStr(outBuf, GP_KEY_SIZE, passKey);

    //First, open SC with the admin keyset (0x01)
    Keyset adminKeyset;
    keyset_createKeys(&adminKeyset, ADMIN_KEYSET, ADMIN_KEYSET_VALUE,"","");
    if(!Daplug_authenticate(adminKeyset,C_MAC,"","")){
        fprintf(stderr, "\ncreateAuthKeyset() : Cannot create authentication keyset : permission denied !\n");
        return 0;
    }

    //Create auth keyset (version AUTH_KEYSET)
    Keyset authKeyset;
    keyset_createKeys(&authKeyset,AUTH_KEYSET,passKey,"","");
    authKeyset.usage = USAGE_GP + 0x80; //+ 0x80 => the parent of the keyset is the keyset itself
    Byte access[2] = {0x00, C_MAC};
    keyset_setKeyAccess(&authKeyset,access);

    if(!Daplug_putKey(authKeyset)){
        fprintf(stderr, "\ncreateAuthKeyset() : Cannot create authentication keyset !\n");
        return 0;
    }

    //Create DAPLUG PASS DIR
    if(!Daplug_selectPath("3f00")){
        fprintf(stderr, "\ncreateAuthKeyset() : Cannot create authentication keyset !\n");
        return 0;
    }
    int file_access[] = {AUTH_KEYSET, AUTH_KEYSET, AUTH_KEYSET};
    if(!Daplug_createDir(DAPLUG_PASS_DIR, file_access)){
        fprintf(stderr, "\nncreateAuthKeyset() : Cannot create authentication keyset !\n");
        return 0;
    }

    fprintf(stdout, "\ncreateAuthKeyset() : Authentication keyset successfuly created.\n");

    return 1;
}

int deleteAuthKeyset(char *pass){

    //Check if auth keyset already exist
    if(!isDefinedKeyset(AUTH_KEYSET)){
        fprintf(stderr, "\ndeleteAuthKeyset() : Cannot delete authentication keyset : Does not exist !\n");
        return 0;
    }

    //SHA1
    unsigned char outBuf[20];
    SHA1((unsigned char *)pass, strlen(pass), outBuf);

    //Left 16
    char passKey[GP_KEY_SIZE*2+1] = "";
    bytesToStr(outBuf, GP_KEY_SIZE, passKey);

    //open SC with the auth keyset to grant permissions
    Keyset authKeyset;
    keyset_createKeys(&authKeyset, AUTH_KEYSET, passKey,"","");
    if(!Daplug_authenticate(authKeyset,C_MAC,"","")){
        fprintf(stderr, "\ndeleteAuthKeyset() : Cannot delete authentication keyset : permission denied !\n");
        return 0;
    }

    //Delete it
    if(!Daplug_deleteKey(AUTH_KEYSET)){
        fprintf(stderr, "\ndeleteAuthKeyset() : Cannot delete authentication keyset !\n");
        return 0;
    }

    fprintf(stdout, "\ncreateAuthKeyset() : Authentication keyset successfuly deleted.\n");

    return 1;
}

//hmac-sha1 keyset
int createGeneratorKeyset(const char *pass, const char *recoveryKey_in, char *recoveryKey_out){

    //Check if generator keyset already exist
    if(isDefinedKeyset(GEN_KEYSET)){
        fprintf(stderr, "\ncreateGeneratorKeyset() : Cannot create generator keyset : already exist !\n");
        return 0;
    }

    //Check if auth keyset already exist
    if(!isDefinedKeyset(AUTH_KEYSET)){
        fprintf(stderr, "\ncreateGeneratorKeyset() : Cannot create generator keyset !\n");
        return 0;
    }

    //AUTH KEYSET VALUE
    //SHA1
    unsigned char outBuf[20];
    SHA1((unsigned char *)pass, strlen(pass), outBuf);
    //Left 16
    char passKey[GP_KEY_SIZE*2+1] = "";
    bytesToStr(outBuf, GP_KEY_SIZE, passKey);


    char diversifier[16*2+1] = "";
    if(recoveryKey_in == NULL){
        //generate random diversifier
        //Display this diversifier and told user to securely save it
        //It will be required to recover user DaplugPass dongle in case of loss
        if(!Daplug_getRandom(16, diversifier)){
            fprintf(stderr, "\ncreateGeneratorKeyset() : Cannot create generator keyset !\n");
            return 0;
        }
    }else{ //recovery mode
        //diversifier validity
        if(!isHexInput(recoveryKey_in) || (strlen(recoveryKey_in)/2) != 16){
            fprintf(stderr, "\ncreateGeneratorKeyset() - Invalid recovery key : %s\n", recoveryKey_in);
            fprintf(stderr, "\ncreateGeneratorKeyset() : Cannot create generator keyset !\n");
            return 0;
        }
        strcpy(diversifier, recoveryKey_in);
    }

    //Diversify auth keyset keys values => generator keyset keys values
    Keyset authKeyset, genKeyset;
    //Auth keyset
    keyset_createKeys(&authKeyset, AUTH_KEYSET, passKey,"","");
    //Generator keyset
    Daplug_computeDiversifiedKeys(authKeyset, &genKeyset, diversifier);
    keyset_setVersion(&genKeyset, GEN_KEYSET);
    genKeyset.usage = USAGE_HMAC_SHA1;
    Byte access[2] = {AUTH_KEYSET, 48};
    keyset_setKeyAccess(&genKeyset,access);

    //Open SC with the auth keyset
    if(!Daplug_authenticate(authKeyset,C_MAC,"","")){
        fprintf(stderr, "\ncreateGeneratorKeyset() : Cannot create generator keyset : permission denied !\n");
        return 0;
    }

    //Save generated diversifier on the dongle : /3F00/GEN_DIV_FILE_ID
    if(!Daplug_selectPath("3f00")){
        fprintf(stderr, "\ncreateGeneratorKeyset() : Cannot create generator keyset !\n");
        return 0;
    }
    if(!Daplug_selectFile(DAPLUG_PASS_DIR)){
        fprintf(stderr, "\ncreateGeneratorKeyset() : Cannot create generator keyset !\n");
        return 0;
    }
    int file_access[] = {AUTH_KEYSET, AUTH_KEYSET, AUTH_KEYSET};
    if(!Daplug_createFile(GEN_DIV_FILE_ID, 16, file_access, 0, 0)){
        fprintf(stderr, "\ncreateGeneratorKeyset() : Cannot create generator keyset !\n");
        return 0;
    }
    if(!Daplug_writeData(0, diversifier)){
        fprintf(stderr, "\ncreateGeneratorKeyset() : Cannot create generator keyset !\n");
        return 0;
    }

    //create keyboard file
    if(!Daplug_selectPath("3f00")){
        fprintf(stderr, "\ncreateGeneratorKeyset() : Cannot create generator keyset !\n");
        return 0;
    }
    if(!Daplug_selectFile(DAPLUG_PASS_DIR)){
        fprintf(stderr, "\ncreateGeneratorKeyset() : Cannot create generator keyset !\n");
        return 0;
    }
    int kb_file_access[] = {AUTH_KEYSET, AUTH_KEYSET, AUTH_KEYSET};
    if(!Daplug_createFile(KEYBOARD_FILE_ID, KEYBOARD_FILE_SIZE, kb_file_access, 0, 0)){
        fprintf(stderr, "\ncreateGeneratorKeyset() : Cannot create generator keyset !\n");
        return 0;
    }

    //create computer login file (the same length as keyboard file)
    if(!Daplug_selectPath("3f00")){
        fprintf(stderr, "\ncreateGeneratorKeyset() : Cannot create generator keyset !\n");
        return 0;
    }
    if(!Daplug_selectFile(DAPLUG_PASS_DIR)){
        fprintf(stderr, "\ncreateGeneratorKeyset() : Cannot create generator keyset !\n");
        return 0;
    }
    int cl_file_access[] = {AUTH_KEYSET, AUTH_KEYSET, AUTH_KEYSET};
    if(!Daplug_createFile(COMPUTER_LOGIN_FILE_ID, KEYBOARD_FILE_SIZE, cl_file_access, 0, 0)){
        fprintf(stderr, "\ncreateGeneratorKeyset() : Cannot create generator keyset !\n");
        return 0;
    }

    //Upload gen keyset (version GEN_KEYSET)
    if(!Daplug_putKey(genKeyset)){
        fprintf(stderr, "\ncreateGeneratorKeyset() : Cannot create generator keyset !\n");
        return 0;
    }

    //Recovery key
    strcpy(recoveryKey_out, diversifier);

    fprintf(stdout, "\ncreateAuthKeyset() : generator keyset successfuly created.\n");

    return 1;

}

int deleteGeneratorKeyset(char *pass){

    //Check if generator keyset already exist
    if(!isDefinedKeyset(GEN_KEYSET)){
        fprintf(stderr, "\ndeleteGeneratorKeyset() : Cannot delete generator keyset : Does not exist !\n");
        return 0;
    }

    //Check if auth keyset already exist
    if(!isDefinedKeyset(AUTH_KEYSET)){
        fprintf(stderr, "\ndeleteGeneratorKeyset() : Cannot delete generator keyset !\n");
        return 0;
    }

    //AUTH KEYSET VALUE
    //SHA1
    unsigned char outBuf[20];
    SHA1((unsigned char *)pass, strlen(pass), outBuf);
    //Left 16
    char passKey[GP_KEY_SIZE*2+1] = "";
    bytesToStr(outBuf, GP_KEY_SIZE, passKey);

    //Open SC with the auth keyset
    Keyset authKeyset;
    keyset_createKeys(&authKeyset, AUTH_KEYSET, passKey,"","");
    if(!Daplug_authenticate(authKeyset,C_MAC,"","")){
        fprintf(stderr, "\ndeleteGeneratorKeyset() : Cannot delete generator keyset : permission denied !\n");
        return 0;
    }

    //Delete it
    if(!Daplug_deleteKey(GEN_KEYSET)){
        fprintf(stderr, "\ndeleteGeneratorKeyset() : Cannot delete generator keyset !\n");
        return 0;
    }

    //Delete Generator Div Dir
    if(!Daplug_selectPath("3f00")){
        fprintf(stderr, "\ndeleteGeneratorKeyset() : Cannot delete generator keyset !\n");
        return 0;
    }
    if(!Daplug_deleteFileOrDir(DAPLUG_PASS_DIR)){
        fprintf(stderr, "\ndeleteGeneratorKeyset() : Cannot delete generator keyset !\n");
        return 0;
    }

    fprintf(stdout, "\ndeleteGeneratorKeyset() : Generator keyset successfuly deleted.\n");

    return 1;

}

int makePasssword(const char *authPass, const char *serviceName, const char *userId, int forComputerLogin){

    //Check if generator keyset already exist
    if(!isDefinedKeyset(GEN_KEYSET)){
        fprintf(stderr, "\nmakePasssword() : Cannot make password !\n");
        return 0;
    }

    //Check if auth keyset already exist
    if(!isDefinedKeyset(AUTH_KEYSET)){
        fprintf(stderr, "\nmakePasssword() : Cannot make password !\n");
        return 0;
    }

    //AUTH KEYSET VALUE
    //SHA1
    unsigned char outBuf[20];
    SHA1((unsigned char *)authPass, strlen(authPass), outBuf);
    //Left 16
    char passKey[GP_KEY_SIZE*2+1] = "";
    bytesToStr(outBuf, GP_KEY_SIZE, passKey);

    //Retreive diversifier
    char diversifier[16*2+1] = "";
    //Open SC with the auth keyset
    Keyset authKeyset;
    keyset_createKeys(&authKeyset, AUTH_KEYSET, passKey,"","");
    if(!Daplug_authenticate(authKeyset,C_MAC,"","")){
        fprintf(stderr, "\nmakePasssword() - Cannot make password : permission denied !\n");
        return 0;
    }

    //Read gen div file : /3F00/GEN_DIV_FILE_ID
    if(!Daplug_selectPath("3f00")){
        fprintf(stderr, "\nmakePasssword() : Cannot make password !\n");
        return 0;
    }
    if(!Daplug_selectFile(DAPLUG_PASS_DIR)){
        fprintf(stderr, "\nmakePasssword() : Cannot make password !\n");
        return 0;
    }
    if(!Daplug_selectFile(GEN_DIV_FILE_ID)){
        fprintf(stderr, "\nmakePasssword() : Cannot make password !\n");
        return 0;
    }
    if(!Daplug_readData(0, 16, diversifier)){
        fprintf(stderr, "\nmakePasssword() : Cannot make password !\n");
        return 0;
    }

    //Diversify auth keyset keys values => generator keyset keys values
    Keyset genKeyset;
    Daplug_computeDiversifiedKeys(authKeyset, &genKeyset, diversifier);
    keyset_setVersion(&genKeyset, GEN_KEYSET);

    //do hmac = generate password
    char inData[DATA_TO_PASS_LEN] = "";
    char hexData[DATA_TO_PASS_LEN*2+1] = "";
    char outData[HMAC_PASS_LEN*2+1] = "";
    char finalOutData[FINAL_PASS_LEN*2+1] = "";

    strcat(inData, serviceName);
    strcat(inData, userId);

    //To hex
    asciiToHex(inData, hexData);
    if(!doHmacSha1(genKeyset, hexData, outData)){
        fprintf(stderr, "\nmakePasssword() : Cannot make password !\n");
        return 0;
    }

    //16 first bytes
    char* tmp = str_sub(outData, 0, FINAL_PASS_LEN*2 -1);
    strcpy(finalOutData, tmp);
    free(tmp);

    //Modhex
    if((tmp = hexstrToModhexstr(finalOutData)) == NULL){
        fprintf(stderr, "\nmakePasssword() : Cannot make password !\n");
        return 0;
    }

    if(!configureKeyboard(tmp, forComputerLogin)){
        fprintf(stderr, "\nmakePasssword() : Cannot make password !\n");
        return 0;
    }

    free(tmp);

    return 1;

}

int doHmacSha1(Keyset hmacKeyset, char *inData, char *outData){

    int options = OTP_0_DIV;

    if(!Daplug_hmac(hmacKeyset.version,options,"","",inData,outData)){
        fprintf(stderr, "\ndoHmacSha1() : Cannot sign data !\n");
        return 0;
    }

    return 1;

}

int configureKeyboard(char *text, int forComputerLogin){

    Keyboard kb;
    keyboard_init(&kb);

    #ifdef __linux__
        keyboard_addSleep(&kb, -1);
        keyboard_addTextMac(&kb,text,1,-1);
    #endif

    #ifdef _WIN32
        keyboard_addOSProbe(&kb,-1,-1,-1);
        keyboard_addIfPC(&kb);
        keyboard_addTextWindows(&kb,text);
    #endif

    #ifdef __APPLE__
        keyboard_addTextMac(&kb,text,1,-1);
    #endif

    if(forComputerLogin){
        //Add carriage return
        keyboard_addReturn(&kb);
    }

    //ensure zeroized to avoid misinterpretaion
    keyboard_zeroPad(&kb,KEYBOARD_FILE_SIZE);

    if(forComputerLogin){

        //=== write content to the computer login file
        //select file
        if(!Daplug_selectPath("3f00")){
            fprintf(stderr, "\nconfigureKeyboard() : Keyboard configuration failed !\n");
            return 0;
        }
        if(!Daplug_selectFile(DAPLUG_PASS_DIR)){
            fprintf(stderr, "\nconfigureKeyboard() : Keyboard configuration failed !\n");
            return 0;
        }
        if(!Daplug_selectFile(COMPUTER_LOGIN_FILE_ID)){
            fprintf(stderr, "\nconfigureKeyboard() : Keyboard configuration failed !\n");
            return 0;
        }
        //write content
        if(!Daplug_writeData(0,kb.content)){
            fprintf(stderr, "\nconfigureKeyboard() : Keyboard configuration failed !\n");
            return 0;
        }
        //==============================================
    }

    // === Write content to the keyboard file
    //select file
    if(!Daplug_selectPath("3f00")){
        fprintf(stderr, "\nconfigureKeyboard() : Keyboard configuration failed !\n");
        return 0;
    }
    if(!Daplug_selectFile(DAPLUG_PASS_DIR)){
        fprintf(stderr, "\nconfigureKeyboard() : Keyboard configuration failed !\n");
        return 0;
    }
    if(!Daplug_selectFile(KEYBOARD_FILE_ID)){
        fprintf(stderr, "\nconfigureKeyboard() : Keyboard configuration failed !\n");
        return 0;
    }
    //write content
    if(!Daplug_writeData(0,kb.content)){
        fprintf(stderr, "\nconfigureKeyboard() : Keyboard configuration failed !\n");
        return 0;
    }
    //Set file as keyboard
    if(!Daplug_useAsKeyboard()){
        fprintf(stderr, "\nconfigureKeyboard() : Keyboard configuration failed !\n");
        return 0;
    }
    //==========================================

    //Activate keyboard emulation
    if(!Daplug_setKeyboardAtBoot(1)){
        fprintf(stderr, "\nconfigureKeyboard() : Cannot play password !\n");
        return 0;
    }

    if(forComputerLogin == 0){

        printf("\nPlease, make the focus on the password text area :\n");
        printf("\n...\n");
        sleep(SLEEP_BEFORE_PASS_GEN);

        //play content
        if(!Daplug_triggerKeyboard()){
            fprintf(stderr, "\nconfigureKeyboard() : Cannot play password !\n");
            return 0;
        }

        //Deativate keyboard emulation
        if(!Daplug_setKeyboardAtBoot(0)){
            fprintf(stderr, "\nconfigureKeyboard() : Cannot play password !\n");
            return 0;
        }

        // === Set the computer login file as keyboard (if it is already set) :
        // => so you can always use your daplug dongle for computer login
        //select file
        if(!Daplug_selectPath("3f00")){
            fprintf(stderr, "\nconfigureKeyboard() : Keyboard configuration failed !\n");
            return 0;
        }
        if(!Daplug_selectFile(DAPLUG_PASS_DIR)){
            fprintf(stderr, "\nconfigureKeyboard() : Keyboard configuration failed !\n");
            return 0;
        }
        if(!Daplug_selectFile(COMPUTER_LOGIN_FILE_ID)){
            fprintf(stderr, "\nconfigureKeyboard() : Keyboard configuration failed !\n");
            return 0;
        }
        //Set file as keyboard
        if(!Daplug_useAsKeyboard()){
            fprintf(stderr, "\nconfigureKeyboard() : Keyboard configuration failed !\n");
            return 0;
        }

        //Activate keyboard emulation again
        if(!Daplug_setKeyboardAtBoot(1)){
            fprintf(stderr, "\nconfigureKeyboard() : Cannot play password !\n");
            return 0;
        }

    }

    return 1;

}

int daplugPassAuth(const char *authPass){

    //AUTH KEYSET VALUE
    //SHA1
    unsigned char outBuf[20];
    SHA1((unsigned char *)authPass, strlen(authPass), outBuf);
    //Left 16
    char passKey[GP_KEY_SIZE*2+1] = "";
    bytesToStr(outBuf, GP_KEY_SIZE, passKey);

    //Open SC with the auth keyset
    Keyset authKeyset;
    keyset_createKeys(&authKeyset, AUTH_KEYSET, passKey,"","");
    if(!Daplug_authenticate(authKeyset,C_MAC,"","")){
        fprintf(stderr, "\ndaplugPassAuth() : Authentication failed !\n");
        return 0;
    }

    return 1;
}

int isDefinedForcomputerLogin(const char *authPass){

    //returns 1 if content exists, 0 if not, -1 if error

    //Authentication
    if(!daplugPassAuth(authPass)){
        fprintf(stderr, "\nisDefinedForcomputerLogin() : An error occured during the check !\n");
        return -1;
    }

    //select the computer login file
    if(!Daplug_selectPath("3f00")){
        fprintf(stderr, "\nisDefinedForcomputerLogin() : An error occured during the check !\n");
        return -1;
    }
    if(!Daplug_selectFile(DAPLUG_PASS_DIR)){
        fprintf(stderr, "\nisDefinedForcomputerLogin() : An error occured during the check !\n");
        return -1;
    }
    if(!Daplug_selectFile(COMPUTER_LOGIN_FILE_ID)){
        fprintf(stderr, "\nisDefinedForcomputerLogin() : An error occured during the check !\n");
        return -1;
    }
    char oldContent[KEYBOARD_FILE_SIZE*2+1] = "";

    if(!Daplug_readData(0, KEYBOARD_FILE_SIZE, oldContent)){
        fprintf(stderr, "\nisDefinedForcomputerLogin() : An error occured during the check !\n");
        return -1;
    }

    //Take a header of 3 bytes (juste for compare, we can take a value other than 3)
    char *tmp = NULL;
    tmp = str_sub(oldContent,0,5);

    fprintf(stderr, "header = %s", tmp);

    if(strcmp(tmp, "ffffff")){ //a new created EF content is ffffff...
       free(tmp);
       return 1;
    }else{
        free(tmp);
        return 0;
    }
}

void cleanDaplugPassCard(const char *password){

    //Check dongle
    if(!Daplug_getFirstDongle()){
        fprintf(stderr, "\nNo dongle inserted !\n");
        return;
    }

    //Check config
    if(!isDefinedKeyset(AUTH_KEYSET) && !isDefinedKeyset(GEN_KEYSET)){
        fprintf(stderr, "\nYour Daplug dongle is not configured yet.\n");
        return;
    }

    //Authentication
    if(!daplugPassAuth(password)){
        fprintf(stderr, "\nAuthentication failed !\n");
        return;
    }

    //Clean FS
    if(!Daplug_selectFile(FS_MASTER_FILE)){
        fprintf(stderr, "\nCannot clean FS !\n");
        return;
    }else{
        if(!Daplug_deleteFileOrDir(DAPLUG_PASS_DIR)){
            fprintf(stderr, "\nCannot clean FS !\n");
            return;
        }
    }

    //Remove keysets
    if(!Daplug_deleteKey(GEN_KEYSET)){
        return;
    }else{
        if(!Daplug_deleteKey(AUTH_KEYSET)){
            return;
        }
    }

    fprintf(stderr, "\nDaplugPass card cleaned successfully.\n");

}

