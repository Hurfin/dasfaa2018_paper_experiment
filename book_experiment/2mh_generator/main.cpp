#include <bits/stdc++.h>
#include <ctime>
using namespace std;

ofstream fout;
int  hashmethod=0;/**ѡ��hash����0Ϊ(ax+b)%c; 1Ϊmurmurhash*/
long long murmurhash_Seed_start_pos=0 ;/**����murmurhash���ӿ�ʼλ��*/
double RUNTIME=0;
const string dir="C:\\Users\\12433\\Desktop\\Data_book\\";
string outPath="";
long long HASHN=1000;
long long C=1000003;/**��Ϊ(A*x+B)%C�е�C ���ڵ���1000000����С���� 1000000����Ϊ������Ϻ���һ����λ��������ȷ����������Ϊ1000000*/
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
vector<string>FILENAME;/**FILENAME[0]�Ǵ��޸� �������������Դ����*/
double epsilon, delta, n_mh;/**��ֵ�����f_hash()�����ͺ���*/
struct nod/**һ���������������ݽṹ*/
{
    vector<string>qianJian;
    vector<string>houJian;
};
struct hashParameter
{
    long long A,B;
};
double f_hash()
{
    double a=epsilon*epsilon/3;
    double f=(2/(a*a))*(log(2/delta));
    return min(f,n_mh);
}
vector<nod>HanShuYiLai;/**����������*/
vector<hashParameter>HASH;/**hash����������*/
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
/**������Ҫ�޸ģ���λ�Ѿ��������ˣ��������óɶ�̬�ģ�����������ʱ��˳���ֵ�������attrZhiyu.txt�ļ��Ȼ��������ʹ��*/
string mytrim(string s)//���»���֮ǰ�����õĶ���ȥ��ֻ�������ֲ���,�����λ���ȵ����� 001 123
{
    ifstream fin;
    fin.open(dir + "attrzhiyu.txt", ios::in);/**ȡ������ֵ��*/
    string attrzhiyu_str;
    long long attrzhiyu_Integer;
    fin>>attrzhiyu_str;
    fin.close();
    attrzhiyu_Integer = atoll(attrzhiyu_str.c_str());


    string k="";
    int i=0;
    while(s[i]!='_'){i++;}
    for(i=i+1;i<s.size();i++)
    {
        k+=s[i];
    }
    if(k.size()<attrzhiyu_str.size())
    {
        string ch="";
        int cnt=attrzhiyu_str.size()-k.size();
        while(cnt--)ch+='0';
        k=ch+k;
    }
    return k;
}
vector<hashParameter> hashi(int k)
{

    set<string>S;
    vector<hashParameter>v;
    /*for(int i=0;i<k;i++)
    {
        hashParameter xx;
        long long aindex = (rand()%10000)+(pnum-10001);
        long long bindex = (rand()%10000)+(pnum-10001);

        xx.A=prime[aindex];
        xx.B=prime[bindex];


        v.push_back(xx);
    }*/
    while(S.size()<k)
    {
        hashParameter xx;
        long long aindex = (rand()%10000)+(pnum-10001);
        long long bindex = (rand()%10000)+(pnum-10001);

        xx.A=prime[aindex];
        xx.B=prime[bindex];

        char sstr[50];
        sprintf(sstr, "%lld_%lld", xx.A, xx.B);

        string str =sstr;
        if(S.find(str)==S.end()){
            S.insert(str);
            v.push_back(xx);
        }
    }

    return v;
}
/**��Ҫ��*/
vector<long long> minhash(vector<string> attrV)//����һ���������������minhashֵ
{
    long long minHASH[HASHN+10];
    for(int i=0;i<=HASHN+2;i++)minHASH[i]=-1;
    for(int i=0;i<attrV.size();i++)
    {
//        if(attrV[i].size()>3)
//            C=1000003;
//        else
//            C=1009;
        for(int k=0;k<HASHN;k++)
        {
            long long ihash;
            if(hashmethod == 0)/**ѡ�õ�һ��hash����*/
                ihash= ( (  HASH[k].A*atoll(attrV[i].c_str()) +HASH[k].B  )% C )+1;
            else if(hashmethod == 1)/**ѡ�õڶ���hash����murmur hash*/
                ihash= (MurmurHash64B(attrV[i].c_str(), attrV[i].size(), prime[k+murmurhash_Seed_start_pos]) % C)+1;
            if(minHASH[k]!=-1)
            {
                minHASH[k]=min(minHASH[k],ihash);
            }
            else
            {
                minHASH[k]=ihash;
            }
        }
    }
    vector<long long>VV;
    for(int i=0;i<HASHN;i++)
    {
        VV.push_back(minHASH[i]);
    }
    return VV;
}
void exec(string filename)
{
    //ofstream ffout;
    //ffout.open(outPath.c_str(),ios::out);
    //ffout<<"("<<filename<<")"<<"\n";
    fout<<"("<<filename<<")"<<"\n";
    //ffout.close();
    string s;
    vector<string>attr;
    vector<string>functionSeti;/**�����������*/
    ifstream fin;
    fin.open(filename.c_str(),ios::in);
    getline(fin,s);//�õ�����
    attr=split(s);

    getline(fin,s);//�õ���������
    functionSeti=split(s);

    map<string,int>M;//�õ�������dataTable�еı��
    for(int i=0;i<attr.size();i++)M[attr[i]]=i;

    vector<vector<string> >dataTable;
    while(getline(fin,s))
    {
        vector<string>t=split(s);
        dataTable.push_back(t);
    }
    fin.close();
    //cout<<"OKKKK dataTable len "<<dataTable.size()<<endl;
    //for(int i=0;i<dataTable.size();i++)cout<<dataTable[i].size()<<endl;
    //**ͳ������ʱ��*/
    //clock_t start,finish;
    //start=clock();

    for(int i=0;i<functionSeti.size();i++)//ö��ÿ����������
    {
    //    cout<<"--FD "<<i<<endl;
        nod func=HanShuYiLai[ atoi(functionSeti[i].c_str()) ];
        vector<string>QianJian=func.qianJian;//�����������Ҫ�õ������ԣ���ǰ����
        vector<string>combinedAttr;//�洢�ϲ��������
        set<string>SET;
        //����Щ����ƴ��һ������
        for(int k=0;k<dataTable.size();k++)//����ÿ��Ԫ��
        {
            string combined="";//��Ϻϲ�������
            for(int w=0;w<QianJian.size();w++)//������Ԫ���Ϻ�������ǰ������������
            {
                combined+=mytrim(dataTable[k][   M[QianJian[w]]    ]);//M[QianJian[w]] Ϊǰ���ϵ�������dataTable����к�
            }
            combinedAttr.push_back(combined);/**�費��Ҫ��Ϊ������set��ȡ���������ɱ�֤��������û�ظ�ֵ����Ҳ������������ʱ����*/
            SET.insert(combined);
        }
        /**��ʱ���һ������������ǰ�����Եĺϲ����������Ļ��ϲ�������*/
        /**��ʱ�����������һ��minhash��*/
        //ofstream fout;
        //fout.open(outPath.c_str(),ios::out|ios::app);
        fout<<"["<<functionSeti[i]<<"]";
        fout<<" {"<<SET.size()<<"}";
        vector<string>fqianJian,fhouJian;
        fqianJian=func.qianJian;
        fhouJian=func.houJian;
        for(int ww=0;ww<fqianJian.size();ww++){
            fout<<" "<<fqianJian[ww];
        }
        fout<<" :";
        for(int ww=0;ww<fhouJian.size();ww++){
            fout<<" "<<fhouJian[ww];
        }
        fout<<" ;";

        /**�õ�minhash*/
        vector<long long>mm=minhash(combinedAttr);
        if(mm.size()!=HASHN){ cout<<"!!!!!!!!!!!!!"<<endl;exit(0);}
        for(int ww=0;ww<mm.size();ww++)
        {
            fout<<" "<<mm[ww];
        }
        fout<<"\n";
        //fout.close();
    }
}

void confirmC()
{
    ifstream fin;
    fin.open(dir + "attrzhiyu.txt", ios::in);/**ȡ������ֵ��*/
    string attrzhiyu_str;
    long long attrzhiyu_Integer;
    fin>>attrzhiyu_str;
    fin.close();
    attrzhiyu_Integer = atoll(attrzhiyu_str.c_str());
    C=prime[lower_bound(prime, prime+pnum, attrzhiyu_Integer)-prime];

}
int main(int argc, char * argv[])
{
    HASHN=atoi(argv[2]);
    srand((unsigned)time(NULL));
    RUNTIME=0;
    /**������__________________________*/
    long long NN=999999;
    GetPrime(NN);
    /**������__________________________*/

//    /**ȷ��Cֵ*/
//    confirmC();
    /**ȷ��hash����*/
    HASH=hashi(HASHN);
    string hashstr = argv[8];
    if(hashstr == "murmurhash")
        hashmethod = 1;
    else if(hashstr == "(ax+b)%c")
        hashmethod = 0;
    murmurhash_Seed_start_pos = rand()%10000;/**��prime[]�������һ��������Ϊmurmurhash����*/

    string s;
    ifstream fin;
    string infile=dir+"FileName_"+argv[6]+".txt";
    fin.open(infile.c_str(),ios::in);
    /**FILENAME[0]�Ǵ��޸� �������������Դ����*/
    int no_file=atoi(argv[1]);
    int cnt_file=0;
    while(fin>>s&&cnt_file<=no_file){ /**ѡ��ǰno_file���ļ���������S0����������������������������*/
        FILENAME.push_back(dir+s);
        cnt_file++;
    }
    fin.close();
    infile="";
  //  infile=dir+"input\\FD.txt";
  infile=dir+"FD.txt";
    fin.open(infile.c_str(),ios::in);
    while(fin>>s)
    {
        HanShuYiLaiAdd(s);
    }
    fin.close();
    //cout<<"OK1"<<endl;
    string filename;//="dataSet\\60_10_0.txt";//"dataSet\\60_10_0.txt";
    outPath=dir+"mh_"+argv[6]+".txt";
    fout.open(outPath.c_str(),ios::out);/**gaidong*/

    for(int i=0;i<FILENAME.size();i++)
    {
        if(i%10==0)
        cout<<"File "<<i<<" ";/**�����鿴����ÿ��10�������һ��*/
        filename=FILENAME[i];
        exec(filename);
    }
    cout<<endl;
    //exec(filename);

    fout.close();/**gaidong*/

    return 0;
}
