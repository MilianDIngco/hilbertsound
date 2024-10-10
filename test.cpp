#include <iostream>
#include <cmath>
#include <AL/al.h>
#include <AL/alc.h>

const int SAMPLE_RATE = 44100;   // CD-quality sample rate
const float DURATION = 10.0f;     // Duration of the tone in seconds
const float FREQUENCY = 1100.0f;  // Frequency of the tone (A4 = 440 Hz)

int main() {
    // Initialize OpenAL and create a context
    ALCdevice *device = alcOpenDevice(nullptr); // open default device
    if (!device) {
        std::cerr << "Error: Could not open sound device." << std::endl;
        return -1;
    }

    ALCcontext *context = alcCreateContext(device, nullptr);
    if (!context || !alcMakeContextCurrent(context)) {
        std::cerr << "Error: Could not create or set context." << std::endl;
        if (context) alcDestroyContext(context);
        alcCloseDevice(device);
        return -1;
    }

    // Generate a sine wave
    int sampleCount = static_cast<int>(SAMPLE_RATE * DURATION);
    short *samples = new short[sampleCount];  // 16-bit audio
    for (int i = 0; i < sampleCount; ++i) {
        float sample = std::sin(2.0f * M_PI * FREQUENCY * i / SAMPLE_RATE); // sine wave formula
        samples[i] = static_cast<short>(sample * 32767);  // scale to 16-bit short
    }

    // Create a buffer and fill it with the generated sine wave data
    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_MONO16, samples, sampleCount * sizeof(short), SAMPLE_RATE);

    // Create a source to play the buffer
    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);

    // Play the sound
    alSourcePlay(source);

    // Wait for the sound to finish playing
    ALint source_state;
    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    while (source_state == AL_PLAYING) {
        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    }

    // Clean up OpenAL resources
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);

    // Close OpenAL context and device
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);

    // Delete the generated sine wave data
    delete[] samples;

    std::cout << "Tone playback finished!" << std::endl;
    return 0;
}