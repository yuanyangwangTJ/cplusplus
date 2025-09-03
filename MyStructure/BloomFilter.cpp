#include <vector>
#include <functional>
#include <cstddef> // for size_t

/**
 * @brief 布隆过滤器
 * 
 * @tparam T 要存储的元素类型。该类型必须被 std::hash 支持。
 */
template <typename T>
class BloomFilter {
public:
    /**
     * @brief 构造一个新的布隆过滤器对象
     * 
     * @param size 位数组的大小 (m)
     * @param num_hashes 哈希函数的数量 (k)
     */
    BloomFilter(size_t size, size_t num_hashes)
        : m_size(size), m_num_hashes(num_hashes), m_bits(size, false) {
    }

    /**
     * @brief向布隆过滤器中添加一个元素
     * 
     * @param item 要添加的元素
     */
    void add(const T& item) {
        // 计算 k 个哈希值
        size_t h = m_hasher(item);
        size_t h1 = h & 0xFFFFFFFF;
        size_t h2 = h >> 32;

        for (size_t i = 0; i < m_num_hashes; ++i) {
            size_t hash_val = (h1 + i * h2) % m_size;
            m_bits[hash_val] = true;
        }
    }

    /**
     * @brief 检查一个元素是否可能存在于布隆过滤器中
     * 
     * @param item 要检查的元素
     * @return如果元素可能存在，则返回 true；如果元素绝对不存在，则返回 false。
     */
    bool contains(const T& item) const {
        // 计算 k 个哈希值
        size_t h = m_hasher(item);
        size_t h1 = h & 0xFFFFFFFF;
        size_t h2 = h >> 32;

        for (size_t i = 0; i < m_num_hashes; ++i) {
            size_t hash_val = (h1 + i * h2) % m_size;
            if (!m_bits[hash_val]) {
                return false;
            }
        }
        return true;
    }

private:
    size_t m_size;       // 位数组的大小
    size_t m_num_hashes; // 哈希函数的数量
    std::vector<bool> m_bits; // 位数组
    std::hash<T> m_hasher;   // 用于计算哈希值的哈希器
};

// --- 使用示例 ---
// 您可以将此部分取消注释以进行测试。

#include <iostream>
#include <string>

int main() {
    // 创建一个大小为 1000，使用 5 个哈希函数的布隆过滤器
    BloomFilter<std::string> bf(1000, 5);

    // 添加一些元素
    bf.add("hello");
    bf.add("world");
    bf.add("bloom");
    bf.add("filter");

    // 检查元素是否存在
    std::cout << "Contains 'hello': " << (bf.contains("hello") ? "Possibly" : "Definitely Not") << std::endl;
    std::cout << "Contains 'world': " << (bf.contains("world") ? "Possibly" : "Definitely Not") << std::endl;
    std::cout << "Contains 'test': " << (bf.contains("test") ? "Possibly" : "Definitely Not") << std::endl;
    std::cout << "Contains 'filter': " << (bf.contains("filter") ? "Possibly" : "Definitely Not") << std::endl;
    
    BloomFilter<int> bf_int(100, 3);
    bf_int.add(123);
    bf_int.add(456);

    std::cout << "Contains 123: " << (bf_int.contains(123) ? "Possibly" : "Definitely Not") << std::endl;
    std::cout << "Contains 789: " << (bf_int.contains(789) ? "Possibly" : "Definitely Not") << std::endl;

    return 0;
}
