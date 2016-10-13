/************************* (C) COPYRIGHT 2016 J&DSoft **************************
* File Name          : NonRecursiveCall.h
* Author             : Litvin Yuriy
* TS Idea            : 2016.06.07 09:22:13
* TS Version         : 2016.10.13 02:51:43
* Description        : This file exports macro M_IF_CALL_IS_RECURSIVE
*                      for detecting and prevention of recursive calls.
*******************************************************************************/

#ifndef JDSOFT_MAIN_QT_NON_RECURSIVE_CALL_H
#define JDSOFT_MAIN_QT_NON_RECURSIVE_CALL_H

#include <QAtomicInteger>


#ifdef Q_ATOMIC_INT8_TEST_AND_SET_IS_ALWAYS_NATIVE

class NonRecursiveCall
{
public:
    inline NonRecursiveCall(QAtomicInteger<qint8> &store) : m_store(store), m_entered(false) { m_entered = m_store.testAndSetAcquire(0, 1); }
    inline bool IsEntered() const { return m_entered; }
    inline ~NonRecursiveCall() { if (m_entered) m_store.testAndSetRelease(1, 0); }

private:
    QAtomicInteger<qint8> &m_store;
    bool m_entered;
};

#define M_IF_CALL_IS_RECURSIVE() static QAtomicInteger<qint8> jd_static_store_20160607092213(0);\
    const NonRecursiveCall jd_scoped_store_20160607092213(jd_static_store_20160607092213);\
    if (!jd_scoped_store_20160607092213.IsEntered())

#else // Q_ATOMIC_INT8_TEST_AND_SET_IS_ALWAYS_NATIVE
#error "On this machine this macros will not work for you. You can improve it and share it ;-)"
// for C++11x we can use std::atomic_bool
#endif // Q_ATOMIC_INT8_TEST_AND_SET_IS_ALWAYS_NATIVE

//!// Example of using:
//!void MainWindow::on_textEdit_textChanged()
//!{
//!        M_IF_CALL_IS_RECURSIVE()
//!        {
//!                qDebug() << "Recursion is detected";
//!                return;
//!        }
//!
//!        // other work with protection of recursive calls
//!}

#endif // JDSOFT_MAIN_QT_NON_RECURSIVE_CALL_H
/************************* (C) COPYRIGHT 2016 J&DSoft *************************/
