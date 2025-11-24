#include<iostream>
#include "clsProcess.h"
using namespace std;

//shahem rajai owies
//160929

void main() {
	clsProcess p;

	

	clsProcess::PrintProcessesInfo();

	p.PrintFullInfo();

	p.SaveProcessesToFile();

	p.PrintOutputFileContent();

}

