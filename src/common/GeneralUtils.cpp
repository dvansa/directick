/*
*
* GeneralUtils.hpp implementations.
*
*
* Copyright (C) 2016 Daniel van Sabben Alsina
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*
*/
#include <directick/common/common.hpp>
#include <directick/common/GeneralUtils.hpp>

long long GetCurrentMilliseconds()
{
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	return ms.count();
}

std::string StrToUppercase(const std::string & str)
{
	std::string upper_str;
	for (char c : str)
	{
		upper_str.push_back(toupper(c));
	}
	return upper_str;
}

std::string EncodeToURL(const std::string & str)
{
	//Used for processing
	std::string tmpstr(str);

	//Output string
	std::string outstr = "";

	//Substitute ' ' for "%20"
	std::size_t space_pos = tmpstr.find(" ");
	while (space_pos != std::string::npos)
	{

		outstr.append(tmpstr.begin(), tmpstr.begin() + space_pos);
		outstr.append("%20");

		tmpstr.assign(tmpstr.begin() + space_pos + 1, tmpstr.end());

		space_pos = tmpstr.find(" ");
	}

	//Append tmpstr tail
	outstr.append(tmpstr.begin(), tmpstr.end());

	return outstr;
}