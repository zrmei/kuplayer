#ifndef GLOAL_H
#define GLOAL_H
#include "common.h"
KUPLAYER_NAMESPACE_BEGIN //namespace begin

template<typename T>
inline void set_no_margin(T *layout)
{
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
}
template<typename T>
inline void delete_list(T *store)
{
    while (store->size()) {
        delete store->at(0);
        store->removeAt(0);
    }

    delete store;
}

inline QString msg_font_style(const QString &text)
{
    QString tmp;
    tmp.insert(0, "<span style=\"font-family:'SimSun'; color:#ff0000;\"><h2>");
    tmp.insert(tmp.size(), text);
    tmp.insert(tmp.size(), "</h2></span>");
    return std::move(tmp);
}

KUPLAYER_NAMESPACE_END // namespace end
#endif // GLOAL_H
