/****
DIAMOND protein aligner
Copyright (C) 2013-2018 Benjamin Buchfink <buchfink@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
****/

#ifndef SOURCE_H_
#define SOURCE_H_

#include "exceptions.h"
#include <string>
#include <vector>

using std::vector;
using std::string;

struct Source
{
	virtual void rewind()
	{
		throw UnsupportedOperation();
	}
	virtual void seek(size_t pos)
	{
		throw UnsupportedOperation();
	}
	virtual void seek_forward(size_t n)
	{
		throw UnsupportedOperation();
	}
	virtual void putback(char c)
	{
		throw UnsupportedOperation();
	}
	virtual size_t read(char *ptr, size_t count) = 0;
	virtual bool read_until(string &dst, char delimiter)
	{
		throw UnsupportedOperation();
	}
	virtual bool read_until(vector<char> &dst, char delimiter)
	{
		throw UnsupportedOperation();
	}
	virtual void close() = 0;
	virtual const string& file_name() const = 0;
	virtual ~Source()
	{}
};

#endif