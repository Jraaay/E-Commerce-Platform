#include "usermanager.h"

userManager::userManager()
{

}

int userManager::getMaxUid()
{
    ifstream infile;
    string uidMaxJson = "";
    int uidMax;
    try
    {
        infile.open("uidMaxFile.json");
        infile >> uidMaxJson;
        infile.close();
        const json j = json::parse(uidMaxJson);
        uidMax = j["uid"];
    }
    catch (exception &e)
    {
        uidMaxJson = "";
        qDebug() << e.what() << endl;
        const int tmp = 0;
        json j;
        j["uid"] = tmp;
        ofstream outFile;
        outFile.open("uidMaxFile.json");
        outFile << j.dump();
        outFile.close();
    }
    return uidMax;
}

vector<sellerClass> userManager::getSellerList()
{
    vector<sellerClass> sellerList;
    ifstream infile;
    string sellerJson = "";
    try
    {
        infile.open("sellerFile.json");
        infile >> sellerJson;
        infile.close();
        const json j = json::parse(sellerJson);
        const vector<string> userListJson = j["data"];
        for (int i = 0; i < (int)userListJson.size(); i++)
        {
            const json jTmp = json::parse(userListJson[i]);
            sellerClass tmp;
            tmp.uid = jTmp["uid"];
            tmp.name = jTmp["name"];
            tmp.type = jTmp["type"];
            tmp.balance = jTmp["balance"];
            tmp.setPass(jTmp["password"]);
            sellerList.push_back(tmp);
        }
    }
    catch (exception &e)
    {
        qDebug() << e.what() << endl;
        vector<string> tmp;
        json j;
        j["data"] = tmp;
        ofstream outFile;
        outFile.open("sellerFile.json");
        outFile << j.dump();
        outFile.close();
    }
    return sellerList;
}

vector<consumerClass> userManager::getConsumerList()
{
    vector<consumerClass> consumerList;
    ifstream infile;
    string consumerJson = "";
    try
    {
        infile.open("consumerFile.json");
        infile >> consumerJson;
        infile.close();
        const json j = json::parse(consumerJson);
        const vector<string> userListJson = j["data"];
        for (int i = 0; i < (int)userListJson.size(); i++)
        {
            const json jTmp = json::parse(userListJson[i]);
            consumerClass tmp;
            tmp.uid = jTmp["uid"];
            tmp.name = jTmp["name"];
            tmp.type = jTmp["type"];
            tmp.balance = jTmp["balance"];
            tmp.setPass(jTmp["password"]);
            consumerList.push_back(tmp);
        }
    }
    catch (exception &e)
    {
        qDebug() << e.what() << endl;
        vector<string> tmp;
        json j;
        j["data"] = tmp;
        ofstream outFile;
        outFile.open("consumerFile.json");
        outFile << j.dump();
        outFile.close();
    }
    return consumerList;
}

void userManager::getUser(int userId, userClass &user)
{
    vector<sellerClass> sellerList = getSellerList();
    vector<consumerClass> consumerList = getConsumerList();
    for (int i = 0; i < (int) sellerList.size(); i++)
    {
        if (sellerList[i].uid == userId)
        {
            user.uid = sellerList[i].uid;
            user.name = sellerList[i].name;
            user.type = sellerList[i].type;
            user.balance = sellerList[i].balance;
            user.setPass(sellerList[i].getPass());
            return;
        }
    }
    for (int i = 0; i < (int) consumerList.size(); i++)
    {
        if (consumerList[i].uid == userId)
        {
            user.uid = consumerList[i].uid;
            user.name = consumerList[i].name;
            user.type = consumerList[i].type;
            user.balance = consumerList[i].balance;
            user.setPass(consumerList[i].getPass());
            return;
        }
    }
}
