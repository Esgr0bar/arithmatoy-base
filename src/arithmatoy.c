#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "utils.h"

int VERBOSE = 0;

const char *get_all_digits() { return "0123456789abcdefghijklmnopqrstuvwxyz"; }
const size_t ALL_DIGIT_COUNT = 36;

unsigned int get_digit_value(char digit) {
    if (digit >= '0' && digit <= '9') return digit - '0';
    if (digit >= 'a' && digit <= 'z') return digit - 'a' + 10;
    return -1; // Invalid digit
}

char to_digit(unsigned int value) {
    if (value >= 0 && value <= 9) return '0' + value;
    if (value >= 10 && value <= 35) return 'a' + (value - 10);
    return '?'; // Invalid value
}

char *reverse(char *str) {
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; ++i) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
    return str;
}
const char *drop_leading_zeros(const char *str) {
    while (*str == '0') ++str;
    if (*str == '\0') --str; // Leave one '0' if all were zeros
    return str;
}
void arithmatoy_free(char *number) { free(number); }

/**
    @brief Adds two numbers represented as strings in the specified base.

    @param base Base of the numbers (input).
    @param lhs First number as a string (input).
    @param rhs Second number as a string (input).
    @return Pointer to the result of the addition as a string, or NULL if allocation fails.

    This function performs the addition of two numbers represented as strings in the given base.
    It handles digit-by-digit addition and manages carries. The result is returned as a string.
*/
char *arithmatoy_add(unsigned int base, const char *lhs, const char *rhs) {
    if (VERBOSE) {
        fprintf(stderr, "add: entering function\n");
    }

    size_t lhs_len = strlen(lhs);
    size_t rhs_len = strlen(rhs);
    size_t max_len = lhs_len > rhs_len ? lhs_len : rhs_len;
    
    char *result = (char *)malloc(max_len + 2); // +1 for possible carry and +1 for '\0'
    if (result == NULL) {
        debug_abort("Memory allocation failed in arithmatoy_add");
    }

    int carry = 0;
    size_t index = 0;
    lhs_len--;
    rhs_len--;

    while (lhs_len < SIZE_MAX || rhs_len < SIZE_MAX || carry) {
        int lhs_digit = lhs_len < SIZE_MAX ? get_digit_value(lhs[lhs_len--]) : 0;
        int rhs_digit = rhs_len < SIZE_MAX ? get_digit_value(rhs[rhs_len--]) : 0;
        int sum = lhs_digit + rhs_digit + carry;
        carry = sum / base;
        sum %= base;
        result[index++] = to_digit(sum);
    }

    result[index] = '\0';
    reverse(result);    
    return drop_leading_zeros(result);
}

/**

    @brief Subtracts two numbers represented as strings in the specified base.

    @param base Base of the numbers (input).
    @param lhs Minuend as a string (input).
    @param rhs Subtrahend as a string (input).
    @return Pointer to the result of the subtraction as a string, or NULL if the result is negative or allocation fails.

    This function performs the subtraction of two numbers represented as strings in the given base.
    It handles digit-by-digit subtraction and manages borrows. If the result would be negative, it returns NULL.
*/
char *arithmatoy_sub(unsigned int base, const char *lhs, const char *rhs) {
    if (lhs == NULL || rhs == NULL || base >= ALL_DIGIT_COUNT) {
        return NULL;
    }

    lhs = drop_leading_zeros(lhs);
    rhs = drop_leading_zeros(rhs);

    size_t lhs_len = strlen(lhs);
    size_t rhs_len = strlen(rhs);

    if (lhs_len < rhs_len || (lhs_len == rhs_len && strcmp(lhs, rhs) < 0)) {
        return NULL;
    }

    char *result = (char *)calloc(lhs_len + 1, sizeof(char));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed in arithmatoy_sub\n");
        return NULL;
    }

    int borrow = 0;
    int i = lhs_len - 1;
    int j = rhs_len - 1;
    int k = lhs_len - 1;

    while (j >= 0) {
        int diff = get_digit_value(lhs[i]) - borrow - get_digit_value(rhs[j]);
        if (diff < 0) {
            diff += base;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result[k--] = to_digit(diff);
        i--;
        j--;
    }

    while (i >= 0) {
        int diff = get_digit_value(lhs[i]) - borrow;
        if (diff < 0) {
            diff += base;
        } else {
            borrow = 0;
        }
        result[k--] = to_digit(diff);
        i--;
    }

    result[lhs_len] = '\0';

    return drop_leading_zeros(result);
}

/**
    @brief Multiplies two numbers represented as strings in the specified base.

    @param base Base of the numbers (input).
    @param lhs First number as a string (input).
    @param rhs Second number as a string (input).
    @return Pointer to the result of the multiplication as a string, or NULL if allocation fails.
   
    This function performs the multiplication of two numbers represented as strings in the given base.
    It handles digit-by-digit multiplication and summation of partial results. The final result is returned as a string.
*/
char *arithmatoy_mul(unsigned int base, const char *lhs, const char *rhs) {
    if (lhs == NULL || rhs == NULL || base >= ALL_DIGIT_COUNT) {
        return NULL;
    }

    lhs = drop_leading_zeros(lhs);
    rhs = drop_leading_zeros(rhs);

    size_t lhs_len = strlen(lhs);
    size_t rhs_len = strlen(rhs);

    if (lhs_len < rhs_len) {
        const char *tmp = lhs;
        lhs = rhs;
        rhs = tmp;
        size_t tmp_len = lhs_len;
        lhs_len = rhs_len;
        rhs_len = tmp_len;
    }

    size_t result_len = lhs_len + rhs_len;
    char *result = (char *)calloc(result_len + 1, sizeof(char));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed in arithmatoy_mul\n");
        return NULL;
    }

    char *partial_result = (char *)calloc(result_len + 1, sizeof(char));
    if (partial_result == NULL) {
        fprintf(stderr, "Memory allocation failed in arithmatoy_mul\n");
        return NULL;
    }

    unsigned int lhs_digit, rhs_digit, product, carry;
    for (size_t i = 0; i < rhs_len; ++i) {
        memset(partial_result, '0', result_len);

        rhs_digit = get_digit_value(rhs[rhs_len - i - 1]);
        if (rhs_digit >= base) {
            return NULL;
        }

        carry = 0;
        for (size_t j = 0; j < lhs_len; ++j) {
            lhs_digit = get_digit_value(lhs[lhs_len - j - 1]);
            if (lhs_digit >= base) {
                return NULL;
            }

            product = lhs_digit * rhs_digit + get_digit_value(partial_result[result_len - j - i - 1]) + carry;
            carry = product / base;
            partial_result[result_len - j - i - 1] = to_digit(product % base);
        }

        if (carry != 0) {
            partial_result[result_len - lhs_len - i - 1] = to_digit(carry);
        }

        char *tmp = result;
        result = arithmatoy_add(base, result, partial_result);

        if (result == NULL) {
            return NULL;
        }
    }
    char *final_result = drop_leading_zeros(result);
    return final_result;
}


void debug_abort(const char *debug_msg) {
    fprintf(stderr, "%s\n", debug_msg);
    abort();
}