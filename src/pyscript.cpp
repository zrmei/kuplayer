/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/

#ifndef PYSCRIPT_CPP
#define PYSCRIPT_CPP

#include "pyscript.h"
USR_NAMESPACE_KUPLAYER //using namespace mei::kuplayer

#include <memory>

using namespace boost;
using namespace boost::python;

PyScript::PyScript()
    : init_(new Python::pyinit)
{
    module = init_->module();
}
PyScript::~PyScript()
{
    delete init_;
}

bool PyScript::GetVideoUrls(QString keyurl,QString format)
{
    std::lock_guard<std::mutex> lock(mu);
    std::shared_ptr<Python::PyThreadStateLock> pylock;
    int i{0};
    PYTHON_CATCH_EXCEPTION_BEGIN
    i = python::call_method<int>(module,"getVideoUrls",
                                 keyurl.toStdString().c_str(),
                                 format.toStdString().c_str()
                                 );
    PYTHON_CATCH_EXCEPTION_END(false)

    return i == 0 ? false : true;
}

bool PyScript::getShowList()
{
    std::lock_guard<std::mutex> lock(mu);
    std::shared_ptr<Python::PyThreadStateLock> pylock;
    
    list tmp;
    PYTHON_CATCH_EXCEPTION_BEGIN
        tmp = python::call_method<list>(module,"getShowList");
    PYTHON_CATCH_EXCEPTION_END(false)
    for(ssize_t i= 0; i<python::len(tmp);++i){
        show_list.append(QString(extract<char*>(tmp[i])));
    }
    qDebug() << show_list;
    return true;
}

QStringList
PyScript::getUrlByName(CLASS name, QString locate, QString classes, QString time)
{
    std::lock_guard<std::mutex> lock(mu);
    std::shared_ptr<Python::PyThreadStateLock> pylock;
    
    python::str p("");
    return_list.clear();
    PYTHON_CATCH_EXCEPTION_BEGIN
            p = python::call_method<str>(module,"GetUrlByname"
                                         ,show_list[name].toStdString().c_str()
                                         ,locate.toStdString().c_str()
                                         ,classes.toStdString().c_str()
                                         ,time.toStdString().c_str());
    PYTHON_CATCH_EXCEPTION_END(return_list)
    return_list << QString(extract<char*>(p));
    return return_list;
}

QStringList PyScript::connect_img_url(QString url, QString name)
{
    std::lock_guard<std::mutex> lock(mu);
    std::shared_ptr<Python::PyThreadStateLock> pylock;
    
    return_list.clear();
    list tmp ;
    PYTHON_CATCH_EXCEPTION_BEGIN
    tmp= python::call_method<list>(module
                                   ,"connect_img_url"
                                   ,url.toStdString().c_str());
    PYTHON_CATCH_EXCEPTION_END(return_list)

    if(python::len(tmp)){
        if(next_page_.contains(name)){
            next_page_.remove(name);
        }
        next_page_.insert(name,QString(extract<char*>(tmp[0])));
        for(ssize_t i = 1; i< python::len(tmp); ++i){
            return_list.append(QString(extract<char*>(tmp[i])));
        }
    }
    return return_list;
}

QStringList PyScript::gotoNextPage(QString name, int index)
{
    std::lock_guard<std::mutex> lock(mu);
    std::shared_ptr<Python::PyThreadStateLock> pylock;
    
    return_list.clear();
    PYTHON_CATCH_EXCEPTION_BEGIN
    return_list  << next_page_.value(name)+QString::number(index)+".html";
    PYTHON_CATCH_EXCEPTION_END(return_list)
    
    return return_list;
}

QStringList PyScript::getplayUrl(QString url)
{
    std::lock_guard<std::mutex> lock(mu);
    std::shared_ptr<Python::PyThreadStateLock> pylock;
    
    return_list.clear();
    python::list sstr;
    PYTHON_CATCH_EXCEPTION_BEGIN
    sstr = python::call_method<list>(module
                                     ,"getplayUrl"
                                     ,url.toStdString().c_str());
    PYTHON_CATCH_EXCEPTION_END(return_list)
    if(python::len(sstr)){
           return_list <<QString(extract<char*>(sstr[0]));
    }else{
        return_list << "";
    }
    return return_list;
}

QStringList PyScript::getAll(CLASS classes,QString url)
{
    std::lock_guard<std::mutex> lock(mu);
    std::shared_ptr<Python::PyThreadStateLock> pylock;
    return_list.clear();
    QString func;
    switch (classes) {
    case TV:
        func = "get_tv_all";
        break;
    case ZONGYI:
        func = "get_zy_all";
        break;
    case COMIC:
        func = "get_comic_all";
        break;
    default:
        return return_list;
    }
    
    python::list sstr;
    PYTHON_CATCH_EXCEPTION_BEGIN
    sstr = python::call_method<list>(module
                                     ,func.toStdString().c_str()
                                     ,url.toStdString().c_str());
    PYTHON_CATCH_EXCEPTION_END(return_list)
    for(ssize_t i=0;i<python::len(sstr); ++i){
        return_list << QString(extract<char*>(sstr[i]));
    }
    return return_list;
}

#endif //PYSCRIPT_CPP
