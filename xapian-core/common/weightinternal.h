/** @file weightinternal.h
 * @brief Xapian::Weight::Internal class, holding database and term statistics.
 */
/* Copyright 2007 Lemur Consulting Ltd
 * Copyright 2009 Olly Betts
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef XAPIAN_INCLUDED_WEIGHTINTERNAL_H
#define XAPIAN_INCLUDED_WEIGHTINTERNAL_H

#include "xapian/weight.h"

#include "xapian/database.h"

#include "internaltypes.h"

#include <map>
#include <string>

namespace Xapian {

/** Class to hold statistics for a given collection. */
class Weight::Internal {
  public:
    /** Total length of all documents in the collection. */
    totlen_t total_length;

    /** Number of documents in the collection. */
    Xapian::doccount collection_size;

    /** Number of relevant documents in the collection. */
    Xapian::doccount rset_size;

    /** Database to get the bounds on doclength and wdf from. */
    Xapian::Database db;

    /** Map of term frequencies for the collection. */
    std::map<std::string, Xapian::doccount> termfreq;

    /** Map of relevant term frequencies for the collection. */
    std::map<std::string, Xapian::doccount> reltermfreq;

    /** Create a Weight::Internal object with global statistics.
     *
     *  All term-specific statistics will be set to 0.
     *
     *  @param stats  Object containing the statistics to use.
     */
    Internal(const Internal & stats);

    /** Create a Weight::Internal object with global and term statistics.
     *
     *  @param stats  Object containing the statistics to use.
     *  @param term   The term to read the term-specific statistics for.
     */
    Internal(const Internal & stats, const std::string & term);

    Internal() : total_length(0), collection_size(0), rset_size(0) { }

    /** Add in the supplied statistics from a sub-database. */
    Internal & operator +=(const Internal & inc);

    /** Get the term-frequency of the given term.
     *
     *  This is "n_t", the number of documents in the collection indexed by
     *  the given term.
     */
    Xapian::doccount get_termfreq(const std::string & term) const;

    /** Set the term-frequency for the given term. */
    void set_termfreq(const std::string & term, Xapian::doccount tfreq);

    /** Get the relevant term-frequency for the given term.
     *
     *  This is "r_t", the number of relevant documents in the collection
     *  indexed by the given term.
     */
    Xapian::doccount get_reltermfreq(const std::string & term) const;

    /** Set the relevant term-frequency for the given term. */
    void set_reltermfreq(const std::string & term, Xapian::doccount rtfreq);

    Xapian::doclength get_average_length() const {
	if (rare(collection_size == 0)) return 0;
	return Xapian::doclength(total_length) / collection_size;
    }

    /** Set the "bounds" stats from Database @a db. */
    void set_bounds_from_db(const Xapian::Database &db_) { db = db_; }

    /// Return a std::string describing this object.
    std::string get_description() const;
};

}

#endif // XAPIAN_INCLUDED_WEIGHTINTERNAL_H
