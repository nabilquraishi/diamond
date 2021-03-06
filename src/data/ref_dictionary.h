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

#ifndef REF_DICTIONARY_H_
#define REF_DICTIONARY_H_

#include <stdint.h>
#include <stdexcept>
#include <vector>
#include <string>
#include "../util/tinythread.h"
#include "../util/ptr_vector.h"
#include "../util/io/output_file.h"

using std::vector;
using std::string;

struct ReferenceDictionary
{

	ReferenceDictionary() :
		next_(0)
	{ }

	void init(unsigned ref_count);

	uint32_t get(unsigned block, unsigned i);

	unsigned length(uint32_t i) const
	{
		return len_[i];
	}

	const char* name(uint32_t i) const
	{
		return name_[i].c_str();
	}

	void init_rev_map();
	
	unsigned original_id(unsigned i) const
	{
		return rev_map_[i];
	}

	uint32_t check_id(uint32_t i) const
	{
		if (i >= next_)
			throw std::runtime_error("Dictionary reference id out of bounds.");
		return i;
	}

	static ReferenceDictionary& get()
	{
		return instance_;
	}

	uint32_t seqs() const
	{
		return next_;
	}

private:

	static ReferenceDictionary instance_;

	tthread::mutex mtx_;
	vector<vector<uint32_t> > data_;
	vector<uint32_t> len_;
	PtrVector<string> name_;
	vector<uint32_t> rev_map_;
	uint32_t next_;

	friend void finish_daa(OutputFile&, const DatabaseFile&);

};

#endif
