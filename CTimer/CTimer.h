/*
 * CTimer.h
 *
 * Created: 1/3/2022 12:47:58 PM
 *  Author: Mike
 */ 


#ifndef CTIMER_H_
#define CTIMER_H_

typedef void (*TimerCallback)(void);

class CTimer
{
	private:
		TimerCallback	m_callback;
		uint32_t		m_target;
	
	public:
		CTimer();
		
		void StartTimer();
		void StopTimer();
		void ResetTimer();
		
		uint32_t	GetTicks();
		void SetCallback(TimerCallback callback, uint32_t target);
};


#endif /* CTIMER_H_ */