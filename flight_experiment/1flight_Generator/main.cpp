/**
����flight

��Ҫ�޸ģ��������� flight������Ӧ��
����book�ϲ���ʹ����Ϊbook��ռ�˶���

Ϊ�˺�book�������ְ�����·����Ϊinput\\flight.txt
*/

#include <bits/stdc++.h>
#include <ctime>
#include <set>
#include <algorithm>

using namespace std;
int min_no_book=1;
//int max_no_book=1000;
int max_no_book=300;
map<string,int> ISBN;
map<string,int>map_src;
vector<set<string> > books;/**�²�ÿ������������һ������������ISBN*/
const string dir="C:\\Users\\12433\\Desktop\\Data_flight\\";
bool cmp(set<string> x,set<string> y){
    if(x.size() >= y.size()){
        return true ;
    }else{
        return false;
    }
}
int main(int argc, char * argv[])
{
//    argv[1]="200";argv[2]= "100";argv[3]= "1";argv[4]= "100"; argv[5]="100";argv[6]= "flight";argv[7]= "SelectedSrcs";

    map<string,int>::iterator it;
   ifstream ifstr;
   string infile=dir+"input\\flight.txt";/**!!!!!!!!!!!!!!!!*/
   ifstr.open(infile.c_str(),ios::in);
   string s="";

   while(ifstr>>s){
        if(map_src.find(s)==map_src.end()){/**û������������¼�һ��ӳ��1...*/
            string src=s;
            ifstr>>s;
            set<string> tmp_S;
            tmp_S.insert(s);
            map_src[src]=books.size();/**������--��Ӧ��������vector<set<string>>books���±�*/
            books.push_back(tmp_S);/**1...�������������books�����һ������*/
        }
        else{
            it=map_src.find(s);/**������������*/

            int pos=it->second;/**������books������ȡ��*/
            ifstr>>s;          /**��ISBN��*/
            books[pos].insert(s);  /**��ISBN�Ų嵽������*/

        }
      //  ISBN.insert(s);
        it=ISBN.find(s);
        if(it==ISBN.end()){/**��Ϊ��ͳ��ISBN�ļ��ϣ��򣩴�С��     ������ʽΪ ISBN--��� ֪���ˣ�����ԭ����Ϊ�˸�����ISBN����������Ϊֵ���*/
            ISBN[s]=ISBN.size();
        }
        getline(ifstr,s);/**û�к��������Ƕ������𣬻���ֻ����������ISBN */
   }
   ifstr.close();
   cout<<"#elements="<<ISBN.size();
   sort(books.begin(),books.end(),cmp);
   ofstream ofstr_filename;
   string tmp_s=dir+"\\FileName_"+argv[6]+".txt";
   ofstr_filename.open(tmp_s.c_str(),ios::out);
   int book_cnt=0;
   for(int i=0;i<books.size();i++){
        if(books[i].size()<min_no_book)
            continue;
        if(books[i].size()>max_no_book&&i!=0)/**?????????????Ϊʲôi==0ʱ��Ϳ��Գ��� max_no_book ��С*/
            continue;
        char tmp[20];

        sprintf(tmp, "%d.txt", book_cnt);
        ofstr_filename<<tmp<<endl;
        //ofstr_filename<<itoa(book_cnt,tmp,10)<<".txt"<<endl;


        ofstream ofstr;
        cout<<"--Source "<<book_cnt<<":"<<books[i].size()<<endl;

        sprintf(tmp, "%d.txt", book_cnt);
        string outfile=dir+tmp;
//        string outfile=dir+itoa(book_cnt,tmp,10)+".txt";
        ofstr.open(outfile.c_str(),ios::out);
        ofstr<<"0 1"<<endl;
        ofstr<<"0"<<endl;
        set<string>::iterator set_it;
        int cnt=0;
        for(set_it=books[i].begin();set_it!=books[i].end();set_it++){
            //cout<<*set_it<<endl;
            if(ISBN[*set_it]){
           //     cout<<ISBN[*set_it]<<endl;
                ofstr<<"0_"<<ISBN[*set_it]<<" "<<"1_"<<cnt<<endl; /**0_�������ISBN�ı�� 1_������ǰ�˳���� */
            }
            else
                cout<<"cannot find in ISBN"<<endl;
            cnt++;
        }
        ofstr.close();
        book_cnt++;
   }
   ofstr_filename.close();
   tmp_s=dir+"\\FD.txt";
   ofstr_filename.open(tmp_s.c_str(),ios::out);
   ofstr_filename<<"(0)1"<<endl;
   ofstr_filename.close();

    /**�����Ե�ֵ�����������mh_generator��ʹ��*/
   ofstream ooout;
   string oooutpath = dir + "attr_zhiyu.txt";
   ooout.open(oooutpath, ios::out);
   ooout<<ISBN.size()<<"\n";
   ooout.close();


   /**�������Ȩ��*/
    string randomweightpath = dir + "random_weight.txt";
    ofstream fout;
    fout.open(randomweightpath.c_str(), ios::out);
    for(int i=0;i<book_cnt;i++)
    {
        fout<<(rand()%10+1)<<"\n";
    }
    fout.close();
   return 0;
}
