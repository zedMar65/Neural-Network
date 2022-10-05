// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <math.h>
#include <string>
#include <random>
#include <iostream>
#include <vector>
#include<cmath>

using namespace std;

void print(string val) {
    cout << val << endl;
}

string input(string message) {
    cout << message << endl;
    string inpu;
    cin >> inpu;
    cout << endl;
    return inpu;
}

class Connection {
public:
    double myder;
    double fullder;
    Connection() {
        weight = randomWeight();
        fullder = 0;
    }
    double weight;
    double randomWeight()
    {
        random_device rd;
        mt19937 mt(rd());
        uniform_int_distribution<int> dist(0, 10);
        return (double)(0.1 * dist(mt));
    }
};
class Neuron {
public:
    Neuron(const int& nextlevel, int MyNum) {

        for (int n = 0; n < nextlevel - 1; n++) {
            myNum = MyNum;
            connections.push_back(Connection());
            //cout << "Made Connection to " << connections.size() - 1 << ", weight: " << connections.back().weight << ", I am " << myNum << endl;;
        }
    }
    double outputValue;
    double inputv;
    void evolvefirst(double inputValue) {
        outputValue = inputValue;
        inputv = inputValue;
    }
    void evolvenext(vector<Neuron>& prevLayer, int my) {
        myNum = my;
        double a = 0.0;
        double b = 0.0;
        double sum = 0.0;
        for (int n = 0; n < prevLayer.size(); n++) {
            a = prevLayer[n].outputValue;
            b = prevLayer[n].connections[myNum].weight;
            sum += a * b;

        }
        inputv = sum;
        outputValue = tang(sum);
    }

    double tang(double sum) {
        if (sum > 0) {
            sum = sum;
        }
        else {
            sum = 0.0;
        }
        return sum;
    }
    double derivativeTang(double sum) {
        if (sum > 0) {
            sum = 1.0;
        }
        else {
            sum = 0.0;
        }
        return sum;
    }
    vector<Connection> connections;
    int myNum;

    void backPrpnx(vector<Neuron>& prevLayer) {
        for (int i = 0; i < prevLayer.size(); i++) {
            Connection& currentCon = prevLayer[i].connections[myNum];
            currentCon.myder = 0;
            for (int y = 0; y < connections.size(); y++) {

                currentCon.myder += prevLayer[i].outputValue * derivativeTang(inputv) * connections[y].myder;
            }
            currentCon.myder /= connections.size();
            currentCon.fullder += currentCon.myder;
        }
    }

    void backPrpfr(vector<Neuron>& prevLayer, double cost, vector<double> targetValues) {

        for (int i = 0; i < prevLayer.size(); i++) {

            Connection& currentCon = prevLayer[i].connections[myNum];
            currentCon.myder = prevLayer[i].outputValue * derivativeTang(inputv) * (2 * (outputValue - targetValues[myNum]));                        //write backprop formula here for all last layer weights(derivatives)
            currentCon.fullder += currentCon.myder;
        }

    }
    void updateWeight(vector<Neuron>& prevLayer, int howF, double learningSpeed) {
        

        for (int i = 0; i < prevLayer.size(); i++) {
            Connection& currentCon = prevLayer[i].connections[myNum];    // Change the 0 to my num iif it works or some shit
            currentCon.weight -= ((currentCon.fullder / howF) * learningSpeed);
            currentCon.fullder = 0;
            //cout << prevLayer[i].myNum << " to " << myNum << "'s weight is: " << currentCon.weight << endl;

        }
    }

private:

};

typedef vector<Neuron> Layer;

class Net {
public:
    double cost = 0;

    vector<Layer> aL;
    Net(vector<int> map) {

        cout << "ok";
        for (int y = 0; y < map.size(); y++) {
            map[y]++;
        }
        for (int i = 0; i < map.size(); i++) {
            cout << "----------------------------" << i + 1 << "-------------------------------\n";
            aL.push_back(Layer());


            int prevLevel;
            for (int j = 0; j < map[i]; j++) {
                prevLevel = 0;
                if (map.size() - 1 != i) {
                    prevLevel = map[i + 1];
                }

                int myNum = j;
                aL.back().push_back(Neuron(prevLevel, myNum));
                cout << "     **" << myNum << "**\n";
            }

        }
    }
    vector<double> giveAnswr() {
        vector<double> ansewr;
        for (int i = 1; i < aL.back().size(); i++) {
            ansewr.push_back((aL.back()[i - 1].outputValue));
        }
        return ansewr;
    }

    void evolve(vector<double>& inputs) {
        for (int i = 0; i < aL[0].size(); i++) {

            if (i + 1 == aL[0].size()) {
                aL[0].back().outputValue = 1.0;
            }
            else {
                aL[0][i].evolvefirst(inputs[i]);
            }



        }

        for (int i = 1; i < aL.size(); i++) {

            vector<Neuron> prvLvl = aL[i - 1];
            aL[i].back().outputValue = 1.0;
            for (int j = 0; j < aL[i].size() - 1; j++) {
                aL[i][j].evolvenext(prvLvl, j);
            }
        }



    }
    void backProp(vector<double>& targetVal) {
        double sumas = 0;
        for (int y = 0; y < aL[aL.size() - 1].size() - 1; y++) {
            sumas += pow(aL[aL.size() - 1][y].outputValue - targetVal[y], 2);
        }
        cost = sumas / (targetVal.size());
        for (int y = 0; y < aL[aL.size() - 1].size() - 1; y++) {

            aL[aL.size() - 1][y].backPrpfr(aL[aL.size() - 2], cost, targetVal);
        }

        for (int i = aL.size() - 2; i > 0; i--) {
            for (int y = 0; y < aL[i].size() - 1; y++) {
                aL[i][y].backPrpnx(aL[i - 1]);
            }
        }

    }
    void updateWeights(int howF, double learningSpeed) {
        for (int i = aL.size() - 1; i > 0; i--) {
            for (int y = 0; y < aL[i].size() - 1; y++) {
                aL[i][y].updateWeight(aL[i - 1], howF, learningSpeed);
            }
        }

    }


private:

};

int main()
{
    double inar;
    vector<int> map;
    vector<double> inputs;
    vector<double> targetVal;
    vector<double> outputss;
    int lay;
    double learningSpeed;
    cout << "Whats your  learning speed?\n";
    cin >> learningSpeed;

    int times = stoi(input("How manny layers?"));
    for (int i = 0; i < times; i++) {
        lay = stoi(input("neurons in n' layer:"));
        map.push_back(lay);
    }
    Net myNet(map);
    int ops = 0;
    times = stoi(input("How manny tests?"));
    int howF = stoi(input("How often to update?"));
    cout << endl << times << endl;
    for (int i = 0; i < times; i++) {
        for (int y = 0; y < map[0]; y++) {
            inar = stod(input("n' Input:"));
            inputs.push_back(inar);
        }
        myNet.evolve(inputs);

        for (int y = 0; y < map.back(); y++) {
            inar = stod(input("n' Target:"));
            targetVal.push_back(inar);
        }
        outputss = myNet.giveAnswr();
        for (int y = 0; y < map.back(); y++) {
            cout << "n' Output: " << outputss[y] << endl;
        }

        myNet.backProp(targetVal);

        if (ops + 1 == howF) {
            myNet.updateWeights(howF, learningSpeed);
            ops = -1;
        }
        ops++;
        targetVal.clear();
        inputs.clear();
    }
    cout << "Dabajau product, nebe testai" << endl;
    int isattest = 1000;
    for (int i = 0; i < isattest; i++) {
        for (int y = 0; y < map[0]; y++) {
            inar = stod(input("n' Input:"));
            inputs.push_back(inar);
        }
        myNet.evolve(inputs);

        outputss = myNet.giveAnswr();
        for (int y = 0; y < map.back(); y++) {
            cout << "n' Output: " << (int)outputss[y] << endl;
        }
        inputs.clear();
    }



}