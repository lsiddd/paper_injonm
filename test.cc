#include <fstream>
#include <stdlib.h>
#include <string>
using namespace std;

int main(){
	stringstream qoeRnti;
	qoeRnti << "rnti/" << rnti << "-qoe.txt";


	std::ifstream qoeRntiFile;

	qoeRntiFile.open(qoeRnti.str());

	if (qoeRntiFile.is_open()){
	    double qoeAtual;

	    while(qoeRntiFile >> qoeAtual){}
	    std::cout << qoeAtual << "\n";
	}
}