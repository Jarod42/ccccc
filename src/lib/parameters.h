/*
** Copyright 2012-2022 Joris Dauphin
*/
/*
**  This file is part of CCCCC.
**
**  CCCCC is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  CCCCC is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with CCCCC. If not, see <http://www.gnu.org/licenses/>.
*/

#include <filesystem>
#include <string>
#include <vector>

namespace ccccc
{

class Parameters
{
public:
	Parameters() = default;

	void Parse(const std::filesystem::path& cccccRoot, int argc, char** argv);

	void AddFile(const std::filesystem::path& filename) { m_files.push_back(filename); }
	void AddInclude(const std::string& includePath) { m_includePaths.push_back(includePath); }
	void AddDefine(const std::string& define) { m_defines.push_back(define); }
	void SetPch(const std::string& pchFile) { m_pch = pchFile; }
	void AddExtra(const std::string& extra) { m_extras.push_back(extra); }
	void SetTemplateFilename(const std::filesystem::path& templateFilename)
	{
		m_template = templateFilename;
	}

	const std::vector<std::filesystem::path>& Filenames() const { return m_files; }
	const std::vector<std::string>& IncludePaths() const { return m_includePaths; }
	const std::vector<std::string>& Defines() const { return m_defines; }
	const std::vector<std::string>& Extras() const { return m_extras; }
	const std::string& GetPch() const { return m_pch; }
	const std::filesystem::path& GetTemplateFilename() const { return m_template; }

private:
	std::vector<std::filesystem::path> m_files;
	std::vector<std::string> m_includePaths;
	std::vector<std::string> m_defines;
	std::string m_pch;
	std::vector<std::string> m_extras;
	std::filesystem::path m_template;
};

} // namespace ccccc
