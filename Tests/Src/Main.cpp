#include <EnergoKodProperty/Property.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>

class Observer;

class WithProperties
{
public:
    Property<WithProperties, int> mIntProperty = Property<WithProperties, int>(this, &WithProperties::getInt, &WithProperties::setInt, &WithProperties::notifyInt);
    Property<WithProperties, std::string> mStringProperty = Property<WithProperties, std::string>(this, &WithProperties::getString, &WithProperties::setString, &WithProperties::notifyString);
public:
    int& getInt();
    void setInt(const int& aValue);
    void notifyInt(int& aOld, int& aNew);

    std::string& getString();
    void setString(const std::string& aValue);
    void notifyString(std::string& aOld, std::string& aNew);

    void addObserver(Observer* aObserver);
protected:
    int mInt;
    std::string mString;
    std::vector<Observer*> mObservers;
};

class Observer
{
public:
    void notifyInt(int aOld, int aNew);
    void notifyString(std::string aOld, std::string aNew);
};

int main()
{
    WithProperties lObject;
    Observer lObserver1, lObserver2;
    lObject.addObserver(&lObserver1);
//    lObject.addObserver(&lObserver2);

    lObject.mIntProperty = 100;
    lObject.mStringProperty = "Takie coś!";

    if(lObject.mIntProperty == 100)
        std::cout << "int asingment OK!" << std::endl;
    else
        std::cout << "string asingment FAILED!" << std::endl;

    if(lObject.mStringProperty == "Takie coś!")
        std::cout << "string asingment OK!" << std::endl;
    else
        std::cout << "string asingment FAILED!" << std::endl;


    lObject.mIntProperty = 444;
    lObject.mIntProperty = 444;
    lObject.mStringProperty = "Coś fajnego!!!";
    lObject.mStringProperty = "Coś fajnego!!!";

    if(lObject.mIntProperty == 444)
        std::cout << "int asingment OK!" << std::endl;
    else
        std::cout << "string asingment FAILED!" << std::endl;

    if(lObject.mStringProperty == "Coś fajnego!!!")
        std::cout << "string asingment OK!" << std::endl;
    else
        std::cout << "string asingment FAILED!" << std::endl;

    int lValue(lObject.mIntProperty);
    lValue += 999;
    lObject.mIntProperty = lValue;
    lObject.mIntProperty += 1;

    std::string lString(lObject.mStringProperty);
    lString += " To jest coś!!!";
    lObject.mStringProperty = lString;
//    lObject.mStringProperty += std::string(" A to dopiero!!!");   // Failed with g++
    static_cast<std::string&>(lObject.mStringProperty) += " A to dopiero!!!";

    std::cout << "Final int value: " << lObject.mIntProperty << std::endl;
//    std::cout << "Final string value: " << lObject.mStringProperty << std::endl;    // Failed with g++
    std::cout << "Final string value: " << static_cast<std::string>(lObject.mStringProperty) << std::endl;

    return EXIT_SUCCESS;
}

void WithProperties::setInt(const int &aValue)
{
    mInt = aValue;
}

int &WithProperties::getInt()
{
    return mInt;
}

void WithProperties::notifyInt(int& aOld, int& aNew)
{
    for(Observer* lObserver : mObservers)
        lObserver->notifyInt(aOld, aNew);
}

void WithProperties::setString(const std::string &aValue)
{
    mString = aValue;
}

std::string& WithProperties::getString()
{
    return mString;
}

void WithProperties::notifyString(std::string& aOld, std::string& aNew)
{
    for(Observer* lObserver : mObservers)
        lObserver->notifyString(aOld, aNew);
}

void WithProperties::addObserver(Observer* aObserver)
{
    mObservers.push_back(aObserver);
}

void Observer::notifyInt(int aOld, int aNew)
{
    std::cout << "Int changed! Old: " << aOld << ", new: " << aNew << std::endl;
}

void Observer::notifyString(std::string aOld, std::string aNew)
{
    std::cout << "string changed! Old: " << aOld << ", new: " << aNew << std::endl;
}
