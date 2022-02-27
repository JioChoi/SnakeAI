#include "Ai.h"

#define INPUT_NUM 24
#define HIDDEN_NUM 16

Ai::Ai() {
	createEmptyNeuron();
	connectNeuron();
	randomizeWeight();
}

Ai::Ai(std::shared_ptr<Ai>& a, std::shared_ptr<Ai>& b) {
	createEmptyNeuron();
	connectNeuron();
	createWeight(a, b);
	mutate();
}

int Ai::calculate() {
	resetValue();

	/* SET INPUT VALUE */
	for (int at = 0; at < neuron.at(0).size(); at++) {
		neuron.at(0).at(at).setValue(input.at(at));
	}

	/* CALCULATE INPUT */
	for (Neuron& input : neuron.at(0)) {
		input.calculate();
	}

	/* CALCULATE HIDDEN 1 */
	for (Neuron& hidden1 : neuron.at(1)) {
		hidden1.addValue(hidden1.biasWeight);
		hidden1.ReLU(); // ReLU value first!
		hidden1.calculate();
	}

	/* CALCULATE HIDDEN 2 */
	for (Neuron& hidden2 : neuron.at(2)) {
		hidden2.addValue(hidden2.biasWeight);
		hidden2.ReLU(); // ReLU value first!
		hidden2.calculate();
	}

	/* OUTPUT SOFTMAX */
	int maxAt = 0;
	float max = 0;
	for (int at = 0; at < 3; at++) {
		neuron.at(3).at(at).addValue(neuron.at(3).at(at).biasWeight);
		neuron.at(3).at(at).sigmoid();
		if (max < neuron.at(3).at(at).getValue()) {
			max = neuron.at(3).at(at).getValue();
			maxAt = at;
		}
	}

	return maxAt;
}

void Ai::createEmptyNeuron() {
	neuron.push_back(std::vector<Neuron>(INPUT_NUM, Neuron()));
	neuron.push_back(std::vector<Neuron>(HIDDEN_NUM, Neuron()));
	neuron.push_back(std::vector<Neuron>(HIDDEN_NUM, Neuron()));
	neuron.push_back(std::vector<Neuron>(3, Neuron()));
}

void Ai::connectNeuron() {
	for (int at = 0; at < 3; at++) {
		for (Neuron& data1 : neuron.at(at)) {
			for (Neuron& data2 : neuron.at(at + 1)) {
				data1.connectNeuron(data2);
			}
		}
	}
}

void Ai::randomizeWeight() {
	for (int at = 0; at < 3; at++) {
		for (Neuron& data : neuron.at(at)) {
			for (int att = 0; att < data.getConnectedNeuronSize(); att++) {
				data.setWeight(att, Tool::randomFloat(-1, 1));
			}
			data.biasWeight = Tool::randomFloat(-1, 1);
		}
	}
}

void Ai::resetValue() {
	for (int at = 0; at < 3; at++) {
		for (Neuron& data : neuron.at(at)) {
			data.setValue(0);
		}
	}
}

void Ai::createWeight(std::shared_ptr<Ai>& a, std::shared_ptr<Ai>& b) {
	int shareAt = 0;
	int sharePoint = Tool::randomInt(0, 744);

	/* Go through input, hidden1, hidden2. */
	for (int layer = 0; layer < 3; layer++) {
		/* Go through neurons in the layer */
		for (int neuronIndex = 0; neuronIndex < neuron.at(layer).size(); neuronIndex++) {
			/* Go through connected neurons. */
			for (int connectedIndex = 0; connectedIndex < neuron.at(layer).at(neuronIndex).getConnectedNeuronSize(); connectedIndex++) {
				if (sharePoint > shareAt) {
					neuron.at(layer).at(neuronIndex).setWeight(connectedIndex, a.get()->neuron.at(layer).at(neuronIndex).getWeight(connectedIndex));
				}
				else {
					neuron.at(layer).at(neuronIndex).setWeight(connectedIndex, b.get()->neuron.at(layer).at(neuronIndex).getWeight(connectedIndex));
				}
				shareAt++;
			}
			if (sharePoint > shareAt) {
				neuron.at(layer).at(neuronIndex).biasWeight = a.get()->neuron.at(layer).at(neuronIndex).biasWeight;
			}
			else {
				neuron.at(layer).at(neuronIndex).biasWeight = b.get()->neuron.at(layer).at(neuronIndex).biasWeight;
			}
			shareAt++;
		}
	}
}

void Ai::mutate() {
	for (int at = 0; at < 3; at++) {
		for (Neuron& data : neuron.at(at)) {
			for (int att = 0; att < data.getConnectedNeuronSize(); att++) {
				if (Tool::randomInt(0, 100) <= 2) {
					data.setWeight(att, Tool::randomFloat(-1, 1));
				}
			}
			if (Tool::randomInt(0, 100) <= 2) {
				data.biasWeight = Tool::randomFloat(-1, 1);
			}
		}
	}
}

Neuron::Neuron() {
	value = 0;
}

void Neuron::connectNeuron(Neuron& target) {
	connectedNeuron.push_back({ &target, 0 });
}

void Neuron::setWeight(int at, float weight) {
	connectedNeuron.at(at).weight = weight;
}

float Neuron::getWeight(int at) {
	return connectedNeuron.at(at).weight;
}

int Neuron::getConnectedNeuronSize() {
	return connectedNeuron.size();
}

void Neuron::setValue(float value) {
	this->value = value;
}

void Neuron::addValue(float value) {
	this->value += value;
}

float Neuron::getValue() {
	return value;
}

void Neuron::calculate() {
	for (Connection& temp : connectedNeuron) {
		temp.neuron->addValue(value * temp.weight);
	}
}

void Neuron::ReLU() {
	value = std::fmax(0, value);
}

void Neuron::sigmoid() {
	value = value / (1 + abs(value)) + 1;
}
