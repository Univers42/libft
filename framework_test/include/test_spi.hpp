/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_spi.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 01:28:20 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 01:45:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_SPI_HPP
#define TEST_SPI_HPP

#include <string>
#include "tester.hpp"

GTEST_DISABLE_MSC_WARNINGS_PUSH_(4251)

namespace testing
{
	class TESTER_API_ ScopedFakeTestPartResultReporter
		: public TestPartResultReporterInterface
	{
	public:
		enum InterceptMode
		{
			INTERCEPT_ONLY_CURRENT_THREAD,
			INTERCEPT_ALL_THREADS
		};

		explicit ScopedFakeTestPartResultReporter(TestPartResultArray *result);
		ScopedFakeTestPartResultReporter(InterceptMode intercept_mode,
										 TestPartResultArray *result);
		~ScopedFakeTestPartResultReporter() override;
		void ReportTestPartResult(const TestPartResult &result) override;

	private:
		void Init();
		const InterceptMode intercept_mode_;
		TestPartResultReporterInterface *old_reporter;
		TestPartResultArray *const result_;

		ScopedFakeTestPartResultReporter(
			const ScopedFakeTestPartResultReporter &) = delete;
		ScopedFakeTestPartResultReporter &operator=(
			const ScopedFakeTestPartResultReporter &) = delete;
	};

	namespace internal
	{
		class TESTER_API_ SingleFailureChecker
		{
		public:
			SingleFailureChecker(const TestPartResultArray *results,
								 TestPartResult::Type type,
								 const std::string &substr);
			~SingleFailureChecker();

		private:
			const TestPartResultArray *const results_;
			const TestPartResult::Type type_;
			const std::string substr_;

			SingleFailureChecker(const SingleFailureChecker &) = delete;
			SingleFailureChecker &operator=(const SingleFailureChecker &) = delete;
		};
	}
}

GTEST_DISABLE_MSC_WARNINGS_POP_()

#define EXPECT_FATAL_FAILURE(statement, substr)                                             \
	do                                                                                      \
	{                                                                                       \
		class GTestExpectFatalFailureHelper                                                 \
		{                                                                                   \
		public:                                                                             \
			static void Execute() { statement; }                                            \
		};                                                                                  \
		::testing::TestPartResultArray gtest_failures;                                      \
		::testing::internal::SingleFailureChecker gtest_checker(                            \
			&gtest_failures, ::testing::TestPartResult::kFatalFailure, (substr));           \
		{                                                                                   \
			::testing::ScopedFakeTestPartResultReporter gtest_reporter(                     \
				::testing::ScopedFakeTestPartResultReporter::INTERCEPT_ONLY_CURRENT_THREAD, \
				&gtest_failures);                                                           \
			GTestExpectFatalFailureHelper::Execute();                                       \
		}                                                                                   \
	} while (::testing::internal::AlwaysFalse())

#define EXPECT_FATAL_FAILURE_ON_ALL_THREADS(statement, substr)                      \
	do                                                                              \
	{                                                                               \
		class GTestExpectFatalFailureHelper                                         \
		{                                                                           \
		public:                                                                     \
			static void Execute() { statement; }                                    \
		};                                                                          \
		::testing::TestPartResultArray gtest_failures;                              \
		::testing::internal::SingleFailureChecker gtest_checker(                    \
			&gtest_failures, ::testing::TestPartResult::kFatalFailure, (substr));   \
		{                                                                           \
			::testing::ScopedFakeTestPartResultReporter gtest_reporter(             \
				::testing::ScopedFakeTestPartResultReporter::INTERCEPT_ALL_THREADS, \
				&gtest_failures);                                                   \
			GTestExpectFatalFailureHelper::Execute();                               \
		}                                                                           \
	} while (::testing::internal::AlwaysFalse())

#define EXPECT_NONFATAL_FAILURE(statement, substr)                                          \
	do                                                                                      \
	{                                                                                       \
		::testing::TestPartResultArray gtest_failures;                                      \
		::testing::internal::SingleFailureChecker gtest_checker(                            \
			&gtest_failures, ::testing::TestPartResult::kNonFatalFailure, (substr));        \
		{                                                                                   \
			::testing::ScopedFakeTestPartResultReporter gtest_reporter(                     \
				::testing::ScopedFakeTestPartResultReporter::INTERCEPT_ONLY_CURRENT_THREAD, \
				&gtest_failures);                                                           \
			if (::testing::internal::AlwaysTrue())                                          \
				statement;                                                                  \
		}                                                                                   \
	} while (::testing::internal::AlwaysFalse())

#define EXPECT_NONFATAL_FAILURE_ON_ALL_THREADS(statement, substr)                    \
	do                                                                               \
	{                                                                                \
		::testing::TestPartResultArray gtest_failures;                               \
		::testing::internal::SingleFailureChecker gtest_checker(                     \
			&gtest_failures, ::testing::TestPartResult::kNonFatalFailure, (substr)); \
		{                                                                            \
			::testing::ScopedFakeTestPartResultReporter gtest_reporter(              \
				::testing::ScopedFakeTestPartResultReporter::INTERCEPT_ALL_THREADS,  \
				&gtest_failures);                                                    \
			if (::testing::internal::AlwaysTrue())                                   \
			{                                                                        \
				statement;                                                           \
			}                                                                        \
		}                                                                            \
	} while (::testing::internal::AlwaysFalse())

#endif /* TEST_SPI_HPP */