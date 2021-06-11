#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

int line_counter = 1;
using namespace std;
struct Keywords {
	const std::string Conditions{ "if"};
	const std::string Loops{ "while" };
	const std::string Streams{ "out_data" };
	const std::string Typenames[4]{ "num", "raln", "logic", "str" };
	vector <std::string> Variable_Names;
}keywords;

struct Vars {
	std::string var_name = "";
	std::string var_type = "";
	void* var_value = nullptr;
};
std::vector <Vars> Variables;

int find_var(std::vector <Vars>& searchVar, std::string name);
int find_var(std::vector <Vars>& searchVar, std::string name);
void make_sstream(std::ifstream& fsin, std::stringstream& fin);
int search_in_keywords(std::string& sword);
bool is_letter(const char& let);
bool is_digit(const char& let);
template<class T>
void do_decleration(std::stringstream& inssline, std::string str);
int find_type(const std::string& str);
void error();
template<class T>
T cast_val(std::string tok);
int hide_num_realization(std::vector <std::string>& tokens);
float hide_raln_realization(std::vector <std::string>& tokens);
std::string hide_str_realization(std::vector <std::string>& tokens);
bool hide_logic_realization(std::vector <std::string>& tokens);
void do_ifs(std::vector <std::string> tokens, std::stringstream& sscope);
void do_math_operations(std::stringstream& inssline, std::string str);
void do_loops(std::vector <std::string> tokens, std::stringstream& sscope);
void do_stream(std::stringstream& ssline);
std::vector <std::string> find_scopes(std::stringstream& ssscope, std::stringstream& tmp);
void choice_procedure(std::stringstream& fin);
void clear_heap();


int main()
{
	std::string path;
	std::cin >> path;
	std::ifstream fsin;
	fsin.open(path);

	if (fsin.is_open()) {
		std::cout << "File is open" << std::endl;
		std::stringstream fin;
		make_sstream(fsin, fin);
		choice_procedure(fin);

		std::cout << fin.str();
	}
	else
	{
		std::cout << "File is not open" << std::endl;
	}
	clear_heap();
	fsin.close();
}

int find_var(std::vector <Vars>& searchVar, std::string name) {
	int count = 0;

	for (Vars i : searchVar) {
		if (i.var_name == name) {
			return count;
		}
		count++;
	}
	return -1;
}

int find_token(std::vector <std::string>& searchtokens, std::string name) {
	int count = 0;
	for (std::string i : searchtokens) {
		if (i == name) {
			return count;
		}
		count++;
	}
	return -1;
}

void make_sstream(std::ifstream& fsin , std::stringstream& fin ) {
	std::string tmp;
	while (!fsin.eof()) {
			getline(fsin, tmp);
			fin << "\n" + tmp;
	 }
}

int search_in_keywords(std::string& sword) {
	if (sword == keywords.Conditions) {
		return 1;
	}
	if (sword == keywords.Loops) {
		return 2;
	}
	if (sword == keywords.Streams) {
		return 3;
	}
	for (int i = 0; i < 4; ++i) {
		if (sword == keywords.Typenames[i]) {
			return 10 + i;
		}
	}
	for (string i : keywords.Variable_Names) {
		if (sword == i) {
			return 5;
		}
	}
	return 6;
}

bool is_letter(const char& let) {
	bool tmp = ((let <= 90 && let >= 65) || (let <= 122 && let >= 97));
	return tmp;
}

bool is_digit(const char& let) {
	bool tmp = (let <= 57 && let >= 48);
	return tmp;
}

template<class T>
void do_decleration(std::stringstream& inssline, std::string str) {
	Vars tmp_var;
	std::string tmp;
	tmp_var.var_type = str;
	while (!inssline.eof())
	{
		inssline >> tmp_var.var_name;
		if (is_letter(tmp_var.var_name[0]) && search_in_keywords(tmp_var.var_name)){
			keywords.Variable_Names.push_back(tmp_var.var_name);
		}
		else
		{
			break;
		}
		inssline >> tmp;
		if (tmp[0] == '=') {
			T tmp_val;
			inssline >> tmp_val;
			tmp_var.var_value = static_cast <void*>(new T);
			*(T*)tmp_var.var_value = tmp_val;
		}
		else if(tmp[0] == ',')
		{
			continue;
		}
		else {
			break;
		}
	}
}

int find_type(const std::string& str) {
	for (int i = 0; i < Variables.size(); i++)
	{
		if (str == Variables[i].var_name) {
			return i;
		}
	}
}

void error() {
	std::cout << "You have error in " << line_counter << "th string" << std::endl;
	exit(1);
}

template<class T>
T cast_val(std::string tok) {
	if (is_letter(tok[0])) {
		if (find_var(Variables, tok)+1)
		{
			T tmp = *(T*)Variables[find_var(Variables, tok)].var_value;
			return tmp;
		}
		else {
			error();
		}
	}
	else if (is_digit(tok[0])) {
		std::stringstream ss(tok);
		T tmp;
		ss >> tmp;
		if (ss.fail() || !ss.eof()) {
			error();
		}
		return tmp;
	}
	else if (tok[0] == '$') {
		tok.erase(0, 1);
		std::stringstream ss(tok);
		T tmp;
		ss >> tmp;
		if (ss.fail() || !ss.eof()) {
			error();
		}
		return tmp;

	}
	else if (tok == "true") {
		tok = "1";
		cast_val< T >(tok);
	}
	else {
		error();
	}

}

int hide_num_realization(std::vector <std::string>& tokens) {
	int ix;
	std::string tmp;
	while (tokens.size() - 1 != 0)
	{
		int i = 0;
		if (find_token(tokens, "*") + 1 )
		{
			i = find_token(tokens, "*");
			ix = cast_val<int>(tokens[i - 1]);
			ix *= cast_val<int>(tokens[i + 1]);
		}
		else if (find_token(tokens, "/") + 1)
		{
			i = find_token(tokens, "/");
			int tmp = cast_val<int>(tokens[i + 1]);
			ix = cast_val<int>(tokens[i - 1]);
			if (tmp == 0)
			{
				error();
			}
			else {
				ix /= tmp;
			}
		}
		else if (find_token(tokens, "%") + 1)
		{
			i = find_token(tokens, "%");
			int tmp = cast_val<int>(tokens[i + 1]);
			ix = cast_val<int>(tokens[i - 1]);
			if (tmp == 0)
			{
				error();
			}
			else {
				ix %= tmp;
			}
		}
		else if (find_token(tokens, "+") + 1)
		{
			i = find_token(tokens, "+");
			ix = cast_val<int>(tokens[i - 1]);
			ix += cast_val<int>(tokens[i + 1]);
		} else if (find_token(tokens, "-") + 1)
		{
			i = find_token(tokens, "-");
			ix = cast_val<int>(tokens[i - 1]);
			ix -= cast_val<int>(tokens[i + 1]);
		}
		else
		{
			error();
		}
		std::stringstream sstok;
		sstok << ix;
		sstok >> tmp;
		tokens.erase(tokens.begin() + i - 1);
		tokens.erase(tokens.begin() + i - 1);
		tokens.erase(tokens.begin() + i - 1);
		tokens.insert(tokens.begin() + i,tmp);
	}
	if (tokens.size() - 1 == 0) {
		return cast_val<int>(tokens[0]);
	}
	else {
		error();
	}
}

float hide_raln_realization(std::vector <std::string>& tokens) {
	float ix;
	std::string tmp;
	while (tokens.size() - 1 != 0)
	{
		int i = 0;
		if (find_token(tokens, "*") + 1)
		{
			i = find_token(tokens, "*");
			ix = cast_val<float>(tokens[i - 1]);
			ix *= cast_val<float>(tokens[i + 1]);
		}
		else if (find_token(tokens, "/") + 1)
		{
			i = find_token(tokens, "/");
			float tmp = cast_val<float>(tokens[i + 1]);
			ix = cast_val<float>(tokens[i - 1]);
			if (tmp == 0)
			{
				error();
			}
			else {
				ix /= tmp;
			}
		}
		else if (find_token(tokens, "+") + 1)
		{
			i = find_token(tokens, "+");
			ix = cast_val<float>(tokens[i - 1]);
			ix += cast_val<float>(tokens[i + 1]);
		}
		else if (find_token(tokens, "-") + 1)
		{
			i = find_token(tokens, "-");
			ix = cast_val<float>(tokens[i - 1]);
			ix -= cast_val<float>(tokens[i + 1]);
		}
		else
		{
			error();
		}
		std::stringstream sstok;
		sstok << ix;
		sstok >> tmp;
		tokens.erase(tokens.begin() + i - 1);
		tokens.erase(tokens.begin() + i - 1);
		tokens.erase(tokens.begin() + i - 1);
		tokens.insert(tokens.begin() + i, tmp);
	}
	if (tokens.size() - 1 == 0) {
		return cast_val<float>(tokens[0]);
	}
	else {
		error();
	}
}
	
std::string hide_str_realization(std::vector <std::string>& tokens) {
	std::string ix;
	std::string tmp;
	while (tokens.size() - 1 != 0)
	{
		int i = 0;
		if (find_token(tokens, "+") + 1)
		{
			i = find_token(tokens, "+");
			ix = cast_val<std::string>(tokens[i - 1]);
			ix += cast_val<std::string>(tokens[i + 1]);
		}
		else
		{
			error();
		}
		tokens.erase(tokens.begin() + i - 1);
		tokens.erase(tokens.begin() + i - 1);
		tokens.erase(tokens.begin() + i - 1);
		tokens.insert(tokens.begin() + i, ix);
	}
	if (tokens.size() - 1 == 0) {
		return cast_val<std::string>(tokens[0]);
	}
	else {
		error();
	}
}

bool hide_logic_realization(std::vector <std::string>& tokens) {
	bool ix;
	std::string tmp;
	while (tokens.size() - 1 != 0)
	{
		int i = 0;
		if (find_token(tokens, "+") + 1)
		{
			i = find_token(tokens, "+");
			ix = cast_val<bool>(tokens[i - 1]);
			ix += cast_val<bool>(tokens[i + 1]);
		}
		else if (find_token(tokens, "-") + 1)
		{
			i = find_token(tokens, "-");
			ix = cast_val<bool>(tokens[i - 1]);
			ix -= cast_val<bool>(tokens[i + 1]);
		}
		else if (find_token(tokens, "<") + 1)
		{
			i = find_token(tokens, "<");
			ix = cast_val<bool>(tokens[i - 1]);
			ix = ix < cast_val<bool>(tokens[i + 1]);
		}
		else if (find_token(tokens, ">") + 1)
		{
			i = find_token(tokens, ">");
			ix = cast_val<bool>(tokens[i - 1]);
			ix = ix > cast_val<bool>(tokens[i + 1]);
		}
		else if (find_token(tokens, "==") + 1)
		{
			i = find_token(tokens, "==");
			ix = cast_val<bool>(tokens[i - 1]);
			ix = ix == cast_val<bool>(tokens[i + 1]);
		}
		else if (find_token(tokens, "||") + 1)
		{
			i = find_token(tokens, "||");
			ix = cast_val<bool>(tokens[i - 1]);
			ix = ix || cast_val<bool>(tokens[i + 1]);
		}
		else if (find_token(tokens, "&&") + 1)
		{
			i = find_token(tokens, "&&");
			ix = cast_val<bool>(tokens[i - 1]);
			ix = ix && cast_val<bool>(tokens[i + 1]);
		}
		else
		{
			error();
		}
		std::stringstream sstok;
		sstok << ix;
		sstok >> tmp;
		tokens.erase(tokens.begin() + i - 1);
		tokens.erase(tokens.begin() + i - 1);
		tokens.erase(tokens.begin() + i - 1);
		tokens.insert(tokens.begin() + i, tmp);
	}
	if (tokens.size() - 1 == 0) {
		return cast_val<bool>(tokens[0]);
	}
	else {
		error();
	}
}

void do_math_operations(std:: stringstream& inssline, std::string str) {
	std::string tmp;
	inssline >> tmp;
	std::vector <std::string> tokens;
	if(tmp == "="){
		while (!inssline.eof())
		{
			inssline >> tmp;
			if (tmp[0] == ';') {
				break;
			}
			tokens.push_back(tmp);
		}
		int index = find_type(str);
		switch (Variables[index].var_type[0]) 
		{
		case 'n':
			*(int*)Variables[index].var_value = hide_num_realization(tokens);
			break;
		case 'r':
			*(float*)Variables[index].var_value = hide_raln_realization(tokens);
			break;
		case 's':
			*(std::string*)Variables[index].var_value = hide_str_realization(tokens);
			break;
		case 'l':
			*(bool*)Variables[index].var_value == hide_logic_realization(tokens);
			break;
		default:
			break;
		}
	}
}

void do_ifs(std::vector <std::string> tokens, std::stringstream& sscope) {
	while (hide_logic_realization(tokens)) {
		choice_procedure(sscope);
	}
}

void do_loops(std::vector <std::string> tokens, std::stringstream& sscope) {
	if (hide_logic_realization(tokens)) {
		choice_procedure(sscope);
	}
}

void do_stream(std::stringstream& ssline) {
	std::string tmp;
	ssline >> tmp;
	if (tmp == "(") {
		ssline >> tmp;
		if (is_letter(tmp[0])) {
			if (find_var(Variables,tmp) + 1)
			{
				int index = find_type(tmp);
				switch (Variables[index].var_type[0])
				{
				case 'n':
					std::cout << cast_val<int>(tmp) << std::endl;
					break;
				case 'r':
					std::cout << cast_val<float>(tmp) << std::endl;;
					break;
				case 's':
					std::cout << cast_val<std::string>(tmp) << std::endl;;
					break;
				case 'l':
					std::cout << cast_val<bool>(tmp) << std::endl;;
					break;
				default:
					break;
				}
			}
			else {
				error();
			}
		}
		else if (is_digit(tmp[0])) {
			std::stringstream ss(tmp);
			int tmp;
			ss >> tmp;
			if (ss.fail() || !ss.eof()) {
				error();
			}
			else {
				std::cout << tmp;
			}
			
		}
		else if (tmp[0] == '$') {
			tmp.erase(0, 1);
			tmp = tmp + ssline.str();
			tmp.erase(tmp[tmp.length() - 2], 2);
			std::cout << tmp << std::endl;
		}
		else if (tmp == "true") {
			tmp = "1";
			std::cout << tmp << std::endl;
		}
		else if (tmp == "false") {
			tmp = "0";
			std::cout << tmp << std::endl;
		}
		else {
			error();
		}
	}
}

std::vector <std::string> find_scopes(std::stringstream& ssscope, std::stringstream& tmp) {
	std::vector <std::string> con_tokens;
	std::string tmps;
	int count = 0;

	while (!ssscope.eof())
	{
		ssscope >> tmps;
		if (tmps == "{") {
			count++;
			if (count == 1) {
				continue;
			}
		}
		else if (tmps == "}") {
			count--;
			if (count == 0) {
				break;
			}
		}
		if (count > 0) {
			tmp << tmps + " ";
		}
		else if (count == 0 && (tmps != "(" && tmps != ")")) {
			con_tokens.push_back(tmps);
		}

	}
	return con_tokens;
}

void clear_heap() {
	for (Vars i : Variables)
	{
		delete i.var_value;
	}
}

void choice_procedure(std::stringstream& fin) {
	
	std::string line;
	getline(fin, line);
	std::stringstream ssline(line);
	std::string str;
	ssline >> str;
	while (!fin.eof())
	{
		switch (search_in_keywords(str))
		{
		case 1:
			do_ifs(find_scopes(fin, ssline), ssline);
			break;
		case 2:
			do_loops(find_scopes(fin, ssline), ssline);
			break;
		case 3:
			do_stream(ssline);
			break;
		case 10:
			do_decleration<int>(ssline, str);
			break;
		case 11:
			do_decleration<float>(ssline, str);
			break;
		case 12:
			do_decleration<bool>(ssline, str);
			break;
		case 13:
			do_decleration<std::string>(ssline, str);
			break;
		case 5:
			do_math_operations(ssline, str);
			break;
		default:
			break;
		}
	}

	if (!ssline.eof()) {
		error();
	}
	++line_counter;
}