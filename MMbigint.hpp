#include <iostream>
#include <string>
#include <vector>

namespace MMbigint {


class UnsignedBigint {
private:
    std::vector<int> digits;
    const size_t digit_length = 9;

    void remove_leading_zeros() {
        size_t pos = digits.size() - 1;
        while (pos != 0 && digits[pos] == 0) {
            pos--;
        }
        digits.resize(pos + 1);
    }

    [[nodiscard]] int compare(const UnsignedBigint &other) const {
        if (digits.size() != other.digits.size()) {
            return static_cast<int>(digits.size()) -
                   static_cast<int>(other.digits.size());
        }

        size_t pos = digits.size() - 1;
        while (digits[pos] == other.digits[pos] && pos != 0) {
            --pos;
        }

        return digits[pos] - other.digits[pos];
    }

public:
    UnsignedBigint() {
        digits.push_back(0);
    }

    UnsignedBigint(const uint32_t number)
        : UnsignedBigint(std::to_string(number)) {
    }

    explicit UnsignedBigint(const std::string &number_str) {
        size_t pos = number_str.size();
        while (pos >= digit_length) {
            pos -= digit_length;
            digits.push_back(std::stoi(number_str.substr(pos, digit_length)));
        }
        if (pos != 0) {
            digits.push_back(std::stoi(number_str.substr(0, pos)));
        }
        remove_leading_zeros();
    }

    [[nodiscard]] std::string to_string() const {
        std::string number_str;
        std::string number_part;
        std::string leading_zeros;
        for (size_t pos = digits.size(); pos > 0; --pos) {
            number_part = std::to_string(digits[pos - 1]);
            if (pos != digits.size()) {
                leading_zeros =
                    std::string(digit_length - number_part.size(), '0');
            }
            number_str += leading_zeros + number_part;
        }
        return number_str;
    }

    explicit operator unsigned int() const {
        unsigned int number{};
        unsigned int base_power = 1;
        for (const int digit : digits) {
            number += base_power * digit;
            base_power *= BASE;
        }
        return number;
    }

    UnsignedBigint &operator+=(const UnsignedBigint &other) {
        digits.resize(std::max(digits.size(), other.digits.size()) + 1, 0);
        for (size_t pos = 0; pos < other.digits.size(); ++pos) {
            digits[pos] += other.digits[pos];
        }
        for (size_t pos = 1; pos < digits.size(); ++pos) {
            if (digits[pos - 1] >= BASE) {
                digits[pos - 1] -= BASE;
                ++digits[pos];
            }
        }
        remove_leading_zeros();
        return *this;
    }

    UnsignedBigint &operator++() {
        return *this += 1;
    }

    UnsignedBigint operator++(int) {
        UnsignedBigint temp = *this;
        ++*this;
        return temp;
    }

    UnsignedBigint &operator-=(const UnsignedBigint &other) {
        for (size_t pos = 0; pos < other.digits.size(); ++pos) {
            digits[pos] -= other.digits[pos];
        }
        for (size_t pos = 1; pos < digits.size(); ++pos) {
            if (digits[pos - 1] < 0) {
                digits[pos - 1] += BASE;
                digits[pos]--;
            }
        }
        remove_leading_zeros();
        return *this;
    }

    UnsignedBigint &operator--() {
        return *this -= 1;
    }

    UnsignedBigint operator--(int) {
        UnsignedBigint temp = *this;
        --*this;
        return temp;
    }

    friend bool operator==(const UnsignedBigint &, const UnsignedBigint &);
    friend bool operator!=(const UnsignedBigint &, const UnsignedBigint &);
    friend bool operator<(const UnsignedBigint &, const UnsignedBigint &);
    friend bool operator>(const UnsignedBigint &, const UnsignedBigint &);
    friend bool operator<=(const UnsignedBigint &, const UnsignedBigint &);
    friend bool operator>=(const UnsignedBigint &, const UnsignedBigint &);
};

bool operator==(const UnsignedBigint &lhs, const UnsignedBigint &rhs) {
    return lhs.compare(rhs) == 0;
}

bool operator!=(const UnsignedBigint &lhs, const UnsignedBigint &rhs) {
    return lhs.compare(rhs) != 0;
}

bool operator<(const UnsignedBigint &lhs, const UnsignedBigint &rhs) {
    return lhs.compare(rhs) < 0;
}

bool operator>(const UnsignedBigint &lhs, const UnsignedBigint &rhs) {
    return lhs.compare(rhs) > 0;
}

bool operator<=(const UnsignedBigint &lhs, const UnsignedBigint &rhs) {
    return lhs.compare(rhs) <= 0;
}

bool operator>=(const UnsignedBigint &lhs, const UnsignedBigint &rhs) {
    return lhs.compare(rhs) >= 0;
}

UnsignedBigint operator+(UnsignedBigint lhs, const UnsignedBigint &rhs) {
    return lhs += rhs;
}

UnsignedBigint operator-(UnsignedBigint lhs, const UnsignedBigint &rhs) {
    return lhs -= rhs;
}

std::istream &operator>>(std::istream &in, UnsignedBigint &number) {
    std::string number_str;
    in >> number_str;
    number = UnsignedBigint(number_str);
    return in;
}

std::ostream &operator<<(std::ostream &out, const UnsignedBigint &number) {
    return out << number.to_string();
}

}  // namespace MMbigint