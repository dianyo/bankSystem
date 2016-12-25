#include <iostream>
#include <map>
#include "md5.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <cmath>
#include <string>
using namespace std;
int d_time=0;
class history
{
    public:
        bool to_from;
        string id;
        int trans_test;
        unsigned long long trans_money;
        int trans_time;
};
class data
{
    public:
        unsigned long long money;
        string password;
        vector<history> his;
};
class trans_data
{
    public:
        string id;
        int trans_score;
};
class create_data
{
    public:
        string id;
        int create_score;
};
map<string,data> bank;
bool find_id(string ID)
{
    map<string,data>::iterator it;
    it=bank.find(ID);
    if(it!=bank.end())
        return true;
    return false;
}
bool password_correct(string ID,string pass)
{
    if(md5(pass)==bank.at(ID).password) return true;
    return false;
}
void create(string ID,string pass)
{
    data create_data;
    create_data.password=md5(pass);
    create_data.money=0;
    bank.insert(pair<string,data>(ID,create_data));
    cout<<"success"<<endl;
}
void del(string ID)
{   bank.erase(ID);}
bool cmp_trans(const history A,const history B)
{
    if(A.trans_time!=B.trans_time) return A.trans_time<B.trans_time;
    return A.trans_test<B.trans_test;
}
void merge_history(string ID,vector<history> k)
{
    vector<history>::iterator it;
    for(it=k.begin();it!=k.end();++it)
        bank[ID].his.push_back(*it);
    sort(bank[ID].his.begin(),bank[ID].his.end(),cmp_trans);
    for(it=bank[ID].his.begin();it!=bank[ID].his.end();it++)
        (*it).id=ID;
}
void change_history(string ID1,string ID2,vector<history> k)
{
    vector<history>:: iterator it1,it2;
    for(it1=k.begin();it1!=k.end();it1++)
    {
        string to_changeID=(*it1).id;
        for(it2=bank[to_changeID].his.begin();it2!=bank[to_changeID].his.end();it2++)
            if((*it2).id==ID2)
                (*it2).id==ID1;

    }
}
void merge_account(string ID1,string ID2)
{
    data merge_data=bank[ID2];
    vector<history>:: iterator it;
    for(it=bank[ID1].his.begin();it!=bank[ID1].his.end();++it)
        (*it).trans_test=1;
    for(it=merge_data.his.begin();it!=merge_data.his.end();++it)
        (*it).trans_test=2;
    bank[ID1].money+=merge_data.money;
    merge_history(ID1,merge_data.his);
    change_history(ID1,ID2,merge_data.his);
    del(ID2);
}
void transfer(string ID1,string ID2,unsigned long long int t_money)
{
    bank[ID1].money-=t_money;
    bank[ID2].money+=t_money;
    history trans_from,trans_to;

    trans_from.to_from=true;
    trans_from.id=ID1;
    trans_from.trans_money=t_money;
    trans_from.trans_time=d_time;
    trans_from.trans_test=0;

    trans_to.to_from=false;
    trans_to.id=ID2;
    trans_to.trans_money=t_money;
    trans_to.trans_time=d_time;
    trans_to.trans_test=0;

    d_time++;

    bank[ID2].his.push_back(trans_from);
    bank[ID1].his.push_back(trans_to);
}
void search_his(string ID,string last_ID)
{
    bool record=0;
    vector<history>::iterator it;
    for(it=bank[last_ID].his.begin();it!=bank[last_ID].his.end();it++)
    {
        if((*it).id==ID)
        {
            if((*it).to_from==false) cout<<"To ";
            else cout<<"From ";
            cout<<(*it).id<<" "<<(*it).trans_money<<endl;

            record=1;
        }

    }
    if(record==0) cout<<"no record"<<endl;
}
bool correct(string ID,string wild_ID)
{
    int len_id=ID.length();
    int i,j;
    for(i=0,j=0;j<len_id;)
    {
        if(wild_ID[i]=='*')
        {
            i++;
            if(i==wild_ID.length()) return true;
            int j_test=j;
            for(int k=1;k<=len_id-j;k++)
            {
                j_test++;
                if(j==ID.length()) return true;
                if(correct(ID.substr(j_test),wild_ID.substr(i))) return true ;
            }
        }
        else if(wild_ID[i]!=ID[j]&&wild_ID[i]!='?') return false;
        else
        {
            i++;
            j++;
        }
    }

    if(i!=wild_ID.length()) return false;
    return true;
}
void wild_find(string wild_ID,string last_ID)
{
    int ans_num=0;
    map<string,data>::iterator it;
    for(it=bank.begin();it!=bank.end();it++)
    {
        if(correct((*it).first,wild_ID) && (*it).first!=last_ID)
        {
            if(ans_num!=0)   cout<<",";
            cout<<(*it).first;
            ans_num++;
        }
        if(ans_num==10) break;
    }
    cout<<endl;
}
bool possible_len(int i)
{
    if(i==0) return true;
    int j=i;
    for(int k=1;k<=1000;k++)
    {
        j-=k;
        if(j==0) return true;
        if(j<0) return false;
    }
}
void before_find_by(vector<string> &k,string s,int len,int try_score,int pos)
{
    if(try_score==0)
    {
        k.push_back(s);
        return;
    }
    if(pos<0||try_score<0) return;
    before_find_by(k,s,len,try_score,pos-1);
    s[pos]='?';
    before_find_by(k,s,len,try_score-len+pos,pos-1);

}
void find_by(vector<create_data> &advice_account,string s,int pos,int len,int &count_ans)
{
    create_data k;

    if(pos==len)
    {
        k.id=s;
        if(!find_id(k.id))
        {
            advice_account.push_back(k);
            count_ans++;
        }
        return ;
    }
    if(pos>len) return ;
    string tmp=s;
    if(s[pos]=='?')
    {
        for(int i=0;i<=9;i++)
        {
            tmp[pos]=(char) '0'+i;
            find_by(advice_account,tmp,pos+1,len,count_ans);
        }
        for(int i=0;i<=25;i++)
        {
            tmp[pos]=(char) 'A'+i;
            find_by(advice_account,tmp,pos+1,len,count_ans);
        }
        for(int i=0;i<=25;i++)
        {
            tmp[pos]=(char) 'a'+i;
            find_by(advice_account,tmp,pos+1,len,count_ans);
        }
    }
    else
        find_by(advice_account,s,pos+1,len,count_ans);
}
void find_by_dif(vector<create_data> &advice_account,string test,int try_score,int &count_ans)
{

    if(count_ans>=10) return;
    int len=0;
    for(int i=0;i<test.length();i++)
    {
        if(test[i]=='?') break;
        len++;
    }
    vector<string> k;
    before_find_by(k,test,len,try_score,len-1);

    vector<string>::iterator it;

    for(it=k.begin();it!=k.end();it++)
        find_by(advice_account,(*it),0,(*it).length(),count_ans);
}
void find_by_len(vector<create_data> &advice_account,string test,int try_score,int &count_ans)
{
    int len=test.length();
    create_data k;
    for(int i=try_score;i>=0;i--)
    {
        if(try_score-i==0 && possible_len(i) &&!find_id(test.substr(0,len-i)))
        {
            k.id=test.substr(0,len-i);
            advice_account.push_back(k);
            count_ans++;
        }
        else if(possible_len(i))
            find_by_dif(advice_account,test.substr(0,len-i),try_score-i,count_ans);


    }
    for(int i=1;i<=try_score;i++)
    {
        if(try_score-i==0 && possible_len(i))
        {
            string k="?",tmp=test;
            int j=i-1;
            while(j--) k+="?";
            tmp+=k;
            find_by(advice_account,tmp,0,tmp.length(),count_ans);

        }
        else if(possible_len(i))
        {
            string k="?",tmp=test;
            int j=i-1;
            while(j--) k+="?";
            tmp+=k;
            find_by_dif(advice_account,tmp,try_score-i,count_ans);

        }
    }
    return ;
}
bool cmp_advice(const create_data A,const create_data B)
{
    if(A.create_score!=B.create_score) return A.create_score<B.create_score;
    return A.id<B.id;
}
int score(string ID1,string ID2)
{
    int ans=0;
    int len1=ID1.length(),len2=ID2.length();
    int delta_L=abs(len1-len2),L=min(len1,len2);
    for(int i=0;i<L;i++)
    {
        if(i<delta_L) ans+=(i+1);
        if(ID1[i]!=ID2[i]) ans+=(L-i);
    }
    return ans;
}
void advice_create(string ID)
{
    int lenID=ID.length();
    string test_ID=ID;
    vector<create_data> advice_account;
    int count_ans=0,try_score=0;
    while(1)
    {
        try_score++;
        find_by_len(advice_account,test_ID,try_score,count_ans);
        if(count_ans>=10) break;
    }
    vector<create_data>::iterator it;
    if(advice_account.empty()) cout<<"NO"<<endl;
    for(it=advice_account.begin();it!=advice_account.end();it++)
        (*it).create_score=score(ID,(*it).id);

    sort(advice_account.begin(),advice_account.end(),cmp_advice);
    count_ans=0;
    for(it=advice_account.begin();it!=advice_account.end();it++)
    {
        count_ans++;
        if(count_ans==1) cout<<(*it).id;
        else cout<<","<<(*it).id;
        if(count_ans==10) break;
    }
    cout<<endl;
}
bool cmp_trans2(const trans_data A,const trans_data B)
{
    if(A.trans_score!=B.trans_score) return A.trans_score<B.trans_score;
    return A.id<B.id;
}

void advice_trans(string ID)
{
    vector<trans_data> trans;
    trans_data k;
    map<string,data>::iterator it;
    for(it=bank.begin();it!=bank.end();it++)
    {
        if((*it).first!=ID)
        {
            k.id=(*it).first;
            k.trans_score=score(ID,(*it).first);
            trans.push_back(k);
        }
    }
    sort(trans.begin(),trans.end(),cmp_trans2);
    vector<trans_data>::iterator it2;
    int count_ans=0;
    for(it2=trans.begin();it2!=trans.end();it2++)
    {
        count_ans++;
        if(count_ans>10) break;
        if(count_ans==1) cout<<(*it2).id;
        else cout<<","<<(*it2).id;
    }
    cout<<endl;
}
int main()
{
    string direction;
    string last_ID;
    while(cin>>direction)
    {
        if(direction=="login")
        {
            string ID;
            string pass;
            cin>>ID>>pass;
            if(find_id(ID)==false) cout<<"ID "<<ID<<" not found"<<endl;
            else if(password_correct(ID,pass)==false) cout<<"wrong password"<<endl;
            else cout<<"success"<<endl;
            last_ID=ID;
        }
        if(direction=="create")
        {
            string ID;
            string pass;
            cin>>ID>>pass;
            if(find_id(ID)==true)
            {
                cout<<"ID "<<ID<<" exists, ";
                advice_create(ID);
            }
            else
                create(ID,pass);
            last_ID=ID;
        }
        if(direction=="delete")
        {
            string ID;
            string pass;
            cin>>ID>>pass;
            if(find_id(ID)==false) cout<<"ID "<<ID<<" not found"<<endl;
            else if(password_correct(ID,pass)==false) cout<<"wrong password"<<endl;
            else
            {
                cout<<"success"<<endl;
                del(ID);
            }
        }
        if(direction=="merge")
        {
            string ID1,ID2;
            string pass1,pass2;
            bool login1=0,login2=0;
            cin>>ID1>>pass1>>ID2>>pass2;

            if(find_id(ID1)==false) cout<<"ID "<<ID1<<" not found"<<endl;
            else if(password_correct(ID1,pass1)==false) cout<<"wrong password1"<<endl;
            else login1=1;

            if(find_id(ID2)==false) cout<<"ID "<<ID2<<" not found"<<endl;
            else if(password_correct(ID2,pass2)==false) cout<<"wrong password2"<<endl;
            else login2=1;

            if(login1&&login2)
            {
                merge_account(ID1,ID2);
                cout<<"success, "<<ID1<<" has "<<bank.at(ID1).money<<" dollars "<<endl;
            }
            last_ID=ID1;
        }
        if(direction=="deposit")
        {
            unsigned long long int deposit_money;
            cin>>deposit_money;
            bank[last_ID].money+=deposit_money;
            cout<<"success, "<<bank[last_ID].money<<" dollars in current account"<<endl;
        }
        if(direction=="withdraw")
        {
            unsigned long long int withdraw_money;
            cin>>withdraw_money;
            if(bank[last_ID].money>=withdraw_money)
            {
                bank[last_ID].money-=withdraw_money;
                cout<<"success, "<<bank[last_ID].money<<" dollars left in current account"<<endl;
            }
            else
                cout<<"fail, "<<bank[last_ID].money<<" dollars only in current account"<<endl;
        }
        if(direction=="transfer")
        {
            string trans_ID;
            unsigned long long t_money;
            cin>>trans_ID>>t_money;
            if(find_id(trans_ID)==false)
            {
                cout<<"ID "<<trans_ID<<" not found, ";
                advice_trans(trans_ID);
            }
            else if(bank[last_ID].money<t_money)
                cout<<"fail, "<<bank[last_ID].money<<" dollars only in current account"<<endl;
            else
            {
                transfer(last_ID,trans_ID,t_money);
                cout<<"success, "<<bank[last_ID].money<<" dollars left in current account"<<endl;
            }
        }
        if(direction=="find")
        {
            string wild_ID;
            cin>>wild_ID;
            wild_find(wild_ID,last_ID);
        }
        if(direction=="search")
        {
            string ID;
            cin>>ID;
            search_his(ID,last_ID);
        }
    }
    return 0;
}
