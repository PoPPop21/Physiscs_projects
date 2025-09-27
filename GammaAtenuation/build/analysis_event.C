#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

void analysis_event() {
  ifstream file("output.csv");
  int eventID, detected;
  int Ntotal = 0;
  int Ndet = 0;
  string line;

  while (getline(file, line)) {
    if (line.empty()) continue;
    stringstream ss(line);
    ss >> eventID;
    char comma;
    ss >> comma >> detected;
    Ntotal++;
    if (detected == 1) Ndet++;
  }

  double transmission = Ntotal > 0 ? double(Ndet) / Ntotal : 0;
  cout << "Transmisión: " << transmission << endl;
}
