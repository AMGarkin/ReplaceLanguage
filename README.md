# ReplaceLanguage
Tool for replacing texts in Black Desert Online *decrypted* localization files (\<game\>\ads\\*.loc).<br>
Tools to decrypt/encrypt localization files: [BDOcrypt](https://github.com/AMGarkin/BDOcrypt)


### Usage:
ReplaceLanguage \<translated\> \<original\> \<target\>

\<translated\>&nbsp;&nbsp;: file which contains translated/modified strings<br>
\<original\>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;: original file with string table<br>
\<target\>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;: output file<br>


Languagedata_??.txt files contains the following tab separated data ("\t" = tab):
\<sheet_number\>\t\<ID1\>\t\<ID2\>\t\<ID3\>\t\<ID4\>\t\<korean_text\>\t\<localized_text\>\t\<version\>\[\t\<note\>\]

ReplaceLanguage reads \<original\> file line by line and tries to find corresponding line in \<translated\> file. If the line is found, program replaces (only) localized_text with the value from <translated>. Otherwise line stays untouched.

This program can be used for example to replace guild quest texts (guild_quest_list_en.txt) in languagedata_en.txt after patch:

    ReplaceLanguage guild_quest_list_en.txt languagedata_en.txt modified.txt

Or it can replace texts in russian game client with corresponding english texts extracted from NA/EU client:

    ReplaceLanguage languagedata_en.txt languagedata_ru.txt modified.txt
