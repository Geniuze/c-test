#ifndef DEF_H
#define DEF_H

#include <time.h>
#include <signal.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <fstream>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <vector>
#include <sys/time.h>
#include "src/jmutex.h"
#include "src/jthread.h"
#include "iconv.h"
#include "regex.h"
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/route.h>
#include <linux/netlink.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <net/if_arp.h>
#include <asm/types.h>
#include <netinet/ether.h>
#include <linux/rtnetlink.h>
#include <openssl/md5.h>

using namespace std;
using namespace jthread;
#define tcout \
    cout << __func__ << "\t:\t"

struct ITest
{
public :

    ITest(){}
    virtual ~ITest(){}
    virtual int Release()=0;
};

class CTest : public ITest
{
public :
    CTest()
    {
        tcout << "new CTest()" << endl;
    }
    ~CTest()
    {
        tcout << "free CTest()" << endl;
    }
    int Release()
    {
        delete this;
        tcout << "    "<< endl;
        return 0;
    }
};
//#pragma pack(push)
//#pragma pack(1)


#define DNS_TYPE_A      1
#define DNS_TYPE_NS     2
#define DNS_TYPE_CNAME  5
#define DNS_TYPE_SOA    6
#define DNS_TYPE_WKS    11
#define DNS_TYPE_PTR    12
#define DNS_TYPE_HINFO  13
#define DNS_TYPE_MX     15
#define DNS_TYPE_AAAA   28
#define DNS_TYPE_AXFR   252
#define DNS_TYPE_ANY    255

struct SDnsPkt
{
    unsigned short transaction_id;
    union{
        unsigned short flags;
        struct{
            unsigned int reply_code:4;
            unsigned int non_authenticated_data:1;
            unsigned int answer_authenticated:1;
            unsigned int reserved:1;
            unsigned int recursion_available:1;
            unsigned int recursion_desired:1;
            unsigned int truncated:1;
            unsigned int authoritative:1;
            unsigned int opcode:4;
            unsigned int response:1;

        }__attribute__((packed));
    }__attribute__((packed));
    unsigned short questions;
    unsigned short answer_rrs;
    unsigned short authority_rrs;
    unsigned short additional_rrs;
}__attribute__((packed));

struct SDnsQuery
{
    char name[256];
    unsigned int name_len;
    unsigned short type;
    unsigned short dns_class;
    unsigned int length;
    int (*parse) (SDnsQuery* pthis, char *data, unsigned int len);
}__attribute__((packed));
struct SDnsAnswer
{
    unsigned short name_pos;
    unsigned short type;
    unsigned short dns_class;
    unsigned int live_time;
    unsigned short data_length;
    union
    {
        char primaryname[256];
        char addr[256];
    }__attribute__((packed));
    unsigned int length;
    int (*parse) (SDnsAnswer *pthis, char *data, unsigned int len);
}__attribute__((packed));

//#pragma pack(pop)

#define BUF_LEN 8


template<typename _CharT, typename _Traits = std::char_traits<_CharT> >

class CMyStream : public basic_streambuf<_CharT, _Traits>
{
private:
    static CMyStream<_CharT, _Traits> * _instance;
    _CharT *_pBuf;
    streamsize _offset;
public:
    CMyStream()
    {
        _pBuf = new _CharT[BUF_LEN];
        bzero(_pBuf, BUF_LEN);
        _offset = 0;
    }
    typedef typename basic_streambuf<_CharT, _Traits>::int_type int_type;
    typedef typename basic_streambuf<_CharT, _Traits>::char_type char_type;

    virtual int_type overflow(int_type c = _Traits::eof())
    {
        if (('\n' == c) || (BUF_LEN-1 == _offset))
        {
            fprintf(stdout, "%s", _pBuf);
            _offset = 0;
            bzero(_pBuf, BUF_LEN);
        }


        *(_pBuf + _offset++) = c;
        return c;
    }
    static CMyStream<_CharT, _Traits> * getInstance()
    {
        if (!_instance)
            _instance = new CMyStream<_CharT, _Traits>;
        return _instance;
    }

};

class MyThread : public JThread
{
public:
    MyThread(){}
    virtual void * Thread()
    {
        JThread::ThreadStarted();
        while(true)
        {
            cout << "threadid[" << pthread_self() << "]" << endl;
            sleep(1);
            //usleep(100000);
        }
        return 0;
    }
};
typedef struct{
    int spe_num;
    char spe_str[32];
}DataMap;

struct request_rt{
    struct nlmsghdr nlhdr;
    struct rtmsg rtm;
};

struct route_info{
 u_int dstAddr;
 u_int srcAddr;
 u_int gateWay;
 char ifName[IF_NAMESIZE];
};
#define WX_OPENID_STR "openid"
#define WX_PUBLICID_STR "token"
struct wx_arg{
    char wx_openid[64];
    char wx_publicid[64];
};
#endif // DEF_H
