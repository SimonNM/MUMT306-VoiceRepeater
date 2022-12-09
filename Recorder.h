#pragma once

class Recorder
{
  public:
    Recorder() {}
    ~Recorder() {}

    void Init(float *mem, size_t size)
    {
        buffer_size_ = size;
        buff_        = mem;

        InitBuff();
        state_ = State::EMPTY;
    }

    /** Handles reading/writing to the Buffer. */
    float process(const float input)
    {
        float sig = 0.f;
        switch(state_)
        {
            case State::EMPTY:
            case State::IDLE: break;
            case State::RECORDING:
                Write(pos_, input);
                recsize_ = pos_;
                pos_ += 1;
                if(pos_ > buffer_size_ - 1)
                {
                    state_ = State::PLAYING;
                    pos_   = 0;
                }
                break;
            case State::PLAYING:
                sig = Read(pos_);
                pos_ += 1;
                if(pos_ > recsize_ - 1)
                {
                    pos_   = 0;
                    state_ = State::IDLE;
                }
                break;
            default: break;
        }
        return sig;
    }

    /** Effectively erases the buffer 
     ** Note: This does not actually change what is in the buffer  */
    inline void clear() { state_ = State::EMPTY; }

    /** Engages/Disengages the recording.
     ** The first time this is triggered a new recording will be started.
     ** Recording will end at second trigger or when buffer is full. 
    */
    inline void trigRecord()
    {
        switch(state_)
        {
            case State::EMPTY:
            case State::IDLE:
                pos_   = 0;
                state_ = State::RECORDING;
                break;
            case State::RECORDING:
                pos_   = 0;
                state_ = State::PLAYING;
                break;
            // recording can not trigger while playing
            case State::PLAYING: break;
            default: state_ = State::EMPTY; break;
        }
    }

    /** Returns true if the recorder is currently being written to. */
    inline const bool isRecording() const { return state_ == State::RECORDING; }

    /** Plays back the recording.
     ** Trigger it again and playback will restart.
    */
    inline void playBack()
    {
        // playback can not trigger if empty of recording
        if(state_ == State::EMPTY || state_ == State::RECORDING)
            return;

        pos_   = 0;
        state_ = State::PLAYING;
    }

    /** Returns true if the recorder is currently read. */
    inline const bool isPlaying() const { return state_ == State::PLAYING; }

    /** Returns true if the recorder is ready to record or play. */
    inline const bool isReady() const
    {
        return state_ == State::EMPTY || state_ == State::IDLE;
    }

  private:
    /** Initialize the buffer */
    void InitBuff() { std::fill(&buff_[0], &buff_[buffer_size_ - 1], 0); }

    /** Get a floating point sample from the buffer */
    inline const float Read(size_t pos) const { return buff_[pos]; }

    /** Write to a known location in the buffer */
    inline void Write(size_t pos, float val) { buff_[pos] = val; }

    /** Internal recorder state */
    enum class State
    {
        EMPTY,
        IDLE,
        RECORDING,
        PLAYING,
    };

    /** Private Member Variables */
    State  state_;
    float *buff_;
    size_t buffer_size_;
    size_t pos_;
    size_t recsize_;
};