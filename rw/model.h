#ifndef RW_TOOLS_MODEL_H
#define RW_TOOLS_MODEL_H



/**********************************************************************
 *
 * RWModel --- maintains a list of dependent clients
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/model.h#1 $
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
#include <rw/ordcltn.h>

class RW_TOOLS_GLOBAL RWModelClient;

/**
 * @ingroup miscellaneous_classes
 *
 * @brief Implements the "Model"
 * leg of a Model-View-Controller architecture
 *
 * This abstract base class has been designed to implement the "Model"
 * leg of a Model-View-Controller architecture. A companion class,
 * RWModelClient, supplies the "View" leg.
 *
 * It maintains a list of dependent RWModelClient objects. When member
 * function changed(void*) is called, the list of dependents is
 * traversed, calling RWModelClient::updateFrom(RWModel*, void*) for each
 * one, with itself as the first argument. Subclasses of RWModelClient
 * should be prepared to accept such a call.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/model.h>
 * (abstract base class)
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 *
 * #include <iostream>
 * #include <rw/model.h>
 *
 * class Dial : public RWModelClient
 * {
 *     int dialNumber;
 * public:
 *     explicit Dial(int dialNumber);
 *     virtual void
 *     updateFrom(RWModel* m, void* d);
 * };
 *
 *
 * class Thermostat : public RWModel
 * {
 *     double setting;
 * public:
 *     Thermostat( Dial* d ) {
 *         addDependent (d);
 *         setting = 0;
 *     }
 *     double temperature() const {
 *         return setting;
 *     }
 *     void setTemperature(double t) {
 *         setting = t;
 *         changed ();
 *     }
 * };
 *
 *
 * void Dial::updateFrom(RWModel* m, void*)
 * {
 *     Thermostat* t = (Thermostat*)m;
 *     double temp = t -> temperature ();
 *
 *     // Redraw graphic.
 *     std::cout << "Dial #" << dialNumber << " says " << temp << std::endl;
 * }
 *
 * Dial::Dial (int num) : dialNumber (num)
 * {
 * }
 *
 * int main ()
 * {
 *     Dial one(1);
 *     Dial two(2);
 *
 *     Thermostat therm(&one);
 *
 *     therm.setTemperature(77);
 *     therm.setTemperature(-4);
 *     therm.addDependent(&two);
 *     therm.setTemperature(47);
 *     return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * Dial #1 says 77
 * Dial #1 says -4
 * Dial #1 says 47
 * Dial #2 says 47
 * @endcode
 */
class RW_TOOLS_GLOBAL RWModel
{
public:
    virtual ~RWModel();

    /**
     * Sets up the internal ordered list of dependents
     * when called by the specializing class.
     */
    RWModel();

    /**
     * Copy constructor. The constructed instance gets a copy of the dependents
     * list from \a m.
     */
    RWModel(const RWModel& m);

    /**
     * Assignment operator. Self gets a copy of the dependents list from \a m.
     */
    RWModel& operator=(const RWModel& m);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed instance takes ownership of the
     * dependents list owned by \a m.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWModel(RWModel && m);

    /**
     * Move assignment. Self takes ownership of the dependents list owned by \a m.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWModel& operator=(RWModel && m);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Adds the object pointed to by \a m to the list of dependents
     * of self.
     */
    void                  addDependent(RWModelClient* m);

    /**
     * Removes the object pointed to by \a m from the list of dependents
     * of self.
     */
    void                  removeDependent(RWModelClient* m);

    /**
     * Allows a peek at the dependent list.
     */
    const RWOrdered*      dependents() const {
        return &dependList;
    }

    /**
     * Traverse the internal list of dependents, calling member function
     * RWModelClient::updateFrom(RWModel*, void*) for each one, with self
     * as the first argument and \a d as the second argument.
     */
    virtual void          changed(void* d = 0);      // Notify all dependents

    /**
     * Swaps the list of dependents in self with that of \a m.
     */
    void swap(RWModel& m);

private:

    RWOrdered             dependList;

};

/**
 * @ingroup miscellaneous_classes
 *
 * @brief Implements the "View"
 * leg of a Model-View-Controller architecture
 *
 * This abstract base class has been designed to implement the "View"
 * leg of a Model-View-Controller architecture.  Class RWModel,
 * supplies the "Model" leg. See class RWModel for details.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/model.h>
 * (abstract base class)
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWModelClient : public RWCollectable
{
public:
    virtual ~RWModelClient();

    /**
     * Deriving classes should supply an appropriate definition for
     * this pure virtual function. The overall semantics of the definition
     * should be to update self from the data presented by the object
     * pointed to by \a p. That is, self is considered a dependent
     * of the object pointed to by \a p. The pointer \a d is available
     * to pass client data.
     *
     * @note
     * Changes to the client list <i>must not</i> happen during a
     * call to updateFrom().
     */
    virtual void          updateFrom(RWModel* p, void* d) = 0;

};


#endif  /* RW_TOOLS_MODEL_H */
