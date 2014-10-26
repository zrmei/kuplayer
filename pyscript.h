/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/
#ifndef PYSCRIPT_H_H
#define PYSCRIPT_H_H
#include "common.h"

class QStringList;
class QString;
#include <QMap>
#include <boost/noncopyable.hpp>
#include <boost/python.hpp>
#include <python2.7/Python.h>

#ifndef PYTHON_DONOT_CATCH_EXCEPTION
#define PYTHON_CATCH_EXCEPTION_BEGIN try{
#define PYTHON_CATCH_EXCEPTION_END }catch(boost::python::error_already_set){std::lock_guard<std::mutex> lock(io_mu);\
printf("\n=================================================================\nThe [%d] line in file\
[%s] has error",__LINE__,__FILE__); PyErr_Print();\
printf("\n=================================================================\n");}
#else
#define PYTHON_CATCH_EXCEPTION_BEGIN
#define PYTHON_CATCH_EXCEPTION_END
#endif

class pyinit final : boost::noncopyable
{
public:
    pyinit(int initsigs = 1)
    {
        assert(initsigs == 0 || initsigs == 1);
        Py_InitializeEx(initsigs);
        PyEval_InitThreads();
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('./pyscript/')");
        _module = PyImport_ImportModule("py_kuplayer");
    }
    ~pyinit(){ /*Py_Finalize();*/ }
    inline bool isInitialized() { return Py_IsInitialized();}
    static void err_print() { PyErr_Print(); }
    inline PyObject* module() { return _module; }
private:
    PyObject* _module;
};

class PyThreadStateLock final : boost::noncopyable
{
public:
    PyThreadStateLock(){ state = PyGILState_Ensure( ); }
    ~PyThreadStateLock(){ PyGILState_Release( state ); }
private:
    PyGILState_STATE state;
};

#include <mutex>
class PyScript final : boost::noncopyable
{
public:
    using dict = boost::python::dict;
    using list = boost::python::list;

public:
    PyScript();
    ~PyScript();
    bool GetVideoUrls(QString keyurl,QString format);
    QStringList connect_img_url(QString,QString);
    bool getShowList();
    QStringList getUrlByName(CLASS,QString,QString,QString);
    QStringList gotoNextPage(QString name,int index);
    QStringList getplayUrl(QString);
    QStringList getAll(CLASS,QString);
    QStringList show_list;

private:
    pyinit *init_;
    PyObject *module;
    QMap<QString,QString> next_page_;

    std::mutex mu;
    std::mutex io_mu;
};
#endif // PYSCRIPT_H_H
