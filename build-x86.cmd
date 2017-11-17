@mkdir obj\x86 2>nul
i686-w64-mingw32-g++.exe -O3 -c ReplaceLanguage.cpp -o obj/x86/ReplaceLanguage.o
@mkdir bin\x86 2>nul
i686-w64-mingw32-g++.exe -o bin/x86/ReplaceLanguage.exe obj/x86/ReplaceLanguage.o -s -static
