

/**********************************************************************
 *
 * Definitions for POSIX services for the parameterized regular expression
 * class, Part 2: regexec() and regfree().
 * NOTE:  Do not change the signature of any of the member functions in this
 * file except in response to changes in the POSIX standard for RE's.  If you
 * do, then you will be unable to use POSIX RE services in the future.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/reposix2.cc#1 $
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



#include <rw/defs.h>
#if !defined(RW_NO_EXCEPTIONS)

#  include <rw/rei.h>

// Below enables users to use regular expression bindings
// included with compiler.
#  if defined(RW_NO_POSIX_RE)

/////////////////////////////////////////////////////////////////////////////
// rwRegexec
//
//  Finds matches in a string with a compiled regular expression.
//
//  Returns:
//    0             if OK
//    REG_NOMATCH   if not OK.
//
//  This function can be replaced by POSIX.2 system service call regexec().
//  See top of rw/rei.h for details.
//
//  Unlike the POSIX.2 regexec(), rwRegexec processes wchar_t strings
//  and embedded NULLs.
//
//  Matching function uses a deque to store states in regular expression
//  nondeterministic finite automaton (NFA).  Each state stored on the deque is
//  composed of its index of the state node in the NFA together with the
//  index of the next input symbol that will fire the transition in the state
//  to the next state.
//
//  States with epsilon transitions are pushed onto front of deque and states
//  with input transitions pushed onto the back of the deque.  The next state
//  to process is always popped off the front of the deque.
//
//  Epsilon states are processed on the present symbol; input states are
//  processed on the next symbol.  Epsilon and input states are separated
//  by the scan state which triggers a read of the next input symbol.
//
//  PRECONDITIONS:
//    1.  There is one and only one input transition out of any state.
//    2.  _OR_ there are either one or two epsilon transitions out of a state.
//    3.  States with two transitions aren't adjacent to each other.
//
/////////////////////////////////////////////////////////////////////////////

#    include <rw/tvdeque.h>
#    include <rw/edefs.h>
#    include <rw/bitvec.h>
#    include <list>

inline void __destroy(RWREState**) { }  // needed for Borland 4.5

template<class charT> int
RWTRegularExpressionImp<charT>::rwRegexec
(
    const charT*  searchStringPtr,    // String to be searched.
    // Length of string to be searched, including embedded nulls.
    size_t        searchStringLength,
    regmatch_t    pmatch[]            // Array of matched strings.
)
{

    if (!compiled_) {
        return REG_NOMATCH;    // regexec() can only return 0 or REG_NOMATCH (POSIX.2)
    }

    const RWREState   scanState(getNextSymbol_, 0); // Get new input.
    const size_t      firstStateIndex(next1(0));
    const RWRENode<charT>
    firstNode(getNode(firstStateIndex));
    size_t            stateIndex(firstStateIndex);  // First state is state 1.
    RWRENode<charT>   node(getNode(stateIndex));    // State machine node.
    size_t            stringIndex(0);               // Position in search string.
    RWREState         theState(stateIndex, 0);      // Set to state 1.

    std::list<RWREState> state;

    // Prevents one state from being added twice to deque; prevents
    // infinite loops in NFA from being executed.
    RWBitVec          stateUsed(nextState_);
    size_t            firstMatch(0);                // 1st poss. symbol of match.
    bool         found(false);                 // Flags a match.
    size_t            leftmost(RW_NPOS);            //Leftmost position in match.
    size_t            longest(RW_NPOS);             //Rightmost position in match

    // Prime the pump.
    // When scanState is popped from the deque, then a new symbol is input.
    state.push_back(scanState);

    while (stateIndex) {  // Try to find leftmost longest match in search string.
        //======================= SCAN NEXT SYMBOL ================================

        if (theState == scanState) {
            if (state.empty()) {
                // Scan with empty deque;
                // exhausted all possible matches at this place in string.
                stateIndex = firstStateIndex;  // Reset to beginning of RE.
                theState.number = stateIndex;
                theState.nextToMatch = firstMatch;
                firstMatch++;

                if
                (
                    firstMatch > searchStringLength ||
                    (
                        leftmost != RW_NPOS &&
                        (leftmost != longest || leftmost != firstMatch)
                    )
                ) {
                    break;    // Out of input or match already found.
                }


                // Reset to beginning of RE.
                stateIndex = firstStateIndex;
                theState.number = stateIndex;
                theState.nextToMatch = firstMatch;
                stringIndex = firstMatch;

                // Push marker back on for the next time we run out of candidate
                // epsilon states on the front of the deque:
                state.push_back(scanState);  // Push on scan marker.
                node = firstNode;           // Go back to start of pattern.
                continue;
            } // if (state.isEmpty())

            // Push marker back on for the next time we run out of candidate epsilon
            // states on the front of the deque.
            state.push_back(scanState);
        } // if (theState == scanState)

        //===================== NONACCEPTING STATE ==============================

        else if (stateIndex == nonacceptingState_) {
            // Nonaccepting state: determine if match is leftmost longest.
            // A nonaccepting state is a state that is appended to a regular
            // expression to help determine if a matched string is the longest
            // possible string to be found at that position.

            found = true;

            if (leftmost == RW_NPOS) {
                leftmost = firstMatch;
            }

            if
            (
                (
                    longest < stringIndex && stringIndex <= searchStringLength &&
                    leftmost == firstMatch
                )  ||
                longest == RW_NPOS
            ) {
                longest = stringIndex;    // Found a longer match.
            }
        }   //  if (stateIndex == nonacceptingState_)

        //============== STATE WITH EPSILON TRANSITIONS =========================

        else if
        (
            node.input_ == nullCharT()        &&
            !node.matchAny_                   &&
            !node.symbolSetIndex_             &&
            !stateUsed.testBit(node.next1_)   &&
            (
                node.anchor_ == notAnchored ||
                (node.anchor_ == anchorFront && stringIndex == 0)      ||
                (node.anchor_ == anchorBack && stringIndex == searchStringLength)
            )
        ) { // Guess which epsilon transition to choose.

            // Guess: Push next1 onto front of deque.
            theState.number = node.next1_;
            theState.nextToMatch = stringIndex;
            state.push_front(theState);
            stateUsed.setBit(theState.number);

            // Don't make any changes to theState before if () below.

            if (!stateUsed.testBit(node.next2_)) {
                // Guess: Push next2 onto front of deque.
                theState.number = node.next2_;
                theState.nextToMatch = stringIndex;
                state.push_front(theState);
                stateUsed.setBit(theState.number);
            }
        } // else guess on epsilon.

        // ===================== MATCH FOUND ==================================

        else if // Exact match or match from character class expression or "."
        (
            searchStringPtr[stringIndex] == node.input_               ||
            node.matchAny_                                            ||
            (
                node.input_ == nullCharT()                              &&
                isMatchedInSymbolSet
                (node.symbolSetIndex_, (unsigned char)searchStringPtr[stringIndex])
            )
        ) {
            // Start finding a match.

            theState.number = node.next1_;      // Push next state onto back of deque.

            if (!stateUsed.testBit(theState.number)) {
                // Push theState onto back of deque.
                theState.nextToMatch = stringIndex + 1;
                state.push_back(theState);
                stateUsed.setBit(theState.number);
            }
        }       // else started finding a match.

        //=================== POP ANOTHER STATE OFF FRONT ========================

        // Pop from front of deque:
        theState = state.front();
        state.pop_front();
        stateIndex = theState.number;

        if (stateIndex != RW_NPOS) {
            stateUsed.clearBit(stateIndex);
            node = getNode(stateIndex);
        }
        stringIndex = theState.nextToMatch;

        if (stringIndex > searchStringLength && stringIndex != RW_NPOS) {
            break;    // Out of input
        }
    } // while

    //=================== CLEANUP AND RETURN ===================================

    if (found) {
        pmatch[0].rm_so = leftmost;
        pmatch[0].rm_eo = longest;          // rm_eo points to char past last char.
    }
    else {
        // No match found at end of while.
        pmatch[0].rm_so = RW_NPOS;
        pmatch[0].rm_eo = RW_NPOS;          // rm_eo points to char past last char.
        return REG_NOMATCH;
    }

    // Found a match.
    return 0;                             // Found a match.
}

#  endif // defined(RW_NO_POSIX_RE)
#endif //!defined(RW_NO_EXCEPTIONS)
