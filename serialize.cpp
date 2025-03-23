#include <iostream>
#include <tuple>
#include <string>
#include <type_traits>
#include <vector>
#include <sstream>
#include <optional>
#include <iomanip>

// A helper to serialize various types to string
template<typename T>
std::string serialize(const T& value) {
    if constexpr (std::is_same_v<T, int>) {
        return std::to_string(value);
    } else if constexpr (std::is_same_v<T, double>) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << value; // Set precision if needed
        return oss.str();
    } else if constexpr (std::is_same_v<T, std::string>) {
        return value; // No need to escape for simplicity
    } else {
        static_assert(!std::is_same_v<T, T>, "Unsupported type");
    }
}

// A helper to deserialize string to various types
template<typename T>
std::optional<T> deserialize(const std::string& str) {
    if constexpr (std::is_same_v<T, int>) {
        try {
            return std::stoi(str);
        } catch (...) {
            return std::nullopt; // Return nullopt on error
        }
    } else if constexpr (std::is_same_v<T, double>) {
        try {
            return std::stod(str);
        } catch (...) {
            return std::nullopt; // Return nullopt on error
        }
    } else if constexpr (std::is_same_v<T, std::string>) {
        return str; // No need to unescape for simplicity
    } else {
        static_assert(!std::is_same_v<T, T>, "Unsupported type");
    }
}

// A struct to demonstrate serialization/deserialization
struct Person {
    std::string name;
    int age;
    double height;

    // Define a method to serialize the struct
    std::string serialize() const {
        return serialize(name) + "," + serialize(age) + "," + serialize(height);
    }

    // Define a static method to deserialize the struct
    static std::optional<Person> deserialize(const std::string& str) {
        std::istringstream ss(str);
        std::string token;
        Person person;

        if (std::getline(ss, token, ',') && !(person.name = deserialize<std::string>(token)).has_value()) {
            return std::nullopt;
        }

        if (std::getline(ss, token, ',') && !(person.age = deserialize<int>(token)).has_value()) {
            return std::nullopt;
        }

        if (std::getline(ss, token, ',') && !(person.height = deserialize<double>(token)).has_value()) {
            return std::nullopt;
        }

        return person; // Return the person object if all fields are correctly deserialized
    }
};

int main() {
    Person person{"Alice", 30, 5.5};

    // Serialize the person
    std::string serialized = person.serialize();
    std::cout << "Serialized: " << serialized << std::endl;

    // Deserialize the person
    auto deserialized = Person::deserialize(serialized);
    if (deserialized) {
        std::cout << "Deserialized: Name: " << deserialized->name
                  << ", Age: " << deserialized->age
                  << ", Height: " << deserialized->height << std::endl;
    } else {
        std::cout << "Deserialization failed!" << std::endl;
    }

    return 0;
}
