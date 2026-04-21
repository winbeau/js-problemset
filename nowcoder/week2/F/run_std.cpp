// run_std.cpp
// 编译: g++ -std=c++11 run_std.cpp -o run_std
// 运行: ./run_std
// 要求: 当前目录下存在已编译的可执行文件 std.exe 以及 1.in ~ 50.in

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
using namespace std;

int main() {
	const char* std_exe = "std.exe";
	//循环中的 "50" 需改为输入文件数量。
	for (int i = 1; i <= 50; ++i) {
		string inFile = to_string(i) + ".in";
		string outFile = to_string(i) + ".out";
		ifstream infile(inFile);
		if (!infile.is_open()) {
			cerr << "警告: 输入文件 " << inFile << " 不存在，已跳过。" << endl;
			continue;
		}
		infile.close();
		string command = string(std_exe) + " < " + inFile + " > " + outFile;
		cout << "执行: " << command << endl;
		int ret = system(command.c_str());
		if (ret != 0) {
			cerr << "错误: 命令返回码 " << ret << endl;
		}
	}
	
	return 0;
}
