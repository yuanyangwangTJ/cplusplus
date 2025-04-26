/*
量化交易策略系统 (中等 40分)
时间限制: 1s
空间限制: 256MB
限定语言: C++(clang++17)
*/

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <deque>
#include <numeric>
#include <stdexcept>

class MarketData {
public:
    MarketData(const std::string& symbol, double price, int volume)
        : symbol_(symbol)
        , price_(price)
        , volume_(volume) {}

    std::string getSymbol() const { return symbol_; }
    double getPrice() const { return price_; }
    int getVolume() const { return volume_; }

private:
    std::string symbol_;
    double price_;
    int volume_;
};

class TradingSignal {
public:
    TradingSignal(const std::string& symbol, double price, int quantity)
        : symbol_(symbol)
        , price_(price)
        , quantity_(quantity) {}

    virtual std::string getType() const { return ""; }
    std::string getSymbol() const { return symbol_; }
    double getPrice() const { return price_; }
    int getQuantity() const { return quantity_; }

private:
    std::string symbol_;
    double price_;
    int quantity_;
};

class BuySignal : public TradingSignal {
public:
    BuySignal(const std::string& symbol, double price, int quantity)
        : TradingSignal(symbol, price, quantity) {}

    /*
    @problem: 重写虚函数，C++更推荐带有override标识符，以防止函数签名、返回值、参数等带来的错误
    */
    // 此处加入override关键字，可有效防止人为失误错写getType函数
    std::string getType() const override { return "BUY"; }
};

class SellSignal : public TradingSignal {
public:
    SellSignal(const std::string& symbol, double price, int quantity)
        : TradingSignal(symbol, price, quantity) {}


    std::string getType() const override { return "SELL"; }
};

class Strategy {
public:
    Strategy(const std::string& name)
        : name_(name) {}
    virtual std::shared_ptr<TradingSignal> generateSignal(const MarketData& data) = 0;
    std::string getName() const { return name_; }

protected:
    std::string name_;
    std::vector<double> historical_prices_;
};

class MovingAverageStrategy : public Strategy {
public:
    MovingAverageStrategy()
        : Strategy("MA") {}

    std::shared_ptr<TradingSignal> generateSignal(const MarketData& data) override {
        historical_prices_.push_back(data.getPrice());

        double ma = std::accumulate(historical_prices_.begin(), historical_prices_.end(), 0.0) /
                    historical_prices_.size();

        if (data.getPrice() > ma * 1.1) {
            return std::make_shared<SellSignal>(data.getSymbol(), data.getPrice(), 100);
        } else if (data.getPrice() < ma * 0.9) {
            return std::make_shared<BuySignal>(data.getSymbol(), data.getPrice(), 100);
        }
        return nullptr;
    }
};

class MomentumStrategy : public Strategy {
public:
    MomentumStrategy()
        : Strategy("Momentum") {}

    std::shared_ptr<TradingSignal> generateSignal(const MarketData& data) override {
        historical_prices_.push_back(data.getPrice());

        /*
        @problem: 这里historical_prices_的访问可能出现越界错误，需要进行边界检查
        */
        // 进行边界检查，防止越界
        if (historical_prices_.size() < 2) {
            return nullptr;
        }
        double prev_price  = historical_prices_[historical_prices_.size() - 2];
        double return_rate = (data.getPrice() - prev_price) / prev_price;

        if (return_rate > 0.02) {
            return std::make_shared<BuySignal>(data.getSymbol(), data.getPrice(), 100);
        } else if (return_rate < -0.02) {
            return std::make_shared<SellSignal>(data.getSymbol(), data.getPrice(), 100);
        }
        return nullptr;
    }
};

class Portfolio {
public:
    void processSignal(const std::shared_ptr<TradingSignal>& signal) {
        if (!signal) return;

        /*
        @problem: 下面的代码逻辑是根据信号的类型进行买卖操作，但是在处理逻辑上存在问题：
        1. 当购买时，需要保证有足够的现金流
        2. 当卖出时，需要保证持有足够数量的股票
        */
        std::string symbol = signal->getSymbol();
        if (signal->getType() == "BUY") {
            // 首先判断是否有足够的现金流
            if (cash_ >= signal->getPrice() * signal->getQuantity()) {
                positions_[symbol] += signal->getQuantity();
                cash_ -= signal->getPrice() * signal->getQuantity();
            } else {
                // 无法完成购买操作
                return;
            }
            
        } else {
            // 需要判断是否持有足够的股票
            if (positions_.find(symbol) != positions_.end() 
                && positions_[symbol] >= signal->getQuantity()) {
                positions_[symbol] -= signal->getQuantity();
                cash_ += signal->getPrice() * signal->getQuantity();
            } else {
                return;
            }
        }

        std::cout << "Executed " << signal->getType() << " " << signal->getQuantity()
                  << " shares of " << symbol << " at " << signal->getPrice() << std::endl;
    }

    void printStatus() const {
        std::cout << "Portfolio Status:" << std::endl;
        std::cout << "Cash: " << cash_ << std::endl;
        for (const auto& [symbol, quantity] : positions_) {
            std::cout << symbol << ": " << quantity << " shares" << std::endl;
        }
    }

private:
    std::unordered_map<std::string, int> positions_;
    double cash_ = 1000000.0;
};

class SignalTracker {
public:
    void trackSignal(const TradingSignal& signal) { signals_.push_back(signal); }

    void printHistory() const {
        for (const auto& signal : signals_) {
            std::cout << "Signal: " << signal.getType() << " " << signal.getSymbol()
                      << " Quantity: " << signal.getQuantity() << " Price: " << signal.getPrice()
                      << std::endl;
        }
    }

private:
    std::vector<TradingSignal> signals_;
};

class TradingEngine {
public:
    void addStrategy(std::shared_ptr<Strategy> strategy) { strategies_.push_back(strategy); }

    void processMarketData(const MarketData& data) {
        for (const auto& strategy : strategies_) {
            auto signal = strategy->generateSignal(data);
            if (signal) {
                signal_tracker_.trackSignal(*signal);
            }
            portfolio_.processSignal(signal);
        }
    }

    void showPortfolioStatus() {
        portfolio_.printStatus();
        std::cout << "\nTrading Signal History:" << std::endl;
        signal_tracker_.printHistory();
    }

private:
    std::vector<std::shared_ptr<Strategy>> strategies_;
    Portfolio portfolio_;
    SignalTracker signal_tracker_;
};

int main() {
    TradingEngine engine;

    engine.addStrategy(std::make_shared<MovingAverageStrategy>());
    engine.addStrategy(std::make_shared<MomentumStrategy>());

    std::vector<MarketData> market_data = {
        MarketData("AAPL", 150.0, 1000), MarketData("AAPL", 152.0, 1200),
        MarketData("AAPL", 148.0, 800), MarketData("AAPL", 145.0, 1500),
        MarketData("AAPL", 153.0, 2000)};

    try {
        for (const auto& data : market_data) {
            std::cout << "\nProcessing market data: " << data.getSymbol()
                      << " Price: " << data.getPrice() << std::endl;
            engine.processMarketData(data);
            engine.showPortfolioStatus();
        }
    } catch (...) {
        std::cerr << "Unexpected error occurred!" << std::endl;
    }

    return 0;
}