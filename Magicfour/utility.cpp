#include "utility.h"

std::random_device	RandomGenerator::rd_;
std::mt19937		RandomGenerator::generator_ = std::mt19937(rd_());