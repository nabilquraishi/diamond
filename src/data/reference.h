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

#ifndef REFERENCE_H_
#define REFERENCE_H_

#include <vector>
#include <string>
#include "../util/io/input_file.h"
#include "../data/seed_histogram.h"
#include "sequence_set.h"

using std::vector;
using std::string;

struct ReferenceHeader
{
	ReferenceHeader() :
		magic_number(0x24af8a415ee186dllu),
		build(Const::build_version),
		db_version(current_db_version),
		sequences(0),
		letters(0)
	{ }
	uint64_t magic_number;
	uint32_t build, db_version;
	uint64_t sequences, letters, pos_array_offset;
#ifdef EXTRA
	enum { current_db_version = 2 };
#else
	enum { current_db_version = 1 };
#endif
};

struct ReferenceHeader2
{
	uint64_t taxon_array_offset;
};

struct Database_format_exception : public std::exception
{
	virtual const char* what() const throw()
	{ return "Database file is not a DIAMOND database."; }
};

struct DatabaseFile : public InputFile
{
	DatabaseFile();
	static void read_header(InputFile &stream, ReferenceHeader &header);
	void rewind();
	bool load_seqs();
	void get_seq();
	void read_seq(string &id, vector<char> &seq);

	enum { min_build_required = 74 };

	size_t pos_array_offset;
	ReferenceHeader ref_header;
#ifdef EXTRA
	ReferenceHeader2 header2;
#endif
};

void make_db();

struct ref_seqs
{
	static const Sequence_set& get()
	{ return *data_; }
	static Sequence_set& get_nc()
	{ return *data_; }
	static Sequence_set *data_;
};

struct ref_ids
{
	static const String_set<0>& get()
	{ return *data_; }
	static String_set<0> *data_;
};

extern Partitioned_histogram ref_hst;
extern unsigned current_ref_block;
extern bool blocked_processing;

inline size_t max_id_len(const String_set<0> &ids)
{
	size_t max (0);
	for(size_t i=0;i<ids.get_length(); ++i)
		max = std::max(max, find_first_of(ids[i].c_str(), Const::id_delimiters));
	return max;
}

inline vector<string> seq_titles(const char *title)
{
	return tokenize(title, "\1");
}

#endif /* REFERENCE_H_ */
