#ifndef CPPAD_CG_EXCEPTION_INCLUDED
#define CPPAD_CG_EXCEPTION_INCLUDED
/* --------------------------------------------------------------------------
 *  CppADCodeGen: C++ Algorithmic Differentiation with Source Code Generation:
 *    Copyright (C) 2012 Ciengis
 *
 *  CppADCodeGen is distributed under multiple licenses:
 *
 *   - Eclipse Public License Version 1.0 (EPL1), and
 *   - GNU General Public License Version 3 (GPL3).
 *
 *  EPL1 terms and conditions can be found in the file "epl-v10.txt", while
 *  terms and conditions for the GPL3 can be found in the file "gpl3.txt".
 * ----------------------------------------------------------------------------
 * Author: Joao Leal
 */

namespace CppAD {
namespace cg {

/**
 * Source code generation exception
 * 
 * @author Joao Leal
 */
class CGException : public std::exception {

protected:
    std::string _message;

public:

    inline CGException(const std::string& message) throw () :
        _message(message) {
    }

    CGException() throw () = delete;

    const char* what() const throw () {
        return _message.c_str();
    }

    virtual ~CGException() throw () {
    }

};

} // END cg namespace
} // END CppAD namespace

#endif