#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "CutTrailingSpaceAndNewline.h"
using namespace std;int main(){int n;string source_program;string file_name="test.cpp";if (ifstream file(file_name);file){source_program={istreambuf_iterator<char>{file},{}};}else
cout << "File does not Exist.";removeTrailingSpaceNewline(source_program);string output_file_name="Clean-"+file_name;ofstream file(output_file_name);file<<source_program;file.close();cout<<"\nSuccessfully Finished the Program.\n";return 0;}