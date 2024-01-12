#include <iostream>

#include "include/MCP3008.h"

using namespace MCP3008Lib;

int main(){
	MCP3008 adc;
	adc.connect();
	while(true){
		
		 //read mcp300x channel 0
		std::cout << adc.read(0) <<'\n'<< adc.read(1) << '\n';
		
	}
	
	
	return 0;
}
