#ifndef PRINTSTATUSBAR_H
#define PRINTSTATUSBAR_H
#include <iostream>

template<typename T, typename U> void showProgressBar(T current_amount, U total_amount, int nbOfstepsToPrint)
{
	if (total_amount < nbOfstepsToPrint || current_amount % (total_amount / nbOfstepsToPrint) != 0) return;

	float progress = static_cast<float>(current_amount) / static_cast<float>(total_amount);
	int barWidth = 70;

	std::cout << '\r' << "[";
	int pos = barWidth * progress;
	for (int i = 0; i < barWidth; ++i) {
		if (i < pos) std::cout << "=";
		else if (i == pos) std::cout << ">";
		else std::cout << " ";
	}
	std::cout << "] " << int(progress * 100.0) << " %\r";
	std::cout.flush();
}
#endif
