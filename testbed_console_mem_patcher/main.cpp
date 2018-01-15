
#include "windows.h"
#include "stdio.h"
#include "wchar.h"
#include <tchar.h>
#include <locale.h> // LC_ALL
#include "testbed.h"
#include "stdio.h"

#include <iostream>
#include <string>

using namespace std;

const char name_hide_proc[] = "hide";
const int command_hide_proc = 0x111DE;

const char name_read_1_byte[] = "read1";
const int command_read_1_byte = 0x1001;

const char name_write_1_byte[] = "write1";
const int command_write_1_byte = 0x2001;

const char name_write_8_bytes[] = "write8";
const int command_write_8_bytes = 0x2008;

const char name_exit[] = "exit";
const int command_exit = 0xFFF1;

const char name_quit[] = "q";
const int command_quit = 0xFFF2;

void print_hello() {
	cout << endl;
	cout << "<<TestBed Memory Patcher console >>" << endl;
	cout << " '" << name_hide_proc << " <Pid in dec> ' -- to hide process with Pid " << endl;
	cout << " '" << name_read_1_byte << " <Address>  <Value in hex>' -- read 1 byte from memory " << endl;
	cout << " '" << name_write_1_byte <<" <Address>  <Value in hex>' -- write 1 byte to memory " << endl;
	cout << " '" << name_write_8_bytes << " <Address>  <Value in hex>' -- write 8 bytes to memory " << endl;
	cout << " '" << name_exit << "' -- exit this app " << endl;
}

int parse(string mystring) {
	int i_res = 0x12345678;

	if (std::string::npos != mystring.find(name_read_1_byte)) {
		i_res = command_read_1_byte;
	}
	if (std::string::npos != mystring.find(name_write_1_byte)){
		i_res = command_write_1_byte;
	}
	if (std::string::npos != mystring.find(name_write_8_bytes)) {
		i_res = command_write_8_bytes;
	}
	if (std::string::npos != mystring.find(name_hide_proc)) {
		i_res = command_hide_proc;
	}

	else if (std::string::npos != mystring.find(name_quit)) {
		i_res = command_quit;
	}
	else if (std::string::npos != mystring.find(name_exit)) {
		i_res = command_exit;
	}
	return i_res;
}

int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
	argc; argv; envp; // to avoid warning C4100
	setlocale(LC_ALL, "");
	setvbuf(stdout, NULL, _IONBF, 0);

//	if (check_windows_support::is_ok()) 
	{
		testbed_for_exploitation :: TestBed my_testbed; // activate testbed
		if (my_testbed.is_ok())
		{
			string string_command = { 0 };
			int code_command = 0;
			ULONG64 address = 0;
			int16_t value1byte = 0;
			ULONG64 value8bytes = 0;
			ULONG64 proc_id = 0;
			do {
				print_hello();
				string_command = { 0 };
				cin >> string_command; // std::getline(std::cin >> std::ws, mystring);
				code_command = parse(string_command);
				switch (code_command)
				{
				case command_hide_proc:
					proc_id = 0; cin >> std::dec >> proc_id;
					my_testbed.hide_proc(proc_id);
					break;
				case command_read_1_byte:
					address = 0; cin >> std::hex >> address;
					my_testbed.read_1byte(address);
					break;
				case command_write_1_byte:
					address = 0; cin >> std::hex >> address;
					value1byte = 0; cin >> std::hex >> value1byte;
					my_testbed.write_1byte(address, value1byte);
					break;
				case command_write_8_bytes:
					address = 0; cin >> std::hex >> address;
					value8bytes = 0; cin >> std::hex >> value8bytes;
					my_testbed.write_8bytes(address, value8bytes);
					break;
				default: { };
				}
			} while (!((code_command == command_quit) || (code_command == command_exit)) );
		}
	}
	cin.ignore();
	cout << "Press enter to exit." << endl;
	cin.ignore(); // std::system("PAUSE");
}