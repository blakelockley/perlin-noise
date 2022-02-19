#include "noise.h"

#include <stdio.h>
#include <stdlib.h>

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
        //calculate the horizontal sampling indices
        int sample_i0 = (i / period) * period;
        int sample_i1 = (sample_i0 + period) % width;  //wrap around

        float horizontal_blend = (i - sample_i0) * frequency;

        for (int j = 0; j < height; j++) {
            //calculate the vertical sampling indices
            int sample_j0 = (j / period) * period;
            int sample_j1 = (sample_j0 + period) % height;  //wrap around
            float vertical_blend = (j - sample_j0) * frequency;

            //blend the top two corners
            float top = lerp(base[sample_i0 * width + sample_j0],
                             base[sample_i1 * width + sample_j0], horizontal_blend);

            //blend the bottom two corners
            float bottom = lerp(base[sample_i0 * width + sample_j1],
                                base[sample_i1 * width + sample_j1], horizontal_blend);

            //final blend
            arr[i * width + j] = lerp(top, bottom, vertical_blend);
        }
    }
}

void generate_perlin_noise(float arr[], int width, int height, int n_octaves) {
    float smooth_noise[n_octaves][width * height];

    float base[width * height];
    generate_white_noise(base, width, height);

    for (int i = 0; i < n_octaves; i++)
        generate_smooth_noise(smooth_noise[i], base, width, height, i);

    float persistance = 0.5f;
    float amplitude = 1.0f;
    float total_amplitude = 0.0f;

    // Blend
    for (int octave = n_octaves - 1; octave >= 0; octave--) {
        amplitude *= persistance;
        total_amplitude += amplitude;

        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
                arr[i * width + j] += smooth_noise[octave][i * width + j] * amplitude;
    }

    // Normalisation
    // for (int i = 0; i < width; i++) {
    //     for (int j = 0; j < height; j++) {
    //         arr[i * width + j] /= total_amplitude;
    //     }
    // }
}
