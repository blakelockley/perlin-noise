#ifndef NOISE_H
#define NOISE_H

void generate_white_noise(float arr[], int width, int height);
void generate_smooth_noise(float arr[], float base[], int width, int height, int octave);
void generate_perlin_noise(float arr[], int width, int height, int n_octaves);

#endif  // NOISE_H
