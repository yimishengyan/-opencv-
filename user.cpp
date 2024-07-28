#include "user.h"
#include <fstream>
#include <sstream>
#include <QDebug>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

User::User(QObject *parent) : QObject(parent)
{

}

User::User(int id, std::string name, bool gender, int age) : id(id), name(name), gender(gender), age(age)
{

}

void User::saveUser(int userId)
{
    std::ofstream outFile("D://cppQtProgramme//User//UserInfo.txt", std::ios::app);

        if (!outFile)
        {
            qDebug() << "Error opening file for appending.";
            return;
        }

        outFile << id << "\n";
        outFile << name << "\n";
        outFile << (gender ? "Male" : "Female") << "\n";
        outFile << age << "\n";

        outFile.close();
}

void User::findUser(int userId)
{
    std::ifstream inFile("D://cppQtProgramme//User//UserInfo.txt");
    std::string line;
    bool found = false;

    if (!inFile)
    {
        qDebug() << "Error opening file for reading.";
        return;
    }

    try {
        while (std::getline(inFile, line))
        {
            // Read ID
            int idFromFile = std::stoi(line); // Convert line to integer

            // Read Name line
            if (!std::getline(inFile, line)) {
                qDebug() << "Error reading Name line.";
                break;
            }
            std::string name = line;

            // Read Gender line
            if (!std::getline(inFile, line)) {
                qDebug() << "Error reading Gender line.";
                break;
            }
            bool gender = (line == "Male");

            // Read Age line
            if (!std::getline(inFile, line)) {
                qDebug() << "Error reading Age line.";
                break;
            }
            int age = std::stoi(line); // Convert line to integer

            // Check if current ID matches the searched ID
            if (idFromFile == userId)
            {
                // Assign values to user object
                this->id = userId;
                this->name = name;
                this->gender = gender;
                this->age = age;

                found = true;
                break;
            }
        }
    } catch (const std::invalid_argument& e) {
        qDebug() << "Invalid argument error: " << e.what();
    } catch (const std::out_of_range& e) {
        qDebug() << "Out of range error: " << e.what();
    }

    inFile.close();

    if (!found)
    {
        qDebug() << "User with ID" << userId << "not found.";
    }
}


void User::deleteUser(int userId,int num){
    std::ifstream inFile("D://cppQtProgramme//User//UserInfo.txt");
    std::ofstream outFile("D://cppQtProgramme//User//TempUserInfo.txt");

    if (!inFile)
    {
        qDebug() << "Error opening input file for reading.";
        return;
    }

    if (!outFile)
    {
        qDebug() << "Error opening output file for writing.";
        inFile.close();
        return;
    }

    std::string line;
    bool found = false;

    try {
        while (std::getline(inFile, line))
        {
            // Read ID
            int idFromFile = std::stoi(line); // Convert line to integer

            // Read Name line
            if (!std::getline(inFile, line)) {
                qDebug() << "Error reading Name line.";
                break;
            }
            std::string name = line;

            // Read Gender line
            if (!std::getline(inFile, line)) {
                qDebug() << "Error reading Gender line.";
                break;
            }
            bool gender = (line == "Male");

            // Read Age line
            if (!std::getline(inFile, line)) {
                qDebug() << "Error reading Age line.";
                break;
            }
            int age = std::stoi(line); // Convert line to integer

            // Check if current ID matches the searched ID
            if (idFromFile == userId)
            {
                // Skip this user's information
                found = true;
                continue;
            }

            // Write the user's information to the new file
            outFile << idFromFile << "\n";
            outFile << name << "\n";
            outFile << (gender ? "Male" : "Female") << "\n";
            outFile << age << "\n";
        }
    } catch (const std::invalid_argument& e) {
        qDebug() << "Invalid argument error: " << e.what();
    } catch (const std::out_of_range& e) {
        qDebug() << "Out of range error: " << e.what();
    }

    inFile.close();
    outFile.close();

    // Delete the original file and rename the temporary file
    if (found)
    {
        if (std::remove("D://cppQtProgramme//User//UserInfo.txt") != 0)
        {
            qDebug() << "Error deleting original file.";
        }
        else if (std::rename("D://cppQtProgramme//User//TempUserInfo.txt", "D://cppQtProgramme//User//UserInfo.txt") != 0)
        {
            qDebug() << "Error renaming temporary file.";
        }
        else
        {
            qDebug() << "User with ID" << userId << "deleted successfully.";
        }

        // Delete user image file
        if(num){
            std::string imagePath = "D://cppQtProgramme//User//UserImg//num" + std::to_string(userId) + ".jpg";
        }
    }
    else
    {
        qDebug() << "User with ID" << userId << "not found.";
        std::remove("D://cppQtProgramme//User//TempUserInfo.txt"); // Remove temporary file if user not found
    }
}

void User::changeUser()
{
    deleteUser(id,0);
    saveUser(id);
}
