#include "noise.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

void generate_white_noise(float arr[], int width, int height) {
    for (int i = 0; i < width * height; i++)
        arr[i] = (float)rand() / (float)RAND_MAX;
}

void generate_smooth_noise(float arr[], float base[], int width, int height, int octave) {
    int period = 1 << octave;
    float frequency = 1.0f / period;

    for (int i = 0; i < width; i++) {
        // Calculate the horizontal sampling indices
        int sample_i0 = (i / period) * period;         // floor
        int sample_i1 = (sample_i0 + period) % width;  // wrap around

        float horizontal_blend = (i - sample_i0) * frequency;

        for (int j = 0; j < height; j++) {
            // Calculate the vertical sampling indices
            int sample_j0 = (j / period) * period;          // floor
            int sample_j1 = (sample_j0 + period) % height;  // wrap around
            float vertical_blend = (j - sample_j0) * frequency;

            // Blend the top two corners
            float top = lerp(base[sample_i0 + sample_j0 * width],
                             base[sample_i1 + sample_j0 * width], horizontal_blend);

            // Blend the bottom two corners
            float bottom = lerp(base[sample_i0 + sample_j1 * width],
                                base[sample_i1 + sample_j1 * width], horizontal_blend);

            // Blend
            arr[i + j * width] = lerp(top, bottom, vertical_blend);
        }
    }
}

void generate_perlin_noise(float arr[], int width, int height, int n_octaves) {
    float *smooth_noise[n_octaves];

    float *base = malloc(width * height * sizeof(float));
    generate_white_noise(base, width, height);

    for (int i = 0; i < n_octaves; i++) {
        smooth_noise[i] = malloc(width * height * sizeof(float));
        generate_smooth_noise(smooth_noise[i], base, width, height, i);
    }

    float persistance = 0.5f;
    float amplitude = 1.0f;
    float total_amplitude = 0.0f;

    memset(arr, 0, sizeof(float) * width * height);

    // Blend
    for (int octave = n_octaves - 1; octave >= 0; octave--) {
        amplitude *= persistance;
        total_amplitude += amplitude;

        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
                arr[i + j * width] += smooth_noise[octave][i + j * width] * amplitude;
    }

    // Normalisation
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            arr[i + j * width] /= total_amplitude;

    // Clean up
    free(base);
    for (int i = 0; i < n_octaves; i++)
        free(smooth_noise[i]);
}
