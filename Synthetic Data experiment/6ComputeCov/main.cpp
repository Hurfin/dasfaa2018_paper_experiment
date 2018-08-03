 #include <bits/stdc++.h>
using namespace std;
vector<string>FILENAME;
//const int MAX_K=5;/**�趨�����㵽ѡ�����ĵ��������*/
int SizeOfS0;
struct YILAI/**һ���������������ݽṹ*/
{
    vector<string>qianJian;
    vector<string>houJian;
};
vector<YILAI>HanShuYiLai;/**�����еĺ�������*/
vector<set<string> >PPi;/**ÿ���ĵ��ļ���*/
vector<set<string> >S0_Si_intersection;/**����S0��Si�Ľ���*/

int MAX_before,MAX_cur;/**�ϴ���󸲸���,��ǰ��󸲸���*/
vector<int>beforeIndexV,curIndexV;/**�ϴ���󸲸Ƕ�Ӧ��ѡ��S'i, ����S0_Si_intersection;�е��±�,Ҳ�ɸ�����ֵ��FILENAME�ҵ���Ӧ���ĵ���*/

string dir="C:\\Users\\12433\\Desktop\\Data\\";


void HanShuYiLaiAdd(string yilai)
{
    int b=0,e=yilai.size()-1;
    while(yilai[b]!='('){b++;}
    while(yilai[e]!=')'){e--;}
    YILAI a;
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
void getSi(string filename)
{
    ifstream fin;
    //cout<<filename<<endl;
    fin.open(filename.c_str(),ios::in);
    string s;
    getline(fin,s);
    vector<string>attr=split(s);
    map<string,int>attrM;/**���Զ�Ӧ���кŵ�ӳ��*/
    for(int i=0; i<attr.size(); i++)attrM[ attr[i] ]=i;

    getline(fin,s);
    vector<string>FDIndexS=split(s);
    vector<int>FDIndex;
    for(int i=0; i<FDIndexS.size(); i++)
    {
        FDIndex.push_back(atoi(FDIndexS[i].c_str()));
    }
    set<string>si;
    while(getline(fin,s))
    {
        /**ÿ��Ԫ��*/
        vector<string> tmp=split(s);
        for(int i=0; i<FDIndex.size(); i++)
        {
            string xx="";
            vector<string>QianJian = HanShuYiLai[FDIndex[i]].qianJian;
            vector<string>HouJian = HanShuYiLai[FDIndex[i]].houJian;
            for(int k=0; k<QianJian.size(); k++)
            {
                if(k==0)
                    xx+=tmp[  attrM[ QianJian[k] ] ];
                else
                    xx=xx+","+tmp[  attrM[ QianJian[k] ] ];
            }
            xx+=":";
            for(int k=0; k<HouJian.size(); k++)
            {
                if(k==0)
                    xx+=HouJian[k];
                else
                    xx=xx+","+HouJian[k];
            }
            /**��Ӧһ��Ԫ������к���������������룬�������԰�io�������ٵ�һ��*/
            si.insert(xx);
        }

    }
    fin.close();
    PPi.push_back(si);
    return ;
}
void getIntersection()
{

    for(int i=1;i<PPi.size();i++)
    {
        set<string>tmp;
        set<string>::iterator iter=tmp.begin();
        set_intersection(PPi[0].begin(),PPi[0].end(),PPi[i].begin(),PPi[i].end(),inserter(tmp,iter));
        S0_Si_intersection.push_back(tmp);
    }

}
void computeAllChoices(std::vector<int>& v, int len, int outlen, int startIndex, int cnt, int *arr, int arrIndex)
{
    if(cnt==0)
    {
        //vector<int>tmp;
        set<string>tmp;
        for(int i=0; i<outlen; i++)
        {
            //tmp.push_back(arr[i]);
            //std::cout << arr[i] << " ";
            set_union(tmp.begin(),tmp.end(),S0_Si_intersection[ arr[i] ].begin(),S0_Si_intersection[ arr[i] ].end(),inserter(tmp,tmp.begin()));
        }
        //std::cout << std::endl;
        //VV.push_back(tmp);
        long long cnt=tmp.size();
        if(MAX_cur<cnt)/**��������˸���ĸ����Ǿ͸���*/
        {
            MAX_cur=cnt;
            curIndexV.clear();
            for(int i=0;i<outlen;i++)
            {
                curIndexV.push_back(arr[i]);
            }
        }

        return ;
    }
    int endIndex = len - cnt;
    for(int i = startIndex; i <= endIndex; i++)
    {
        arr[arrIndex] = v[i];
        computeAllChoices(v, len, outlen, i+1, cnt-1, arr, arrIndex+1);
    }
}
int main(int argc, const char* argv[])
{
//    string s;
//    ifstream fin;
//    s="";
//    s=s+argv[1]+"_"+argv[2]+"_"+argv[3]+"_"+argv[4]+"_"+argv[5]+"_"+argv[6]+"_SelectedSrcs.txt";
//    string file=dir+s;
//    fin.open(file.c_str(),ios::in);
//    /**FILENAME[0]��S0 �������������Դ����S(i) S(i+1)....*/
//    fin>>s; //��mh-greedy ��greedy ��ö��
//    while(fin>>s)
//        FILENAME.push_back(s);
//    fin.close();
//    file=dir+"input\\FD.txt";
//    fin.open(file.c_str(),ios::in);
//    while(fin>>s)
//    {
//        HanShuYiLaiAdd(s);
//    }
//    fin.close();
    string s;
    ifstream fin;
    ifstream fin2;
    s="";
    s=s+argv[1]+"_"+argv[2]+"_"+argv[3]+"_"+argv[4]+"_"+argv[5]+"_"+argv[6]+"_"+argv[7]+".txt";
    string file=dir+s;
    fin.open(file.c_str(),ios::in);
    /**FILENAME[0]��S0 �������������Դ����S(i) S(i+1)....*/
    fin>>s; //��mh-greedy ��greedy ��ö��
    while(fin>>s)
        FILENAME.push_back(s);
    fin.close();
    file=dir+"input\\FD.txt";
    fin.open(file.c_str(),ios::in);
    while(fin>>s)
    {
        HanShuYiLaiAdd(s);
    }
    fin.close();


    for(int i=0; i<FILENAME.size();i++)
    {
        //string filename="dataSet\\"+FILENAME[i];
        string filename=dir+FILENAME[i];
        getSi(filename);
    }
    getIntersection();

    /*for(int i=0;i<PPi.size();i++)
    /*for(int i=0;i<PPi.size();i++)
    {
        for(set<string>::iterator it=PPi[i].begin();it!=PPi[i].end();it++)
        {
            cout<<*it<<" ";
        }cout<<endl;
    }*/

    /**�ͷŵ��ĵ����ϵ��ڴ�*/
    SizeOfS0=PPi[0].size();
    PPi.clear();
    //cout<<S0_Si_intersection.size()<<endl;
    /*for(int i=0;i<S0_Si_intersection.size();i++)
    {
        for(set<string>::iterator it=S0_Si_intersection[i].begin();it!=S0_Si_intersection[i].end();it++)
        {
            cout<<*it<<" ";
        }
        cout<<endl;
    }*/

    //exec();
       ofstream oout;
//    s=argv[3];
//    s+="_SelectedSrcs.txt";
//    file=dir+s;
    string tmp_s=argv[7];
    if(tmp_s=="random_SelectedSrcs")
        s="output\\random_result.txt";
    else if(tmp_s=="greedy_SelectedSrcs")
        s="output\\greedy_result.txt";
    else if(tmp_s == "SelectedSrcs")
        s="output\\mh_greedy_result.txt";
    else if(tmp_s == "bf_greedy_SelectedSrcs")
        s="output\\bf_greedy_result.txt";
    file=dir+s;
    oout.open(file.c_str(),ios::out|ios::app);

//    for(int i=0;i<S0_Si_intersection.size();i++)
//    {
//        oout<<S0_Si_intersection[i].size()<<" ";
//    }oout<<"\n";
   // oout.close();




    set<string>UNION;
    for(int i=0;i<S0_Si_intersection.size();i++)
    {
        for(set<string>::iterator it=S0_Si_intersection[i].begin();it!=S0_Si_intersection[i].end();it++)
        {
            UNION.insert(*it);
        }
    }

    if(tmp_s == "bf_greedy_SelectedSrcs")
        oout<<argv[1]<<"_"<<argv[2]<<"_"<<argv[3]<<"_"<<argv[4]<<"_"<<argv[5]<<"_"<<argv[6]<<"_"<<argv[7]<<"_"<<argv[8]<<"_"<<argv[9]<<"\t"<<SizeOfS0<<"\t"<<UNION.size()<<"\n";
    else
        oout<<argv[1]<<"_"<<argv[2]<<"_"<<argv[3]<<"_"<<argv[4]<<"_"<<argv[5]<<"\t"<<SizeOfS0<<"\t"<<UNION.size()<<"\n";
    oout.close();
    return 0;
}
