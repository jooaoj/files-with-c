#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

struct wav_header {
    char riff[4];
    int32_t flength; // includes 44-bytes of header
    char wave[4];
    char fmt[4]; // Format chunk marker; includes trailing null
    int32_t chunk_size;
    int16_t format; // 1 = PCM
    int16_t num_chans;
    int32_t sample_rate;
    int32_t bytes_per_second;
    int16_t bytes_per_sample;
    int16_t bits_per_sample;
    char data[4];
    int32_t dlength; // excludes 44-bytes of header
};

int main() {
    const int sample_rate = 8000;
    struct wav_header wavh;

    // Start with hard-coding needed data
    strncpy(wavh.riff, "RIFF", sizeof(wavh.riff));
    strncpy(wavh.wave, "WAVE", sizeof(wavh.wave));
    strncpy(wavh.fmt, "fmt ", sizeof(wavh.fmt));
    strncpy(wavh.data, "data", sizeof(wavh.data));

    wavh.chunk_size = 16;
    wavh.format = 1;
    wavh.num_chans = 1;
    wavh.sample_rate = sample_rate;
    wavh.bits_per_sample = 16;
    wavh.bytes_per_sample = (wavh.bits_per_sample / 8) * wavh.num_chans;
    wavh.bytes_per_second = wavh.sample_rate * wavh.bytes_per_sample;

    const int duration_seconds = 10;
    const int buffer_size = sample_rate * duration_seconds;

    wavh.dlength = buffer_size * wavh.bytes_per_sample;
    wavh.flength = wavh.dlength + 44;

    // Play a constant C-note

    short int buffer[buffer_size];

    // Cosine-wave of frequency
    const float frequency = 256.0;
    const short int amplitude = 1000;
    for (int i = 0; i < buffer_size; i++) {
        buffer[i] = (short int) (cos((2 * M_PI * frequency * i) / sample_rate) * amplitude);
    }

    FILE *fp = fopen("test.wav", "w");
    fwrite(&wavh, 1, sizeof(wavh), fp);
    fwrite(buffer, 2, buffer_size, fp);

    return 0;
}
