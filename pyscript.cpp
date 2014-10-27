/********************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/13
 ********************************************/

#ifndef PYSCRIPT_CPP
#define PYSCRIPT_CPP

#include "pyscript.h"
#include <memory>

using namespace boost;
using namespace boost::python;

PyScript::PyScript()
    : init_(new pyinit)
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
    std::shared_ptr<PyThreadStateLock> pylock;
    int i{0};
    PYTHON_CATCH_EXCEPTION_BEGIN
    i = python::call_method<int>(module,"getVideoUrls",
                                 keyurl.toStdString().c_str(),
                                 format.toStdString().c_str()
                                 );
    PYTHON_CATCH_EXCEPTION_END

    return i == 0 ? false : true;
}

bool PyScript::getShowList()
{
    list tmp;
    try{
        tmp = python::call_method<list>(module,"getShowList");
    }catch(boost::python::error_already_set){
        return false;
    }
    for(ssize_t i= 0; i<python::len(tmp);++i){
        show_list.append(QString(extract<char*>(tmp[i])));
    }
    return true;
}

QStringList
PyScript::getUrlByName(CLASS name, QString locate, QString classes, QString time)
{
    std::lock_guard<std::mutex> lock(mu);
    std::shared_ptr<PyThreadStateLock> pylock;
    python::str p("");
    PYTHON_CATCH_EXCEPTION_BEGIN
            p = python::call_method<str>(module,"GetUrlByname"
                                         ,show_list[name].toStdString().c_str()
                                         ,locate.toStdString().c_str()
                                         ,classes.toStdString().c_str()
                                         ,time.toStdString().c_str());
    PYTHON_CATCH_EXCEPTION_END
            QStringList tmp ;
    tmp << QString(extract<char*>(p));
    return tmp;
}

QStringList PyScript::connect_img_url(QString url, QString name)
{
    std::lock_guard<std::mutex> lock(mu);
    std::shared_ptr<PyThreadStateLock> pylock;

    QStringList play_list;
    list tmp ;
    PYTHON_CATCH_EXCEPTION_BEGIN
            tmp= python::call_method<list>(module
                                           ,"connect_img_url"
                                           ,url.toStdString().c_str());
    PYTHON_CATCH_EXCEPTION_END

    if(python::len(tmp)){
        if(next_page_.contains(name)){
            next_page_.remove(name);
        }
        next_page_.insert(name,QString(extract<char*>(tmp[0])));
        for(ssize_t i = 1; i< python::len(tmp); ++i){
            play_list.append(QString(extract<char*>(tmp[i])));
        }
    }
    return play_list;
}

QStringList PyScript::gotoNextPage(QString name, int index)
{
    std::lock_guard<std::mutex> lock(mu);
    std::shared_ptr<PyThreadStateLock> pylock;

    QStringList tmp;
    PYTHON_CATCH_EXCEPTION_BEGIN
            tmp  << next_page_.value(name)+QString::number(index)+".html";
    PYTHON_CATCH_EXCEPTION_END

            return tmp;
}

QStringList PyScript::getplayUrl(QString url)
{
    std::lock_guard<std::mutex> lock(mu);
    std::shared_ptr<PyThreadStateLock> pylock;

    QStringList tmp;
    python::list sstr;
    PYTHON_CATCH_EXCEPTION_BEGIN
            sstr = python::call_method<list>(module
                                             ,"getplayUrl"
                                             ,url.toStdString().c_str());
    PYTHON_CATCH_EXCEPTION_END
    if(python::len(sstr)){
           tmp <<QString(extract<char*>(sstr[0]));
    }else{
        tmp << "";
    }

    return tmp;
}

QStringList PyScript::getAll(CLASS classes,QString url)
{
    std::lock_guard<std::mutex> lock(mu);
    std::shared_ptr<PyThreadStateLock> pylock;
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
        return QStringList();
    }
    QStringList tmp;
    python::list sstr;
    PYTHON_CATCH_EXCEPTION_BEGIN
            sstr = python::call_method<list>(module
                                             ,func.toStdString().c_str()
                                             ,url.toStdString().c_str());
    PYTHON_CATCH_EXCEPTION_END
    for(ssize_t i=0;i<python::len(sstr); ++i){
        tmp << QString(extract<char*>(sstr[i]));
    }
    return tmp;
}

#endif //PYSCRIPT_CPP
