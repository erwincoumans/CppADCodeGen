#ifndef CPPAD_CG_C_LANG_DEFAULT_REVERSE2_VAR_NAME_GEN_INCLUDED
#define CPPAD_CG_C_LANG_DEFAULT_REVERSE2_VAR_NAME_GEN_INCLUDED
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
 * Creates variables names for the source code generated for second-order
 * reverse mode calculations.
 * The independent variables are considered to have been registered first,
 * followed by a first level of additional variables and then a second.
 * 
 * @author Joao Leal
 */
template<class Base>
class CLangDefaultReverse2VarNameGenerator : public VariableNameGenerator<Base> {

protected:
    VariableNameGenerator<Base>* _nameGen;
    // the lowest variable ID used for the first independent variable level
    const size_t _minLevel1ID;
    // array name of the independent variables (1st level)
    const std::string _level1Name;
    // the lowest variable ID used for the second independent variable level
    const size_t _minLevel2ID;
    // array name of the independent variables (2nd level)
    const std::string _level2Name;
    // auxiliary string stream
    std::stringstream _ss;
public:

    CLangDefaultReverse2VarNameGenerator(VariableNameGenerator<Base>* nameGen,
                                         size_t n,
                                         size_t n1) :
        _nameGen(nameGen),
        _minLevel1ID(n + 1),
        _level1Name("tx1"),
        _minLevel2ID(_minLevel1ID + n1),
        _level2Name("py2") {

        CPPADCG_ASSERT_KNOWN(_nameGen != nullptr, "The name generator must not be null");

        initialize();
    }

    CLangDefaultReverse2VarNameGenerator(VariableNameGenerator<Base>* nameGen,
                                         size_t n,
                                         const std::string& level1Name,
                                         size_t n1,
                                         const std::string& level2Name) :
        _nameGen(nameGen),
        _minLevel1ID(n + 1),
        _level1Name(level1Name),
        _minLevel2ID(_minLevel1ID + n1),
        _level2Name(level2Name) {

        CPPADCG_ASSERT_KNOWN(_nameGen != nullptr, "The name generator must not be null");
        CPPADCG_ASSERT_KNOWN(_level1Name.size() > 0, "The name for the first level must not be empty");
        CPPADCG_ASSERT_KNOWN(_level2Name.size() > 0, "The name for the second level must not be empty");

        initialize();
    }

    virtual const std::vector<FuncArgument>& getDependent() const override {
        return _nameGen->getDependent();
    }

    virtual const std::vector<FuncArgument>& getTemporary() const override {
        return _nameGen->getTemporary();
    }

    virtual size_t getMinTemporaryVariableID() const override {
        return _nameGen->getMinTemporaryVariableID();
    }

    virtual size_t getMaxTemporaryVariableID() const override {
        return _nameGen->getMaxTemporaryVariableID();
    }

    virtual size_t getMaxTemporaryArrayVariableID() const override {
        return _nameGen->getMaxTemporaryArrayVariableID();
    }

    virtual size_t getMaxTemporarySparseArrayVariableID() const override {
        return _nameGen->getMaxTemporarySparseArrayVariableID();
    }

    virtual std::string generateDependent(size_t index) override {
        return _nameGen->generateDependent(index);
    }

    virtual std::string generateIndependent(const OperationNode<Base>& independent) override {
        size_t id = independent.getVariableID();
        if (id < _minLevel1ID) {
            return _nameGen->generateIndependent(independent);
        } else {
            _ss.clear();
            _ss.str("");
            if (id < _minLevel2ID) {
                _ss << _level1Name << "[" << (id - _minLevel1ID) << "]";
            } else {
                _ss << _level2Name << "[" << (id - _minLevel2ID) << "]";
            }
            return _ss.str();
        }
    }

    virtual std::string generateTemporary(const OperationNode<Base>& variable) override {
        return _nameGen->generateTemporary(variable);
    }

    virtual std::string generateTemporaryArray(const OperationNode<Base>& variable) override {
        return _nameGen->generateTemporaryArray(variable);
    }

    virtual std::string generateTemporarySparseArray(const OperationNode<Base>& variable) override {
        return _nameGen->generateTemporarySparseArray(variable);
    }

    virtual std::string generateIndexedDependent(const OperationNode<Base>& var,
                                                 const IndexPattern& ip) override {
        return _nameGen->generateIndexedDependent(var, ip);
    }

    virtual std::string generateIndexedIndependent(const OperationNode<Base>& independent,
                                                   const IndexPattern& ip) override {
        size_t varType = independent.getInfo()[0];
        if (varType == 0) {
            return _nameGen->generateIndexedIndependent(independent, ip);
        } else {
            CPPADCG_ASSERT_KNOWN(independent.getOperationType() == CGLoopIndexedIndepOp, "Invalid node type");
            CPPADCG_ASSERT_KNOWN(independent.getArguments().size() > 0, "Invalid number of arguments");
            CPPADCG_ASSERT_KNOWN(independent.getArguments()[0].getOperation() != nullptr, "Invalid argument");
            CPPADCG_ASSERT_KNOWN(independent.getArguments()[0].getOperation()->getOperationType() == CGIndexOp, "Invalid argument");

            _ss.clear();
            _ss.str("");

            const IndexOperationNode<Base>& index = static_cast<const IndexOperationNode<Base>&> (*independent.getArguments()[0].getOperation());

            if (varType == 1) {
                _ss << _level1Name << "[" << CLanguage<Base>::indexPattern2String(ip, index.getIndex()) << "]";
            } else {
                _ss << _level2Name << "[" << CLanguage<Base>::indexPattern2String(ip, index.getIndex()) << "]";
            }
            return _ss.str();
        }

    }

    virtual const std::string& getIndependentArrayName(const OperationNode<Base>& indep) override {
        if (indep.getVariableID() < _minLevel1ID)
            return _nameGen->getIndependentArrayName(indep);
        else if (indep.getVariableID() < _minLevel2ID)
            return _level1Name;
        else
            return _level2Name;
    }

    virtual size_t getIndependentArrayIndex(const OperationNode<Base>& indep) override {
        size_t id = indep.getVariableID();

        if (id < _minLevel1ID)
            return _nameGen->getIndependentArrayIndex(indep);
        else if (id < _minLevel2ID)
            return id - _minLevel1ID;
        else
            return id - _minLevel2ID;
    }

    virtual bool isConsecutiveInIndepArray(const OperationNode<Base>& indepFirst,
                                           const OperationNode<Base>& indepSecond) override {
        size_t id1 = indepFirst.getVariableID();
        size_t id2 = indepSecond.getVariableID();

        if ((id1 < _minLevel1ID) != (id2 < _minLevel1ID))
            return false;

        if (id1 < _minLevel1ID && id2 < _minLevel1ID)
            return _nameGen->isConsecutiveInIndepArray(indepFirst, indepSecond);

        if ((id1 < _minLevel2ID) != (id2 < _minLevel2ID))
            return false;

        return id1 + 1 == id2;
    }

    virtual bool isInSameIndependentArray(const OperationNode<Base>& indep1,
                                          const OperationNode<Base>& indep2) override {
        size_t l1;
        if (indep1.getOperationType() == CGInvOp) {
            size_t id = indep1.getVariableID();
            l1 = id < _minLevel1ID ? 0 : (id < _minLevel2ID ? 1 : 2);
        } else {
            l1 = indep1.getInfo()[0]; //CGLoopIndexedIndepOp
        }

        size_t l2;
        if (indep2.getOperationType() == CGInvOp) {
            size_t id = indep2.getVariableID();
            l2 = id < _minLevel1ID ? 0 : (id < _minLevel2ID ? 1 : 2);
        } else {
            l2 = indep2.getInfo()[0]; //CGLoopIndexedIndepOp
        }

        return l1 == l2;
    }

    virtual void setTemporaryVariableID(size_t minTempID,
                                        size_t maxTempID,
                                        size_t maxTempArrayID,
                                        size_t maxTempSparseArrayID) override {
        _nameGen->setTemporaryVariableID(minTempID, maxTempID, maxTempArrayID, maxTempSparseArrayID);
    }

    inline virtual ~CLangDefaultReverse2VarNameGenerator() {
    }

private:

    inline void initialize() {
        this->_independent = _nameGen->getIndependent(); // copy
        this->_independent.push_back(FuncArgument(_level1Name));
        this->_independent.push_back(FuncArgument(_level2Name));
    }

};

} // END cg namespace
} // END CppAD namespace

#endif