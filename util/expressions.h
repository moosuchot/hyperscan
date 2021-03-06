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

#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include <map>
#include <string>
#include <list>

typedef std::map<unsigned, std::string> ExpressionMap;
typedef std::list<unsigned> SignatureSet;

// load all of the expressions from the given directory into the given
// expression map. Exits on failure.
void loadExpressions(const std::string &inDir, ExpressionMap &exprMap);

void loadExpressionsFromFile(const std::string &fname, ExpressionMap &exprMap);

// load a list of signature IDs
void loadSignatureList(const std::string &inFile, SignatureSet &signatures);

// produce a new expression map only containing those signatures in the
// expression list
void generateExprMap(const SignatureSet &signatures,
    const ExpressionMap &allExprs, ExpressionMap &out);

// trim expression map to only the given signatures (in-place)
void limitBySignature(ExpressionMap &exprMap, const SignatureSet &signatures);
#endif
