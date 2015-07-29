// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SimulatedAnnealing.cpp
// Description	: SimulatedAnnealing implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#ifndef __SIMULATEDANNEALING_H__
#define __SIMULATEDANNEALING_H__

// Library Includes
#include <random>

// Local Includes

template<typename State, typename Temperature, typename EnergyFunction, typename ScheduleFunction, typename PerturbFunction>
State SimulatedAnnealing(const State& _krInitialState,
						 const Temperature& _krInitialTemp,
						 const Temperature& _krMinTemperature,
						 const EnergyFunction& _krEnergyFunction,
						 const ScheduleFunction& _krScheduleFunction,
						 const PerturbFunction& _krPerturb,
						 int _iIterMax)
{
	// Create a random number generator.
	std::random_device rd;
	std::mt19937 randomGenerator(rd());

	// Set the old state and energy to the initial state and energy.
	State state = _krInitialState;
	auto energy = _krEnergyFunction(_krInitialState);

	State bestState = state;
	auto bestEnergy = energy;

	// Create a distribution for the random number generator between 0, and 1.
	std::uniform_real_distribution<decltype(energy)> random(0, 1);

	// Get the initial temperature.
	Temperature temperature = _krInitialTemp;

	while (temperature > _krMinTemperature) {
		for (int i = 0; i < _iIterMax; ++i) {
			// Get a new state, by perturbing.
			State newState = _krPerturb(state);

			// Get the energy of the new state.
			auto newEnergy = _krEnergyFunction(newState);

			if (newEnergy > bestEnergy) {
				bestState = newState;
				bestEnergy = newEnergy;
				state = newState;
				energy = newEnergy;
				continue;
			}

			auto deltaEnergy = newEnergy - energy;

			if (deltaEnergy >= 0 ||
				random(randomGenerator) > std::exp(-deltaEnergy / temperature)) {
				state = newState;
				energy = newEnergy;
			}
		}

		temperature = _krScheduleFunction(temperature);
	}

	return bestState;
}

#endif	// __SIMULATEDANNEALING_H__