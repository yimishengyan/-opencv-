#ifndef USER_H
#define USER_H

#include <QObject>
#include <opencv2/core.hpp>

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = nullptr);
    User(int id, std::string name, bool gender, int age);

    void saveUser(int userId);
    void findUser(int userId);
    void deleteUser(int userId,int num);
    void changeUser();

    int id;
    std::string name;
    bool gender ;
    int age;

signals:

};

#endif // USER_H

