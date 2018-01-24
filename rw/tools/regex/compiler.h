#ifndef RW_REGEX_COMPILER_H
#define RW_REGEX_COMPILER_H



/**********************************************************************
 *
 * compiler.h - contains the class definition for the NFA regex
 *              compiler
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex/compiler.h#1 $
 *
 **********************************************************************
 *
 * Copyright (c) 1989-2015 Rogue Wave Software, Inc.  All Rights Reserved.
 * 
 * This computer software is owned by Rogue Wave Software, Inc. and is
 * protected by U.S. copyright laws and other laws and by international
 * treaties.  This computer software is furnished by Rogue Wave Software, Inc.
 * pursuant to a written license agreement and may be used, copied, transmitted,
 * and stored only in accordance with the terms of such license agreement and 
 * with the inclusion of the above copyright notice.  This computer software or
 * any other copies thereof may not be provided or otherwise made available to
 * any other person.
 * 
 * U.S. Government Restricted Rights.  This computer software: (a) was
 * developed at private expense and is in all respects the proprietary
 * information of Rogue Wave Software, Inc.; (b) was not developed with 
 * government funds; (c) is a trade secret of Rogue Wave Software, Inc. for all
 * purposes of the Freedom of Information Act; and (d) is a commercial item and
 * thus, pursuant to Section 12.212 of the Federal Acquisition Regulations (FAR)
 * and DFAR Supplement Section 227.7202, Government's use, duplication or
 * disclosure of the computer software is subject to the restrictions set forth
 * by Rogue Wave Software, Inc.
 *
 **********************************************************************/


#include <rw/tools/regex.h>
#include <rw/tools/regex/defs.h>
#include <rw/tools/regex/bufman.h>
#include <rw/tools/regex/vector.h>

/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWRegexCompiler                                                        */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This class can be used to compile a regular expression into a state    */
/* machine for future matching operations.                                */
/*                                                                        */
/**************************************************************************/
class                    RWRegexAltSetting;
template <class E> class RWRegexImp;
template <class E> class RWRegexState;
template <class E> class RWRegexLiteralString;
template <class E> class RWRegexLexicon;

template <class E>
class RWRegexCompiler
{
    friend class RWRegexLiteralString<E>;

public:
    RWRegexCompiler() :
        regex_(0)
    {}
    ~RWRegexCompiler() {}

    void         setRegex(RWRegexImp<E>* newRegex) {
        regex_ = newRegex;
    }
    void         preprocess();
    void         compile();

private:
    typedef typename RWTRegexTraits<E>::Char     RChar;
    typedef typename RWTRegex<E>::RWTRegexStatus RStatus;

    // Buffer size constants
    enum Constants {
        NLSize = 192,
        BGTableSize = 192,
        GammaVectorSize = 64
    };

    // Preliminary regex "state", or node, types
    enum RWRegexNodeType {
        Null,
        Alpha,
        Omega,
        Or,
        Beta,
        Gamma,
        Bracket,
        AnchorBegin,
        AnchorEnd,
        Literal,
        Dot,
        Plus,
        Star,
        Question,
        ArbCard,
        SBeta,
        SGamma,
        Collapsed
    };

    // Preprocessed regex node
    struct RWRegexNode {
        RWRegexNode() { }
        RWRegexNode(RWRegexNodeType  newType,
                    size_t           newLength,
                    size_t           newMyID,
                    size_t           newMyAlpha,
                    size_t           newMyNext = size_t(-1),
                    size_t           newMyClose = size_t(-1)) :
            type_(newType),
            length_(newLength),
            myID_(newMyID),
            myAlpha_(newMyAlpha),
            myNext_(newMyNext),
            myClose_(newMyClose),
            anchorEnd_(false) { }

        RWRegexNodeType  type_;      // Type of node
        size_t           length_;    // string length
        size_t           myID_;      // ID of node in preliminary node vector
        size_t           myAlpha_;   // ID of this node's alpha node
        size_t           myNext_;    // ID of this node's next node
        size_t           myClose_;   // ID of this node's close node, if alpha node
        bool        anchorEnd_; // True iff node represents a state end-anchored
    };

    friend struct RWRegexNode;

    static bool     isSpecial(const RChar& c);
    static bool     isCard(RWRegexNodeType t);
    static bool     isCard(const RChar& c);
    static bool     isRep(const RChar& c);

    bool cParen(const RChar* buf, size_t i);
    bool oParen(const RChar* buf, size_t i);
    bool vBar(const RChar* buf, size_t i);

    void fillBGTable(const RChar*&, size_t);

    RWRegexNodeType
    validate(RWRegexNodeType, RWRegexNodeType, const RChar*);

    bool validateArbCard(const RChar*, size_t);

    void collapseBetaStates();
    void buildGammaVector();
    void setNextStates();
    void setCloseStates();
    void allocateRegexStates();
    void pass1();
    void pass2();

    // Steps for pass1
    void createStates();
    void addSinkStates();
    void allocateMatchInfo();

    // Case handlers for createStates()
    RWRegexState<E>* handleAnchor(size_t, RWRegexState<E>*&);
    RWRegexState<E>* handleAlpha(size_t, RWRegexState<E>*&);
    RWRegexState<E>* handleBeta(size_t, RWRegexAltSetting&);
    RWRegexState<E>* handleData(size_t, RWRegexLexicon<E>*, RWRegexState<E>*&);
    RWRegexState<E>* handleSBeta(size_t&, const RChar*&, RWRegexState<E>*&);

    // State transition queries
    RWRegexState<E>*     primary(size_t weight, size_t i);
    RWRegexState<E>*     failed(size_t weight, size_t i);
    RWRegexState<E>*     lookahead(size_t weight, size_t i);

    // Get cardinality based on cardinality character
    void setCard(RWRegexState<E>*, const RChar*, size_t);

    // Additional utility methods
    RWRegexLexicon<E>*   makeLexicon(const RChar*&             str,
                                     size_t                    len);
    RWRegexLexicon<E>*   parseBracketSet(const RChar*&         str,
                                         size_t                len);
    int                  parseDecNumber(const RChar* c, size_t len) const;
    int                  parseHexNumber(const RChar* c, size_t len) const;
    RChar                nextChar(const RChar*& c, size_t len, void* args = 0) const;
    void                 regexThrow(size_t        status,
                                    const RChar*  offset) const;
    size_t               endOfBracketSet(const RChar* cur, size_t i, size_t start, size_t length);
    size_t               parseLiteral(const RChar* cur, size_t end, RWRegexNodeType& last);

    // Reference to owning regex object
    RWRegexImp<E>*                     regex_;

    // Preliminary, pre-compiled node size and buffer
    size_t                             nlSize_;
    RWBufMan<RWRegexNode, NLSize>      nl_;

    // Beta/gamma state table buffer; used to mark locations
    // where beta and gamma nodes need to be inserted in the
    // preliminary node list
    typedef unsigned char              BGElement;
    RWBufMan<BGElement, BGTableSize>   bgTable_;

    // Gamma node count, and gamma vector; used to store the
    // next state information for all states at end of alternation
    // paths.
    size_t                             gammaCount_;
    RWBufMan<size_t, GammaVectorSize>  gammaVector_;

    // State ID map; used to map node ID's to actual state
    // ID's.
    RWBufMan<size_t, NLSize>           stateIDMap_;
};

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("compiler.cc")
#endif

#if defined(RW_NO_IMPLICIT_INCLUSION)
#  include <rw/tools/regex/compiler.cc>
#endif

#endif
