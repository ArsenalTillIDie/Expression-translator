#include "uttranslator.h"

#include <gtest.h>

TEST(Translator, detects_illegal_characters) {
    std::string exprStr = "1+1~";
    Expression expr(exprStr);
    ASSERT_ANY_THROW(expr.solve());
}

TEST(Translator, too_many_opening_brackets) {
    std::string exprStr = "1+(1+2+(4-3)";
    Expression expr(exprStr);
    ASSERT_ANY_THROW(expr.solve());
}

TEST(Translator, too_many_closing_brackets) {
    std::string exprStr = "1+(1+2)+4-3)";
    Expression expr(exprStr);
    ASSERT_ANY_THROW(expr.solve());
}

TEST(Translator, can_add) {
    std::string exprStr = "1+1";
    Expression expr(exprStr);
    double res = expr.solve();
    std::cout << res;
    ASSERT_TRUE(res > 1.9 && res < 2.1);
}

TEST(Translator, can_subtract) {
    std::string exprStr = "3-1";
    Expression expr(exprStr);
    double res = expr.solve();
    std::cout << res;
    ASSERT_TRUE(res > 1.9 && res < 2.1);
}

TEST(Translator, can_multiply) {
    std::string exprStr = "1*2";
    Expression expr(exprStr);
    double res = expr.solve();
    std::cout << res;
    ASSERT_TRUE(res > 1.9 && res < 2.1);
}

TEST(Translator, can_divide) {
    std::string exprStr = "10/5";
    Expression expr(exprStr);
    double res = expr.solve();
    std::cout << res;
    ASSERT_TRUE(res > 1.9 && res < 2.1);
}

TEST(Translator, cannot_divide_by_zero) {
    std::string exprStr = "1/0";
    Expression expr(exprStr);
    ASSERT_ANY_THROW(expr.solve());
}

TEST(Translator, correct_order_of_operations) {
    std::string exprStr = "2+2*2";
    Expression expr(exprStr);
    double res = expr.solve();
    std::cout << res;
    ASSERT_TRUE(res > 5.9 && res < 6.1);
}

TEST(Translator, brackets_work) {
    std::string exprStr = "(2+2)*2";
    Expression expr(exprStr);
    double res = expr.solve();
    std::cout << res;
    ASSERT_TRUE(res > 7.9 && res < 8.1);
}

TEST(Translator, invalid_term_sequence) {
    std::string exprStr = "1++3";
    Expression expr(exprStr);
    ASSERT_ANY_THROW(expr.solve());
}