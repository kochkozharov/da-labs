#pragma once
#include <cstddef>
#include <vector>

using ll = long long;

std::size_t RelativeToAbsolute(const std::pair<size_t, size_t>& rel, const std::vector<std::size_t> &prefixSums);
std::pair<size_t, size_t> AbsoluteToRelative(std::size_t absoluteIndex, const std::vector<std::size_t>& prefixSums);
std::vector<std::size_t> ZFunction(const std::vector<ll> &arr);