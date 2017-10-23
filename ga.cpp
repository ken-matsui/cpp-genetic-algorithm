#include "ga.h"

Ga::Ga(unsigned int seed_, const int genum_, const int innum_) : GENE_NUM(genum_), IND_NUM(innum_) {
	mt.seed(seed_);

	// 初期集団の生成と適応度の評価
	gene.resize(IND_NUM);
	fitness.resize(GENE_NUM);
	for(const auto& g : gene | boost::adaptors::indexed()) {
		g.value().resize(GENE_NUM);
		std::generate(g.value().begin(), g.value().end(), [this]() { return calcProb(0.5); });
	}
	// 適応度の更新
	evaluation();
	// エリートの保存
	elite_selection(0);
}

void Ga::one_point_crossover(float Pc_) {
	// 親のペア数をランダムに決定
	std::uniform_int_distribution<> pair_dist(0, IND_NUM/2);
	int pair_num = pair_dist(mt);
	for(int i = 0; i < pair_num; ++i) {
		// 親を選択
		std::uniform_int_distribution<> parent_dist(0, IND_NUM-1);
		std::pair<int, int> parents = std::make_pair(parent_dist(mt), parent_dist(mt));
		while(parents.first == parents.second) { // 重複の削除
			parents.second = parent_dist(mt);
		}
		// 確率によって交叉する．
		if(calcProb(Pc_)) {
			// ランダムで交叉位置を決定．
			std::uniform_int_distribution<> cut_dist(0+1, GENE_NUM-1-1);
			int cut_point = cut_dist(mt);
			// cut_point以降をswap(交叉)する
			for(int i = cut_point; i < gene[parents.first].size(); ++i)
				std::iter_swap(gene[parents.first].begin()+i, gene[parents.second].begin()+i);
		}
	}
}

void Ga::mutation(float Pm_) {
	for(auto& ind : gene)
		for(std::vector<bool>::reference g : ind)
			if(calcProb(Pm_)) g.flip();
}

void Ga::evaluation() {
	for(const auto& g : gene | boost::adaptors::indexed())
		fitness.at(g.index()) = count(g.value());
}

void Ga::elite_selection(int generation_) {
	int index;
	switch(generation_) {
	default: // N-1 世代のエリートを N 世代のinferiorityに代入
		index = std::min_element(fitness.begin(), fitness.end()) - fitness.begin();
		gene.at(index) = elite;
		evaluation(); // 適応度の更新
	case 0: // N 世代のエリートを保存
		index = std::max_element(fitness.begin(), fitness.end()) - fitness.begin();
		elite = gene.at(index);
	}
}

void Ga::roulette_selection() {
	float fit_sum = std::accumulate(fitness.begin(), fitness.end(), 0.0);
	// rouletteに従って次世代を決定
	std::vector<std::vector<bool>> new_gene;
	for(int i = 0; i < IND_NUM; ++i) {
		std::uniform_real_distribution<> dist(0.0, 1.0);
		float dart = dist(mt);
		float range = 0.0;
		for(const auto& fit : fitness | boost::adaptors::indexed()) {
			range += fit.value() / fit_sum;
			if(range >= dart) {
				// 新世代の生成
				new_gene.push_back(gene.at(fit.index()));
				break;
			}
		}
	}
	gene = new_gene;
}

void Ga::print_gene(int generation_) {
	std::cout << std::endl << "Generation: " << generation_ << std::endl;
	for(const auto& ind : gene | boost::adaptors::indexed()) {
		std::cout << ind.index() << ": ";
		for(const bool g : ind.value())
			std::cout << (g == true ? "*" : " ");
		std::cout << " : " << fitness.at(ind.index()) << std::endl;
	}
	std::cout <<  "max: " << *std::max_element(fitness.begin(), fitness.end());
	std::cout << " min: " << *std::min_element(fitness.begin(), fitness.end());
	std::cout << std::fixed; // setprecisionで少数部のみの桁数を指定可能にする．
	std::cout << " avg: " << std::setprecision(2) << std::accumulate(fitness.begin(), fitness.end(), 0.0) / fitness.size();
	std::cout << std::endl;
}

int Ga::count(const std::vector<bool>& e_) {
	return std::count_if(e_.begin(), e_.end(), [](bool b_) { return b_; });
}

bool Ga::calcProb(float prob) {
	std::uniform_real_distribution<> dist(0.0, 1.0);
	return dist(mt) < prob ? true : false;
}

Ga::~Ga() {
}