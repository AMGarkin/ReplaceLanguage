@mkdir obj\x64 2>nul
x86_64-w64-mingw32-g++.exe -O3 -c ReplaceLanguage.cpp -o obj/x64/ReplaceLanguage.o
@mkdir bin\x64 2>nul
x86_64-w64-mingw32-g++.exe -o bin/x64/ReplaceLanguage.exe obj/x64/ReplaceLanguage.o -s -static
