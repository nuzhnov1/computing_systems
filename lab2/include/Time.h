#ifndef TIME_H
#define TIME_H

#include <chrono>
#include <functional>


namespace Time {
    using _duration = std::chrono::steady_clock::duration;

    template <typename R, typename ...Args>
    std::pair<R, _duration> elapse_time(std::function<R(Args...)> function, Args... args) {
        auto begin = std::chrono::steady_clock::now();
        auto result = function(args...);
        auto end = std::chrono::steady_clock::now();
        auto duration = end - begin;

        return std::make_pair(std::move(result), duration);
    }
}


#endif //TIME_H
