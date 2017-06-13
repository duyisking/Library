#include "admin.h"
#include "ui_admin.h"

Admin::Admin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);
    setWindowTitle("Libpro");
    Admin::setWindowState(Qt::WindowMaximized);
    ui->registrationTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QLinkedList<Account>::iterator it=temp_accounts.begin();
    int cnt=0;
    for(;it!=temp_accounts.end();it++)
    {
        ui->registrationTable->insertRow(cnt);
        QTableWidgetItem *item = new QTableWidgetItem("");
        item->setCheckState(Qt::Unchecked);
        ui->registrationTable->setItem(cnt, 0, item);
        QLinkedList<User>::iterator it1;
        int id=(*it).getID().toInt();
        if(id>=users.size())
            it1=temp_users.begin()+id-users.size();
        else it1=users.begin()+id;
        ui->registrationTable->setItem(cnt, 1, new QTableWidgetItem((*it).getAcc()));
        if((*it).getRole()=="L")
            ui->registrationTable->setItem(cnt, 2, new QTableWidgetItem("Thủ thư"));
        else ui->registrationTable->setItem(cnt, 2, new QTableWidgetItem("Độc giả"));
        ui->registrationTable->setItem(cnt, 3, new QTableWidgetItem((*it1).getName()));
        ui->registrationTable->setItem(cnt, 4, new QTableWidgetItem((*it1).getDoP()));
        cnt++;
    }
}

Admin::~Admin()
{
    delete ui;
}

void Admin::closeEvent (QCloseEvent *event)
{
    emit closed();
    event->accept();
}



void Admin::on_searchButton_clicked()
{
    // Substring search
    // using Knuth–Morris–Pratt algorithm

    // Get word
    QString word=ui->searchUsers->text();
    if(word.length()==0)
        return;
    // Create table
    int* table=new int[word.length()+1];
    int pos=1, cnd=0;
    table[0]=-1;
    while(pos<word.length())
    {
        if(compare(word[pos], word[cnd]))
        {
            table[pos]=table[cnd];
            pos++;
            cnd++;
        }
        else
        {
            table[pos]=cnd;
            cnd=table[cnd];
            while(cnd>=0 && !compare(word[pos], word[cnd]))
                cnd=table[cnd];
            pos++;
            cnd++;
        }
    }
    table[pos]=cnd;
    // Search through the data
    QLinkedList<Account>::iterator it=accounts.begin();
    int cnt=0;
    ui->usersTable->setRowCount(0);
    for(;it!=accounts.end();it++)
    {
        QLinkedList<User>::iterator it1=users.begin()+(*it).getID().toInt();
        if((*it).getRole()!="R") continue;
        QString s[]={(*it1).getID(), (*it1).getName(), (*it1).getAddress(), (*it1).getEmail()};
        int m=0, i=0, pre=cnt;
        for(int j=0; j<4; j++)
        {
            while(m+i < s[j].length())
            {
                if(compare(word[i], s[j][m+i]))
                {
                    i++;
                    if(i==word.length())
                    {
                        ui->usersTable->insertRow(cnt);
                        ui->usersTable->setItem(cnt, 0, new QTableWidgetItem((*it1).getID()));
                        ui->usersTable->setItem(cnt, 1, new QTableWidgetItem((*it).getAcc()));
                        ui->usersTable->setItem(cnt, 2, new QTableWidgetItem((*it1).getName()));
                        ui->usersTable->setItem(cnt, 3, new QTableWidgetItem((*it1).getAddress()));
                        ui->usersTable->setItem(cnt, 4, new QTableWidgetItem((*it1).getDateofBirth()));
                        ui->usersTable->setItem(cnt, 5, new QTableWidgetItem((*it1).getEmail()));
                        ui->usersTable->setItem(cnt, 6, new QTableWidgetItem((*it1).getSex()));
                        ui->usersTable->setItem(cnt, 7, new QTableWidgetItem((*it1).getDoP()));
                        if((*it).getActive())
                            ui->usersTable->setItem(cnt, 8, new QTableWidgetItem("Kích hoạt"));
                        else ui->usersTable->setItem(cnt, 8, new QTableWidgetItem("Đã hủy"));
                        cnt++;
                        break;
                    }
                }
                else
                {
                    if(table[i]>-1)
                    {
                        m+=i-table[i];
                        i=table[i];
                    }
                    else
                    {
                        m+=i+1;
                        i=0;
                    }
                }
            }
            if(pre!=cnt) break;
            m=0, i=0;
        }
    }
    delete table;
}

void Admin::on_Agree_clicked()
{
    for(int i=0; i<ui->registrationTable->rowCount(); i++)
        if(ui->registrationTable->item(i,0)->checkState()==Qt::Checked)
        {
            QLinkedList<Account>::iterator a=temp_accounts.begin()+i;
            accounts.append(*a);
            if((*a).getID().toInt()>=users.size())
            {
                QLinkedList<User>::iterator u=temp_users.begin()+(*a).getID().toInt()-users.size();
                users.append(*u);
                users.last().setDoP(ToString(QDate::currentDate()));
                users.last().setID(QString::number(users.size()-1));
                accounts.last().setID(users.last().getID());
                temp_users.erase(u);
            }
            temp_accounts.erase(a);
            ui->registrationTable->removeRow(i);
        }
}



void Admin::on_Deny_clicked()
{
    for(int i=0; i<ui->registrationTable->rowCount(); i++)
        if(ui->registrationTable->item(i,0)->checkState()==Qt::Checked)
        {
            QLinkedList<Account>::iterator a=temp_accounts.begin()+i;
            if((*a).getID().toInt()>=users.size())
            {
                QLinkedList<User>::iterator u=temp_users.begin()+(*a).getID().toInt()-users.size();
                temp_users.erase(u);
            }
            temp_accounts.erase(a);
            ui->registrationTable->removeRow(i);
        }
}