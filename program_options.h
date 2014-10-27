/**************************************
*     MadeBy : MeiZhaorui(Mason)
*     E-Mail : listener_mei@163.com
*       Date : 2014/10/20
**************************************/
#ifndef PROGRAM_OPTIONS_H
#define PROGRAM_OPTIONS_H

char* VERSION =
"\t****************************************\n\
\t*     MadeBy : MeiZhaorui(Mason)       *\n\
\t*     E-Mail : listener_mei@163.com    *\n\
\t*       Date : 2014/10/20              *\n\
\t****************************************\n";
class QStringList;
namespace opt{
bool complie(int argc,char *argv[],char* version);
}

#endif // PROGRAM_OPTIONS_H
