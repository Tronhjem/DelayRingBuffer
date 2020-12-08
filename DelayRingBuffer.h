#pragma once

class DelayRingBuffer {
public:
	DelayRingBuffer(int size, int sampleRate) :
		mSizeInSamples{ size }
	{
		mBuffer = new float[size] { 0 };
        mSampleRate = sampleRate;
	}

	~DelayRingBuffer()
	{
		delete[] mBuffer;
	}

	void WriteDataBlock(short size, const float* in_dataStart);
	void AddDataBlock(short size, const float* in_dataStart);
	void WriteSample(float sample);
	void AddSample(float sample);
    float* GetReadPointer(int index);
    void GetDataBlock(short size, float* data);
    float GetSampleAtReadHead();
	int GetSize();
	void SetReadHeadPosition(int index);
    void SetDelayInMilliSeconds(float delay);

private:
    float interpolate(int v0, int v1, float t);
	void SetDelayInSamples(int delay);
	int mSizeInSamples;	
	int mWriteHead = 0;
	int mReadHead = 0;
	float mLerpValue = 0;
    int mSampleRate = 0;
	float* mBuffer;
    
};