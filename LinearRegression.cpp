#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include <string>
#include <sstream>
#include <chrono>


using namespace std;


// sigmoid to keep between 0-1
double sigmoid(double x) {
 return 1 / (1 + exp(-x));
}


int main() {


   // reads file
   ifstream file("titanic_project.csv");
   string line;
   vector<vector<string> > data;
   while (getline(file, line))
   {
       vector<string> row;
       istringstream ss(line);
       string token;
       while (getline(ss, token, ','))
           row.push_back(token);
       data.push_back(row);
   }


   // makes into matrex and removes every column but sex
   int totalColumns = 2;
   int trainingAmount = 800;
   double** X = new double*[trainingAmount];
   double* y = new double[trainingAmount];
   for (int i = 1; i < data.size(); ++i)
   {
       y[i - 1] = stod(data[i][1]);
       X[i - 1] = new double[totalColumns];
       X[i - 1][0] = 1;
       if (data[i][3] == "0")
           X[i - 1][1] = 0;
       else
           X[i - 1][1] = 1;
   }


   // binary classification
   double survivalThreshold = 0.5;
   for (int i = 0; i < trainingAmount; ++i)
   {
       if (y[i] >= survivalThreshold)
           y[i] = 1;
       else
           y[i] = 0;
   }


   //Start time
   auto startTime = chrono::steady_clock::now();
  
   // Train logistic regression model using gradient descent
   int totalTimes = 10000;
   double learningRate = 0.01;
   double* w = new double[totalColumns];
   double* b = new double[1];
   for (int i = 0; i < totalColumns; ++i)
       w[i] = 0;
   b[0] = 0;


   for (int k = 0; k < totalTimes; ++k)
   {
       double* dw = new double[totalColumns];
       double db = 0;
       for (int i = 0; i < trainingAmount; ++i)
       {
           double z = b[0];
           for (int j = 0; j < totalColumns; ++j)
               z += w[j] * X[i][j];
           double a = sigmoid(z);
           dw[0] += (a - y[i]) * X[i][0];
           dw[1] += (a - y[i]) * X[i][1];
           db += a - y[i];
       }
       for (int i = 0; i < totalColumns; ++i)
           w[i] -= learningRate * dw[i] / trainingAmount;
       b[0] -= learningRate * db / trainingAmount;
       delete[] dw;
   }


   // starts timer
   auto endTime = chrono::steady_clock::now();
   auto timeDifference = endTime - startTime;
   auto trainingTime = chrono::duration <double, milli> (timeDifference).count();


   // tests the model and adds sensitivity and specificity
   int totalCorrect = 0;
   int truePositives = 0;
   int falsePositives = 0;
   int trueNegatives = 0;
   int falseNegatives = 0;
   for (int i = 0; i < trainingAmount; ++i) {
       double z = b[0];
       for (int j = 0; j < totalColumns; ++j){
           z += w[j] * X[i][j];
       }
       double a = sigmoid(z);
       int prediction = round(a);
       if (prediction == y[i]){
           totalCorrect++;
       }
       if (prediction == 1 && y[i] == 1){
           truePositives++;
       }
       else if (prediction == 1 && y[i] == 0){
           falsePositives++;
       }
       else if (prediction == 0 && y[i] == 0){
           trueNegatives++;
       }
       else if (prediction == 0 && y[i] == 1){
           falseNegatives++;
       }
   }


   double accuracy = (double)totalCorrect / trainingAmount;
   double sensitivity = (double)truePositives / (truePositives + falseNegatives);
   double specificity = (double)trueNegatives / (trueNegatives + falsePositives);


   cout << "Accuracy: " << accuracy << endl;
   cout << "Sensitivity: " << sensitivity << endl;
   cout << "Specificity: " << specificity << endl;
   cout << "Training time: " << trainingTime << " ms" << endl;


   return 0;
}
