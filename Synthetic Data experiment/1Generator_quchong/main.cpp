#include <bits/stdc++.h>

using namespace std;

string dir = "C:\\Users\\12433\\Desktop\\Data\\";

//int per_attr;
int no_tuple=2000;/**S0��Ԫ�����������min_perc��max_percȷ������Si��Ԫ�����*/
const int size_domain=1000;/**���������ϵ�ֵ���С*/
int min_perc=10;/**���ɵ�Ԫ�����İٷֱȵ��½缴һ�����ɵ��ĵ�����Ҫ��(no_tuple)*(min_perc/100)��Ԫ��*/
int max_perc=100;/**����Ԫ�����İٷֱȵ��Ͻ�Ĭ��ֵ��Ϊ100%(�ȵ���Ҫ�ı�ʱ���޸ģ��˴���Ϊ��������)��һ���ĵ�Ԫ�������Ϊ(no_tuple)*(min_perc/100)��Ԫ��*/
int _sources = 100;/**��S0֮�����ɼ����ĵ�*/
int _hash;

struct HANSHUYILAI  /**�����������ݽṹ*/
{
    vector<string>QianJian;
    vector<string>HouJian;
};
vector<HANSHUYILAI>FD;  /**�洢���к�������*/
HANSHUYILAI HanshuYiLaiAdd(string yilai)  /**����һ�������������ַ���,���ת��Ϊ��Ӧ���ݽṹ�󷵻�*/
{
    int b=0,e=yilai.size()-1;
    while(yilai[b]!='('){b++;}
    while(yilai[e]!=')'){e--;}
    HANSHUYILAI a;
    string s="";
    for(int i=b+1; i<e; i++)
    {
        if(yilai[i]==','){
            a.QianJian.push_back(s);
            s = "";
        }
        else{
            s+=yilai[i];
        }
    }
    a.QianJian.push_back(s);
    s = "";
    for(int i=e+1; i<yilai.size(); i++)
    {
        if(yilai[i]==','){
            a.HouJian.push_back(s);
        }
        else{
            s+=yilai[i];
        }
    }
    a.HouJian.push_back(s);
    return a;
}
void readFD(string path)  /**��ȡFD����*/
{
    ifstream fin;
    fin.open(path.c_str(), ios::in);
    string s;
    while(fin>>s)
    {
        FD.push_back(HanshuYiLaiAdd(s));  /**��������������FD��vector<HASHUYILAI>����*/
    }
    fin.close();
}
void constructS0(string s0path)/**����S0�ĵ�*/
{
    ofstream fout;
    fout.open(s0path.c_str(), ios::out);
    set<string>S;
    for(int i=0;i<FD.size();i++) /**���������ϵ�����ͳ�Ƶ�һ��*/
    {
        for(int k=0;k<FD[i].QianJian.size();k++)
        {
            S.insert(FD[i].QianJian[k]);
        }
        for(int k=0;k<FD[i].HouJian.size();k++)
        {
            S.insert(FD[i].HouJian[k]);
        }
    }
    for(set<string>::iterator it=S.begin();it!=S.end();it++)
    {
        if(it==S.begin())
            fout<<*it;
        else
            fout<<" "<<*it;
    }
    fout<<"\n";
    for(int i=0;i<10;i++)/**����������*/
    {
        if(i==0)
            fout<<i;
        else
            fout<<" "<<i;
    }
    fout<<"\n";
    set<string>flag;/**����ȥ��*/
    for(int i=0;i<no_tuple;i++)/**��s0����no_tuple��Ԫ��{ע:ȥ�ذ汾}*/
    {
        string tuple_str = "";
        for(int j=0;j<S.size();j++)/**��һ��Ԫ���ÿ�������϶��������һ����ֵ,ֵ��Ϊ[0,size_domain-1]*/
        {
            char attr_value[20];
            sprintf(attr_value, "%d_%d", j, rand()%size_domain);
            if(j==0)
                tuple_str = tuple_str + attr_value;
            else
                tuple_str = tuple_str + " " + attr_value;
        }
        if(flag.find(tuple_str)==flag.end()){
            flag.insert(tuple_str);
            fout<<tuple_str<<"\n";
        }
        else{/**�����ظ�ֵ��Ҫ��������Ԫ��*/
            i--;
        }
    }
    fout.close();

    cout<<flag.size()<<endl;
}
void constructSi(string sipath)/**����һ��Si�ĵ�*/
{
    /**��Ҫ�趨�˴��ĵ�Ԫ������С*/
    int si_no_tuple_min = ceil(((double)min_perc/100)*no_tuple);/**si�ĵ���Ԫ������½�*/
    int si_no_tuple_max = ceil(((double)max_perc/100)*no_tuple);/**si�ĵ���Զ������Ͻ�*/
    int si_no_tuple = rand()%(si_no_tuple_max-si_no_tuple_min+1)+si_no_tuple_min;/**ȷ���ĵ�Si��Ԫ�����*/
    /**��10�����������ѡ��һ��*/
    int FD_choiced_index = rand()%10;

    /**ȡ�������������������*/
    vector<string>attr;
    for(int i=0;i<FD[FD_choiced_index].QianJian.size();i++)attr.push_back(FD[FD_choiced_index].QianJian[i]);
    for(int i=0;i<FD[FD_choiced_index].HouJian.size();i++)attr.push_back(FD[FD_choiced_index].HouJian[i]);

    ofstream fout;
    fout.open(sipath, ios::out);
    for(int i=0;i<attr.size();i++)if(i==0){fout<<attr[i];}else{fout<<" "<<attr[i];}/**�������*/
    fout<<"\n";
    fout<<FD_choiced_index<<"\n";/**�����ѡFD�����*/
    set<string>flag;/**����ȥ��*/
    for(int i=0;i<si_no_tuple;i++)/**��si����si_no_tuple��Ԫ��*/
    {
        string tuple_str = "";
        for(int j=0;j<attr.size();j++)
        {
            char attr_value[20];
            sprintf(attr_value, "%s_%d", attr[j].c_str(), rand()%size_domain);
            if(j==0){
                tuple_str = tuple_str + attr_value;
            }
            else{
                tuple_str = tuple_str + " " + attr_value;
            }
        }
        if(flag.find(tuple_str)==flag.end()){
            flag.insert(tuple_str);
            fout<<tuple_str<<"\n";
        }
        else{/**�����ظ�ֵ��Ҫ��������Ԫ��*/
            i--;
        }
    }
    fout.close();
    cout<<flag.size()<<endl;
}
int main(int argc, const char *argv[])
{
    _sources = atoi(argv[1]);
//    _hash = argv[2];
//    argv[3];

    max_perc = atoi(argv[4]);
    no_tuple = atoi(argv[6]);

    srand(time(NULL));
    string pathFD = dir + "input\\FD.txt";
    readFD(pathFD);  /**��ȡFD����*/
    char s0path0[50];
    sprintf(s0path0, "%s%d_%d.txt",dir.c_str(), no_tuple, 0);
    string s0path = s0path0;
    constructS0(s0path);  /**����S0*/
    for(int i=1;i<=_sources;i++)  /**���� _sources �� Si*/
    {
        char sipath0[50];
        sprintf(sipath0, "%s%d_%d.txt", dir.c_str(), no_tuple, i);
        string sipath = sipath0;
        constructSi(sipath);
    }

    char Filename[50];
    sprintf(Filename, "%sFileName_%d.txt", dir.c_str(), no_tuple);
    ofstream fout;
    fout.open(Filename, ios::out);
    for(int i=0;i<=_sources;i++)
    {
        char fn[30];
        sprintf(fn, "%d_%d.txt", no_tuple, i);
        fout<<fn<<"\n";
    }
    fout.close();
    /**�������Ȩ��*/
    string randomweightpath = dir + "random_weight.txt";
    fout.open(randomweightpath.c_str(), ios::out);
    for(int i=0;i<=_sources;i++)
    {
        fout<<(rand()%10+1)<<"\n";
    }
    fout.close();
    return 0;
}
