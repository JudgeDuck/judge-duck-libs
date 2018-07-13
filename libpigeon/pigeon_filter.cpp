#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>

extern "C" bool _pigeon_filter(const char *path) {
	static bool _init = false;
	static bool _init_ing = false;
	static std::vector<std::string> rules;
	if (_init_ing) {
		return true;
	} else if (!_init) {
		_init_ing = true;
		FILE *f = fopen("../judge-duck-libs/libpigeon/config.txt", "r");
		if (f) {
			std::string tmp;
			while (!feof(f)) {
				char c = fgetc(f);
				if (c == EOF) break;
				if (c == '\n') {
					if (tmp != "") rules.push_back(tmp);
					tmp = "";
				} else {
					tmp += c;
				}
			}
			fclose(f);
		}
		_init_ing = false;
		_init = true;
		return _pigeon_filter(path);
	} else {
		for (int i = 0; i < (int) rules.size(); i++) {
			std::string s = rules[i];
			if (s[0] != '+' && s[0] != '-') continue;
			bool allow = s[0] == '+';
			if (s.length() == 1) {
				return allow;
			}
			int len = s.length();
			int pos = 0;
			for (int i = 1; i < len; i++) {
				if (s[i] == '*') {
					pos = i;
					break;
				}
			}
			if (!pos) {
				if (s.substr(1) == path) {
					return allow;
				}
			} else {
				int len_path = strlen(path);
				if (len_path >= len - 1 - 1 && s.substr(1, pos - 1) == std::string(path, pos - 1) && s.substr(pos + 1) == std::string(path + len_path - (len - pos - 1))) {
					return allow;
				}
			}
		}
		return false;
	}
}
