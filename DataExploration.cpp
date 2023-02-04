/* 
This program calculates the sum, mean, median, and range for a vector
As well as calculates the covariance and the variance between two vectors listen in Boston.csv file.
This program was also deffived from Karen Mazidi's program titled C++ data exploration
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <numeric>

using namespace std;

//prints the sim, mean, median, and range
void print_stats(const std::vector<double>& vec){
	int numObservations = vec.size();

	//calculates the sum
	double sum_vec = vec.at(0);
	for (int i = 1; i < numObservations; i++){
		sum_vec += vec.at(i);
	}
	cout << "Sum = " << sum_vec << endl;

	//calculates the mean
	double mean_vec = sum_vec / numObservations;
	cout << "Mean = " << mean_vec << endl;

	//calculates the median
	double median_vec;
	//checks even or odd
	if (numObservations % 2 == 0) {
		//if even, average middle 2 values
		int midIndex1 = numObservations / 2 - 1;
		int midIndex2 = numObservations / 2;
		median_vec = (vec.at(midIndex1) + vec.at(midIndex2)) / 2;
	} else {
		//if odd, gets middle value
		int midIndex = numObservations / 2;
		median_vec = vec.at(midIndex);
	}
	cout << "Median = " << median_vec << endl;

	//calculates the minimum
	double min_vec = vec.at(0);
	for (int i = 1; i < numObservations; i++){
		if (vec.at(i) < min_vec) {
    		min_vec = vec.at(i);
  		}
	}
	//calculates the maximum
	double max_vec = vec.at(0);
	for (int i = 1; i < numObservations; i++){
		if (vec.at(i) > max_vec) {
    		max_vec = vec.at(i);
  		}
	}
	cout << "Range =  " << min_vec << " " << max_vec << endl << endl;
}

//calculates the standard deviation of a vector
double standardDeviation(const std::vector<double>& vec) {
    int n = vec.size();
    double mean = std::accumulate(vec.begin(), vec.end(), 0.0) / n;

    double squaredDifferenceSum = 0.0;
    for (int i = 0; i < n; i++) {
        squaredDifferenceSum += pow(vec[i] - mean, 2);
    }

    return sqrt(squaredDifferenceSum / (n - 1));
}

//calculates the covariance of a vector
double covar(const std::vector<double>& rm, const std::vector<double>& medv) {
    int numObservations = rm.size();

	//recalculates the sum and the mean of vector rm and medv
	double sum_rm = rm.at(0);
	double sum_medv = rm.at(0);
	for (int i = 1; i < numObservations; i++){
		sum_rm += rm.at(i);
		sum_medv += medv.at(i);
	}
	double mean_rm = sum_rm / numObservations;
	double mean_medv = sum_medv / numObservations;

    //Calculates covariance
    double covar = 0;
    for (int i = 1; i < numObservations; i++) {
        covar += (rm[i] - mean_rm) * (medv[i] - mean_medv);
    }
    covar /= (numObservations - 1);

    return covar;
}

//calculates the correlation of a vector
double cor(const std::vector<double>& rm, const std::vector<double>& medv){
	double firstDeviation = standardDeviation(rm);
	double secondDeviation = standardDeviation(medv);
	double cor = covar(rm, medv) / (firstDeviation * secondDeviation);
	return cor;
}

//driver function
int main(int argc, char** argv) {
	ifstream inFS; // Input file stream
	string line;
	string rm_in, medv_in;
	const int MAX_LEN = 1000;
	vector<double> rm(MAX_LEN);
	vector<double> medv(MAX_LEN);
	vector<double> temp1(MAX_LEN); //is needed because later on rm is later sorted. This is used to reset rm.
	vector<double> temp2(MAX_LEN); //is needed because later on medv is later sorted. This is used to reset medv.
	
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
		temp1.at(numObservations) = rm.at(numObservations);
		medv.at(numObservations) = stof(medv_in);
		temp2.at(numObservations) = medv.at(numObservations);

		numObservations++;
	}

	rm.resize(numObservations);
	medv.resize(numObservations);
	temp1.resize(numObservations);
	temp2.resize(numObservations);

	cout << "new length " << rm.size() << endl;

	cout << "Closing file Boston.csv." << endl;
	inFS.close(); //Done with file, so close it

	cout << "Number of records: " << numObservations << endl;

	sort(rm.begin(), rm.end());
	sort(medv.begin(), medv.end());

	cout << "\nStats for rm " << endl;
	print_stats(rm);
	cout << "\nStats for medv " << endl;
	print_stats(medv);

	//reset rm and medv for covar and cor to work
	rm = temp1;
	medv = temp2;

	cout << "\n Covariance = " << covar(rm, medv) << endl;

	cout << "\n Correlation = " << cor(rm, medv) << endl;

	cout << "\nProgram terminated.";

	return 0;
}