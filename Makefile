CFLAGS		+= -s -O3

ifeq ($(shell uname -s), Darwin)
  CC		= gcc-8
  CXX		= g++-8
else
  CLIBS	+= -static
endif

all:  obj bin ReplaceLanguage

ReplaceLanguage: ReplaceLanguage.o
	 $(CXX) $(CFLAGS) -o bin/ReplaceLanguage obj/ReplaceLanguage.o $(CLIBS)
ReplaceLanguage.o: ReplaceLanguage.cpp
	 $(CXX) $(CFLAGS) -c ReplaceLanguage.cpp -o obj/ReplaceLanguage.o $(CLIBS)

obj:
	 mkdir -p obj
bin:
	 mkdir -p bin

clean:
	 rm obj/ReplaceLanguage.o bin/ReplaceLanguage
	 