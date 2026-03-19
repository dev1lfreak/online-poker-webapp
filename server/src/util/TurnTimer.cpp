#include "../../include/util/TurnTimer.hpp"

namespace poker {
    TurnTimer::TurnTimer(boost::asio::io_context &io)
        : timer(io) {
    }

    void TurnTimer::start(int seconds, std::function<void()> cb) {
        timer.expires_after(std::chrono::seconds(seconds));
        timer.async_wait([cb](auto ec) {
            if (!ec) cb();
        });
    }

    void TurnTimer::cancel() {
        timer.cancel();
    }
}
