@mkdir obj\x64 2>nul
x86_64-w64-mingw32-g++.exe -O3 -c ReplaceLanguage-no_links-cyrillic_only.cpp -o obj/x64/RL-cyrillic.o
@mkdir bin\x64 2>nul
x86_64-w64-mingw32-g++.exe -o bin/x64/RL-cyrillic.exe obj/x64/RL-cyrillic.o -s -static
