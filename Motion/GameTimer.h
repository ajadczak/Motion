#ifndef GAMETIMER_H
#define GAMETIMER_H

#pragma once

class GameTimer
{
public:
	GameTimer(void);
	float DeltaTime()const;
	float TotalTime()const;
	void Reset(void);
	void Start(void);
	void Stop(void);
	void Tick(void);

private:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
};

#endif // GAMETIMER_H