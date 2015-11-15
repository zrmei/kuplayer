#!/usr/bin/env python
# -*-coding:utf-8-*-

import urllib
import urllib2
import re
import threading
import time
import base64
import json

# from py_kuplayer import *
# L = get_html("http://box.zhangmen.baidu.com/x?op=12&count=1&title=最炫民族风$$凤凰传奇$$$$")
# print L

            

fake_headers = {
    'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
    'Accept-Charset': 'UTF-8,*;q=0.5',
    'Accept-Language': 'en-US,en;q=0.8',
    'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:13.0) Gecko/20100101 Firefox/13.0'
}

def get_html(url):
    req = urllib2.Request(url,headers=fake_headers)
    response = urllib2.urlopen(req)
    return response.read()


class Youku():

    def __init__(self, *args):
        self.url = None
        self.vid = None
        self.streams = {}
        # self.audiolang = None

    stream_types = [
        {'id': 'hd2', 'container': 'flv'},
        {'id': 'mp4', 'container': 'mp4'},
        {'id': 'flv', 'container': 'flv'}
    ]

    @staticmethod
    def generate_ep(vid, ep):
        f_code_1 = 'becaf9be'
        f_code_2 = 'bf7e5f01'

        def trans_e(a, c):
            f = h = 0
            b = list(range(256))
            result = ''
            while h < 256:
                f = (f + b[h] + ord(a[h % len(a)])) % 256
                b[h], b[f] = b[f], b[h]
                h += 1
            q = f = h = 0
            while q < len(c):
                h = (h + 1) % 256
                f = (f + b[h]) % 256
                b[h], b[f] = b[f], b[h]
                if isinstance(c[q], int):
                    result += chr(c[q] ^ b[(b[h] + b[f]) % 256])
                else:
                    result += chr(ord(c[q]) ^ b[(b[h] + b[f]) % 256])
                q += 1

            return result

        e_code = trans_e(f_code_1, base64.b64decode(bytes(ep)))
        sid, token = e_code.split('_')
        new_ep = trans_e(f_code_2, '%s_%s_%s' % (sid, vid, token))
        return base64.b64encode(bytes(new_ep)), sid, token

    @staticmethod
    def parse_m3u8(m3u8):
        return re.findall(r'(http://[^?]+)\?ts_start=0', m3u8)

    @staticmethod
    def match_vid(text, pattern):
        match = re.search(pattern, text)
        if match:
            return match.group(1)
        else:
            return None

    @staticmethod
    def get_vid_from_url(url):
        return Youku.match_vid(url, r'youku\.com/v_show/id_([a-zA-Z0-9=]+)')

    def prepare(self, **kwargs):
        assert self.url or self.vid

        if self.url and not self.vid:
            self.vid = self.__class__.get_vid_from_url(self.url)

        playlist = json.loads(
        	get_html('http://v.youku.com/player/getPlayList/VideoIDS/%s/Pf/4/ctype/12/ev/1' % self.vid)
        	)
        playlistdata0 = playlist['data'][0]

        self.ep = playlistdata0['ep']
        self.ip = playlistdata0['ip']

        for stream_type in self.stream_types:
            if stream_type['id'] in playlistdata0['streamsizes']:
                stream_id = stream_type['id']
                stream_size = int(playlistdata0['streamsizes'][stream_id])
                self.streams[stream_id] = {'container': stream_type['container'],  'size': stream_size}
                
    def extract(self, **kwargs):
        if 'stream_id' in kwargs and kwargs['stream_id']:
            stream_id = kwargs['stream_id']

            if stream_id not in self.streams:
                exit(2)
        else:
            stream_id = "flv"

        new_ep, sid, token = self.__class__.generate_ep(self.vid, self.ep)
        m3u8_query = urllib.urlencode(dict(
            ctype=12,
            ep=new_ep,
            ev=1,
            keyframe=1,
            oip=self.ip,
            sid=sid,
            token=token,
            ts=int(time.time()),
            type=stream_id,
            vid=self.vid,
        ))
        m3u8_url = 'http://pl.youku.com/playlist/m3u8?' + m3u8_query
  
        m3u8 = get_html(m3u8_url)
        self.streams[stream_id]['src'] = self.parse_m3u8(m3u8)


    def getVideoUrls(self, url, **kwargs):
        self.url = url
        self.prepare(**kwargs)
        self.extract(**kwargs)

        return self.download(**kwargs)

    def download(self, **kwargs):
        if 'stream_id' in kwargs and kwargs['stream_id']:
            stream_id = kwargs['stream_id']
        else:
            stream_id = 'flv'

        urls = self.streams[stream_id]['src']

        return urls



def getVideoUrls(url,f):
    site = Youku()
    return site.getVideoUrls(url,stream_id=f)


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
    the_page = get_html(url)
    re_db1 = re.compile('(?<=class="btnShow btnplay" href=")http://v\.youku\.com/v_show/.*?(?=\?from)')
    re_db = re.compile('(?<=href=")http://v\.youku\.com/v_show/.*?(?=\?from)')
    videourl = re_db1.findall(the_page)
    if not videourl:
        videourl = re_db.findall(the_page)
    return videourl

#TV
def get_tv_all(url):
    the_page = get_html(url)
    re_qb = re.compile("(?<=title=)\"第[0-9]{,3}.*?<a class=.*?\.html", re.DOTALL)
    table = downUrl(the_page, re_qb, 1, -1)
    L = []
    for i in table:
        L.append(i+"$$"+table[i])
    return L

# print(get_tv_all("http://v.youku.com/v_show/id_XMTI1ODc5MjU2NA==.html"))

#zy
def get_zy_all(url):
    the_page = get_html(url)
    re_qb = re.compile('(?<=class="program").*?\.html', re.DOTALL)
    table = downUrl(the_page, re_qb, 3, -1)
    L=[]
    for i in sorted(table):
        L.append(i+"$$"+table[i])
    return L
#comic
def get_comic_all(url):
    the_page = get_html(url)
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
    the_page = get_html("http://www.youku.com/v/");
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
    the_page = get_html(url)
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