#include<bits/stdc++.h>
using namespace std;
string dir="C:\\Users\\12433\\Desktop\\Data_flight\\";
long long The_number_of_generate_file;/**�����ļ�����*/
long long HASHN;/**����hash�����ĸ���*/
long long Max_select;/**���ѡ�������Դ����*/
long long no_FD = 1;/**���м���FD*/
double MAX_cov = 0;/**��󸲸�ֵ*/
bool if_add_random_weight = false;/**�Ƿ��ÿ������Դһ�����Ȩ��*/
vector<double>file_random_weight;/**����ÿ������Դ�����Ȩ��*/
//vector<long long>S0_per_FD_set_size;/**����S0��ÿ��FD��ͶӰ�ļ��ϴ�С,��������x^: x^=s^*(a+b^)/(1+s^) �����е�a��ֵ*/
//long long FD_domain;/**�������ù�ʽ b^ = n*hashn/sum(hj) �е�nֵ�Ĵ�С,��FDǰ�����Ե�ֵ���С,b^��������Si�Ĵ�С*/
vector<double> FD_domain;/**(�ںϳ�������������,��Ϊ�е�FDֵ��ͬ.��ʵ������һ��ֵ���ɣ���Ϊֵ������ͬ��)
                             �������ù�ʽ b^ = n*hashn/sum(hj) �е�nֵ�Ĵ�С,��FDǰ�����Ե�ֵ���С,b^��������Si�Ĵ�С*/
vector<string>filename;/**�洢�ļ����б�*/
vector<bool> si_if_selected;/**���Si�Ƿ��ѱ�ѡ����ѡ��Ϊtrue��δѡ��Ϊfalse*/



struct source_Si
{
    map<int, int>FDindex_mhindex;/**�洢����������Ŷ�Ӧ��ǩ��������mh���±�*/
    vector<vector<double> >mh;/**�洢s0�ĸ���FD��ǩ������*/
    vector<double>setSzie;/**mh�ļ���{}�е�ֵ����Si��FD�ϵ�ͶӰ�ļ��ϴ�С*/
};

struct source_Si_union
{
    map<int, int>FDindex_mhindex;/**�洢����������Ŷ�Ӧ��ǩ��������mh���±�*/
    vector<vector<double> >mh;/**�洢����si�ڸ���FD�ϵ�ǩ���Ĳ�(��ȡǩ����Сֵ)*/
};
source_Si_union SOURCE_SI_UNION;/**�洢����si�Ĳ���*/
source_Si_union tmp_SOURCE_SI_UNION;/**��ʱ�洢*/

vector<source_Si>SOURCE_SI;/**�洢mh�ļ�������*/
void synthetic_setDomain()/**���úϳ������ϸ���FD��ֵ��*/
{
    /**���ø���FD��ֵ�򣨶��ںϳ����ݣ�*/
    for(int i=0;i<no_FD;i++)
    {
        if(i<7){
            FD_domain.push_back(1000);
        }
        else{
            FD_domain.push_back(1000000);
        }
    }
}

void book_or_flight_setDomain()/**������ʵ������FD��ֵ��*/
{
    ifstream fin;
    string path = dir + "attr_zhiyu.txt";
    fin.open(path.c_str(), ios::in);
    string s;
    fin>>s;
    fin.close();
    for(int i=0;i<no_FD;i++)
    {
        FD_domain.push_back(atof(s.c_str()));
    }
//    FD_domain = atoll(s.c_str());
}

void readFilename(string path, long long no_file)
{
    ifstream fin;
    fin.open(path.c_str(), ios::in);
    string s;
    no_file++;/**����S0*/
    while(fin>>s && no_file--)
    {
        filename.push_back(s);
    }
    fin.close();
}

void readMH(string path)
{
    ifstream fin;
    fin.open(path.c_str(), ios::in);
    string s;
    while(fin>>s)
    {
        if(s[0]=='(')/**����һ���µ��ļ���*/
        {
            source_Si x;
            SOURCE_SI.push_back(x);/**���һ���ļ������ݽṹ��������������õ�������ļ�������*/
        }
        else if(s[0]=='[')/**����һ��FD*/
        {
            string FD_str="";
            for(int k=1;k<s.size()-1;k++)/**ȡ��FD���*/
                FD_str += s[k];
            int FDi_index = SOURCE_SI[ SOURCE_SI.size()-1 ].FDindex_mhindex.size();
            SOURCE_SI[ SOURCE_SI.size()-1 ].FDindex_mhindex[ atoi(FD_str.c_str()) ] = FDi_index;
            fin>>s;/**��{}*/
            string FDi_set_size_str="";
            for(int k=1;k<s.size()-1;k++)/**ȡ��Si��һ��FD�ϵ�ͶӰ�ļ��ϵĴ�С*/
                FDi_set_size_str += s[k];
            SOURCE_SI[ SOURCE_SI.size()-1 ].setSzie.push_back(atof(FDi_set_size_str.c_str()));
            while(fin>>s && s!=";");/**��ת��ǩ������*/
            vector<double>sigv;/**����ǩ��*/
            for(int i=0;i<HASHN;i++)/**��ȡǩ��*/
            {
                fin>>s;
                sigv.push_back(atof(s.c_str()));
            }
            SOURCE_SI[ SOURCE_SI.size()-1 ].mh.push_back(sigv);
        }
    }
    fin.close();
}

double computeCov(int si_index)/**���Ƽ���si��S��s0�ĸ���ֵ*/
{
    //for(int i=0;i<SOURCE_SI[si_index].FDindex_mhindex.size();i++)/**ѭ��si��ÿ��FD*/
    source_Si_union new_SOURCE_SI_UNION;/**���si��Ĳ���S*/
    new_SOURCE_SI_UNION = SOURCE_SI_UNION;/**�ȰѾɵ�S���ƹ���*/
    /**�����µ�si*/
    for(map<int,int>::iterator it = SOURCE_SI[si_index].FDindex_mhindex.begin(); it!=SOURCE_SI[si_index].FDindex_mhindex.end(); it++)/**ѭ��si��ÿ��FD*/
    {
        int si_FD = it->first;/**si��FD*/
        int si_FD_mh_index = it->second;/**FD��mh�е����(si��)*/
        if(new_SOURCE_SI_UNION.FDindex_mhindex.find(si_FD)!=new_SOURCE_SI_UNION.FDindex_mhindex.end()){/**���(����S��)�Ѿ������FD*/
            int sourceUnion_FD_mh_index = new_SOURCE_SI_UNION.FDindex_mhindex[si_FD];/**ȡ��FD��mh�е����(����S��)*/
            for(int i=0;i<HASHN;i++)
            {
                new_SOURCE_SI_UNION.mh[sourceUnion_FD_mh_index][i] = min(new_SOURCE_SI_UNION.mh[sourceUnion_FD_mh_index][i], SOURCE_SI[si_index].mh[si_FD_mh_index][i]);
            }
        }
        else{/**�����û�����FD*/
            int mhindex = new_SOURCE_SI_UNION.FDindex_mhindex.size();
            new_SOURCE_SI_UNION.FDindex_mhindex[si_FD] = mhindex;/**����һ��(FD---mh�±�)������*/
            new_SOURCE_SI_UNION.mh.push_back(SOURCE_SI[si_index].mh[si_FD_mh_index]);
        }
    }
    double sum_per_FD_intersection=0;
    for(map<int,int>::iterator it = new_SOURCE_SI_UNION.FDindex_mhindex.begin(); it!=new_SOURCE_SI_UNION.FDindex_mhindex.end(); it++)/**ѭ������S��ÿ��FD*/
    {
        int FD = it->first;/**FD*/
        int FD_mh_index = it->second;/**FD��mh�е����*/

        double sum_hash_value=0;/**�ۼ�hashֵ*/
        double _sim = 0;/**��������s^*/
        for(int i=0;i<HASHN;i++)
        {
            sum_hash_value += new_SOURCE_SI_UNION.mh[ FD_mh_index ][i];
            if(SOURCE_SI[0].mh[ SOURCE_SI[0].FDindex_mhindex[FD] ][i] == new_SOURCE_SI_UNION.mh[ FD_mh_index ][i]){
                _sim++;/**ǩ������ͬ�ĸ�����������si��s0���ƶ�*/
            }
        }
        _sim = _sim/HASHN;/**�õ�s^*/
        double _predict_S_size = FD_domain[FD]*HASHN / sum_hash_value;/**�õ����Ƶ�S��С*/
        //double _predict_S_S0_intersection = _sim*( S0_per_FD_set_size[FD] + _predict_S_size )/( 1 + _sim );/**���Ƶ�S0��S�Ľ�����С*/
        /**SOURCE_SI[0].setSzie[ SOURCE_SI[0].FDindex_mhindex[FD] ]����S0��ÿ��FD��ͶӰ�ļ��ϴ�С,��������x^: x^=s^*(a+b^)/(1+s^) �����е�a��ֵ*/
        double _predict_S_S0_intersection = _sim*( SOURCE_SI[0].setSzie[ SOURCE_SI[0].FDindex_mhindex[FD] ] + _predict_S_size )/( 1 + _sim );/**���Ƶ�S0��S�Ľ�����С*/
        sum_per_FD_intersection += _predict_S_S0_intersection;/**��ÿ��FD�ϵĽ�����С���*/
    }
    tmp_SOURCE_SI_UNION = new_SOURCE_SI_UNION;/**��ʱ����*/
    return sum_per_FD_intersection;
}
int MAX_Cov()/**����ֵΪѡ�ĸ�si���Եõ���������(cov(SUsi)-cov(S))����Ӧ��si�����*/
{
    source_Si_union final_union;
    int select_index = -1;
    double MAX_newcov_sub_oldcov = 0;
    for(int i=1;i<=The_number_of_generate_file;i++)
    {
        if(si_if_selected[i] == true) continue;

        double cov_value = computeCov(i);/**����S�м���si��õ��Ķ�S0�Ĺ��Ƹ���ֵ*/
        double newcov_sub_oldcov;/**����(cov(SUsi)-cov(S))��(cov(SUsi)-cov(S))/ci*/
        if(if_add_random_weight == true)/**���Ҫ�����Ȩ��*/
            newcov_sub_oldcov = (cov_value - MAX_cov)/file_random_weight[i];
        else
            newcov_sub_oldcov = cov_value - MAX_cov;
        if(MAX_newcov_sub_oldcov < newcov_sub_oldcov)
        {
            select_index = i;
            MAX_newcov_sub_oldcov = newcov_sub_oldcov;/**�����������*/
            MAX_cov = cov_value;/**��¼��󸲸�ֵ*/

            final_union = tmp_SOURCE_SI_UNION;
        }
    }
    SOURCE_SI_UNION = final_union;/**����ѡ�����ѵ�si�����Ĳ���S*/
    return select_index;
}
/**
   argv1����Դ����
   argv2��ϣ��������
   argv3ѡ������Դ����k
   argv4Ŀ�����ݼ���Ԫ�������ռ������ı���
   argv5����Դ��Ԫ�������ռ��������
   argv6Ԫ���������
   argv7�Ƿ�Ҫ��ÿ������Դ���һ�����Ȩ��"add_random_weight_YES" "add_random_weight_NO"
*/
int main(int argc, const char *argv[])
{
    /*argv[1]="1000";
    argv[2]="200";
    argv[3]="5";
    argv[4]="100";
    argv[5]="100";
    argv[6]="2000";
    argv[7]="add_random_weight_YES";
    */
    srand(time(NULL));
    if_add_random_weight = ((strcmp(argv[9],"add_random_weight_YES")==0)?true:false);
    The_number_of_generate_file = atoll(argv[1]);
    HASHN = atoll(argv[2]);
    Max_select = atoll(argv[3]);

//    synthetic_setDomain();/**���úϳ����ݸ���FDֵ��*/
    book_or_flight_setDomain();/**������ʵ����FDֵ��*/

    string filename_path = dir + "FileName_" + argv[6] + ".txt";
    readFilename(filename_path, The_number_of_generate_file);
    The_number_of_generate_file = filename.size() - 1;/**����ʵ�����ɵ��ļ�����������*/
    for(int i=0;i<The_number_of_generate_file+5;i++)/**���Si�Ƿ�ѡ���*/
        si_if_selected.push_back(false);
    if(if_add_random_weight == true){
        ifstream fiin;
        string randomweightpath = dir + "random_weight.txt";
        fiin.open(randomweightpath.c_str(), ios::in);
        cout<<"#add_random_weight"<<endl;
        for(int i=0;i<=The_number_of_generate_file;i++)/**��ÿ���ļ��������һ��1-10��Ȩ��*/
        {
            string sss;
            fiin>>sss;
            file_random_weight.push_back(atof(sss.c_str()));
            cout<<"--source "<<i<<" weight:"<<file_random_weight[file_random_weight.size()-1]<<endl;
        }
        fiin.close();
    }

    //-------------------------------------------------------
    clock_t start, finish;
    double totaltime = 0;
    start = clock();
    //-------------------------------------------------------
    string mhpath;
    mhpath = dir + "mh_" + argv[6] + ".txt";
    readMH(mhpath);/**��ȡmh�ļ�*/
    //-------------------------------------------------------
    vector<long long>select_file_to_cov;
    for(int i=0;i<Max_select;i++)
    {
        int select_si_index = MAX_Cov();/**����ֵΪѡ�ĸ�si���Եõ���������(cov(SUsi)-cov(S))����Ӧ��si�����*/
        if(select_si_index == -1)break;
        si_if_selected[select_si_index] = true;/**��ѡ����Si�����*/
        select_file_to_cov.push_back(select_si_index);/**��ѡ�����ļ����±걣�棬����������*/
    }
    finish = clock();
    totaltime = (double)(finish - start)/(double)CLOCKS_PER_SEC;
    cout<<"������󸲸�ֵ:"<<MAX_cov<<endl;
    //-------------------------------------------------------
    ofstream fout;
    string file = dir + "output\\mh_greedy_runtime.txt";
    fout.open(file.c_str(), ios::out|ios::app);
    fout<<argv[1]<<"_"<<argv[2]<<"_"<<argv[3]<<"_"<<argv[4]<<"_"<<argv[5]<<"\t"<<totaltime<<endl;
    fout.close();
    /**��ѡ�������Դ��������ļ�*/
    cout<<"#Src="<<select_file_to_cov.size()<<endl;
    cout<<"#Covered="<<MAX_cov<<endl;
    file = dir + argv[1] + "_" + argv[2] + "_" + argv[3] + "_" + argv[4] + "_" + argv[5] + "_" + argv[6] + "_SelectedSrcs.txt";
    fout.open(file.c_str(), ios::out);
    fout<<"mh_greedy\n";
    fout<<filename[0]<<"\n";
    for(int i=0;i<select_file_to_cov.size();i++)
    {
        fout<<filename[ select_file_to_cov[i] ]<<"\n";
        cout<<"--source "<<select_file_to_cov[i]<<endl;
    }
    fout.close();
    //---------------------------------------------------------
    cout<<"RANDOM:"<<endl;
    file = dir + argv[1] + "_" + argv[2] + "_" + argv[3] + "_" + argv[4] + "_" + argv[5] + "_" + argv[6] + "_random_SelectedSrcs.txt";
    fout.open(file.c_str(), ios::out);
    fout<<"random\n";
    set<int>rand_set;
    while(rand_set.size() < Max_select)
    {
        int r = rand()%(filename.size()-1) + 1;
        if(rand_set.find(r)!=rand_set.end())
            r = rand()%(filename.size()-1) + 1;
        rand_set.insert(r);
        fout<<r<<".txt\n";
        cout<<"--Source "<<r<<endl;
    }
    fout.close();
    return 0;
}
