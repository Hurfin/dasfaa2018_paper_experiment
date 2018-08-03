#include<bits/stdc++.h>
using namespace std;
typedef unsigned long long ULL;
string dir = "C:\\Users\\12433\\Desktop\\Data_flight\\";
const unsigned long long Filter_size = 1<<16;/**����bloom filter�Ĵ�С*/
const int no_FD = 1;/**FD����*/
int no_file;/**�����ļ�����*/
int PRE_REMAIN_HASHN = 1000;/**Ԥ�����õĹ�ϣ��������,����FD���Լ����������Ҫ��hash��������ʹ��*/
ULL murmurhash_Seed_start_pos ;/**����murmurhash���ӿ�ʼλ��*/
vector<string>FileName;/**�ļ���*/
bitset<Filter_size>S0_filter[no_FD],S_filter[no_FD],tmp_filter[no_FD],tmp_final_union[no_FD];/**S0�Ĺ�����,S�Ĺ�����*/
vector<ULL>FD_hashnum;/**��ϣ�����ĸ���*/
set<string>S0FDSET[no_FD];/**�洢S0��ÿ��FD��Ԫ��,���Լ��������FD*/
int MAX_select;/**ѡ������Դ����*/
vector<bool>si_if_selected;/**�������Դ�Ƿ��Ѿ���ѡ��*/
bool if_add_random_weight=false;/**�Ƿ�������Ȩ��*/
int  hashmethod=0;/**ѡ��hash����0Ϊ(ax+b)%c; 1Ϊmurmurhash*/
double MAX_cov=0;/**��¼��󸲸�ֵ*/
vector<double>file_random_weight;/**����ÿ������Դ�����Ȩ��*/

struct source_si
{
    int FD;/**��¼��si��fd*/
    bitset<Filter_size>si_bloom_filter;/**si�Ĺ���FD�Ĺ�����*/
};
vector<source_si>Si_filter;/**�洢����si��filter  ��һ��λ�÷ŵ���s1*/

struct nod/**һ���������������ݽṹ*/
{
    vector<string>qianJian;
    vector<string>houJian;
};
vector<nod>HanShuYiLai;/**�洢��������*/

struct hashParameter
{
    long long A,B;
};
vector<hashParameter>HASH;/**hash����������*/


/**������______________________*/
const int maxn=100000;// N=999999ʱ��78498������
const int maxnn=999999;
long long prime[maxn+10];
bool isprime[maxnn+10];
long long pnum;
void GetPrime(long long N )
{
    pnum=0;
    memset(isprime,true,sizeof(isprime));
    isprime[1]=false;
    //i Ϊ�������
    for(long long  i=2;i<=N;i++)
    {
        if(isprime[i])
        {
            prime[pnum++]=i;
        }
        for(long long  j=0;j<pnum && prime[j]*i<=N;j++)
        {
            isprime[prime[j]*i]=false;
            if(i%prime[j]==0)break;
        }
    }
}
/**������______________________*/

vector<hashParameter> hashi(int k)
{
    set<string>S;
    vector<hashParameter>v;
    while(S.size()<k)
    {
        hashParameter xx;
        long long aindex = (rand()%10000)+(pnum-10001);
        long long bindex = (rand()%10000)+(pnum-10001);

        xx.A=prime[aindex];
        xx.B=prime[bindex];

        char sstr[50];
        sprintf(sstr, "%lld_%lld", xx.A, xx.B);

        //cout<<sstr<<endl;

        string str =sstr;
        if(S.find(str)==S.end()){
            S.insert(str);
            v.push_back(xx);
        }
    }
    return v;
}

void HanShuYiLaiAdd(string yilai)
{
    int b=0,e=yilai.size()-1;
    while(yilai[b]!='('){b++;}
    while(yilai[e]!=')'){e--;}
    nod a;
    string s;
    s="";
    for(int i=b+1;i<e;i++)
    {
        if(yilai[i]==',')
        {
            a.qianJian.push_back(s);
            s="";
        }
        else{
            s+=yilai[i];
        }
    }
    a.qianJian.push_back(s);
    s="";
    for(int i=e+1;i<yilai.size();i++)
    {
        if(yilai[i]==',')
        {
            a.houJian.push_back(s);
            s="";
        }
        else
        {
            s+=yilai[i];
        }
    }
    a.houJian.push_back(s);
    HanShuYiLai.push_back(a);
}

ULL get_hash_functions_amount(double n)/**����nΪ(the number of inserted elements)*/
{
    return (ULL)ceil(( (double)Filter_size / n)*log(2));
}

double get_Estimated_Size(ULL filtersize, ULL hashNum, ULL cnt)/**����Ϊ��������С,hash������Ŀ,��������1�ĸ���*/
{
//    return ceil( -1*((double)filtersize / (double)hashNum)*log( 1 - ( (double)cnt / (filtersize) )));
    return ( -1*((double)filtersize / (double)hashNum)*log( 1 - ( (double)cnt / (filtersize) )));
}

vector<string> split(string s)//���ո�ָ��ַ���
{
    vector<string>v;
    string w="";

    for(int i=0;i<s.size();i++)
    {
        if(s[i]==' ')
        {
            v.push_back(w);
            w="";
        }
        else
        {
            w+=s[i];
        }
    }
    v.push_back(w);
    return v;
}

string mytrim(string s)//���»���֮ǰ�����õĶ���ȥ��ֻ�������ֲ���,�����λ���ȵ�����
{
    string k="";
    int i=0;
    while(s[i]!='_')i++;
    for(i=i+1;i<s.size();i++)
    {
        k+=s[i];
    }
    if(k.size()<4)
    {
        string ch="";
        int cnt=4-k.size();
        while(cnt--)ch+='0';
        k=ch+k;
    }
    return k;
}
ULL getHashValue_method1(string s, ULL filtersize, int hashi)
{
//    cout<<"hashi="<<(ULL(((ULL)atoll(s.c_str())*HASH[hashi].A + HASH[hashi].B) % filtersize))<<endl;
    return ULL(((ULL)atoll(s.c_str())*HASH[hashi].A + HASH[hashi].B) % filtersize);
}
unsigned long long MurmurHash64B ( const void * key, int len, unsigned int seed )
{
	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	unsigned int h1 = seed ^ len;
	unsigned int h2 = 0;

	const unsigned int * data = (const unsigned int *)key;

	while(len >= 8)
	{
		unsigned int k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;

		unsigned int k2 = *data++;
		k2 *= m; k2 ^= k2 >> r; k2 *= m;
		h2 *= m; h2 ^= k2;
		len -= 4;
	}

	if(len >= 4)
	{
		unsigned int k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;
	}

	switch(len)
	{
	case 3: h2 ^= ((unsigned char*)data)[2] << 16;
	case 2: h2 ^= ((unsigned char*)data)[1] << 8;
	case 1: h2 ^= ((unsigned char*)data)[0];
			h2 *= m;
	};

	h1 ^= h2 >> 18; h1 *= m;
	h2 ^= h1 >> 22; h2 *= m;
	h1 ^= h2 >> 17; h1 *= m;
	h2 ^= h1 >> 19; h2 *= m;

	unsigned long long h = h1;

	h = (h << 32) | h2;

	return h;
}
void builtS0Filter(string s0path)
{
    ifstream fin;
    fin.open(s0path.c_str(), ios::in);
    string s;
    getline(fin, s);
    vector<string>attr = split(s);
    getline(fin, s);
    vector<string>fd_str = split(s);
    map<string,int>M;/**�������Ե��кŵ�ӳ��*/
    for(int i=0;i<attr.size();i++)M[ attr[i] ] = i;

    while(getline(fin, s))
    {
        vector<string>tupi = split(s);/**����һ��Ԫ��*/
        for(int i=0;i<fd_str.size();i++)/**����ÿ��FD*/
        {
            int fd = atoi(fd_str[i].c_str());/**FD���*/
            string x="";
            for(int j=0;j<HanShuYiLai[fd].qianJian.size();j++)/**FD��ǰ������*/
            {
                string vi = tupi[ M[ HanShuYiLai[fd].qianJian[j] ] ];/**ǰ�������ϵ�ֵ*/
                x += mytrim(vi);/**ȥ��j_����*/
            }
           // /**��x���������S0_filter[fd]����x��k��ɢ��ֵ���������*/
            S0FDSET[fd].insert(x);/**����set������ͳ�ƽ�Ҫ�����Ԫ����Ŀ���ڼ���hash�����ĸ���*/
        }
    }
    fin.close();

    for(int i=0;i<no_FD;i++)/**�������FD����Ĺ�ϣ��������*/
    {
        ULL hnum = get_hash_functions_amount(S0FDSET[i].size());
        FD_hashnum.push_back(hnum);
    }
    /**��Ԫ�ز���bloom filter*/
    for(int i=0;i<no_FD;i++)
    {
        for(set<string>::iterator it=S0FDSET[i].begin();it!=S0FDSET[i].end();it++)
        {
            for(int hashi=0;hashi<FD_hashnum[i];hashi++)
            {
                ULL hashvalue;
                if(hashmethod == 0)
                    hashvalue = getHashValue_method1(*it, Filter_size, hashi);/**  ��һ�ֹ�ϣ����(a*x+b)%c�Ĺ�ϣ  */
//                ULL hashvalue = MurmurHash64B((*it).c_str(), (*it).size(), prime[pnum-1]) % Filter_size;/**  �ڶ��ֹ�ϣ����murmurhash seed���ó������������� */
                else if(hashmethod == 1)
                    hashvalue = MurmurHash64B((*it).c_str(), (*it).size(), prime[hashi+murmurhash_Seed_start_pos]) % Filter_size;/**  �ڶ��ֹ�ϣ����murmurhash seed���ó������������� */

                S0_filter[i][ hashvalue ] = 1;
            }

        }
    }
    cout<<"S0��������С"<<endl;
    for(int i=0;i<no_FD;i++)cout<<S0_filter[i].count()<<" ";cout<<endl;

}

void builtSiFilter(string sipath)
{
    source_si SI;/**����һ��SI�Ľṹ*/
    ifstream fin;
    fin.open(sipath.c_str(), ios::in);
    string s;
    getline(fin, s);
    vector<string>attr = split(s);
    getline(fin, s);
    SI.FD = atoll(s.c_str());/**�õ�FD*/
    map<string,int>M;
    for(int i=0;i<attr.size();i++)M[ attr[i] ] = i;
    while(getline(fin, s))
    {
        vector<string>tupi = split(s);
        string x="";
        for(int i=0;i<HanShuYiLai[SI.FD].qianJian.size();i++)
        {
            x += mytrim(tupi[ M[ HanShuYiLai[SI.FD].qianJian[i] ] ]);
        }

        for(int hashi=0;hashi<FD_hashnum[SI.FD];hashi++)
        {
            ULL hashvalue;
            if(hashmethod == 0)
                hashvalue = getHashValue_method1(x, Filter_size, hashi);/**  ��һ�ֹ�ϣ����(a*x+b)%c�Ĺ�ϣ  */
            else if(hashmethod == 1)
                hashvalue = MurmurHash64B(x.c_str(), x.size(), prime[hashi+murmurhash_Seed_start_pos]) % Filter_size;/**  �ڶ��ֹ�ϣ����murmurhash seed���ó������������� */

            SI.si_bloom_filter[ hashvalue ] = 1;
        }
    }
    fin.close();
    //cout<<sipath<<" "<<SI.FD<<" "<<SI.si_bloom_filter.count()<<endl;
    Si_filter.push_back(SI);/**�����õ�si�ϵĹ���FD�Ĺ������ŵ�Si_filter��*/
}
double computeCov(int si_index)/**���Ƽ���si��S��s0�ĸ���ֵ*/
{
    for(int i=0;i<no_FD;i++)/**��֮ǰ�Ĳ���filter copy��tmp_filter��*/
        tmp_filter[i] = S_filter[i];
    tmp_filter[ Si_filter[si_index - 1].FD ] = tmp_filter[ Si_filter[si_index - 1].FD ] | Si_filter[si_index - 1].si_bloom_filter;/**��si��FD�ϵ�filter�Ͳ�����filter����һ��*/
    double _sum_per_FD_cov=0;/**�ۼ�S0��S��ÿ��FD�ϵĽ���*/
    for(int i=0;i<no_FD;i++)/**ö��ÿ��FD*/
    {

        double n_S0 = get_Estimated_Size(Filter_size, FD_hashnum[i], S0_filter[i].count());/**����s0�ڸ�FD�ϵĴ�С ����Ϊ��������С,hash������Ŀ,��������1�ĸ���*/
        double n_S = get_Estimated_Size(Filter_size, FD_hashnum[i], tmp_filter[i].count());/**����S�ڸ�FD�ϵĴ�С ����Ϊ��������С,hash������Ŀ,��������1�ĸ���*/
        bitset<Filter_size>bs = S0_filter[i] | tmp_filter[i];
        double n_S0_S_union = get_Estimated_Size(Filter_size, FD_hashnum[i], bs.count());/**����S0��S�Ĳ�����С*/
        double n_S0_S_intersection = n_S0 + n_S - n_S0_S_union;/**��S0��S�Ľ�����С*/
        _sum_per_FD_cov += n_S0_S_intersection;/**�ۼ�ÿ��FD�ϵĽ���*/
    }
    return _sum_per_FD_cov;
}
int MAX_COV()
{
    int select_index = -1;
    double MAX_newcov_sub_oldcov = 0;
    for(int i=1;i<=no_file;i++)
    {
        if(si_if_selected[i] == true) continue;

        double cov_value = computeCov(i);/**����S�м���si��õ��Ķ�S0�Ĺ��Ƹ���ֵ*/
        double newcov_sub_oldcov;/**����(cov(SUsi)-cov(S))��(cov(SUsi)-cov(S))/ci*/
        if(if_add_random_weight == true)/**���Ҫ�����Ȩ��*/
            newcov_sub_oldcov = (cov_value - MAX_cov) / file_random_weight[i];
        else
            newcov_sub_oldcov = cov_value - MAX_cov;
        if(MAX_newcov_sub_oldcov < newcov_sub_oldcov)
        {
            select_index = i;
            MAX_newcov_sub_oldcov = newcov_sub_oldcov;/**�����������*/
            MAX_cov = cov_value;/**��¼��󸲸�ֵ*/

            for(int w=0;w<no_FD;w++)tmp_final_union[w]=tmp_filter[w];
        }
    }
    for(int w=0;w<no_FD;w++) S_filter[w]=tmp_final_union[w];/**����ѡ�����ѵ�si�����Ĳ���S*/
    return select_index;
}
/**
   argv1����Դ����
   argv2��ϣ��������(��ϣ������������Ҫ�Լ��趨,���ݹ�ʽ�Լ��㡣���ó�null)
   argv3ѡ������Դ����k
   argv4Ŀ�����ݼ���Ԫ�������ռ������ı���
   argv5����Դ��Ԫ�������ռ��������
   argv6Ԫ���������
   argv7ComputeCov.exe������������ȥѡ����greedy��mh_greedy��bf_greedy����random�ֱ��Ӧ����  "greedy_SelectedSrcs" "mh_greedy_SelectedSrcs" "bf_greedy_SelectedSrc" "random_SelectedSrcs"
   argv8 ѡ��hash��������Ϊ "murmurhash" �� "(ax+b)%c"
   argv9�Ƿ�Ҫ��ÿ������Դ���һ�����Ȩ��"add_random_weight_YES" "add_random_weight_NO"

*/

int main(int argc, const char *argv[])
//int main()
{
    /*char *argv[10];
    argv[1]="1000";
    argv[2]="murmurhash";
    argv[3]="5";
    argv[4]="100";
    argv[5]="100";
    argv[6]="2000";
    argv[7]="add_random_weight_NO";
    */
    no_file = atoi(argv[1]);
    MAX_select = atoi(argv[3]);
    if_add_random_weight = ((strcmp(argv[9],"add_random_weight_YES")==0)?true:false);
    string hashstr = argv[8];
    if(hashstr == "murmurhash")
        hashmethod = 1;
    else if(hashstr == "(ax+b)c")
        hashmethod = 0;

    srand(time(NULL));
    long long NN=999999;
    GetPrime(NN);
    murmurhash_Seed_start_pos = rand()%10000;/**��prime[]�������һ��������Ϊmurmurhash����*/

    for(int i=0;i<no_file+5;i++)/**����δ��ѡ��*/
        si_if_selected.push_back(false);

    /**��ȡ�ļ���*/
    string filenamepath=dir+"FileName_"+argv[6]+".txt";/**�ļ����б�*/
    ifstream fin;
    fin.open(filenamepath.c_str(), ios::in);
    string s;
    int cnt_file = no_file + 1;/**����S0*/
    while(fin>>s&&cnt_file--)
    {
        FileName.push_back(s);
    }
    fin.close();

    no_file = FileName.size()-1;/**���ݶ�����ʵ�ʸ�����������*/

    clock_t start, finish;
    double totaltime = 0;
    start = clock();
    if(if_add_random_weight == true){
        ifstream fiin;
        string randomweightpath = dir + "random_weight.txt";
        fiin.open(randomweightpath.c_str(), ios::in);
        cout<<"#add_random_weight"<<endl;
        for(int i=0;i<=no_file;i++)/**��ÿ���ļ��������һ��1-10��Ȩ��*/
        {
            string sss;
            fiin>>sss;
            file_random_weight.push_back(atof(sss.c_str()));
            cout<<"--source "<<i<<" weight:"<<file_random_weight[file_random_weight.size()-1]<<endl;
        }
        fiin.close();

    }

    cout<<"OK1"<<endl;

    /**��ȡFD*/
    string fdfilepath = dir + "FD.txt";
    fin.open(fdfilepath.c_str(), ios::in);
    while(fin>>s)
    {
        HanShuYiLaiAdd(s);
    }
    fin.close();


    HASH = hashi(PRE_REMAIN_HASHN);

    cout<<"OK2"<<endl;

    /**����S0�ĸ���FD�ϵĹ�����*/
    string s0path = dir + FileName[0];
    builtS0Filter(s0path);
    for(int i=1;i<FileName.size();i++)
    {
        string sipath = dir + FileName[i];
        builtSiFilter(sipath);
    }

    cout<<"OK3"<<endl;

    vector<long long>select_file_to_cov;
    for(int i=0;i<MAX_select;i++)
    {
        int select_si_index = MAX_COV();/**����ֵΪѡ�ĸ�si���Դﵽ��󸲸�*/
        if(select_si_index == -1)break;
        si_if_selected[select_si_index] = true;
        select_file_to_cov.push_back(select_si_index);
    }
    finish = clock();
    totaltime = (double)(finish - start)/(double)(CLOCKS_PER_SEC);

    cout<<"OK4"<<endl;
//    cout<<"������󸲸�ֵ:"<<MAX_cov<<endl;
//    for(int i=0;i<select_file_to_cov.size();i++)cout<<select_file_to_cov[i]<<endl;
    //-------------------------------------------------------------------
    ofstream fout;
    string file = dir + "output\\bf_greedy_runtime.txt";
    fout.open(file.c_str(), ios::out|ios::app);
    fout<<argv[1]<<"_"<<argv[2]<<"_"<<argv[3]<<"_"<<argv[4]<<"_"<<argv[5]<<"_"<<argv[6]<<"_"<<argv[7]<<"_"<<argv[8]<<"_"<<argv[9]<<"\t"<<totaltime<<endl;
    fout.close();
    /**��ѡ�������Դ������ļ�*/
    cout<<"#Src="<<select_file_to_cov.size()<<endl;
    cout<<"#Covered="<<MAX_cov<<endl;
    //file = dir + argv[1] + "_" + argv[2] + "_" + argv[3] + "_" + argv[4] + "_" + argv[5] + "_" + argv[6] + "_" + argv[7] + "_" + argv[8] + "_" + argv[9] + ".txt";
    file = dir + argv[1] + "_" + argv[2] + "_" + argv[3] + "_" + argv[4] + "_" + argv[5] + "_" + argv[6] + "_" + "bf_greedy_SelectedSrcs" + ".txt";

    fout.open(file.c_str(), ios::out);
    fout<<"bf_greedy\n";
    fout<<FileName[0]<<"\n";
    for(int i=0;i<select_file_to_cov.size();i++)
    {
        fout<<FileName[ select_file_to_cov[i] ]<<"\n";
        cout<<"--source "<<select_file_to_cov[i]<<endl;
    }
    fout.close();

    return 0;
}
