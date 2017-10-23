#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <algorithm>
#include <boost/range/adaptor/indexed.hpp>

class Ga {

public:
	// Copy & Move prohibited.
	Ga(const Ga&) = delete;
	Ga& operator=(const Ga&) = delete;

	// Implicit type conversion prohibited of Constructor.
	explicit Ga(unsigned int seed_, const int genum_, const int innum_);
	~Ga();

	void one_point_crossover(float Pc_);
	void mutation(float Pm_);
	void evaluation();
	void elite_selection(int generation_);
	void roulette_selection();
	void print_gene(int generation_);

private:
	const int GENE_NUM; // １個体での遺伝子長
	const int IND_NUM; // １集団での個体数

	std::mt19937 mt;

	std::vector<std::vector<bool>> gene; // 遺伝子 <IND_NUM<GENE_NUM>>
	std::vector<int> fitness; // 適応度 <IND_NUM>
	std::vector<bool> elite; // エリート <GENE_NUM>

	int count(const std::vector<bool>& e_);
	bool calcProb(float prob);
};