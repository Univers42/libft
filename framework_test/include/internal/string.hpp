/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 13:41:57 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/03 14:19:03 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_HPP
#define STRING_HPP

#include <cstdint>
#include <string>
#include <sstream>
#include <cwchar>
#include <cstring>
#include "tester.hpp"

namespace testing
{
	namespace internal
	{

		class TESTER_API_ String
		{
		public:
			// Clone a 0-terminated C string using new[]. Caller must delete[] the result.
			// Returns nullptr if input is nullptr.
			static const char *CloneCString(const char *c_str);

			// Compare two C strings (handles nullptr). nullptr != any non-null string.
			static bool CStringEquals(const char *lhs, const char *rhs);

			// Convert a wide C string to a UTF-8 std::string.
			// nullptr => "(null)". On conversion failure returns "(failed to convert from wide string)".
			static std::string ShowWideCString(const wchar_t *wide_c_str);

			// Compare two wide C strings (handles nullptr).
			static bool WideCStringEquals(const wchar_t *lhs, const wchar_t *rhs);

			// Compare two C strings, ignoring case (handles nullptr).
			static bool CaseInsensitiveCStringEquals(const char *lhs, const char *rhs);

			// Compare two wide C strings, ignoring case (handles nullptr).
			static bool CaseInsensitiveWideCStringEquals(const wchar_t *lhs, const wchar_t *rhs);

			// Return true if str ends with suffix ignoring case.
			static bool EndsWithCaseInsensitive(const std::string &str,
												const std::string &suffix);

			// Format utilities
			static std::string FormatIntWidth2(int val);
			static std::string FormatIntWidthN(int val, int width);
			static std::string FormatHexInt(int val);
			static std::string FormatHexUint32(uint32_t val);
			static std::string FormatByte(unsigned char value);

		private:
			String(); // prevent instantiation
		};

		// Convert a stringstream to std::string (exported)
		TESTER_API_ std::string StringStreamToString(::std::stringstream *stream);
	}
}

#endif