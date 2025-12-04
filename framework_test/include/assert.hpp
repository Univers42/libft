/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assert.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 01:44:51 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 01:44:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTING_ASSERT_HPP
#define TESTING_ASSERT_HPP
#pragma once

#include <string>
#include "message.hpp"

namespace testing
{

    class AssertionResult
    {
    public:
        AssertionResult(bool success = true);
        AssertionResult(const AssertionResult &other);
        ~AssertionResult();

        AssertionResult &operator=(AssertionResult other);
        void swap(AssertionResult &other);

        // logical negation: returns a result with inverted success flag, preserving message
        AssertionResult operator!() const;

        // append text or Message
        AssertionResult &operator<<(const std::string &s);
        AssertionResult &operator<<(const tester::Message &m);

        // accessors
        bool success() const;
        std::string message() const;

    private:
        bool success_;
        std::string *message_;
    };

    AssertionResult AssertionSuccess();
    AssertionResult AssertionFailure();
    AssertionResult AssertionFailure(const tester::Message &message);

} // namespace testing

#endif // TESTING_ASSERT_HPP
