#!/usr/bin/env python
# -*-coding:utf-8-*-

import urllib
import urllib2
import re
import threading
import time

def getVideoUrls(raw_url, f='normal'):
    url = "http://www.flvcd.com/parse.php?format=%s&kw=%s" % (f, raw_url)
    response = urllib2.urlopen(url)
    the_page = response.read().decode("gbk")

    re_qb = re.compile((r'(?<=<a href=).*? (?=class="link")'))
    re_mulqb = re.compile((r'(?<=<BR><a href=).*? (?=target)'))

    videourl = re_mulqb.findall(the_page)
    if not videourl:
        videourl = re_qb.findall(the_page)
    count = len(videourl)
    if count == 0:
        return 0
    else:
        f = open("./.playlist",'w')
        for i in range(count):
            print >> f,videourl[i].split("\"")[1]
        f.close()
        return 1

def OpenUrl(url):
    user_agent='Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:16.0) Gecko/20100101 Firefox/16.0'
    headers={'User-Agent' : user_agent}
    values = {'name' : 'MeiZhaorui(Mason)',
              'location' : 'China',
              'language' : 'Python2.7' }
    data = urllib.urlencode(values)
    req = urllib2.Request(url, data,headers)
    response = urllib2.urlopen(req)
    return response.read()

def downUrl(the_page, re_qb, key=-2, source=1):
    videourl = re_qb.findall(the_page)
    table_aa = {}
    for i in range(len(videourl)):
        tmp = videourl[i].strip().split("\"")
        table_aa[tmp[key]] = tmp[source]
    return table_aa  # table

def video_url(the_page):
    re_qb = re.compile(r'(?<=<a href=)"http://www\.youku\.com/show_page.*?"(?=></a>)')
    re_qc = re.compile(r'(?<=<a href=)"http://v\.youku\.com/v_show/.*?"(?=></a>)')
    table = downUrl(the_page, re_qc)
    if not table:
        table = downUrl(the_page, re_qb)
    return table

def video_img(the_page):
    re_qb = re.compile(r'(?<=<img src=)"http://\w+\.ykimg\.com.*?(?=>)', re.DOTALL)
    return downUrl(the_page, re_qb)

def getplayUrl(url):
    the_page = OpenUrl(url)
    re_db1 = re.compile('(?<=href=")http://v\.youku\.com/v_show/.*?(?=" target="_blank"><em>播放正片</em></a>)')
    re_db = re.compile('(?<=href=")http://v\.youku\.com/v_show/.*?(?=" target="_blank"><em>播放.*?</a>)')
    videourl = re_db1.findall(the_page)
    if not videourl:
        videourl = re_db.findall(the_page)
    return videourl

#TV
def get_tv_all(url):
    the_page = OpenUrl(url)
    re_qb = re.compile("(?<=title=)\"第[0-9]{,3}.*?<a class=.*?(?=>)", re.DOTALL)
    table = downUrl(the_page, re_qb, 1, -2)
    L = []
    for i in table:
        L.append(i+"$$"+table[i])
    return L
    
#zy
def get_zy_all(url):
    the_page = OpenUrl(url)
    re_qb = re.compile('(?<=class="program").*?\.html', re.DOTALL)
    table = downUrl(the_page, re_qb, 3, -1)
    L=[]
    for i in sorted(table):
        L.append(i+"$$"+table[i])
    return L
#comic
def get_comic_all(url):
    the_page = OpenUrl(url)
    re_qb = re.compile('(?<=<a class="A").*?播放', re.DOTALL)
    table = downUrl(the_page, re_qb, 10, 1)
    L=[]
    for i in sorted(table):
        L.append(i.split('>')[1].split('<')[0]+"$$"+table[i])
    return L

def getGotoPage(the_page):
    re_db = re.compile("(?<=location\.href =).*?(?=\+)")
    videourl = re_db.findall(the_page)
    return 'http://www.youku.com' + videourl[0].split("'")[1]

def getShowList():
    re_qb = re.compile('(?<=<li><a  href=").*?(?=</a>)')
    the_page = OpenUrl("http://www.youku.com/v/");
    table = downUrl(the_page, re_qb, -1, 0)
    L = []
    L.append("http://www.youku.com" + table['>电视剧'][:-5])
    L.append("http://www.youku.com" + table['>电影'  ][:-5])
    L.append("http://www.youku.com" + table['>综艺'  ][:-5])
    L.append("http://www.youku.com" + table['>音乐'  ][:-5])
    L.append("http://www.youku.com" + table['>动漫'  ][:-5])    
    return L


def GetUrlByname(name, locate, classes, time):
    return name + '_g_' + classes + '_a_' + locate + '_s_1_d_1_r_' + time + '.html'


class mythread(threading.Thread):
    def __init__(self,the_page):
        threading.Thread.__init__(self)
        self.the_page = the_page

    def run(self):
        self.table1 = video_img(self.the_page)

    def mcheck(self):
        while self.is_alive():
            pass
        return self.table1

class mythread1(threading.Thread):
    def __init__(self,the_page):
        threading.Thread.__init__(self)
        self.the_page = the_page

    def run(self):
        self.table1 = video_url(self.the_page)

    def mcheck(self):
        while self.is_alive():
            pass
        return self.table1

def connect_img_url(url):
    the_page = OpenUrl(url)
    t = mythread(the_page)
    t1 = mythread1(the_page)
    t.setDaemon(True)
    t.start()
    t1.setDaemon(True)
    t1.start()

    L = []
    L.append(getGotoPage(the_page))
    
    table1 = t.mcheck()
    table2 = t1.mcheck()   
    for i in table1:
        L.append(i+"$$"+table1[i]+"$$"+table2[i])
    return L
