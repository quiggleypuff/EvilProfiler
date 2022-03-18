# EvilProfiler

EvilProfiler is a simple .NET profiler shellcode loader.

It __does not__ do any profiling.

It decrypts bundled xor encrypted shellcode and executes it in a new thread of the process it is set to profile.

## Compiling EvilProfiler

The `src` folder contains a Visual Studio solution. Just replace `payload.bin` with whatever xor'd shellcode you want, and compile it.

By default the xor key is `key1337`

To use a different key, just change `key` value in `dllmain.cpp`

The included `payload.bin` is a messagebox from `msfvenom`, xor'd with the key `key1337`

## xor script

The `scripts` folder contains a simple python script to xor shellcode, or any file really.

        useage: ./xor.py <input> <xor key> <output>
