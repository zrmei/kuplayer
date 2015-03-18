/*
   Copyright (C) 2015 MeiZhaorui(Mason) <listener_mei@163.com>
   
   The File is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   The File is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with the Library; if not, see
   <http://www.gnu.org/licenses/>.
*/

#ifndef PYSCRIPT_H_H
#define PYSCRIPT_H_H

#include "common.h"

class QStringList;
class QString;
#include <QMap>
#include <mutex>

#include <boost/python.hpp>
#include <python2.7/Python.h>

#ifndef PYTHON_DONOT_CATCH_EXCEPTION
#define PYTHON_CATCH_EXCEPTION_BEGIN try{
#define PYTHON_CATCH_EXCEPTION_END(return_value) }catch(boost::python::error_already_set){\
        printf("\n=================================================================\n"\
               "The [%d] line function[%s] in file[%s] has error\n",__LINE__,__func__,__FILE__); PyErr_Print();\
        printf("\n=================================================================\n");\
        return return_value;}
#else
#define PYTHON_CATCH_EXCEPTION_BEGIN
#define PYTHON_CATCH_EXCEPTION_END
#endif
namespace Python
{

class pyinit final
{
    Q_DISABLE_COPY(pyinit)
public:
    pyinit(int initsigs = 1) {
        assert(initsigs == 0 || initsigs == 1);
        Py_InitializeEx(initsigs);
        PyEval_InitThreads();
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('../pyscript/')");
        _module = PyImport_ImportModule("py_kuplayer");
    }
    ~pyinit() { /*Py_Finalize(); */}
    inline bool isInitialized() { return Py_IsInitialized();}
    static void err_print() { PyErr_Print(); }
    inline PyObject *module() { return _module; }
private:
    PyObject *_module;
};

class PyThreadStateLock final
{
    Q_DISABLE_COPY(PyThreadStateLock)
public:
    PyThreadStateLock() { state = PyGILState_Ensure(); }
    ~PyThreadStateLock() { PyGILState_Release(state); }
private:
    PyGILState_STATE state;
};

}
KUPLAYER_NAMESPACE_BEGIN //namespace begin

class PyScript final
{
    Q_DISABLE_COPY(PyScript)

    using dict = boost::python::dict;
    using list = boost::python::list;
public:
    PyScript();
    ~PyScript();
    bool getShowList();
    bool GetVideoUrls(QString keyurl, QString format);

    QStringList connect_img_url(QString, QString);
    QStringList getUrlByName(CLASS, QString, QString, QString);
    QStringList gotoNextPage(QString name, int index);
    QStringList getplayUrl(QString);
    QStringList getAll(CLASS, QString);

    QStringList show_list;
private:
    Python::pyinit *init_;
    PyObject *module;
    QStringList return_list;
    QMap<QString, QString> next_page_;

    std::mutex mu;
};

KUPLAYER_NAMESPACE_END // namespace end
#endif // PYSCRIPT_H_H
