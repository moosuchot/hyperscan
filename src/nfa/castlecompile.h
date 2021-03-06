/*
 * Copyright (c) 2015, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** \file
 * \brief Castle: multi-tenant repeat engine, compiler code.
 */

#ifndef NFA_CASTLECOMPILE_H
#define NFA_CASTLECOMPILE_H

#include "nfa_kind.h"
#include "ue2common.h"
#include "nfagraph/ng_repeat.h"
#include "util/alloc.h"
#include "util/depth.h"

#include <map>
#include <memory>
#include <set>
#include <vector>

struct NFA;

namespace ue2 {

class CharReach;
class NGHolder;
struct CompileContext;

/**
 * \brief Prototype for a Castle engine: contains at least one CastleRepeat.
 *
 * Currently, all repeats in a Castle must have the same character
 * reachability.
 *
 * A CastleProto is converted into a single NFA, with each top triggering a
 * unique repeat. A CastleProto can contain at most CastleProto::max_occupancy
 * elements.
 */
struct CastleProto {
    static constexpr size_t max_occupancy = 65536; // arbitrary limit
    explicit CastleProto(const PureRepeat &pr);
    const CharReach &reach() const;

    u32 add(const PureRepeat &pr);

    /**
     * \brief Merge in the given repeat, returning the top used.
     *
     * If the repeat already exists in this castle, we will re-use (and return)
     * the old top. If it doesn't, it will be added and assigned a new top.
     * Returns \ref max_occupancy if capacity would be exceeded.
     */
    u32 merge(const PureRepeat &pr);

    /** \brief Mapping from unique top id to repeat. */
    std::map<u32, PureRepeat> repeats;
};

std::set<ReportID> all_reports(const CastleProto &proto);
depth findMinWidth(const CastleProto &proto);
depth findMaxWidth(const CastleProto &proto);

/**
 * \brief Remap tops to be contiguous.
 *
 * Remap the tops in the given CastleProto so that they're contiguous in the
 * range [0 .. N-1].
 */
void remapCastleTops(CastleProto &proto, std::map<u32, u32> &top_map);

/**
 * \brief Construct an NFA from a CastleProto.
 *
 * NOTE: Tops must be contiguous, i.e. \ref remapCastleTops must have been run
 * first.
 */
ue2::aligned_unique_ptr<NFA>
buildCastle(const CastleProto &proto,
            const std::map<u32, std::vector<std::vector<CharReach>>> &triggers,
            const CompileContext &cc);

/**
 * \brief Merge two CastleProto prototypes together, if possible.
 *
 * Returns true if merge of all repeats in c2 into c1 succeeds, and fills
 * mapping with the repeat indices.
 */
bool mergeCastle(CastleProto &c1, const CastleProto &c2,
                 std::map<u32, u32> &top_map);

/**
 * \brief True if the two castles are identical with respect to the reports
 * given; i.e. the same tops lead to the same repeats, just with report1 in c1
 * and report2 in c2.
 *
 * Repeats leading to other reports are ignored.
 */
bool is_equal(const CastleProto &c1, ReportID report1, const CastleProto &c2,
              ReportID report2);

/**
 * \brief True if the two castles given are identical.
 */
bool is_equal(const CastleProto &c1, const CastleProto &c2);

/**
 * \brief True if the given castle contains more than a single instance of any
 * of the reports in the given set.
 */
bool requiresDedupe(const CastleProto &proto, const std::set<ReportID> &reports);

/**
 * \brief Build an NGHolder from a CastleProto.
 */
std::unique_ptr<NGHolder> makeHolder(const CastleProto &castle, nfa_kind kind,
                                     const CompileContext &cc);

} // namespace ue2

#endif // NFA_CASTLECOMPILE_H
