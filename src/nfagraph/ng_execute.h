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
 * \brief Execute an NFA over a given input, returning the set of states that
 * are active afterwards.
 */

#ifndef NG_EXECUTE_H
#define NG_EXECUTE_H

#include "ng_holder.h"

#include <set>
#include <vector>

namespace ue2 {

class CharReach;
struct ue2_literal;

void execute_graph(const NGHolder &g, const ue2_literal &input,
                   std::set<NFAVertex> *states, bool kill_sds = false);

void execute_graph(const NGHolder &g, const std::vector<CharReach> &input,
                   std::set<NFAVertex> *states);

/** on exit, states contains any state which may still be enabled after
 * receiving an input which corresponds to some path through the input_dag from
 * start or startDs to accept. input_dag MUST be acyclic aside from self-loops.
 */
void execute_graph(const NGHolder &g, const NGHolder &input_dag,
                   std::set<NFAVertex> *states);

/* as above, but able to specify the source states for the input graph */
void execute_graph(const NGHolder &g, const NGHolder &input_dag,
                   const std::set<NFAVertex> &input_start_states,
                   std::set<NFAVertex> *states);

} // namespace ue2

#endif
