#include "daisy_patch_sm.h"
#include "Recorder.h"

using namespace daisy;
using namespace patch_sm;

DaisyPatchSM hw;
Switch       button;

#define RecBufSize 48000 * 60 // 60 seconds of recording at 48kHz
#define BufSize 25600
#define Sensitivity 50
#define SigErgThresh 1 / 1000.f // Threshold for noise in input
#define SigRatio 0.9            // Leaky integration coefficient

Recorder            recorder;
float DSY_SDRAM_BSS recBuffer[RecBufSize];
float               runE       = 0.f; // running signal energy
int                 noiseCount = 0;   // keeps track of noise input
float DSY_SDRAM_BSS buffer[BufSize];
size_t              pos = 0;

void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    // Process hardware peripherals
    hw.ProcessAllControls();
    button.Debounce();

    // Output gain (volume control)
    float out_level = hw.GetAdcValue(CV_1);

    // On button press, playback recording
    if(button.RisingEdge())
        recorder.playBack();

    // if you hold the button longer than 1000 ms (1 sec), clear the recording
    if(button.TimeHeldMs() >= 1000.f)
        recorder.clear();

    // Start recording on noise above threshold
    if(noiseCount >= Sensitivity && recorder.isReady())
        recorder.trigRecord();

    // End recording if there's no more noise
    if(recorder.isRecording() && noiseCount == 0)
        recorder.trigRecord();

    hw.SetLed(recorder.isRecording());
    float sigE = 0.f;
    // Process audio
    for(size_t i = 0; i < size; i++)
    {
        // fill in buffer
        buffer[pos] = IN_L[i];
        pos += 1;
        if(pos > BufSize - 1)
            pos = 0;

        // calculate signal energy
        sigE += IN_L[i] * IN_L[i];
        // store signal = loop signal * loop gain + in * in_gain
        float sig = recorder.process(buffer[pos]) * out_level;
        OUT_L[i]  = sig;
        OUT_R[i]  = sig;
    }

    // Determine energy change in signal
    runE = (1 - SigRatio) * sigE + SigRatio * runE;
    hw.PrintLine("Input Signal Energy: " FLT_FMT3, FLT_VAR3(runE * 1000.f));

    // Adjust noise count if signal energy above threshold
    if(runE > SigErgThresh && noiseCount != 100)
        noiseCount += 1;
    else if(noiseCount != 0)
        noiseCount -= 1;
}

int main(void)
{
    // Init hardware
    hw.Init();
    hw.SetAudioBlockSize(256); // number of samples handled per callback
    hw.StartLog();

    // Init recorder
    recorder.Init(recBuffer, RecBufSize);

    // Init button for playback
    button.Init(hw.B7);

    std::fill(&buffer[0], &buffer[BufSize - 1], 0);
    // Start audio callback
    hw.StartAudio(AudioCallback);
    while(1) {}
}
