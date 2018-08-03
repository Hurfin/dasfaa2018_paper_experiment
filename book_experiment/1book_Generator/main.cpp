/**
����book

��Ҫ�޸ģ��������� flight������Ӧ��
����book�ϲ���ʹ����Ϊbook��ռ�˶���

��ΪISBN��ʽ���̶��������ǰ�����޸�Ϊ�����������ƥ��

*/

#include <bits/stdc++.h>
#include <ctime>
#include <set>
#include <algorithm>

#include<regex>
using namespace std;
string ISBN_regex_pattern1="\\d{9}[\\d|\\w]";
string ISBN_regex_pattern2="\\d{12}[\\d|\\w]";

string ISBN_regex_pattern3="[0-9]{9}[0-9|a-z|A-Z]";
string ISBN_regex_pattern4="[0-9]{12}[0-9|a-z|A-Z]";

regex pattern1(ISBN_regex_pattern1);
regex pattern2(ISBN_regex_pattern2);
regex pattern3(ISBN_regex_pattern3);
regex pattern4(ISBN_regex_pattern4);


int min_no_book=1;
//int max_no_book=1000;
int max_no_book=300;
map<string,int> ISBN;
map<string,int>map_src;
vector<set<string> > books;/**�²�ÿ������������һ������������ISBN*/
const string dir="C:\\Users\\12433\\Desktop\\Data_book\\";
bool cmp(set<string> x,set<string> y){
    if(x.size() >= y.size()){
        return true ;
    }else{
        return false;
    }
}
bool check_if_ISBN(string s)/**����ǲ���ISBN��*/
{

    if(regex_match(s,pattern1)||regex_match(s,pattern2))return true;
    return false;
}

int main(int argc, char * argv[])
{
//    argv[1]="200";argv[2]= "100";argv[3]= "1";argv[4]= "100";argv[5]= "100"; argv[6]="book";argv[7]= "SelectedSrcs";

    map<string,int>::iterator it;
   ifstream ifstr;
   string infile=dir+"input\\book.txt";/**!!!!!!!!!!!!!!!!!!!*/
   ifstr.open(infile.c_str(),ios::in);
   string s="";
   string booknamestr="";

   while(ifstr>>s){
        string ISBNstr;
        if(check_if_ISBN(s)){/**��ISBN*/
            //booknamestr = "";
            ISBNstr = s;

            //getline(ifstr,s);
        }
        else{
            booknamestr+=s;
            continue;
        }

        if(map_src.find(booknamestr)==map_src.end()){/**û������������¼�һ��ӳ��1...*/
            //string ISBNstr=s;
            //ifstr>>s;
            set<string> tmp_S;
            tmp_S.insert(ISBNstr);
            map_src[booknamestr]=books.size();/**������--��Ӧ��������vector<set<string>>books���±�*/
            books.push_back(tmp_S);/**1...�������������books�����һ������*/
        }
        else{
            it=map_src.find(booknamestr);/**������������*/

            int pos=it->second;/**������books������ȡ��*/
           // ifstr>>s;          /**��ISBN��*/

            books[pos].insert(ISBNstr);  /**��ISBN�Ų嵽������*/

        }
      //  ISBN.insert(s);
        it=ISBN.find(s);
        if(it==ISBN.end()){/**��Ϊ��ͳ��ISBN�ļ��ϣ��򣩴�С��     ������ʽΪ ISBN--��� ֪���ˣ�����ԭ����Ϊ�˸�����ISBN����������Ϊֵ���*/
            ISBN[s]=ISBN.size();
        }

        getline(ifstr,s);/**û�к��������Ƕ������𣬻���ֻ����������ISBN */
        booknamestr="";/**����ַ���׼��������һ����*/
   }
   ifstr.close();
   cout<<"#elements="<<ISBN.size()<<endl;
   sort(books.begin(),books.end(),cmp);
   ofstream ofstr_filename;
   string tmp_s=dir+"\\FileName_"+argv[6]+".txt";/**��Ҫ���ļ��������һ���ļ���*/
   ofstr_filename.open(tmp_s.c_str(),ios::out);
   int book_cnt=0;
   for(int i=0;i<books.size();i++){
        if(books[i].size()<min_no_book)
            continue;
        if(books[i].size()>max_no_book&&i!=0)/**?????????????Ϊʲôi==0ʱ��Ϳ��Գ��� max_no_book ��С  ֪������Ϊ�˱�֤S0�������ļ��϶�Ҫ��*/
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
