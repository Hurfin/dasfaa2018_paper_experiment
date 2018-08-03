#include <bits/stdc++.h>
#include <ctime>
#include <set>
#include <algorithm>

using namespace std;
vector<int>::iterator ik;
set<string>::iterator it;
set<string>::iterator itt;
vector<vector<int> > FD;
const int no_FD=10;
const int no_attr=10;
int no_source=100;
vector<set<string> > pattern;
set<string> CoveredSet;/**?????   */
vector<int> SelectedSrc;
int vis_pattern[1000000+10];
int max_no_set=10;/** */
vector<string> File;/**�洢(FileName_****.txt)����ļ���*/
string dir="C:\\Users\\12433\\Desktop\\Data\\";
bool if_add_random_weight = false;
vector<double>file_random_weight;/**����ÿ������Դ�����Ȩ��*/
void init_FD(){
    vector<int> S;

    S.push_back(0);
    S.push_back(5);
    FD.push_back(S);
    S.clear();

    S.push_back(0);
    S.push_back(6);
    FD.push_back(S);
    S.clear();

    S.push_back(0);
    S.push_back(7);
    FD.push_back(S);
    S.clear();

    S.push_back(1);
    S.push_back(8);
    FD.push_back(S);
    S.clear();

    S.push_back(1);
    S.push_back(9);
    FD.push_back(S);
    S.clear();

    S.push_back(4);
    S.push_back(8);
    FD.push_back(S);
    S.clear();

    S.push_back(4);
    S.push_back(9);
    FD.push_back(S);
    S.clear();

    S.push_back(2);
    S.push_back(3);
    S.push_back(5);
    FD.push_back(S);
    S.clear();

    S.push_back(2);
    S.push_back(3);
    S.push_back(6);
    FD.push_back(S);
    S.clear();

    S.push_back(2);
    S.push_back(3);
    S.push_back(7);
    FD.push_back(S);
    S.clear();
}
void ComputePattern(){
    int fd=-1; /**���浱ǰʹ�õ�FD�����*/
    ifstream ifstr;
    set<string> S;
    string s,p,tmp;
    string record[no_attr];/**����������������������������use for what  --����S0���ݲ��ֵĵ�һ��*/
    //�����޸��ĵ�����ȡģʽ����
    ifstr.open((dir+File[0]).c_str(),ios::in);
    getline(ifstr,s); //�����Լ���
    getline(ifstr,s); //��FD����
    int row=0;
    while(ifstr){
        for(int j=0;j<no_attr;j++){ /**????????����S0�����ݲ��ֵĵ�һ��   �洢���ݲ���ÿһ�е�ֵ*/
            ifstr>>s;
            record[j]=s;
        }
        for(int j=0;j<no_FD;j++){/**������õ���һ��ֵѭ��ÿ��FD*/
            p="";
            tmp="";
            for(int k=0;k<FD[j].size()-1;k++){/**Ϊʲô����size()-2�ͽ����ˣ��ǲ����д��󰡣��������record�Ǹ�ɶ��-_-|  ������ֻҪǰ����Ϊ���ֻ��һ������*/
                p+=record[FD[j][k]];          /**��ÿ��FD��ǰ�������ϵ�ֵ attr_num ȡ��*/
                p+=",";
            }
            tmp=record[FD[j][FD[j].size()-1]];/**ȡ����������ϵ�ֵ��tmp����*/
            for(int k=0;k<tmp.length();k++){ /**ֻ�Ѻ�������ϵ�ֵ attr_num �е�attrȡ��, ׷�ӵ��ַ���p����*/
                if(tmp[k]=='_')
                    break;
                p+=tmp[k];
            }
     //       cout<<p<<endl;
            S.insert(p);/**�õ���Ӧһ��FD���ַ���(attr1_num1,attr2_num2,...,attrn-1_numn-1,attrn)������S*/
        }
        row++;
    }
    pattern.push_back(S);
    ifstr.close();
 //   cout<<"#row="<<row<<endl;
    //������Դ����ȡģʽ����
    for(int i=1;i<File.size();i++){
    //        cout<<i<<endl;
        int row=0;
        fd=-1;
        S.clear();
        ifstr.open((dir+File[i]).c_str(),ios::in);
        getline(ifstr,s); //�����Լ���
        getline(ifstr,s); //��FD����/
        fd=atoi(s.c_str());/**��ȡ��ǰ���ĸ�FD*/
        while(ifstr){
            p="";
            tmp="";
            for(int j=0;j<FD[fd].size()-1;j++){/**��ȡ��ǰ���Ե�����ǰ��*/
                ifstr>>s;
                p+=s;
                p+=",";
            }
            ifstr>>s;
            for(int k=0;k<s.length();k++){/**��ȡ�����attr_num��attr����*/
                if(s[k]=='_')
                    break;
                p+=s[k];
            }
      //      cout<<p<<endl;
            if(pattern[0].find(p)!=pattern[0].end())/**??????????????? ��ǰSi��Ԫ����S0�д��ڲ���ӵ�pattern����Ϊ�˻�ȡ����si��s0�Ľ����� */
                S.insert(p);
            row++;
        }
        pattern.push_back(S);
        ifstr.close();
   //     cout<<"#row="<<row<<endl;
   }
    cout<<"To Cover "<<pattern[0].size()<<"elements"<<endl;
}
int GreedySelectSource(){
    int MaxCov = -1;
    int src_MaxCov =-1;
    int pos;

    double MAX_newcov_sub_oldcov = 0;
    double oldcov=0;
    set<string> NewCover;

    for(int i=1;i<pattern.size();i++){
        if(SelectedSrc[i]==1)
            continue;
        set_union(CoveredSet.begin(), CoveredSet.end(), pattern[i].begin(), pattern[i].end(), inserter(NewCover, NewCover.begin()));
   //     cout<<NewCover.size()<<endl;
        int tmp=NewCover.size();/*****һ���������Ȩ�أ���������������****/
        double newcov_sub_oldcov;/**����(cov(SUsi)-cov(S))��(cov(SUsi)-cov(S))/ci*/
        if(if_add_random_weight == true)/**���Ҫ�����Ȩ��*/
            newcov_sub_oldcov = (double)(tmp-oldcov)/file_random_weight[i];
        else
            newcov_sub_oldcov = (double)(tmp-oldcov);
        if(newcov_sub_oldcov>MAX_newcov_sub_oldcov)
        {
            oldcov = NewCover.size();
            MAX_newcov_sub_oldcov = newcov_sub_oldcov;
            MaxCov=NewCover.size();
            src_MaxCov=i;
        }
        /**if(tmp > MaxCov){
            MaxCov=NewCover.size();
            src_MaxCov=i;
        }*/
        NewCover.clear();
    }
    if(MaxCov==CoveredSet.size()){
        cout<<"no new elements can be covered!"<<endl;
        return -1;
    }
    //����
   set_union(CoveredSet.begin(), CoveredSet.end(), pattern[src_MaxCov].begin(), pattern[src_MaxCov].end(), inserter(CoveredSet, CoveredSet.begin()));
    return src_MaxCov;
}

int main(int argc, char * argv[])
{
    /*argv[1]="1000";
    argv[2]="200";
    argv[3]="5";
    argv[4]="100";
    argv[5]="100";
    argv[6]="2000";*/
    //argv[7]="add_random_weight_YES";
   vector<int>SelSrcs;
   string s;
   no_source=atoi(argv[1]);/**�����ļ�����*/
   max_no_set=atoi(argv[3]);/**ѡ�������Դ����*/

   srand(time(NULL));
   if_add_random_weight = (strcmp(argv[9],"add_random_weight_YES")==0?true:false);
   if(if_add_random_weight == true){
        ifstream fiin;
        string randomweightpath = dir + "random_weight.txt";
        fiin.open(randomweightpath.c_str(), ios::in);
        cout<<"#add_random_weight"<<endl;
        for(int i=0;i<=no_source;i++)/**��ÿ���ļ��������һ��1-10��Ȩ��*/
        {
            string sss;
            fiin>>sss;
            file_random_weight.push_back(atof(sss.c_str()));
            cout<<"--source "<<i<<" weight:"<<file_random_weight[file_random_weight.size()-1]<<endl;
        }
        fiin.close();
    }


   clock_t start,finish;
   double totaltime;
   start=clock();
   init_FD();
   for(int i=0;i<=no_source;i++)/**��������������ǵ�*/
        SelectedSrc.push_back(0);
   ifstream ifstr;
   string File_name;
   string infile=dir+"FileName_"+argv[6]+".txt";
   ifstr.open(infile.c_str(),ios::in);
   int cntt= no_source+1;
   while(ifstr>>File_name&&cntt--){    /**�����ļ����б�File��*/
        File.push_back(File_name);
   }
   ifstr.close();

   ComputePattern();/**�õ�S0��ȫ���͸���Si��S0�Ľ���*/
    for(int i=0;i<max_no_set;i++){
        int src=GreedySelectSource();
        if(src==-1)
            break;
   //     cout<<"--source "<<src<<endl;
        SelSrcs.push_back(src);
        SelectedSrc[src]=1;
    }
   finish=clock();
   totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
 //  cout<<"\n�˳��������ʱ��Ϊ"<<totaltime<<"�룡"<<endl;
    ofstream oout;
    string file=dir+"output\\greedy_runtime.txt";
    oout.open(file.c_str(),ios::out|ios::app);
    oout<<argv[1]<<"_"<<argv[2]<<"_"<<argv[3]<<"_"<<argv[4]<<"_"<<argv[5]<<"\t"<<totaltime<<"\n";
    oout.close();


//    s+="output\\greedy_result.txt";
//    file=dir+s;
//    oout.open(file.c_str(),ios::out|ios::app);
//    oout<<argv[1]<<"_"<<argv[2]<<"_"<<argv[3]<<"_"<<argv[4]<<"_"<<argv[5]<<"\t"<<pattern[0].size()<<"\t"<<CoveredSet.size()<<"\n";
//    oout.close();

    s="";
    s=s+argv[1]+"_"+argv[2]+"_"+argv[3]+"_"+argv[4]+"_"+argv[5]+"_"+argv[6]+"_greedy_SelectedSrcs.txt";
    file=dir+s;
    oout.open(file.c_str(),ios::out);
    oout<<"greedy\n";
    oout<<File[0]<<"\n";
    for(int i=0;i<SelSrcs.size();i++)
    {
        oout<<File[SelSrcs[i]]<<"\n";
    }
    oout.close();

   return 0;
}
