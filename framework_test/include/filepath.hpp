/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filepath.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 00:56:34 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 01:24:12 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEPATH_HPP
#define FILEPATH_HPP

#include <string>
#include <utility>
#include <cstddef>
#include <limits.h>

#ifndef TEST_API
#define TEST_API
#endif

namespace testing
{
	namespace internal
	{
		class TEST_API FilePath
		{
		public:
			FilePath() : pathname_("") {}
			FilePath(const FilePath &rhs) : pathname_(rhs.pathname_) {}
			FilePath(FilePath &&rhs) noexcept : pathname_(std::move(rhs.pathname_)) {}
			explicit FilePath(std::string pathname) : pathname_(std::move(pathname))
			{
				Normalize();
			}
			FilePath &operator=(const FilePath &rhs)
			{
				Set(rhs);
				return (*this);
			}
			FilePath &operator=(FilePath &&rhs) noexcept
			{
				pathname_ = std::move(rhs.pathname_);
				return (*this);
			}
			void Set(const FilePath &rhs)
			{
				pathname_ = rhs.pathname_;
			}
			const std::string &string() const
			{
				return pathname_;
			}
			const char *c_str() const
			{
				return pathname_.c_str();
			}
			static FilePath GetCurrentDir();
			static FilePath MakeFilename(const FilePath &directory,
										 const FilePath &base_name, int number,
										 const char *extension);
			static FilePath ConcatPaths(const FilePath &directory,
										const FilePath &relative_path);
			static FilePath GenerateUniqueFileName(const FilePath &directory,
												   const FilePath &base_name,
												   const char *extension);
			bool IsEmpty() const { return pathname_.empty(); }
			FilePath RemoveTrailingPathSeparator() const;
			FilePath RemoveDirectoryName() const;
			FilePath RemoveFileName() const;
			FilePath RemoveExtension(const char *extension) const;
			bool CreateDirectoriesRecursively() const;
			bool CreateFolder() const;
			bool FileOrDirectoryExists() const;
			bool DirectoryExists() const;
			bool IsDirectory() const;
			bool IsRootDirectory() const;
			bool IsAbsolutePath() const;

		private:
			void Normalize();
			const char *FindLastPathSeparator() const;
			size_t CalculateRootLength() const;
			std::string pathname_;
		};
	}
}
#endif