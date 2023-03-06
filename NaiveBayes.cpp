#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>
#include <cmath>
#include <random>


using namespace std;


//labels Person parts
struct Person {
   int pclass;
   int survived;
   int sex;
   double age;
};


//reads file and sotres
vector<Person> readFile(string filename) {
   vector<Person> data;
   ifstream infile("titanic_project.csv");
   string line;
   getline(infile, line);
   //get lines and store them
   while (getline(infile, line)) {
       Person p;
       stringstream ss(line);
       string value;
       getline(ss, value, ',');
       getline(ss, value, ',');
       p.pclass = stoi(value);
       getline(ss, value, ',');
       p.survived = stoi(value);
       getline(ss, value, ',');
       p.sex = stoi(value);
       getline(ss, value, ',');
       p.age = stoi(value);
       data.push_back(p);
   }
   infile.close();
   return data;
}


vector<int> sample(int n, int k, int seed) {
   vector<int> idx(n);
   for (int i = 0; i < n; i++) {
       idx[i] = i;
   }
   mt19937 g(seed);
   shuffle(idx.begin(), idx.end(), g);
   idx.resize(k);
   return idx;
}


//starts naive bayes
struct NaiveBayes {
   vector<int> class_counts;
   vector<vector<int> > feature_counts;
   vector<vector<double> > feature_probs;
   NaiveBayes() {}
   void train(vector<Person>& data) {
       int num_classes = 2;
       int num_features = 5;
       class_counts.resize(num_classes);
       feature_counts.resize(num_classes);
       feature_probs.resize(num_classes);
       for (int i = 0; i < num_classes; i++) {
           feature_counts[i].resize(num_features);
           feature_probs[i].resize(num_features);
       }
       for (auto& p : data) {
           class_counts[p.survived]++;
           feature_counts[p.survived][p.pclass]++;
           feature_counts[p.survived][p.sex]++;
           feature_counts[p.survived][p.age]++;
       }
       for (int i = 0; i < num_classes; i++) {
           int count = class_counts[i];
           for (int j = 0; j < num_features; j++){
                               feature_probs[i][j] = (double)feature_counts[i][j] / count;
           }
       }
   }
   //gets details of person and stores it
   int predict(Person& p) {
       int num_classes = 2;
       double class_probs[num_classes];
       for (int i = 0; i < num_classes; i++) {
           class_probs[i] = log((double)class_counts[i] / class_counts[0]);
           class_probs[i] += log(feature_probs[i][p.pclass]);
           class_probs[i] += log(feature_probs[i][p.survived]);
           class_probs[i] += log(feature_probs[i][p.sex]);
           class_probs[i] += log(feature_probs[i][p.age]);    
       }
       if (class_probs[0] > class_probs[1]) {
           return 0;
       } else {
           return 1;
       }
   }
   vector<int> predict(vector<Person>& data) {
       vector<int> predictions;
       for (auto& p : data) {
           predictions.push_back(predict(p));
       }
       return predictions;
   }
};


// calculate sensitivity and specificity
void evaluate(vector<Person>& data, vector<int>& predictions, double& sensitivity, double& specificity) {
  int truePositives = 0;
  int falseNegatives = 0;
  int trueNegatives = 0;
  int falsePositives = 0;
  for (int i = 0; i < data.size(); i++) {
      if (predictions[i] == 1 && data[i].survived == 1) {
          truePositives++;
      } else if (predictions[i] == 0 && data[i].survived == 1) {
          falseNegatives++;
      } else if (predictions[i] == 0 && data[i].survived == 0) {
          trueNegatives++;
      } else {
          falsePositives++;
      }
  }
  sensitivity = (double)truePositives / (truePositives + falseNegatives);
  specificity = (double)trueNegatives / (trueNegatives + falsePositives);
}


//driver function
int main() {
   vector<Person> data = readFile("desktop/titanic_project.csv");
   int n = data.size();
   vector<int> idx = sample(n, (n * 0) + 800, 1234);
   vector<Person> train, test;
   for (int i = 0; i < n; i++) {
       if (find(idx.begin(), idx.end(), i) != idx.end()) {
           train.push_back(data[i]);
       } else {
           test.push_back(data[i]);
       }
   }
  
   //starts time and begins training
   auto startTime = chrono::steady_clock::now();
   NaiveBayes nb;
   nb.train(train);
   auto endTime = chrono::steady_clock::now();
   auto timeDifference = endTime - startTime;
   auto trainingTime = chrono::duration <double, milli> (timeDifference).count();
   vector<int> predictions = nb.predict(test);
   int totalCorrect = 0;
   for (int i = 0; i < test.size(); i++) {
       if (predictions[i] == test[i].survived) {
           totalCorrect++;
       }
   }


   //output results
   double accuracy = (double)totalCorrect / test.size();
   cout << "Accuracy: " << accuracy << endl;
   double sensitivity, specificity;
   evaluate(test, predictions, sensitivity, specificity);
   cout << "Sensitivity: " << sensitivity << endl;
   cout << "Specificity: " << specificity << endl;
   cout << "Training time: " << trainingTime << " seconds" << endl;


   return 0;
}
