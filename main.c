#include <windows.h>
#include <stdio.h>

//simple app to add to the windows right click menu a shell cmd, please rememer to run it with administrator privileges.

void addMenu();
void removeMenu();
LONG setRegValue(HKEY hkey, LPCSTR subkey, LPCSTR valueName, LPCSTR data);

int main(int argc, char **argv)
{
	
    if (argc != 2){ printf("Usage: %s add|remove\n", argv[0]); return 1;}

    if (strcmp(argv[1], "add") == 0){
        
        addMenu();
        printf("Menu Option added successfully.\n");
    }
    else if (strcmp(argv[1], "remove") == 0) {
        
        removeMenu();
        printf("Menu Option removed successfully.\n");
    
    }
    else {
        printf("Invalid argument. Usage: %s add|remove\n", argv[0]);
    }
    
    return 0;
}

void addMenu(){
    
    HKEY hKey;

    if (RegCreateKeyEx(HKEY_CLASSES_ROOT, "Directory\\Background\\shell\\OpenCmdHere", 0, NULL, REG_OPTION_NON_VOLATILE, 
    KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS){ // if successful returns error_success
        
        setRegValue(hKey, NULL, "", "Open Command Prompt");
        setRegValue(hKey, NULL, "Icon", "cmd.exe");
        
        HKEY commandK; //reg subkey as the cmd
        
        if (RegCreateKeyEx(hKey, "command", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &commandK, NULL) == ERROR_SUCCESS) {
            
            setRegValue(commandK, NULL, "", "cmd.exe /s /k pushd \"%V\"");
            RegCloseKey(commandK);}
    
    RegCloseKey(hKey);}
}

void removeMenu() {
    
    RegDeleteKeyEx(HKEY_CLASSES_ROOT, "Directory\\Background\\shell\\OpenCmdHere\\command", KEY_WOW64_64KEY, 0);
    RegDeleteKeyEx(HKEY_CLASSES_ROOT, "Directory\\Background\\shell\\OpenCmdHere", KEY_WOW64_64KEY, 0);
}

LONG setRegValue(HKEY hKey, LPCSTR subKey, LPCSTR valueName, LPCSTR data) {
    HKEY hk;
    // Open the specified subkey for setting values (if needed)
    LONG result = RegOpenKeyEx(hKey, subKey, 0, KEY_SET_VALUE, &hk);
    
    if (result == ERROR_SUCCESS) {
        
        result = RegSetValueEx(hk, valueName, 0, REG_SZ, (const BYTE*)data, strlen(data) + 1);
        RegCloseKey(hk); 
    }
    
    return result; // Return the result code for success or failure
    
}