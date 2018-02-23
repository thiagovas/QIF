#include "channel.h"

Channel(int n_in, n_out) : n_in(n_in_), n_out(n_out_) {}

// This function parses a channel string.
void ParseInput(std::string input_str) {}

// This function returns a string that represents the
// current channel.
std::string to_string() {}

// This function transposes the current channel.
// The input becomes the output, and the output becomes the
// input; matrix-wise we'll have p(x|y) instead of p(y|x).
void Transpose() {}

// This function randomizes the current channel.
// Maintaining the channel dimensions.
void Randomize() {}
