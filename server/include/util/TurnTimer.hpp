#pragma once

#include <boost/asio.hpp>
#include <functional>

namespace poker {
    class TurnTimer {
    public:
        TurnTimer(boost::asio::io_context &io);

        void start(int seconds, std::function<void()> cb);

        void cancel();

    private:
        boost::asio::steady_timer timer;
    };
}
