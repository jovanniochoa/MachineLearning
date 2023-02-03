#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
	ifstream inFS; // Input file stream
	string line;
	string rm_in, medv_in;
	const int MAX_LEN = 1000;
	vector<double> rm(MAX_LEN);
	vector<double> medv(MAX_LEN);
	
	//Try to open file
	cout << "Opening file Boston.csv." << endl;

	inFS.open("Boston.csv");
	if (!inFS.is_open()) {
		cout << "Could not open file Boston.csv." << endl;
		return 1; //1 indicates error
	}
	
	// Can now use inFS stream like cin stream
	//Bostoncsv should contain two doubles

	cout << "Reading line 1" << endl;
	getline(inFS, line);

	//echo heading
	cout << "heading: " << line << endl;

	int numObservations = 0;
	while (inFS.good()){
		
		getline(inFS, rm_in, ',');
		getline(inFS, medv_in, '\n');

		rm.at(numObservations) = stof(rm_in);
		medv.at(numObservations) = stof(medv_in);

		numObservations++;
	}

	rm.resize(numObservations);
	medv.resize(numObservations);

	cout << "new length " << rm.size() << endl;

	cout << "Closing file Boston.csv." << endl;
	inFS.close(); //Done with file, so close it

	cout << "Number of records: " << numObservations << endl;

	cout << "\nStats for rm " << endl;
	//print_stats(rm);

	//cout << "\n Covariance = " << covar(rm, medv) << endl;

	//cout << "\n Correlation = " << cor(rm, medv) << endl;

	cout << "\nProgram terminated.";

	return 0;
}

