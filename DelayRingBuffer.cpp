#include "DelayRingBuffer.h"

// Overwrites existing data in the buffer.
void DelayRingBuffer::WriteDataBlock(short size, const float* in_dataStart)
{
	for (size_t i = 0; i < size; i++)
	{
		WriteSample(in_dataStart[i]);
	}
}

// Adds data to existing data in the buffer.
void DelayRingBuffer::AddDataBlock(short size, const float* in_dataStart)
{
	for (size_t i = 0; i < size; i++)
	{
		AddSample(in_dataStart[i]);
	}
}

// Writes sample at write head. Advances write head.
void DelayRingBuffer::WriteSample(float sample)
{
	if (mWriteHead >= mSizeInSamples)
		mWriteHead = 0;
	int index = mWriteHead;
	mWriteHead++;
	mBuffer[index] = sample;
}

// Write to sample at write head. Advances write head.
void DelayRingBuffer::AddSample(float sample)
{
	if (mWriteHead >= mSizeInSamples)
		mWriteHead = 0;
	int index = mWriteHead;
	mWriteHead++;
	mBuffer[index] += sample;
}

// Get read pointer for buffer. At start.
float* DelayRingBuffer::GetReadPointer(int index)
{
	if (index < mSizeInSamples)
		return &mBuffer[index];
}

// Get Sample at read head. Advances readhead.
float DelayRingBuffer::GetSampleAtReadHead()
{
	if (mReadHead >= mSizeInSamples)
		mReadHead = 0;

	int index = mReadHead;
	mReadHead++;

	if (mLerpValue > 0)
	{
		float v1 = mBuffer[index];
		float v2 = mBuffer[mReadHead % mSizeInSamples];
		return interpolate(v1, v2, mLerpValue);
	}
	else
		return mBuffer[index];
}

// Get Sample block, starting from read head. Advances head equal to size.
void DelayRingBuffer::GetDataBlock(short size, float* out_data)
{
	for (size_t i = 0; i < size; i++)
	{
		out_data[i] = GetSampleAtReadHead();
	}
}

int DelayRingBuffer::GetSize()
{
	return mSizeInSamples;
}

// Set read head buffer position
void DelayRingBuffer::SetReadHeadPosition(int index)
{
	mReadHead = index;
	// wrap back around to the end.
	if (mReadHead < 0)
		mReadHead += mSizeInSamples;
}

// Sets delay in miliseconds.
void DelayRingBuffer::SetDelayInMilliSeconds(float delay)
{
	float delayInSamples = (delay / 1000) * (float)mSampleRate;
	SetDelayInSamples(delayInSamples);
}

void DelayRingBuffer::SetDelayInSamples(float delay)
{
	float readHeadPrecise = mWriteHead - delay;
	int roundedHead = (int)readHeadPrecise;
	mLerpValue = readHeadPrecise - roundedHead;

	SetReadHeadPosition(roundedHead);
}

float DelayRingBuffer::interpolate(float v0, float v1, float t)
{
	return v0 + t * (v1 - v0);
}