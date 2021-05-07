#pragma once
#include <chrono>

template <class TimeT = std::chrono::milliseconds,
class ClockT = std::chrono::steady_clock>
class Timer
{
    using timep_t = typename ClockT::time_point;
    timep_t _start = ClockT::now(), _end = {};
    bool m_set = false;

public:
	bool is_set() const
	{
        return m_set;
	}
	
    void tick() {
        m_set = true;
        _end = timep_t{};
        _start = ClockT::now();
    }

    void tock() { _end = ClockT::now(); }

    template <class TT = TimeT>
    TT duration() const {
        gsl_Expects(_end != timep_t{} && "toc before reporting");
        return std::chrono::duration_cast<TT>(_end - _start);
    }
};

