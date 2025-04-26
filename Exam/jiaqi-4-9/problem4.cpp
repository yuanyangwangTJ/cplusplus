#include <chrono>
#include <iostream>
#include <random>
#include <optional>
#include <memory>
#include <utility>
#include <cassert>
 
constexpr int MTU = 64;
constexpr int MAX_POSSIBLE_PACKET_ID = 1000000;
 
unsigned rng_seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 rng{rng_seed};
 
int rand_int(const int max) {
    return std::uniform_int_distribution{0, max}(rng);
}
 
std::string rand_string(const int length) {
    std::string s;
    s.reserve(length);
    for (int i = 0; i < length; ++i) {
        s += std::uniform_int_distribution<char>{}(rng);
    }
    return s;
}
 
struct NoPacketLossPattern {
    bool operator()(int _) const {
        return false;
    }
};
 
struct DropFirstXPacketLossPattern {
    explicit DropFirstXPacketLossPattern(int max_packet_id) {
        x_ = rand_int(max_packet_id);
    }
 
    bool operator()(const int packet_id) const {
        return packet_id < x_;
    }
 
private:
    int x_;
};
 
struct FixedBatchRandomPacketLossPattern {
    explicit FixedBatchRandomPacketLossPattern(const int batch_size) {
        batch_size_ = batch_size;
    }
 
    bool operator()(const int packet_id) {
        if (packet_id % batch_size_ == 0) {
            next_packet_loss_idx_ = std::uniform_int_distribution{0, batch_size_ - 1}(rng);
        }
        return packet_id % batch_size_ == next_packet_loss_idx_;
    }
 
private:
    int next_packet_loss_idx_{-1};
    int batch_size_;
};
 
 
class Encoder {
public:
    // Set the data to be encoded, guarantee that this function is called before encode_packet
    virtual void set_data(std::string data) = 0;
 
    // Encode a packet, return nullopt if all data has been encoded
    virtual std::optional<std::string> encode_packet() = 0;
 
    virtual ~Encoder() = default;
};
 
class Decoder {
public:
    // Decode a packet, return nullopt if the data is not yet complete
    // The argument is nullopt if no more packets are available
    virtual std::optional<std::string> decode_packet(std::optional<std::string> packet) = 0;
 
    virtual ~Decoder() = default;
};
 
 
template<typename Pattern>
class TestDriver {
public:
    TestDriver(std::unique_ptr<Encoder> encoder, std::unique_ptr<Decoder> decoder, Pattern pattern)
            : encoder_{std::move(encoder)}, decoder_{std::move(decoder)}, pattern_{pattern} {
    }
 
    std::pair<std::optional<std::string>, int> simulate(std::string data) {
        encoder_->set_data(data);
        int packet_id = 0, num_packet = 0;
        while (true) {
            if (packet_id > MAX_POSSIBLE_PACKET_ID) {
                throw std::runtime_error("Packet ID exceeds maximum possible value");
            }
 
            auto packet = encoder_->encode_packet();
 
            if (!packet) {
                return {decoder_->decode_packet(std::nullopt), num_packet};
            }
 
            auto packet_loss = pattern_(packet_id++);
 
            if (packet->size() > MTU) {
                throw std::runtime_error("Packet size exceeds MTU");
            }
 
            if (packet_loss) {
                continue;
            }
 
            num_packet++;
 
            if (auto decoded_data = decoder_->decode_packet(packet)) {
                return {decoded_data, num_packet};
            }
        }
    }
 
    void test(const int length) {
        std::string data = rand_string(length);
        auto [decoded_data, num_packets] = simulate(data);
        if (!decoded_data) {
            throw std::runtime_error("Failed to decode data");
        }
        if (*decoded_data != data) {
            throw std::runtime_error("Decoded data does not match original data");
        }
        printf("Test passed, used %d packets\n", num_packets);
    }
 
private:
    std::unique_ptr<Encoder> encoder_{};
    std::unique_ptr<Decoder> decoder_{};
    Pattern pattern_;
};
 
 
class Channel1Encoder final : public Encoder {
public:
    void set_data(std::string data) override {
        data_ = std::move(data);
    }
 
    std::optional<std::string> encode_packet() override {
        if (idx_ >= data_.size()) {
            return std::nullopt;
        }
        std::string packet = data_.substr(idx_, MTU);
        idx_ += MTU;
        return packet;
    }
 
private:
    std::string data_;
    int idx_{0};
};
 
 
class Channel1Decoder final : public Decoder {
public:
    std::optional<std::string> decode_packet(const std::optional<std::string> packet) override {
        if (!packet) {
            return std::move(data_);
        }
        data_ += *packet;
        return std::nullopt;
    }
 
    std::string get_data() {
        return data_;
    }
 
private:
    std::string data_;
};
 
class Channel2Encoder final : public Encoder {
public:
    void set_data(std::string data) override {
        data_ = std::move(data);
    }
 
    std::optional<std::string> encode_packet() override {
        if (idx_ >= data_.size()) {
            return std::nullopt;
        }
        std::string packet = data_.substr(idx_, MTU);
        if (!second_time_sent_) {
            second_time_sent_ = true;
        } else {
            idx_ += MTU;
            second_time_sent_ = false;
        }
        return packet;
    }
 
private:
    std::string data_;
    bool second_time_sent_{false};
    int idx_{0};
};
 
using Channel2Decoder = Channel1Decoder;
 
 
class Channel3Encoder final : public Encoder {
public:
    void set_data(std::string data) override {
        data_ = std::move(data);
        // Randomly decide how many initial packets to lose
        lost_packets_ = rand_int(1001); // Between 0 and 1000 packets lost
        idx_ = 0; // Reset the index to start encoding
    }

    std::optional<std::string> encode_packet() override {
        // Skip the first lost_packets_ packets (simulating lost packets)
        if (lost_packets_ > 0) {
            --lost_packets_;
            return std::nullopt; // Simulating a lost packet
        }

        // If there are still data left to encode
        if (idx_ >= data_.size()) {
            return std::nullopt; // No more data to encode
        }

        std::string packet = data_.substr(idx_, MTU);
        idx_ += MTU; // Move the index forward
        return packet;
    }

private:
    std::string data_;
    int idx_{0};
    int lost_packets_{0}; // How many packets to simulate as lost at the beginning
};
    
     
class Channel3Decoder final : public Decoder {
public:
    std::optional<std::string> decode_packet(const std::optional<std::string> packet) override {
        // If no packet is received, return nullopt indicating more packets are needed
        if (!packet) {
            return std::nullopt;
        }

        // Add the received packet to the accumulated data
        data_ += *packet;

        // Check if we have accumulated enough data to reconstruct the original message
        if (data_.size() >= original_data_size_) {
            return data_; // Return the fully decoded data
        }

        // Otherwise, continue collecting packets
        return std::nullopt;
    }

    void set_original_data_size(int size) {
        original_data_size_ = size;
    }

private:
    std::string data_;
    int original_data_size_{0}; // The size of the original data
};

 
class Channel4Encoder final : public Encoder {
public:
    void set_data(std::string data) override {
        data_ = std::move(data);
        idx_ = 0; // Reset the index to start encoding
    }

    std::optional<std::string> encode_packet() override {
        // Ensure we're not past the end of the data
        if (idx_ >= data_.size()) {
            return std::nullopt; // No more data to encode
        }

        // Handle sending packets in batches of 11
        if (packet_counter_ == 10) {
            // Randomly decide which packet in this batch to lose
            if (rand_int(11) == 0) {
                // Skip this packet (simulate packet loss)
                packet_counter_ = 0;
                return std::nullopt; // Simulate loss
            }
        }

        // Otherwise, encode the packet
        std::string packet = data_.substr(idx_, MTU);
        idx_ += MTU; // Move the index forward

        // Increment packet counter and return packet
        packet_counter_ = (packet_counter_ + 1) % 11;
        return packet;
    }

private:
    std::string data_;
    int idx_{0};
    int packet_counter_{0}; // Track every 11 packets
};
    
 
class Channel4Decoder final : public Decoder {
public:
    std::optional<std::string> decode_packet(const std::optional<std::string> packet) override {
        // If no packet is received, return nullopt indicating more packets are needed
        if (!packet) {
            return std::nullopt;
        }

        // Add the received packet to the accumulated data
        data_ += *packet;

        // Check if we have accumulated enough data to reconstruct the original message
        if (data_.size() >= original_data_size_) {
            return data_; // Return the fully decoded data
        }

        // Otherwise, continue collecting packets
        return std::nullopt;
    }

    void set_original_data_size(int size) {
        original_data_size_ = size;
    }

private:
    std::string data_;
    int original_data_size_{0}; // The size of the original data
};
    
 
int main() {
    auto channel1_driver = TestDriver{
            std::make_unique<Channel1Encoder>(),
            std::make_unique<Channel1Decoder>(),
            NoPacketLossPattern{},
    };
    channel1_driver.test(65535);
 
    auto channel2_driver = TestDriver{
            std::make_unique<Channel2Encoder>(),
            std::make_unique<Channel2Decoder>(),
            FixedBatchRandomPacketLossPattern{2},
    };
    channel2_driver.test(65535);
 
    auto channel3_driver = TestDriver{
            std::make_unique<Channel3Encoder>(),
            std::make_unique<Channel3Decoder>(),
            DropFirstXPacketLossPattern{1000},
    };
    channel3_driver.test(65535);
 
    auto channel4_driver = TestDriver{
            std::make_unique<Channel4Encoder>(),
            std::make_unique<Channel4Decoder>(),
            FixedBatchRandomPacketLossPattern{11},
    };
    channel4_driver.test(65535);
}