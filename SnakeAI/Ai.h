#pragma once
#include <iostream>
#include <vector>

#include "Tool.h"

class Neuron {
public:
	Neuron();
	void connectNeuron(Neuron& target);
	void setWeight(int at, double weight);
	double getWeight(int at);
	int getConnectedNeuronSize();
	void setValue(double value);
	void addValue(double value);
	double getValue();

	void calculate();
	void ReLU();
	void sigmoid();

	std::string getWeightData();

	double biasWeight;
private:
	struct Connection {
		Neuron* neuron;
		double weight;
	};

	double value;
	std::vector<Connection> connectedNeuron;
};

class Ai {
public:
	Ai();
	Ai(std::shared_ptr<Ai> &a, std::shared_ptr<Ai> &b);
	Ai(std::shared_ptr<Ai>& a);
	std::vector<double> input;
	int calculate();

	std::string getWeightData();
private:
	void createEmptyNeuron();
	void connectNeuron();
	void randomizeWeight();
	void resetValue();
	void createWeight(std::shared_ptr<Ai>& a, std::shared_ptr<Ai>& b);
	void mutate();
	std::vector<std::vector<Neuron>> neuron;
};