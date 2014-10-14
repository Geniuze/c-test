

#include "def.h"

template<typename _CharT, typename _Traits>
CMyStream<_CharT, _Traits> * CMyStream<_CharT, _Traits>::_instance = NULL;
char a[] = "abcbcbcbcbcb";
int test_init()
{
    int i(10);
    tcout << "i=" << i << endl;
    return i;
}

int test_const()
{
    int i(10);
    const int *pint = &i;
    tcout << "*pint=" << *pint << endl;\
    /** error */
    /*
    *pint = 20;
    cout << "*pint=" << *pint << endl;
    */

    int itmp = 20;
    pint = &itmp;
    tcout << "*pint=" << *pint << endl;

    /**  double const ptr  **/
    const int * const pcint = &i;
    tcout << "*pcint = " << *pcint << endl;

    return 0;
}

int test_ope_new()
{
    char *p = NULL;
    __int64_t llint = 0xFFFFFFFF;
#if 0
    try{
        p = new char[llint];
    }
    catch(bad_alloc e)
    {
        tcout << "new error" << endl;
        return 1;
    }
#else
    p = new char[llint];
    //p = new(nothrow) char[llint];
    if (NULL == p)
    {
        tcout << "new error nothrow" << endl;
        return 1;
    }
#endif
    //memset(p, 0, 100);
    bzero(p, 100);
    strcpy(p, "new buf");
    tcout << p << endl;
    return 0;
}

typedef basic_string<char>::size_type S_T;
static const S_T npos = -1;
vector<string> split(const string& src, string delimit, string null_subst)
{
    vector<string> v;
    if(src.empty() || delimit.empty())
    {
        return v;
    }

    S_T deli_len = delimit.size();
    S_T cur_pos = npos, last_pos = 0;
    while( (cur_pos = src.find(delimit, last_pos))!= npos )
    {
        if(cur_pos == last_pos)
        {
            v.push_back(null_subst);
        }
        else
        {
            v.push_back(src.substr(last_pos, cur_pos - last_pos));
        }
        last_pos = cur_pos + deli_len;
    }
    string last_one = src.substr(last_pos);
    v.push_back(last_one.empty()? null_subst:last_one);
//	return std::move(v);
    return v;
}
int test_string()
{
    /*
    string str(10, 'c');
    tcout << "str=" << str << endl;

    tcout << "please input your string : " ;
    getline(cin, str);
    tcout << "your string is \'" << str << "\'" << endl;
    tcout << "str.length()=" << str.length() << " and str.size()=" << str.size() << endl;

    string a = "add /config.ini/wlan1/TEST=0;";
    vector<string> vec_items = split(a, ";", "");
    for (vector<string>::iterator it = vec_items.begin(); it != vec_items.end(); it++)
    {
        if ((it->find("/") == string::npos) || (it->find("=") == string::npos))
            continue;
        string path = it->substr(it->find("/"), it->find("=")-it->find("/"));
        string value = it->substr(it->find("=")+1);
        cout << "path:" << path << ";value:" << value << endl;
    }
*/
    /*
    int pos = ap_model.find("AP");
    if (pos != string::npos)
    {
        string tmp = ap_model.substr(0, pos);
        tcout << tmp << endl;
    }
    */
    string str = "/WA718V100R001C06B011.bin";
    str = str.substr(1);
    tcout << str << endl;
    return 0;
}
int test_file_stream()
{
    ofstream out("out", ofstream::trunc | ofstream::app);
    string str("file info");
    tcout << "write to file" << endl;
    out.write(str.c_str(), str.length());
    out.close();
    tcout << str << endl;

    string strRead("");
    tcout << "read from file" << endl;
    ifstream in("out");

    in.seekg(0,ios_base::end);
    int len = in.tellg();
    tcout << "cout = " << len << endl;
    in.seekg(0,ios_base::beg);

    char *read = new char[len+1];
    bzero(read, len+1);
    in.read(read, len);
    in.close();
    strRead = read;
    tcout << strRead << endl;
    return 0;
}

int test_delete_ope()
{
    while(true)
    {
        ITest *test = new CTest();
        test->Release();
        usleep(2000);
    }
    return 0;

}
int test_getHostByName()
{
    struct hostent *he = NULL;
    string name="www.sina.com.cn";
    he = gethostbyname(name.c_str());
    if (NULL == he)
    {
        return -1;
    }
    char buf[1024] = {0};
    int len = sizeof(buf);

    tcout << "name:" << name << endl;
    tcout << "iptype:" << he->h_addrtype << endl;
    for(int i=0; he->h_addr_list[i] != NULL; i++)
    {
        inet_ntop(he->h_addrtype, he->h_addr_list[i], buf, len);
        tcout << "host:" << buf << endl;
        bzero(buf, len);
    }
    return 0;
}
int test_arg(int argc, char **argv)
{
    tcout << "argc:" << argc << endl;
    for (int i = 0; i < argc; i++)
        tcout << "argv[" << i << "]:" << argv[i] << endl;
    return 0;
}
int DnsHead_parse(SDnsPkt *pthis, char *data)
{
    bzero(pthis, sizeof(*pthis));
    SDnsPkt *tmp = (SDnsPkt *)data;

    pthis->transaction_id = htons(tmp->transaction_id);
    pthis->flags = htons(tmp->flags);
    pthis->questions = htons(tmp->questions);
    pthis->answer_rrs = htons(tmp->answer_rrs);
    pthis->authority_rrs = htons(tmp->authority_rrs);
    pthis->additional_rrs = htons(tmp->additional_rrs);
    return 0;
}

int DnsName_parse(char *ret_name, char *pos, int name_len, char *dns_head)
{
    int offset = 0;
    while((*pos != 0) && (offset != name_len))
    {
        if ((unsigned char)(*pos) == 0xc0)
        {
            DnsName_parse(ret_name, dns_head+(htons(*((unsigned short*)pos))-0xc000), name_len, dns_head);
            pos += 2;
            offset += 2;
            return offset;
        }
        else
        {
            *ret_name = '.';
            ++ret_name;
            memcpy(ret_name, pos+1, *pos);
            ret_name += *pos;
            offset += *pos + 1;
            pos += *pos + 1;
        }
    }
    //正常情况会有'\0'出现
    if (*pos == 0)
    {
        offset += 1;
        pos += 1;
    }
    return offset;
}

int DnsQuery_parse(SDnsQuery* pthis,char *data, char *dns_head)
{
    char *pos = data;

    pthis->name_len = DnsName_parse(pthis->name, pos, sizeof(pthis->name), dns_head);
    pos += pthis->name_len;

    pthis->type = htons(*((unsigned short*)pos));
    pos += 2;

    pthis->dns_class = htons(*((unsigned short *)pos));
    pos += 2;

    pthis->length = pos - data;
    return pthis->length;
}
int DnsAnswer_parse (SDnsAnswer *pthis, char *data, char *dns_head)
{
    SDnsAnswer *tmp = (SDnsAnswer *)data;
    char * pos = data;

    pthis->name_pos = htons(tmp->name_pos);
    pos += 2;

    pthis->type = htons(tmp->type);
    pos += 2;

    pthis->dns_class = htons(tmp->dns_class);
    pos += 2;

    pthis->live_time = htonl(tmp->live_time);
    pos += 4;

    pthis->data_length = htons(tmp->data_length);
    pos += 2;

    switch(pthis->type)
    {
    case DNS_TYPE_A:
    {
        memcpy(pthis->addr, pos, pthis->data_length);
        break;
    }
    case DNS_TYPE_CNAME:
    {
        DnsName_parse(pthis->primaryname, pos, pthis->data_length, dns_head);
        break;
    }
    default:break;
    }

    pos += pthis->data_length;

    pthis->length = pos - data;
    return pthis->length;
}

int test_dns(int argc, char ** argv)
{
    char data[] = {0x76, 0x6d, 0x81, 0x80, 0x00, 0x01, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03, 0x77, 0x77, 0x77,
                   0x05, 0x62, 0x61, 0x69, 0x64, 0x75, 0x03, 0x63, 0x6f, 0x6d, 0x00, 0x00, 0x01, 0x00, 0x01, 0xc0,
                   0x0c, 0x00, 0x05, 0x00, 0x01, 0x00, 0x00, 0x04, 0x33, 0x00, 0x0f, 0x03, 0x77, 0x77, 0x77, 0x01,
                   0x61, 0x06, 0x73, 0x68, 0x69, 0x66, 0x65, 0x6e, 0xc0, 0x16, 0xc0, 0x2b, 0x00, 0x01, 0x00, 0x01,
                   0x00, 0x00, 0x00, 0xbd, 0x00, 0x04, 0x6f, 0x0d, 0x64, 0x5b, 0xc0, 0x2b, 0x00, 0x01, 0x00, 0x01,
                   0x00, 0x00, 0x00, 0xbd, 0x00, 0x04, 0x6f, 0x0d, 0x64, 0x5c};

    char *pos = data;
    SDnsPkt tmp;
    bzero(&tmp, sizeof(tmp));

    DnsHead_parse(&tmp, pos);

    //    SDnsPkt *pkt = (SDnsPkt*)&tmp;
    //    tcout << hex << pkt->transaction_id << endl;
    //    tcout << hex << pkt->flags << endl;
    //    tcout << hex << (pkt->flags) << endl;
    //    tcout << hex << pkt->response << endl;
    //    tcout << hex << pkt->opcode << endl;
    //    tcout << hex << pkt->authoritative << endl;

    //    tcout << hex << pkt->truncated << endl;
    //    tcout << hex << pkt->recursion_desired << endl;
    //    tcout << hex << pkt->recursion_available << endl;
    //    tcout << hex << pkt->reserved << endl;
    //    tcout << hex << pkt->answer_authenticated << endl;

    //    tcout << hex << pkt->non_authenticated_data << endl;

    //    tcout << hex << pkt->reply_code << endl;

    //    tcout << hex << pkt->questions << endl;
    //    tcout << hex << pkt->answer_rrs << endl;

    //    tcout << hex << pkt->authority_rrs << endl;
    //    tcout << hex << (pkt->additional_rrs) << endl;

    pos += sizeof(tmp);

    SDnsQuery *query = (SDnsQuery*)malloc(sizeof(*query) * (tmp.questions));
    for (int i = 0; i < (tmp.questions); i++)
    {
        pos += DnsQuery_parse(&query[i], pos, data);
        tcout << hex<< "name=" << query[i].name <<endl;
        //        tcout << hex<< "name_len=" << query[i].name_len <<endl;
        //        tcout << hex<< "type=" << (query[i].type) <<endl;
        //        tcout << hex<< "dns_class=" << (query[i].dns_class) <<endl;

    }
    SDnsAnswer *answer = (SDnsAnswer*)malloc(sizeof(*answer) * tmp.answer_rrs);
    for (int i = 0; i < tmp.answer_rrs; i++)
    {
        pos += DnsAnswer_parse(&answer[i], pos, data);
        //        tcout << hex<< "name_pos=" << answer[i].name_pos << endl;
        //        tcout << hex<< "type=" << answer[i].type << endl;
        //        tcout << hex<< "dns_class=" << answer[i].dns_class << endl;
        //        tcout << hex<< "live_time=" << answer[i].live_time << endl;
        if (DNS_TYPE_A == answer[i].type)
            tcout << dec<< "addr=" << (int)answer[i].addr[0] << "." << (int)answer[i].addr[1] << "." << (int)answer[i].addr[2] << "." << (int)answer[i].addr[3] << endl;
        //        else if (DNS_TYPE_CNAME == answer[i].type)
        //            tcout << hex << "primaryname=" << answer[i].primaryname << endl;
    }

    free(query);
    free(answer);
    return 0;
}


int test_system(int argc, char **argv)
{
    system("while true; do sleep 10; done;");
    return 0;
}

int test_translate(int argc, char ** argv)
{
    string str = "\"\\n\"";

    for (string::iterator it = str.begin(); it != str.end(); it ++)
    {
        tcout << "*it=" << (int)*it << endl;
    }
    return 0;
}

int test_parse_int(int argc, char **argv)
{
    char str[512] = "-90";
    unsigned int i;
    sscanf(str, "%i", &i);
    tcout << "i = " << (int)i << endl;
    sprintf(str, "%x", i);
    tcout << "str = " << str << endl;
    sscanf(str, "%i", &i);
    tcout << "i = " << i << endl;
    tcout << "i = " << (int)i << endl;


    tcout << "i = ";
    printf("%d\n", i);
    return 0;
}

int test_pcap(int argc, char ** argv)
{
    return 0;
}

int test_sizeof(int argc, char **argv)
{
    int *test[10] = {0};
    tcout << "size = " << sizeof(test) << endl;
    return 0;
}
int test_streambuf(int argc, char ** argv)
{
    CMyStream<char> *stream = CMyStream<char>::getInstance();
    cout.rdbuf(stream);

    MyThread t[10];
    for (int i = 0; i < 10; i++)
        t[i].Start();
    cout << "Hello" << endl;
    getchar();
    return 0;
}

int test_time(int argc, char ** argv)
{
    time_t t = time(NULL);
    char tchr[64] = {0};
    strcpy(tchr, ctime(&t));
    tchr[strlen(tchr)-1] = 0;
    printf("time:[%s]\n", tchr);
    return 0;
}

static double division(double dividend, double divisor)
{
    if (!divisor)
        throw divisor;
    return dividend/divisor;
}

int test_try_catch(int argc, char ** argv)
{
    double result = 0.0;
    try{
        result = division(24, 3);
    }
    catch (double e)
    {
        tcout << "error = " << e << endl;
    }
    catch(...){
        tcout << "other error" << endl;
    }

    tcout << "result = " << result << endl;

    char *p = NULL;
    try{
        int i = 0;
        while(i++ < 10)
        {
            tcout << "i = " << i << endl;
            p = new char[0x7fffffff];
        }

    }
    catch(exception e){
        tcout << "i catch it !" << endl;
    }
    return 0;
}
static int hexchar2int(char);

int urldecode(string& dst, string& src)
{
    for (unsigned int i=0; i < src.length(); i++)
    {
        switch(src[i])
        {
        case '%':
        {
            if (isxdigit(src[i+1]) && isxdigit(src[i+2]))
            {
                char c = hexchar2int(src[i+1]);
                c <<= 4;
                c += hexchar2int((src[i+2]));
                i+=2;
                dst.push_back(c);
                break;
            }
            dst.push_back(src[i]);
            break;
        }
        case '+':
        {
            dst.push_back(' ');
            break;
        }
        default:
        {
            dst.push_back(src[i]);
            break;
        }
        }
    }
    return 0;

}
static int hexchar2int(char c){
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    else if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    else
        return -1;
}

int test_urldecode(int argc, char ** argv)
{
    string s = "name=%D2%BB%B6%FE%C8%FD%&a=test test";
    string d("");
    urldecode(d, s);
    tcout << d << endl;
    return 0;
}

int test_iconv(int argc, char **argv)
{
    string src = "中国";
    string dst = "";
    char * dstchar = NULL;
    iconv_t cd  = iconv_open("gb2312", "utf-8");
    char *in, *out;
    unsigned int lin, lout;
    in = const_cast<char *>(src.c_str());
    cout << "in=" << in << endl;
    lin = src.length();
    lout = lin * 6;
    out = (char *)malloc(lout);
    if (NULL == out)
        return -1;
    memset(out, 0, lout);
    dstchar = out;
    cout << "iconv return = " << iconv(cd, &in, &lin, &out, &lout) << endl;
    iconv_close(cd);

    dst.assign(dstchar);
    cout << "lout = "<< lout << ";dst = " << dst << ";ldst = " << dst.length() << endl;
    free(dstchar);
    return 0;
}

int test_endian(int argc, char ** argv)
{
    unsigned int a=0x12345678;
    char *p = (char *)&a;
    unsigned int b = htonl(a);
    p = (char *)&b;
    tcout << hex << "p:" << (int)p[0] << (int)p[1] << (int) p[2] << (int)p[3] << endl;
    return 0;
}
int test_fizzbuzzwhizz(int argc, char ** argv)
{
    if (argc != 4)
    {
        tcout << "need three arguments..." << endl;
        return 1;
    }
    int a[3] = {0};
    a[0] = atoi(argv[1]);
    a[1] = atoi(argv[2]);
    a[2] = atoi(argv[3]);
    string s[3] = {"fizz","buzz","whizz"};
    string result("");

    tcout << "numbers:" << a[0] << "," << a[1] << "," << a[2] << endl;
    for (int i = 1; i <= 100; i++)
    {
        int tmp = i;
        char value[8] = {0};
        sprintf(value, "%d", i);
        while(tmp)
        {
            result = tmp%10==a[0]?s[0]:"";
            if (!result.empty())
                break;
            tmp /= 10;
        }
        result = !result.empty()?result:(i==a[0]*a[1]*a[2]?s[0]+s[1]+s[2]:i==a[0]*a[1]?s[0]+s[1]:i==a[0]*a[2]?s[0]+s[2]:i==a[1]*a[2]?s[1]+s[2]:"");
        result = !result.empty()?result:(!a[0]?"0":(i%a[0]==0?s[0]:(!a[1]?"0":i%a[1]==0?s[1]:(!a[2]?"0":i%a[2]==0?s[2]:value))));
        tcout << result << endl;
    }
    return 0;
}
int test_fizzbuzzwhizz2(int argc, char ** argv)
{
    int a,b,c,i;
    cin>>a>>b>>c;
    for(i = 1; i <= 100; i++){
        string flag;
        (i%10==a || i/10%10==a || i/100==a) ? (flag = ("Fizz")) : (flag = flag + ((i%a==0)?"Fizz":"") + ((i%b==0)?"Buzz":"") + ((i%c==0)?"Whizz":""));
        (flag.length()>0)?cout<<flag<<endl:cout<<i<<endl;
    }
    return 0;
}

static int fizzbuzzwhizz(int spe_count, DataMap *spe, int data_count, int *datas)
{
    string result("");
    for (int i = 0; i < spe_count; i++)
        cout << "spe_num=" << spe[i].spe_num << ", spe_str=" << spe[i].spe_str << endl;

    for (int i = 0; i < data_count; ++i)
    {
        int tmp = datas[i];
        for (; tmp; tmp /= 10)
            if (tmp % 10 == spe[0].spe_num)
            {
                result = spe[0].spe_str;
                break;
            }
        if (!result.empty())
        {
            tcout << result << endl;
            continue;
        }

    }
    return 0;
}

int Combination(int *arr, int size)
{
    int t = 1<<size;
    cout << "t=" << t << endl;
    for (int i = 1; i < t; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (((i >> j) & 0x01) == 0x01)
            {
                cout << arr[j];
            }
        }
        cout << endl;
    }
    return 0;
}

int test_substr(int argc, char **argv)
{
    string s = "ssssssssssssssssssss";
    string ss("");
    unsigned int pos = 0;
    pos = s.find("---");
    if (pos != s.npos)
        ss = s.substr(pos);
    tcout << ss << endl;
    return 0;
}

int test_getip(int argc, char ** argv)
{
    int err, nm = 10;
    char errbuf[1024] = {0};
    char match[100] = {0};
    regmatch_t pmatch[nm];
    char bematch[1024] = {0};
    const char *head = "inet addr:";
    const char *content = "([0-9.]+)";
    char pattern[100] = {0};
    strcpy(pattern, head);
    strcat(pattern, content);
    regex_t reg;
    FILE *f = fopen("/home/zz/1", "r+");
    if (NULL == f)
    {
        perror("fopen");
        return -2;
    }
    fread(bematch, sizeof(bematch), 1, f);
    tcout << "read:" << bematch << endl;

    fclose(f);
    if (0 > (err = regcomp(&reg, pattern, REG_EXTENDED)))
    {
        regerror(err, &reg, errbuf, sizeof(errbuf));
        tcout << "err:" << errbuf << endl;
        return -1;
    }

    err = regexec(&reg, bematch, nm, pmatch, 0);

    if (err == REG_NOMATCH)
    {
        tcout << "no match.." << endl;
        return -1;
    }
    else if (err)
    {
        regerror(err, &reg, errbuf, sizeof(errbuf));
        tcout << "err:" << errbuf << endl;
        return -1;
    }

    for (int i = 0; i < nm && pmatch[i].rm_so != -1; i++)
    {
        int len = pmatch[i].rm_eo - pmatch[i].rm_so;
        if (len)
        {
            memset(match, 0, sizeof(match));
            memcpy(match, bematch+pmatch[i].rm_so, len);
            if (strstr(match, head) == NULL)
                tcout << "match:" << match << endl;
        }
    }
    return 0;

}
int test_getip_byioctl(int argc, char ** argv)
{
    struct ifreq ifr;
    char buf[100] = {0};

    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > s)
    {
        perror("socket");
        return -1;
    }
    strcpy(ifr.ifr_ifrn.ifrn_name, "eth0");
    if (ioctl(s, SIOCGIFADDR, &ifr) < 0)
    {
        perror("ioctl");
        close(s);
        return -1;
    }

    struct sockaddr_in *sin = (struct sockaddr_in*)&ifr.ifr_ifru.ifru_addr;
    inet_ntop(AF_INET, &sin->sin_addr.s_addr, buf, sizeof(buf));
    tcout << "buf:" << buf << endl;
    close(s);
    return 0;
}

int get_ipv4route(int argc, char ** argv)
{
    int s = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    if (s < 0)
    {
        perror("socket");
        return -1;
    }

    struct sockaddr_nl snl;
    snl.nl_family = AF_NETLINK;
    snl.nl_pid = getpid();
    snl.nl_pad = 0;
    snl.nl_groups = 0;

    if (0 > bind(s, (struct sockaddr*)&snl, sizeof(snl)))
    {
        perror("bind");
        return -1;
    }
    struct request_rt rrt;
    bzero(&rrt.nlhdr, sizeof(rrt.nlhdr));
    rrt.nlhdr.nlmsg_type = RTM_GETROUTE;
    rrt.nlhdr.nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;
    rrt.nlhdr.nlmsg_pid = getpid();
    rrt.nlhdr.nlmsg_seq = 0;

    rrt.rtm.rtm_family = AF_INET;
    rrt.rtm.rtm_table = RT_TABLE_MAIN;

    rrt.nlhdr.nlmsg_len = NLMSG_LENGTH(sizeof(rrt.rtm));
    if (0 > send(s, &rrt, rrt.nlhdr.nlmsg_len, 0))
    {
        perror("send");
        close(s);
        return -1;
    }
    char buf[10240] = {0};
    char *bufPtr = buf;

    int msgLen = 0;
    int readLen = 0;
    struct nlmsghdr *nlHdr;
    do
    {
        /* Recieve response from the kernel */
        if((readLen = recv(s, bufPtr, sizeof(buf) - msgLen, 0)) < 0)
        {
            perror("SOCK READ: ");
            return -1;
        }
        cout << "readLen = " << readLen << endl;

        nlHdr = (struct nlmsghdr *)bufPtr;

        /* Check if the header is valid */
        if((NLMSG_OK(nlHdr, readLen) == 0) || (nlHdr->nlmsg_type == NLMSG_ERROR))
        {
            perror("Error in recieved packet");
            return -1;
        }

        /* Check if the its the last message */
        if(nlHdr->nlmsg_type == NLMSG_DONE)
        {
            break;
        }
        else
        {
            /* Else move the pointer to buffer appropriately */
            bufPtr += readLen;
            msgLen += readLen;
        }

        /* Check if its a multi part message */
        if((nlHdr->nlmsg_flags & NLM_F_MULTI) == 0)
        {
            /* return if its not */
            break;
        }
    }
    while((nlHdr->nlmsg_seq != 0) || (nlHdr->nlmsg_pid != getpid()));

    cout << "msgLen = " << msgLen << endl;
    nlHdr = (struct nlmsghdr *)buf;
    for (; NLMSG_OK(nlHdr, msgLen); nlHdr = NLMSG_NEXT(nlHdr, msgLen))
    {
        cout << "seqnum = " << nlHdr->nlmsg_seq << endl;
        cout << "pid = " << nlHdr->nlmsg_pid << endl;
        cout << "len = " << nlHdr->nlmsg_len << endl;
        cout << "flags = " << hex << nlHdr->nlmsg_flags << endl;
        cout << "one msg done" << endl;

        struct route_info rtInfo;
        struct rtmsg *rtMsg;
        struct rtattr *rtAttr;
        int rtLen;

        rtMsg = (struct rtmsg *)NLMSG_DATA(nlHdr);

        /* If the route is not for AF_INET or does not belong to main routing table then return. */
        if((rtMsg->rtm_family != AF_INET) || (rtMsg->rtm_table != RT_TABLE_MAIN))
            return -1;

        /* get the rtattr field */
        rtAttr = (struct rtattr *)RTM_RTA(rtMsg);
        rtLen = RTM_PAYLOAD(nlHdr);

        for(; RTA_OK(rtAttr,rtLen); rtAttr = RTA_NEXT(rtAttr,rtLen))
        {
            switch(rtAttr->rta_type)
            {
            case RTA_OIF:
                if_indextoname(*(int *)RTA_DATA(rtAttr), rtInfo.ifName);
                cout << "ifname:" << rtInfo.ifName << endl;
                break;

            case RTA_GATEWAY:
                memcpy(&rtInfo.gateWay, RTA_DATA(rtAttr), sizeof(rtInfo.gateWay));
                cout << "gateWay:" << hex << ntohl(rtInfo.gateWay) << endl;
                break;

            case RTA_PREFSRC:
                memcpy(&rtInfo.srcAddr, RTA_DATA(rtAttr), sizeof(rtInfo.srcAddr));
                cout << "srcAddr:" << hex << ntohl(rtInfo.srcAddr) << endl;
                break;

            case RTA_DST:
                memcpy(&rtInfo.dstAddr, RTA_DATA(rtAttr), sizeof(rtInfo.dstAddr));
                cout << "dstAddr:" << hex << ntohl(rtInfo.dstAddr) << endl;
                break;
            default:
                cout << "rtAttr->rta_type = " << rtAttr->rta_type << ";unknown:" << hex << (*(int*)RTA_DATA(rtAttr)) << endl;
                break;
            }
        }
    }


    close(s);
    return 0;
}
int test_md5(int argc, char **argv)
{
    const char *data = "10ZZ00000006400000000http://10.0.1.2/htmls/portal/default.html10";
    tcout << data << endl;
    char *p = (char*)data;
    MD5_CTX c;
    MD5_Init(&c);
    while(*p)
        MD5_Update(&c, p++, 1);
    unsigned char md[100] = {0};

    MD5_Final(md, &c) ;
    for (int i = 0; i < 16; i++)
    {
        char h = (md[i]&0xf0) >> 4;
        char l = md[i]&0x0f;
        cout << hex << (int)h << (int)l;
    }
    tcout << data << endl;
    tcout << data << endl;
    string src = data;
    string dst("");
    MD5((const unsigned char *)src.c_str(), src.length(), md);
    for (int i = 0; i < 16; i++)
    {
        char h = (md[i]&0xf0) >> 4;
        char l = md[i]&0x0f;
        char tmp[32] = {0};
        sprintf(tmp, "%x%x", h, l);
        dst += tmp;
    }
    cout << dst << endl;
    return 0;
}

int test_broadcast(int argc, char **argv)
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > fd)
    {
        tcout << "socket error:" << strerror(errno) << endl;
    }
    else
    {
        int on=1;
        setsockopt(fd,SOL_SOCKET,SO_REUSEADDR | SO_BROADCAST,&on,sizeof(on));

        struct sockaddr_in sin;
        sin.sin_family = AF_INET;
        sin.sin_port = htons(8080);
        sin.sin_addr.s_addr = htonl(INADDR_ANY);
        int ret = bind(fd, (struct sockaddr *)&sin, sizeof(sin));
        if (0 > ret)
        {
            tcout << "bind error:" << strerror(errno) << endl;
        }
        else
        {
            struct sockaddr_in sin_client;
            sin_client.sin_family = AF_INET;
            sin_client.sin_port = htons(8080);
            inet_pton(AF_INET, "10.0.3.110", &sin_client.sin_addr.s_addr);

            unsigned int mask;
            inet_pton(AF_INET, "255.255.252.0",  &mask);
            tcout << hex << sin_client.sin_addr.s_addr << endl;
            tcout << hex << mask << endl;

            unsigned int mask_ = ~mask;

            tcout << hex << mask_ << endl;
            unsigned int bca = sin_client.sin_addr.s_addr | (~mask);
            tcout << hex << bca << endl;

            char tosend[] = "Hello World!";
            ret = sendto(fd, tosend, sizeof(tosend), 0, (struct sockaddr *)&sin_client, sizeof(sin_client));
            tcout << "send ret = " << ret << "[" << strerror(errno) << "]" << endl;
        }
    }
    return 0;
}
int test_uid(int argc, char **argv)
{
    uid_t uid = getuid();
    tcout << "uid = " << uid << endl;
    uid = geteuid();
    tcout << "uid = " << uid << endl;
    tcout << "setuid ret = " << setuid(0) << endl;
    tcout << "effective uid = " << geteuid() << endl;
    tcout << "real uid = " << getuid() << endl;
    tcout << "setuid ret = " << setuid(uid) << endl;
    tcout << "effective uid = " << geteuid() << endl;
    tcout << "real uid = " << getuid() << endl;
    return 0;
}
int test_classpfunc(int argc, char **argv)
{
    class A{
    public:
        int pfunc(int a){
            tcout << a << endl;
            return 0;
        }
        int func(int (A::*f)(int))
        {
            int a = 10;
            (this->*f)(a);
            return 0;
        }
    };
    A a;
    a.func(&A::pfunc);
    return 0;
}
int test_class_setvalue(int argc, char ** argv)
{
    class A
    {
    private:
        int value;
    public:
        A(){}
        int setValue(int value)
        {
            value = value;
        }
        int Value()
        {
            return value;
        }

    };
    A a;
    a.setValue(1);
    tcout << a.Value() << endl;
}
int get_argsFromUrl(char* url, wx_arg* args)
{
    //openid,token
    if (NULL == url || NULL == args)
        return -1;
    memset(args, 0, sizeof(*args));
    char* ptr = NULL;
    ptr = strstr(url, "?");
    if (NULL == ptr)
        return -1;
    ptr += 1;

    while(*ptr != '\0')
    {
        if (strncmp(ptr, WX_OPENID_STR"=", strlen(WX_OPENID_STR"=")) == 0)
        {
            ptr += strlen(WX_OPENID_STR"=");
            int i = 0;
            while((*ptr != '\0') && (*ptr != '&'))
            {
                args->wx_openid[i++] = *ptr++;
            }
        }
        else if (strncmp(ptr, WX_PUBLICID_STR"=", strlen(WX_PUBLICID_STR"=")) == 0)
        {
            ptr += strlen(WX_PUBLICID_STR"=");
            int i = 0;
            while((*ptr != '\0') && (*ptr != '&'))
            {
                args->wx_publicid[i++] = *ptr++;
            }
        }
        else
        {
            ptr++;
        }
    }
    printf("%s[%s=%s,%s=%s]\n", __func__, WX_OPENID_STR, args->wx_openid, WX_PUBLICID_STR, args->wx_publicid);
    return 0;
}

int test_argsFromUrl(int argc, char ** argv)
{
    char *url = "http://yun.dunchongnet.com/?openid=oFil_t5vxS0PrhABHQM3GdLmB67I&token=gh_27677853fce4";
    wx_arg args;
    tcout << "return = " << get_argsFromUrl(url, &args) << endl;
    cout << strerror(errno) << endl;
    return 0;
}
void threadfunc(sigval_t st)
{
    tcout << st.sival_int << endl;
}
void signalfunc(int sig)
{
    tcout << "sig = " << sig << endl;
}
void sigactionfunc(int sig, siginfo_t *st, void *data)
{
    //st为信号本身的额外数据
    tcout << endl;
}

int test_timer_thread(int argc, char **argv)
{
    timer_t timerid;
    struct sigevent evp;
    bzero(&evp, sizeof(evp));
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = threadfunc;
    evp.sigev_value.sival_int = 123;
    //evp._sigev_un._sigev_thread._function
    int ret = timer_create(CLOCK_REALTIME, &evp, &timerid);
    if (ret < 0)
    {
        perror("timer_create:");
        return -1;
    }
    struct itimerspec it;
    it.it_interval.tv_sec = 1;
    it.it_interval.tv_nsec = 0;
    it.it_value.tv_sec = 1;
    it.it_value.tv_nsec = 0;

    ret = timer_settime(timerid, 0, &it, NULL);
    if (ret < 0)
    {
        perror("timer_settime:");
        return -1;
    }
    getchar();
    return 0;
}
int test_timer_signal(int argc, char **argv)
{
    timer_t timerid;

    struct sigaction sa;
    bzero(&sa, sizeof(sa));
    //sa.sa_handler = signalfunc;
    sa.sa_sigaction = sigactionfunc;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);


    //signal(SIGUSR1, signalfunc);
    struct sigevent evp;
    evp.sigev_signo = SIGUSR1;
    evp.sigev_notify = SIGEV_SIGNAL;

    int ret = timer_create(CLOCK_REALTIME, &evp, &timerid);
    if (ret < 0)
    {
        perror("timer_create:");
        return -1;
    }
    struct itimerspec it;
    it.it_interval.tv_sec = 1;
    it.it_interval.tv_nsec = 0;
    it.it_value.tv_sec = 1;
    it.it_value.tv_nsec = 0;

    ret = timer_settime(timerid, 0, &it, NULL);
    if (ret < 0)
    {
        perror("timer_settime:");
        return -1;
    }
    while(true)
    {
        sleep(1);
        tcout << "after sleep(1)" << endl;
    }
    tcout << "end" << endl;
    return 0;
}
