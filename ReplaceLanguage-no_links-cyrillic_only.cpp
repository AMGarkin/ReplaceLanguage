#include <string>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

struct DataRow {
	int sheet;
	int id1;
	int id2;
	int id3;
	int id4;
	std::wstring kr;
	std::wstring loc;
	std::wstring rest;

	///overloading operator < because of std::sort function
	bool operator < (const DataRow& row) const {
		if (sheet == row.sheet) {
			if (id1 == row.id1) {
				if (id2 == row.id2) {
					if (id3 == row.id3) {
						return (id4 < row.id4);
					}
					return (id3 < row.id3);
				}
				return (id2 < row.id2);
			}
			return (id1 < row.id1);
		}
		return (sheet < row.sheet);
	}

	friend std::wistream& operator>>(std::wistream& in, DataRow& data);
	friend std::wofstream& operator<<(std::wofstream& out, DataRow& data);
};

///overloading operator >>, so I can use it to read DataRow type
std::wistream& operator>>(std::wistream& is, DataRow& data)
{
	is >> data.sheet;
	is >> data.id1;
	is >> data.id2;
	is >> data.id3;
	is >> data.id4;
	is >> std::ws; //discard whitespace
	std::getline(is, data.kr, L'\t');
	std::getline(is, data.loc, L'\t');
	std::getline(is, data.rest);

	return is;
}

///overloading operator <<, so I can use it to write DataRow type
std::wofstream& operator<<(std::wofstream& os, DataRow& data)
{
	os << data.sheet << L'\t';
	os << data.id1   << L'\t';
	os << data.id2   << L'\t';
	os << data.id3   << L'\t';
	os << data.id4   << L'\t';
	os << data.kr    << L'\t';
	os << data.loc   << L'\t';
	os << data.rest;

	return os;
}

bool isUTF8Cyrillic(std::wstring s)
{
	unsigned int i;

	for (i = 0; i <= s.length() - 2; ) {
		if (s[i] >= 0xD0 && s[i] <= 0xD3) {
			if (s[i+1] >= 0x80 && s[i+1] <= 0xBF) {
				return true;
			} else {
				i += 2;
			}
		} else {
			i++;
		}
	}

	return false;
}

int main(int argc, char *argv[])
{
	if (argc < 4)
	{
		std::cerr   << "Usage: " << argv[0] << " <translated> <original> <target>\n\n"
					<< "<translated>\t: file which contains translated/modified strings\n"
					<< "<original>\t: original file with string table\n"
					<< "<target>\t: output file" << std::endl;
		return 1;
	}

	std::string translated_name = argv[1];
	std::string original_name = argv[2];
	std::string target_name = argv[3];

	std::wifstream translated(translated_name);
	if (translated.fail()){
		std::cerr << "Error opening " << translated_name << std::endl;
		return 1;
	}
	std::wifstream original(original_name);
	if (original.fail()){
		std::cerr << "Error opening " << original_name << std::endl;
		return 1;
	}
	std::wofstream target(target_name);
	if (target.fail()){
		std::cerr << "Error opening " << target_name << std::endl;
		return 1;
	}

	///create lookup table
	std::vector<DataRow> lookup;
	std::wstring row;

	std::cout << "Reading \"" << translated_name << "\"..." ;
	while (std::getline(translated, row)){
		DataRow dataRow;
		std::wstringstream wss(row);

		wss >> dataRow;

		lookup.push_back(dataRow);
	}
	std::cout << " (" << lookup.size() << " lines found)" << std::endl;

	///sorting is necessary so I can speed up searching using a map (lookupHelper)
	std::sort(lookup.begin(), lookup.end());

	///lookupHelper is a map which assigns pair of pointers, start and end (something like row numbers),
	///to the sheet number (first number on the row in text file)
	std::map<int, std::pair<std::vector<DataRow>::iterator, std::vector<DataRow>::iterator> > lookupHelper;
	std::vector<DataRow>::iterator it, first, last;
	first = lookup.begin();
	int sheet = first->sheet;
	for (it = lookup.begin(); it != lookup.end(); ++it) {
		last = it;
		if (sheet < it->sheet){
	        lookupHelper[sheet] = std::make_pair(first, --last); ///last is now the first line of the new sheet, so we need to get previous value
			first = it;
			sheet = it->sheet;
	    }
	}
	lookupHelper[sheet] = std::make_pair(first, last); ///write the last one

	//main loop
	std::cout << "Copying lines from \"" << original_name << "\" to \"" << target_name << "\"..." << std::endl;

	bool found = false;
	unsigned int changed = 0, unchanged = 0;
	std::map<int, std::pair<std::vector<DataRow>::iterator, std::vector<DataRow>::iterator> >::iterator iHelp; ///I love this type!

	while (std::getline(original, row)){
		std::wstringstream wss;
		DataRow originalRow;

		wss.str(row);
		wss >> originalRow;

		found = false;
		iHelp = lookupHelper.find(originalRow.sheet);
		if (iHelp != lookupHelper.end()){
			it = iHelp->second.first;                  ///I don't want to search for key multiple times using map[key], so I'm using less comprehensible
			while (it <= iHelp->second.second) {       ///dereferencing form iterator. It points to map pair<key, value> and I have stored in value
				if (originalRow.sheet == it->sheet &&  ///another pair. It looks like this: pair<key, value<first, second>>
					originalRow.id1 == it->id1 &&      ///Pair members are accessible as .first and .second, so I have to use: pointer->second.first
					originalRow.id2 == it->id2 &&
					originalRow.id3 == it->id3 &&
					originalRow.id4 == it->id4 &&
					originalRow.kr.substr(0,4) != L"http" &&
					isUTF8Cyrillic(it->loc)) {

					found = true;
					break;
				} else {
					it++;
				}
			}
		}

		if (found){
			originalRow.loc = it->loc;
			changed++;
		} else {
			unchanged++;
		}
		target << originalRow << '\n';
	}

	std::cout << "Processed lines: " << changed + unchanged <<  " (translated: " << changed << ", original: " << unchanged << ")\nDone!" << std::endl;

	return 0;
}
