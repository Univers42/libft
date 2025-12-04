/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_filepath.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 00:30:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 01:24:12 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <errno.h>
#include <iostream>

#include "../include/filepath.hpp"

namespace testing
{
	namespace internal
	{
		const char kPathSeparator = '/';
		const char kCurrentDirectoryString[] = "./";

		static inline bool IsPathSeparator(char c)
		{
			return (c == kPathSeparator);
		}

		static std::string StreamableToString(int n)
		{
			std::ostringstream oss;
			oss << n;
			return oss.str();
		}

		static bool EndsWithCaseInsensitive(const std::string &s, const std::string &suffix)
		{
			if (suffix.size() > s.size())
				return false;
			return std::equal(suffix.rbegin(), suffix.rend(), s.rbegin(),
							  [](char a, char b)
							  { return std::tolower((unsigned char)a) == std::tolower((unsigned char)b); });
		}

		FilePath FilePath::GetCurrentDir()
		{
			char cwd[PATH_MAX + 1] = {'\0'};
			if (getcwd(cwd, sizeof(cwd)) == nullptr)
				return FilePath(std::string());
			// return current dir without forcing trailing separator here
			return FilePath(std::string(cwd));
		}
		FilePath FilePath::RemoveExtension(const char *extension) const
		{
			if (!extension || extension[0] == '\0')
				return *this;
			std::string dot_extension = std::string(".") + extension;
			if (EndsWithCaseInsensitive(pathname_, dot_extension))
			{
				return FilePath(pathname_.substr(0, pathname_.length() - dot_extension.length()));
			}
			return *this;
		}
		const char *FilePath::FindLastPathSeparator() const
		{
			return strrchr(c_str(), kPathSeparator);
		}
		size_t FilePath::CalculateRootLength() const
		{
			// For POSIX: root is "/" if path starts with '/'
			if (!pathname_.empty() && IsPathSeparator(pathname_[0]))
				return 1;
			return 0;
		}
		FilePath FilePath::RemoveDirectoryName() const
		{
			const char *const last_sep = FindLastPathSeparator();
			return (last_sep ? FilePath(std::string(last_sep + 1)) : *this);
		}
		FilePath FilePath::RemoveFileName() const
		{
			const char *const last_sep = FindLastPathSeparator();

			std::string dir;
			if (last_sep)
				dir = std::string(c_str(), static_cast<size_t>(last_sep + 1 - c_str()));
			else
				dir = kCurrentDirectoryString;
			return FilePath(dir);
		}
		FilePath FilePath::MakeFilename(const FilePath &directory,
										const FilePath &base_name, int number,
										const char *extension)
		{
			std::string file;
			if (extension == nullptr)
				extension = "";

			if (number == 0)
				file = base_name.string() + (extension[0] ? std::string(".") + extension : std::string());
			else
				file = base_name.string() + "_" + StreamableToString(number) + (extension[0] ? std::string(".") + extension : std::string());
			return ConcatPaths(directory, FilePath(file));
		}
		FilePath FilePath::ConcatPaths(const FilePath &directory,
									   const FilePath &relative_path)
		{
			const FilePath dir(directory.RemoveTrailingPathSeparator());
			if (directory.IsEmpty())
				return relative_path;
			if (dir.IsEmpty())
				return relative_path;
			// avoid duplicate separator
			std::string combined = dir.string();
			if (!IsPathSeparator(combined.back()))
				combined.push_back(kPathSeparator);
			combined += relative_path.string();
			return FilePath(combined);
		}
		bool FilePath::FileOrDirectoryExists() const
		{
			struct stat file_stat{};
			return ::stat(pathname_.c_str(), &file_stat) == 0;
		}
		bool FilePath::DirectoryExists() const
		{
			struct stat file_stat{};
			if (::stat(pathname_.c_str(), &file_stat) != 0)
				return false;
			return S_ISDIR(file_stat.st_mode);
		}
		bool FilePath::IsRootDirectory() const
		{
			size_t root_length = CalculateRootLength();
			return (root_length > 0 && root_length == pathname_.size() &&
					IsPathSeparator(pathname_[root_length - 1]));
		}
		bool FilePath::IsAbsolutePath() const
		{
			return (CalculateRootLength() > 0);
		}
		FilePath FilePath::GenerateUniqueFileName(const FilePath &directory,
												  const FilePath &base_name,
												  const char *extension)
		{
			FilePath full_pathname;
			int number = 0;
			while (true)
			{
				FilePath candidate = MakeFilename(directory, base_name, number++, extension);
				if (!candidate.FileOrDirectoryExists())
					return candidate;
				// protect against infinite loop (practically unlikely)
				if (number > 1000000)
					return candidate;
			}
		}
		bool FilePath::IsDirectory() const
		{
			return (!pathname_.empty() && IsPathSeparator(pathname_.c_str()[pathname_.length() - 1]));
		}
		bool FilePath::CreateDirectoriesRecursively() const
		{
			// If empty or already exists as directory -> ok
			if (pathname_.empty() || DirectoryExists())
				return true;

			// compute parent
			FilePath parent = RemoveTrailingPathSeparator().RemoveFileName();
			if (parent.string() == pathname_) // safety
				return false;
			if (!parent.CreateDirectoriesRecursively())
				return false;
			return this->CreateFolder();
		}
		bool FilePath::CreateFolder() const
		{
			if (pathname_.empty())
				return false;
#if defined(_WIN32)
			int result = _mkdir(pathname_.c_str());
#else
			int result = mkdir(pathname_.c_str(), 0777);
#endif
			if (result == 0)
				return true;
			if (errno == EEXIST)
				return DirectoryExists();
			return false;
		}
		FilePath FilePath::RemoveTrailingPathSeparator() const
		{
			if (IsDirectory() && pathname_.length() > 1)
				return FilePath(pathname_.substr(0, pathname_.length() - 1));
			return *this;
		}
		void FilePath::Normalize()
		{
			// collapse multiple separators, remove trailing separators (except root)
			std::string out;
			out.reserve(pathname_.size());
			for (size_t i = 0; i < pathname_.size(); ++i)
			{
				char c = pathname_[i];
				if (IsPathSeparator(c))
				{
					if (out.empty() || !IsPathSeparator(out.back()))
						out.push_back(kPathSeparator);
				}
				else
					out.push_back(c);
			}
			// remove trailing separator unless root "/"
			if (out.size() > 1 && IsPathSeparator(out.back()))
				out.pop_back();
			pathname_.swap(out);
		}

	} // namespace internal
} // namespace testing
