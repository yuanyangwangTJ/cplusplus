#pragma once

#include <variant>
#include <vector>
#include <unordered_map>
#include <string_view>
#include <cctype>
#include <cassert>
#include <stdexcept>
#include <optional>
#include <charconv>
#include <regex>
#include <sstream>
#include <iostream>

#include "print.h"

// 模板元编程获取variant索引
template <typename T, typename>
struct get_index;

template <size_t I, typename... Ts>
struct get_index_impl {};

template <size_t I, typename T, typename... Ts>
struct get_index_impl<I, T, T, Ts...> : std::integral_constant<size_t, I>{};

template <size_t I, typename T, typename U, typename... Ts>
struct get_index_impl<I, T, U, Ts...> : get_index_impl<I+1, T, Ts...>{};

template <typename T, typename... Ts>
struct get_index<T, std::variant<Ts...>> : get_index_impl<0, T, Ts...>{};

template <typename T, typename... Ts>
constexpr auto get_index_v = get_index<T, Ts...>::value;

enum class value_t {
	Null = 0x0,
	Bool,
	Double,
	String,
	Array,
	Object,
	Undefined
};

class JsonValue;

using JsonArray = std::vector<JsonValue>;
using JsonObject = std::unordered_map<std::string, JsonValue>;

class JsonValue {
public:
	using variant_t = std::variant<
		std::monostate,
		bool,
		int,
		double,
		std::string,
		JsonArray,
		JsonObject
	>;

	// 编译期获取索引
	constexpr static auto NullIdx = get_index_v<std::monostate, variant_t>;
	constexpr static auto BoolIdx = get_index_v<bool, variant_t>;
	constexpr static auto IntIdx = get_index_v<int, variant_t>;
	constexpr static auto DoubleIdx = get_index_v<double, variant_t>;
	constexpr static auto StringIdx = get_index_v<std::string, variant_t>;
	constexpr static auto ArrayIdx = get_index_v<JsonArray, variant_t>;
	constexpr static auto ObjectIdx = get_index_v<JsonObject, variant_t>;

	variant_t inner_;

	template <class T>
	bool is() const {
		return std::holds_alternative<T>(inner_);
	}

	template <class T>
	T const& get() const {
		return std::get<T>(inner_);
	}

	template <class T>
	T& get() {
		return std::get<T>(inner_);
	}

	// 转化为string
	std::string toString() const;

	friend std::ostream& operator<<(std::ostream& os, const JsonValue& val) {
		os << val.toString();
		return os;
	}

	JsonValue& operator[](const std::string& key) {
		if (this->is<JsonObject>()) {
			auto&& obj = this->get<JsonObject>();
			if (obj.find(key) != obj.end()) {
				return obj[key];
			}
			throw std::logic_error("json object not find key");
		}
		else {
			throw std::logic_error("json value isn't object");
		}
	}
};

inline std::string JsonValue::toString() const
{
	std::ostringstream oss;
	oss << std::boolalpha;
	switch (inner_.index())
	{
		case NullIdx:
			oss << "null"; break;
		case BoolIdx:
			oss << get<bool>(); break;
		case IntIdx:
			oss << get<int>(); break;
		case DoubleIdx:
			oss << get<double>(); break;
		case StringIdx:
			oss << '"' << get<std::string>() << '"';
			break;
		case ArrayIdx:
		{
			const auto& arr = get<JsonArray>();
			oss << '[';
			for (auto it = arr.begin(); it != arr.end(); ++it) {
				if (it != arr.end() - 1) {
					oss << it->toString() << ", ";
				}
				else {
					oss << it->toString();
				}
			}
			oss << ']';
			break;
		}

		case ObjectIdx:
		{
			const auto& obj = get<JsonObject>();
			oss << '{';
			for (auto it = obj.begin(); it != obj.end(); ++it) {
				if (it != obj.begin()) oss << ',';
				oss << '"' << it->first << "\": " << it->second.toString();
			}
			oss << '}';
			break;
		}

		default:
			break;
	}
	return oss.str();
}

class Parser {
public:
	using string_type = std::string_view;

	static JsonValue FromString(std::string_view sv) {
		static Parser instance;
		instance.Init(sv);
		return instance.parse();
	}

	void Init(std::string_view sv) {
		str_ = sv;
		idx_ = 0;
		iter_ = str_.begin();
	}

	JsonValue parse();

private:
	void parseWhitespace();
	JsonValue parseValue();
	JsonValue parseNull();
	JsonValue parseBool();
	JsonValue parseNumber();
	JsonValue parseArray();
	JsonValue parseString();
	JsonValue parseObject();

private:
	string_type str_;
	[[maybe_unused]] string_type::iterator iter_;
	string_type::size_type idx_{};
};

inline JsonValue Parser::parse() {
	// 空字符串直接返回
	if (str_.empty()) {
		return {};
	}

	parseWhitespace();
	return parseValue();
}

inline JsonValue Parser::parseValue()
{
	switch (str_[idx_])
	{
		case 'n':
			return parseNull();
		case 't':
		case 'f':
			return parseBool();
		case '"':
			return parseString();
		case '[':
			return parseArray();
		case '{':
			return parseObject();
		default:
		{
			char ch = str_[idx_];
			if (ch >= '0' && ch <= '9' || ch == '+' || ch == '-')
				return parseNumber();
			else {
				throw std::logic_error("error occurs in json format");
			}
		}

	}
}

// 跳过空白字符
inline void Parser::parseWhitespace()
{
	while (std::isspace(str_[idx_])) idx_++;
}

// 解析 null
inline JsonValue Parser::parseNull() {
	assert(str_[idx_] == 'n');

	if (str_.compare(idx_, 4, "null") == 0) {
		idx_ += 4;
		return {};
	}
	throw std::logic_error("parse null error");
}

inline JsonValue Parser::parseBool()
{
	assert(str_[idx_] == 't' || str_[idx_] == 'f');

	if (str_.compare(idx_, 4, "true") == 0) {
		idx_ += 4;
		return { true };
	}
	if (str_.compare(idx_, 5, "false") == 0) {
		idx_ += 5;
		return { false };
	}
	throw std::logic_error("parse bool error");
}

// 尝试解析从s中解析出数字
template<typename T>
std::optional<T> tryParseNumber(std::string_view s) {
	T value{};
	auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
	if (ec == std::errc() && ptr == s.data() + s.size()) {
		return value;
	}
	return {};
}

// 解析数字
inline JsonValue Parser::parseNumber()
{
	assert((str_[idx_] >= '0' && str_[idx_] <= '9') || str_[idx_] == '+' || str_[idx_] == '-');

	std::regex num_re {"[+-]?[0-9]+(\\.[0-9]*)?([eE][+-]?[0-9]+)?"};
	std::cmatch match;
	auto s = str_.substr(idx_);
	if (std::regex_search(s.data(), s.data() + s.size(), match, num_re)) {
		auto str = match.str();
		idx_ += str.size();

		// 尝试解析为int或double
		if (auto num = tryParseNumber<int>(str)) {
			return { *num };
		}
		if (auto num = tryParseNumber<double>(str)) {
			return { *num };
		}
	}

	throw std::logic_error("parse number error");
}

char unescapeChar(char ch) {
	switch (ch) {
		case 'n': return '\n';
		case 'r': return '\r';
		case '0': return '\0';
		case 't': return '\t';
		case 'v': return '\v';
		case 'f': return '\f';
		case 'b': return '\b';
		case 'a': return '\a';
		default: return ch;
	}
}

// 只解析'"'开头的字符串
inline JsonValue Parser::parseString()
{
	assert(str_[idx_] == '"');
	enum {
		Raw, Escaped
	} phase = Raw;

	std::string s;
	for (++idx_; idx_ < str_.size(); ++idx_) {
		char ch = str_[idx_];
		if (phase == Raw) {
			if (ch == '\\') {
				phase = Escaped;
			}
			else if (ch == '"') {
				++idx_;
				break;
			}
			else {
				s += ch;
			}
		}
		else {
			s += unescapeChar(ch);
			phase = Raw;
		}
	}
	return { std::move(s) };
}

inline JsonValue Parser::parseArray() 
{
	assert(str_[idx_] == '[');
	JsonArray res;
	++idx_;
	parseWhitespace();
	if (str_[idx_] == ']') {
		++idx_;
		return { std::move(res) };
	}

	while (true) {
		res.emplace_back(parseValue());
		parseWhitespace();
		char ch = str_[idx_];
		if (ch == ']') {
			++idx_;
			break;
		}

		if (ch != ',') {
			throw std::logic_error("expect ',' in parse array");
		}
		++idx_;	// 跳过','
		parseWhitespace();
	}
	return { std::move(res) };
}

inline JsonValue Parser::parseObject()
{
	assert(str_[idx_] == '{');
	
	JsonObject res;
	++idx_;
	parseWhitespace();
	// {}
	if (str_[idx_] == '}') {
		++idx_;
		return { std::move(res) };
	}

	while (true) {
		auto&& key = parseValue();
		if (!key.is<std::string>()) {
			throw std::logic_error("expect string as key in parse object");
		}
		parseWhitespace();
		if (str_[idx_] != ':') {
			throw std::logic_error("expect ':' in parse object");
		}
		++idx_;		// 跳过':'
		parseWhitespace();
		auto&& value = parseValue();
		res.try_emplace(key.get<std::string>(), std::move(value));

		parseWhitespace();
		char ch = str_[idx_];
		++idx_;		// 跳过','或'}'
		if (ch == '}') {
			break;
		}
		
		if (ch != ',') {
			throw std::logic_error("expect ',' in parse object");
		}
		parseWhitespace();
	}
	return { std::move(res) };
}
