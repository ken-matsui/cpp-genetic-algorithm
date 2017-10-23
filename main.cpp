#include "ga.h"

const int N = 15; // １個体での遺伝子長
const int M = 15; // １集団での個体数
const int T = 100; // 世代数
const float Pc = 0.30; // 交叉確率
const float Pm = 0.10; // 突然変異確率

int main(int argc, char *argv[]) {
	unsigned int seed;
	if(argc < 2) { // 引数与えなかった時
		std::random_device rnd;
		seed = rnd();
	}
	else {
		seed = std::stoi(argv[1]);
	}

	Ga ga{ seed, N, M };
	ga.print_gene(0);

	for(int i = 1; i < T; ++i) {
		ga.one_point_crossover(Pc);
		ga.mutation(Pm);
		ga.evaluation();
		ga.elite_selection(i);
		ga.roulette_selection();
		ga.print_gene(i);
	}

	return 0;
}