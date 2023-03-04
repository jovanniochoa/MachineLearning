#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include <string>
#include <sstream>

#include <armadillo>

using namespace std;
using namespace arma;

int main()
{
    // Load the data
    ifstream file("data/titanic.csv");
    string line;
    vector<vector<string>> data;
    while (getline(file, line))
    {
        vector<string> row;
        istringstream ss(line);
        string token;
        while (getline(ss, token, ','))
            row.push_back(token);
        data.push_back(row);
    }

    // Convert data to matrix and remove unwanted columns
    mat X(data.size() - 1, 3);
    vec y(data.size() - 1);
    for (int i = 1; i < data.size(); ++i)
    {
        y(i - 1) = stod(data[i][2]);
        X(i - 1, 0) = stod(data[i][0]);
        X(i - 1, 1) = stod(data[i][3]);
        if (data[i][4] == "male")
            X(i - 1, 2) = 0;
        else
            X(i - 1, 2) = 1;
    }

    // Convert labels to factors
    vec factors = unique(y);
    for (int i = 0; i < y.n_elem; ++i)
    {
        if (y(i) == factors(0))
            y(i) = 0;
        else
            y(i) = 1;
    }

    // Handle missing values
    double age_median = median(X.col(1));
    for (int i = 0; i < X.n_rows; ++i)
    {
        if (isnan(X(i, 1)))
            X(i, 1) = age_median;
    }

    // Divide into train and test
    int n = X.n_rows;
    vector<int> idx(n);
    for (int i = 0; i < n; ++i)
        idx[i] = i;
    std::shuffle(idx.begin(), idx.end(), std::mt19937(std::random_device()()));
    int split_idx = n * 0.75;
    uvec train_idx = sort(conv_to<uvec>::from(vec(idx.begin(), idx.begin() + split_idx)));
    uvec test_idx = sort(conv_to<uvec>::from(vec(idx.begin() + split_idx, idx.end())));
    mat X_train = X.rows(train_idx);
    mat X_test = X.rows(test_idx);
    vec y_train = y(train_idx);
    vec y_test = y(test_idx);

    // Build a logistic regression model
    mat X_train_aug(X_train.n_rows, X_train.n_cols + 1);
    X_train_aug.col(0).fill(1);
    X_train_aug.cols(1, X_train.n_cols) = X_train;
    mat w(X_train_aug.n_cols, 1, fill::zeros);
    double learning_rate = 0.01;
    int max_iterations = 1000;
    for (int i = 0; i < max_iterations; ++i)
    {
        vec p = 1 / (1 + exp(-X_train_aug * w));
        vec error = y_train - p;
        mat gradient = X_train_aug.t() * error;
        w += learning_rate * gradient;
    }
}
