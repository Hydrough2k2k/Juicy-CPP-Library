#pragma once

/* JuicyTime.h created by Hydrough
* Version 0.1.0 (alpha)
*/

#include <chrono>
#include <vector>
#include <string>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;

/* Juicy Timer usage guide:
* 
* Run the initialise function, to initialise as many timers as you want, eg. 5:
* initialiseTimer(int timerCount);
* 
* Start a timer, pass an ID if you want to:
* startTimer(int timerID);
* 
* Get Elapsed Time:
* 
* getDetailedElapsedTime(); gives every time unit it's own value
* 
* printElapsedTime(); prints elapsed time as text
* 
* getElapsedTime(); prints elapsed time as a fraction
*/

namespace JuicyTime {

	//typedef enum JTimeErr {  };
	typedef enum timeFormat { TimeFormat_NanoSec, TimeFormat_MicroSec, TimeFormat_MilliSec, TimeFormat_Seconds, TimeFormat_Minutes, TimeFormat_Hours, TimeFormat_Days };
	typedef enum dateFormat { DateFormat_yyyy_mm_dd, DateFormat_yy_mm_dd, DateFormat_dd_mm_yyyy, DateFormat_dd_mm_yy, DateFormat_mm_dd_yyyy, DateFormat_mm_dd_yy };

	struct timeStruct { unsigned int days, hours, minutes, seconds, milliSec, microSec, nanoSec; };
	const std::string months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun,", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	const int divRatio[] = { 1, 1000, 1000, 1000, 60, 60, 24 };

	inline static unsigned int initialisedTimers = 0;
	inline static bool initialised = false;
	inline static std::vector<std::chrono::steady_clock::time_point> timer;

	// Run this code first to initialise the timers
	// Running this more than once will not do anything
	static int initialiseTimer(unsigned int timerCount = 1) {
		if (initialised || timerCount == 0) return 1; // Insert Error Code TBD

		initialisedTimers = timerCount;
		for (unsigned int i = 0; i < timerCount; i++)
			timer.push_back(high_resolution_clock::now());
		initialised = true;
		return 0;
	}

	// Start a timer, return 0 if successful
	int startTimer(unsigned int timerID = 0) {
		if (initialisedTimers <= timerID) return 1; // Insert Error Code TBD
		timer[timerID] = high_resolution_clock::now();
		return 0;
	}

	timeStruct convertTime(long long deltaTime) {
		timeStruct elapsedTime;

		elapsedTime.nanoSec = deltaTime % 1000;
		deltaTime /= 1000;
		elapsedTime.microSec = deltaTime % 1000;
		deltaTime /= 1000;
		elapsedTime.milliSec = deltaTime % 1000;
		deltaTime /= 1000;
		elapsedTime.seconds = deltaTime % 60;
		deltaTime /= 60;
		elapsedTime.minutes = deltaTime % 60;
		deltaTime /= 60;
		elapsedTime.hours = deltaTime % 24;
		deltaTime /= 24;
		elapsedTime.days = deltaTime;

		return elapsedTime;
	}

	// Returns seconds, minutes, hours etc. all as separate values
	timeStruct getDetailedElapsedTime(unsigned int timerID = 0) {
		if (initialisedTimers <= timerID) return {}; // Insert Error Code TBD

		std::chrono::steady_clock::time_point timeNow = high_resolution_clock::now();
		long long deltaTime = duration_cast<nanoseconds>(timeNow - timer[timerID]).count();
		timeStruct elapsedTime = convertTime(deltaTime);

		return elapsedTime;
	}

	// Returns a formatted string to show elapsed time
	std::string printElapsedTime(unsigned int timerID = 0, bool printEmptyValues = false) {
		if (initialisedTimers <= timerID) return ""; // Insert Error Code TBD

		std::chrono::steady_clock::time_point timeNow = high_resolution_clock::now();
		long long deltaTime = duration_cast<nanoseconds>(timeNow - timer[timerID]).count();
		timeStruct elapsedTime = convertTime(deltaTime);
		std::string str = "";

		if (elapsedTime.days > 0 || printEmptyValues) {
			str += std::to_string(elapsedTime.days) + " Day";
			if (elapsedTime.days != 1) str += "s";
			str += ", ";
			printEmptyValues = true;
		}
		if (elapsedTime.hours > 0 || printEmptyValues) {
			str += std::to_string(elapsedTime.hours) + " Hour";
			if (elapsedTime.hours != 1) str += "s";
			str += ", ";
			printEmptyValues = true;
		}
		if (elapsedTime.minutes > 0 || printEmptyValues) {
			str += std::to_string(elapsedTime.minutes) + " Minute";
			if (elapsedTime.minutes != 1) str += "s";
			str += ", ";
			printEmptyValues = true;
		}
		if (elapsedTime.seconds > 0 || printEmptyValues) {
			str += std::to_string(elapsedTime.seconds) + " Second";
			if (elapsedTime.seconds != 1) str += "s";
			str += ", ";
			printEmptyValues = true;
		}
		if (elapsedTime.milliSec > 0 || printEmptyValues) {
			str += std::to_string(elapsedTime.milliSec) + " Millisecond";
			if (elapsedTime.milliSec != 1) str += "s";
			str += ", ";
			printEmptyValues = true;
		}
		if (elapsedTime.microSec > 0 || printEmptyValues) {
			str += std::to_string(elapsedTime.microSec) + " Microsecond";
			if (elapsedTime.microSec != 1) str += "s";
			str += ", ";
			printEmptyValues = true;
		}
		if (elapsedTime.nanoSec > 0 || printEmptyValues) {
			str += std::to_string(elapsedTime.nanoSec) + " Nanosecond";
			if (elapsedTime.nanoSec != 1) str += "s";
		}
			
		return str;
	}

	// Return a given time unit, seconds by default, eg. 9.723
	double getElapsedTime(unsigned int timerID = 0, timeFormat format = TimeFormat_Seconds) {
		if (initialisedTimers <= timerID) return 0; // Insert Error Code TBD

		std::chrono::steady_clock::time_point timeNow = high_resolution_clock::now();
		long long deltaTime = duration_cast<nanoseconds>(timeNow - timer[timerID]).count();

		int i = 0;
		for (; i < format; i++)
			deltaTime /= divRatio[i];

		return (double)deltaTime / divRatio[i];
	}
}
