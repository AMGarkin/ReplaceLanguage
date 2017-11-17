@rem x86/x64 depends on which environment you use
@rem x64 = Start > Visual Studio 2017 > x64 Native Tools Command prompt for VS 2017
@rem x86 = Start > Visual Studio 2017 > x64_x86 Cross Tools Command prompt for VS 2017

@mkdir obj\VCC 2>nul
@mkdir bin\VCC 2>nul
cl -Ox -W2 -GL -Gy -MT -EHsc -Foobj\VCC\ -Febin\VCC\ ReplaceLanguage.cpp -link -LTCG
