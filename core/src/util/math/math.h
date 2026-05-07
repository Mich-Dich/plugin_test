#pragma once

#include <glm/glm.hpp>
#include <concepts>

// FORWARD DECLARATIONS ================================================================================================


namespace GLT::math {


	// CONSTANTS =======================================================================================================

	// MACROS ==========================================================================================================

	// TYPES ===========================================================================================================

    // Define concepts for the required operations
    template<typename T>
    concept less_than_comparable = requires(T a, T b) { { a < b } -> std::convertible_to<bool>; };


    template<typename T>
    concept greater_than_or_equal_comparable = requires(T a, T b) { { a >= b } -> std::convertible_to<bool>; };


    template<typename T>
    concept value_size_comparable = less_than_comparable<T> && greater_than_or_equal_comparable<T>;


    template<typename T>
    concept addable = requires(T a, T b) { { a + b } -> std::convertible_to<T>; };


    template<typename T>
    concept subtractable = requires(T a, T b) { { a - b } -> std::convertible_to<T>; };

	// STATIC VARIABLES ================================================================================================

	// FUNCTION DECLARATION ============================================================================================

    bool is_valid_vec3(const glm::vec3& vec);


	bool decompose_transform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);


	bool compose_transform(glm::mat4& transform, const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);


    f32 calc_array_average(const f32* array, u32 size);


    f32 calc_array_max(const f32* array, u32 size);


    // @brief Returns the smaller of two values
    // @tparam T Type that supports less-than comparison
    // @param left First value to compare
    // @param right Second value to compare
    // @return The smaller of the two input values
    template<less_than_comparable T>
    static FORCE_INLINE T min(const T left, const T right) { return left < right ? left : right; }


    // @brief Returns the larger of two values
    // @tparam T Type that supports greater-than-or-equal comparison
    // @param left First value to compare
    // @param right Second value to compare
    // @return The larger of the two input values
    template<greater_than_or_equal_comparable T>
    static FORCE_INLINE T max(const T left, const T right) { return left >= right ? left : right; }


    // @brief Constrains a value between minimum and maximum bounds
    // @tparam T Type that supports size comparison operations
    // @param value Input value to clamp
    // @param min Lower bound for clamping
    // @param max Upper bound for clamping
    // @return Value constrained to the range [min, max]
    template<value_size_comparable T>
    static FORCE_INLINE T clamp(const T value, const T min, const T max) { return (value < min) ? min : (value > max) ? max : value; }


    // @brief Performs linear interpolation between two values
    // @tparam T Type of values to interpolate between (floating point or integer)
    // @param a Starting value
    // @param b Ending value
    // @param time Interpolation factor (0.0 returns a, 1.0 returns b)
    // @return Interpolated value between a and b based on time
    template<typename T>
    static FORCE_INLINE T lerp(const T a, const T b, const float time);


    // @brief Exchanges the values of two variables
    // @tparam T Type of variables to swap
    // @param a First variable to swap
    // @param b Second variable to swap
    template<typename T>
    static FORCE_INLINE void swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }


    // Absolute value function
    template<less_than_comparable T>
    static FORCE_INLINE T abs(const T value) { return value < 0 ? -value : value; }


    // @brief Combines hash values with improved avalanche properties
    // @tparam T The type of the value to hash.
    // @tparam Rest Additional types to hash.
    // @param seed The seed value for the hash.
    // @param v The value to hash.
    // @param rest Additional values to hash.
    template <typename T, typename... Rest>
    constexpr void hash_combine(std::size_t& seed, const T& v, const Rest&... rest);


    // Base case for single argument
    template <typename T>
    constexpr void hash_combine(std::size_t& seed, const T& v);

    // ===================================================================================
    // Vector
    // ===================================================================================

    glm::vec3 get_forward_vector(const glm::vec3 direction);


    glm::vec3 get_right_vector(const glm::vec3 direction);


    glm::vec3 get_up_vector(const glm::vec3 direction);

	// TEMPLATE DECLARATION ============================================================================================

	// CLASS DECLARATION ===============================================================================================

}

#include "math.inl"
