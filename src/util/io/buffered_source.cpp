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

#include <algorithm>
#include <string.h>
#include "buffered_source.h"

template<typename _t>
void append(_t &container, const char *ptr, size_t n)
{
	container.append(ptr, n);
}

template<>
void append<vector<char> >(vector<char> &container, const char *ptr, size_t n)
{
	container.insert(container.end(), ptr, ptr + n);
}

BufferedSource::BufferedSource(Source* source):
	source_(source)
{
	fetch();
}

void BufferedSource::rewind()
{
	source_->rewind();
	fetch();
}

void BufferedSource::seek(size_t pos)
{
	source_->seek(pos);
	fetch();
}

void BufferedSource::seek_forward(size_t n)
{
	source_->seek_forward(n);
	fetch();
}

size_t BufferedSource::read(char *ptr, size_t count)
{
	if (count <= avail_) {
		pop(ptr, count);
		return count;
	}
	else {
		size_t total = 0;
		do {
			const size_t n = std::min(count, avail_);
			pop(ptr, n);
			ptr += n;
			count -= n;
			total += n;
			if (avail_ == 0)
				fetch();
		} while (count > 0 && avail_ > 0);
		return total;
	}
}

void BufferedSource::close()
{
	source_->close();
}

const string& BufferedSource::file_name() const
{
	return source_->file_name();
}

void BufferedSource::pop(char *dst, size_t n)
{
	assert(n <= avail_);
	memcpy(dst, next(), n);
	start_ += n;
	avail_ -= n;
}

template<typename _t>
bool BufferedSource::read_to(_t &container, char delimiter)
{
	int d = delimiter;
	container.clear();
	do {
		const char *p = (const char*)memchr((void*)next(), d, avail_);
		if (p == 0)
			append(container, next(), avail_);
		else {
			const size_t n = p - next();
			append(container, next(), n);
			start_ += n + 1;
			avail_ -= n + 1;
			return true;
		}
	} while (fetch());
	return container.empty();
}

bool BufferedSource::read_until(string &dst, char delimiter)
{
	return read_to(dst, delimiter);
}

bool BufferedSource::read_until(vector<char> &dst, char delimiter)
{
	return read_to(dst, delimiter);
}